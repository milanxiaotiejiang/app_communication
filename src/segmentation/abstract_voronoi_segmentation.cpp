//
// Created by Looper on 2022/9/15.
//

#include "segmentation/abstract_voronoi_segmentation.h"
#include "segmentation/contains.h"

static bool DEBUG_DISPLAYS_SHOW = false;

AbstractVoronoiSegmentation::AbstractVoronoiSegmentation() {

}

bool AbstractVoronoiSegmentation::determineRoomIndexFromRoomID(const std::vector<Room> &rooms, const int room_id,
                                                               size_t &room_index) {
    bool found_id = false;
    for (size_t r = 0; r < rooms.size(); r++) {
        if (rooms[r].getID() == room_id) {
            room_index = r;
            found_id = true;
            break;
        }
    }
    return found_id;
}

void AbstractVoronoiSegmentation::mergeRoomPair(std::vector<Room> &rooms, const int target_index,
                                                const int room_to_merge_index, cv::Mat &segmented_map,
                                                const double map_resolution) {
    const int target_id = rooms[target_index].getID();
    const int room_to_merge_id = rooms[room_to_merge_index].getID();
    rooms[target_index].mergeRoom(rooms[room_to_merge_index], map_resolution);
    rooms[room_to_merge_index].setRoomId(rooms[target_index].getID(), segmented_map);
    rooms.erase(rooms.begin() + room_to_merge_index);
    std::sort(rooms.begin(), rooms.end(), sortRoomsAscending);

    for (size_t i = 0; i < rooms.size(); ++i) {
        std::vector<int> &neighbor_ids = rooms[i].getNeighborIDs();
        std::vector<int>::iterator it = std::find(neighbor_ids.begin(), neighbor_ids.end(), room_to_merge_id);
        if (it != neighbor_ids.end()) {
            std::vector<int>::iterator it2 = std::find(neighbor_ids.begin(), neighbor_ids.end(), target_id);
            if (it2 != neighbor_ids.end())
                neighbor_ids.erase(it);
            else
                *it = target_id;
        }

        std::map<int, int> &neighbor_statistics = rooms[i].getNeighborStatistics();
        std::map<int, int>::iterator it3 = neighbor_statistics.find(room_to_merge_id);
        if (it3 != neighbor_statistics.end()) {
            std::map<int, int>::iterator it4 = neighbor_statistics.find(target_id);
            if (it4 != neighbor_statistics.end())
                it4->second += it3->second;
            else
                neighbor_statistics[target_id] = it3->second;
            neighbor_statistics.erase(it3);
        }
    }
}

void
AbstractVoronoiSegmentation::drawVoronoi(cv::Mat &img, const std::vector<std::vector<cv::Point2f>> &facets_of_voronoi,
                                         const cv::Scalar voronoi_color, const cv::Mat &eroded_map) {
    for (std::vector<std::vector<cv::Point2f>>::const_iterator current_contour = facets_of_voronoi.begin();
         current_contour != facets_of_voronoi.end(); ++current_contour) {
        cv::Point2f last_point = current_contour->back();
        for (int i = 0; i < current_contour->size(); ++i) {
            bool inside = true;
            cv::Point2f current_point = current_contour->at(i);
            if (
                    ((int) current_point.x < 0) ||
                    ((int) current_point.x >= eroded_map.cols) ||
                    ((int) current_point.y < 0) ||
                    ((int) current_point.y >= eroded_map.rows) ||
                    eroded_map.at<uchar>((int) current_point.y, (int) current_point.x) == 0 ||
                    ((int) last_point.x < 0) ||
                    ((int) last_point.x >= eroded_map.cols) ||
                    ((int) last_point.y < 0) ||
                    ((int) last_point.y >= eroded_map.rows) ||
                    eroded_map.at<uchar>((int) last_point.y, (int) last_point.x) == 0
                    ) {
                inside = false;
            }
            if (inside) {
                cv::line(img, last_point, current_point, voronoi_color, 1);
            }
            last_point = current_point;
        }
    }
}

void AbstractVoronoiSegmentation::createVoronoiGraph(cv::Mat &map_for_voronoi_generation) {
    cv::Mat map_to_draw_voronoi_in = map_for_voronoi_generation.clone();
    cv::Mat temporary_map_to_calculate_voronoi = map_for_voronoi_generation.clone();

    cv::erode(temporary_map_to_calculate_voronoi, temporary_map_to_calculate_voronoi, cv::Mat());
    cv::dilate(temporary_map_to_calculate_voronoi, temporary_map_to_calculate_voronoi, cv::Mat());

    //********************1. 获取OpenCV delaunay训练规则******************************
    cv::Rect rect(0, 0, map_to_draw_voronoi_in.cols, map_to_draw_voronoi_in.rows);
    cv::Subdiv2D subdiv(rect);

    std::vector<std::vector<cv::Point>> hole_contours;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::Mat temp = map_to_draw_voronoi_in.clone();
    //提取所有轮廓，并且将其组织为双层结构。顶层为连通域的外围边界，次层为孔的内层边界  获取每个轮廓的每个像素，相邻两个点的像素位置相差1
    cv::findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
    cv::drawContours(map_to_draw_voronoi_in, contours, -1, cv::Scalar(255), cv::FILLED);
    for (int current_contour = 0; current_contour < contours.size(); current_contour++) {
        for (int current_point = 0; current_point < contours[current_contour].size(); current_point++) {
            cv::Point fp = contours[current_contour][current_point];
            subdiv.insert(fp);
        }
        if (hierarchy[current_contour][2] == -1 && hierarchy[current_contour][3] != -1) {
            hole_contours.push_back(contours[current_contour]);
        }
    }

    //********************2. Get facets and draw voronoi-Graph******************************
    cv::Mat eroded_map;
    cv::erode(temporary_map_to_calculate_voronoi, eroded_map, cv::Mat(), cv::Point(-1, -1), 2);

    const cv::Scalar voronoi_color(127);
    std::vector<std::vector<cv::Point2f> > voronoi_facets;
    std::vector<cv::Point2f> voronoi_centers;
    subdiv.getVoronoiFacetList(std::vector<int>(), voronoi_facets, voronoi_centers);

    drawVoronoi(map_to_draw_voronoi_in, voronoi_facets, voronoi_color, eroded_map);

    for (int v = 0; v < map_to_draw_voronoi_in.rows; v++) {
        for (int u = 0; u < map_to_draw_voronoi_in.cols; u++) {
            if (map_for_voronoi_generation.at<unsigned char>(v, u) == 0) {
                map_to_draw_voronoi_in.at<unsigned char>(v, u) = 0;
            }
        }
    }

    map_for_voronoi_generation = map_to_draw_voronoi_in;
}

void
AbstractVoronoiSegmentation::pruneVoronoiGraph(cv::Mat &voronoi_map, std::set<cv::Point, cv_Point_comp> &node_points) {
    for (int v = 1; v < voronoi_map.rows - 1; v++) {
        for (int u = 1; u < voronoi_map.cols - 1; u++) {
            if (voronoi_map.at<unsigned char>(v, u) == 127) {
                int neighbor_count = 0;
                for (int row_counter = -1; row_counter <= 1; row_counter++) {
                    for (int column_counter = -1; column_counter <= 1; column_counter++) {
                        if (row_counter == 0 && column_counter == 0)
                            continue;

                        if (voronoi_map.at<unsigned char>(v + row_counter, u + column_counter) == 127) {
                            neighbor_count++;
                        }
                    }
                }
                if (neighbor_count > 2) {
                    node_points.insert(cv::Point(u, v));
                }
            }
        }
    }

    for (int step = 0; step < 100; step++) {
        for (int v = 0; v < voronoi_map.rows; v++) {
            for (int u = 0; u < voronoi_map.cols; u++) {
                if (voronoi_map.at<unsigned char>(v, u) == 127) {
                    int neighbor_count = 0;
                    for (int row_counter = -1; row_counter <= 1; row_counter++) {
                        for (int column_counter = -1; column_counter <= 1; column_counter++) {
                            if (row_counter == 0 && column_counter == 0)
                                continue;

                            const int nv = v + row_counter;
                            const int nu = u + column_counter;
                            if (nv >= 0 && nu >= 0 && nv < voronoi_map.rows && nu < voronoi_map.cols &&
                                voronoi_map.at<unsigned char>(nv, nu) == 127) {
                                neighbor_count++;
                            }
                        }
                    }
                    if (neighbor_count <= 1 && node_points.find(cv::Point(u, v)) == node_points.end()) {
                        voronoi_map.at<unsigned char>(v, u) = 255;
                    }
                }
            }
        }
    }
}

void AbstractVoronoiSegmentation::mergeRooms(cv::Mat &map_to_merge_rooms, std::vector<Room> &rooms,
                                             double map_resolution_from_subscription, double max_area_for_merging) {
    // 1. 遍历每个像素，向具有相同ID的房间添加点
    for (int y = 0; y < map_to_merge_rooms.rows; y++) {
        for (int x = 0; x < map_to_merge_rooms.cols; x++) {
            int current_id = map_to_merge_rooms.at<int>(y, x);
            if (current_id != 0) {
                for (size_t current_room = 0; current_room < rooms.size(); current_room++) {
                    if (rooms[current_room].getID() == current_id) {
                        rooms[current_room].insertMemberPoint(cv::Point(x, y), map_resolution_from_subscription);
                        break;
                    }
                }
            }
        }
    }
    // 2. 为每个点添加邻居ID
    for (int current_room = 0; current_room < rooms.size(); current_room++) {
        const int current_id = rooms[current_room].getID();
        std::vector<cv::Point> considered_neighbors;
        const std::vector<cv::Point> &current_points = rooms[current_room].getMembers();
        for (int current_point = 0; current_point < current_points.size(); current_point++) {
            for (int row_counter = -1; row_counter <= 1; row_counter++) {
                for (int col_counter = -1; col_counter <= 1; col_counter++) {
                    if (row_counter == 0 && col_counter == 0) {
                        continue;
                    }
                    const int label = map_to_merge_rooms.at<int>(current_points[current_point].y + row_counter,
                                                                 current_points[current_point].x + col_counter);
                    if (label == current_id) {
                        continue;
                    }
                    // 收集邻居ID
                    if (label != 0)
                        rooms[current_room].addNeighborID(label);

                    // 邻域统计
                    cv::Point neighbor_point(current_points[current_point].x + col_counter,
                                             current_points[current_point].y + row_counter);
                    if (!contains(considered_neighbors, neighbor_point)) {
                        rooms[current_room].addNeighbor(label);
                        considered_neighbors.push_back(neighbor_point);
                    }
                }
            }
        }
    }
    // 3. 合并条件
    std::sort(rooms.begin(), rooms.end(), sortRoomsAscending);

    // a） 房间只有一个邻居，周围最多有75%的墙壁
    for (int current_room_index = 0; current_room_index < rooms.size();) {
        Room &current_room = rooms[current_room_index];
        bool merge_rooms = false;
        size_t merge_index = 0;

        if (current_room.getNeighborCount() == 1 &&
            current_room.getArea() < max_area_for_merging &&
            current_room.getWallToPerimeterRatio() <= 0.75) {
            merge_rooms = determineRoomIndexFromRoomID(
                    rooms, current_room.getNeighborWithLargestCommonBorder(), merge_index);
        }

        if (merge_rooms) {
            mergeRoomPair(rooms, merge_index, current_room_index, map_to_merge_rooms, map_resolution_from_subscription);
            current_room_index = 0;
        } else
            current_room_index++;
    }

    if (DEBUG_DISPLAYS_SHOW) {
        cv::imshow("merge rooms A neighbor result", map_to_merge_rooms);
        cv::waitKey();
    }

    // b) small rooms
    for (int current_room_index = 0; current_room_index < rooms.size();) {
        Room &current_room = rooms[current_room_index];
        bool merge_rooms = false;
        size_t merge_index = 0;

        const int max_border_neighbor_id = current_room.getNeighborWithLargestCommonBorder();
        if (current_room.getArea() < 2.0 &&
            (double) current_room.getNeighborStatistics()[max_border_neighbor_id] / current_room.getPerimeter() > 0.2) {
            merge_rooms = determineRoomIndexFromRoomID(rooms, max_border_neighbor_id, merge_index);
            if ((double) rooms[merge_index].getWallToPerimeterRatio() > 0.8)
                merge_rooms = false;
        }

        if (merge_rooms) {
            mergeRoomPair(rooms, merge_index, current_room_index, map_to_merge_rooms, map_resolution_from_subscription);
            current_room_index = 0;
        } else
            current_room_index++;
    }

    if (DEBUG_DISPLAYS_SHOW) {
        cv::imshow("merge rooms small result", map_to_merge_rooms);
        cv::waitKey();
    }

    // c)将一个房间与一个邻居合并，该邻居最多有两个邻居和足够的墙比（连接房间内的各个部分）
    for (int current_room_index = 0; current_room_index < rooms.size();) {
        Room &current_room = rooms[current_room_index];
        bool merge_rooms = false;
        size_t merge_index = 0;

        const int max_border_neighbor_id = current_room.getNeighborWithLargestCommonBorder();
        if ((current_room.getNeighborCount() == 1 ||
             current_room.getPerimeterRatioOfXLargestRooms(1) > 0.98) &&
            current_room.getWallToPerimeterRatio() > 0.5 &&
            (double) current_room.getNeighborStatistics()[max_border_neighbor_id] / current_room.getPerimeter() > 0.15
                ) {
            // 与共享最多相邻像素的邻居合并
            merge_rooms = determineRoomIndexFromRoomID(rooms, max_border_neighbor_id, merge_index);
            if (rooms[merge_index].getNeighborCount() > 2 &&
                rooms[merge_index].getPerimeterRatioOfXLargestRooms(2) < 0.95)
                merge_rooms = false;
        }

        if (merge_rooms) {
            mergeRoomPair(rooms, merge_index, current_room_index, map_to_merge_rooms, map_resolution_from_subscription);
            current_room_index = 0;
        } else
            current_room_index++;
    }

    if (DEBUG_DISPLAYS_SHOW) {
        cv::imshow("merge rooms pair result", map_to_merge_rooms);
        cv::waitKey();
    }

    // d) 合并共享其周长重要部分的房间
    for (int current_room_index = 0; current_room_index < rooms.size();) {
        Room &current_room = rooms[current_room_index];
        bool merge_rooms = false;
        size_t merge_index = 0;

        std::map<int, int, std::greater<int> > neighbor_room_statistics_inverse;    // common border length, room_id
        current_room.getNeighborStatisticsInverse(neighbor_room_statistics_inverse);
        for (std::map<int, int, std::greater<int> >::iterator it = neighbor_room_statistics_inverse.begin();
             it != neighbor_room_statistics_inverse.end(); ++it) {
            if (it->second == 0)
                continue;        // skip wall

            const double neighbor_border_ratio =
                    (double) current_room.getNeighborStatistics()[it->second] / current_room.getPerimeter();
            if (neighbor_border_ratio > 0.2 ||
                (neighbor_border_ratio > 0.1 &&
                 current_room.getWallToPerimeterRatio() > (1 - 2 * neighbor_border_ratio - 0.05) &&
                 current_room.getWallToPerimeterRatio() < (1 - neighbor_border_ratio))
                    ) {
                // 与共享最多相邻像素的邻居合并
                merge_rooms = determineRoomIndexFromRoomID(rooms, it->second, merge_index);
                if ((double) rooms[merge_index].getNeighborStatistics()[current_room.getID()] /
                    rooms[merge_index].getPerimeter() <= 0.1)
                    merge_rooms = false;
                if (merge_rooms)
                    break;
            }
        }

        if (merge_rooms) {
            mergeRoomPair(rooms, merge_index, current_room_index, map_to_merge_rooms, map_resolution_from_subscription);
            current_room_index = 0;
        } else
            current_room_index++;
    }

    if (DEBUG_DISPLAYS_SHOW) {
        cv::imshow("merge rooms inverse result", map_to_merge_rooms);
        cv::waitKey();
    }

    // e) 最大的房间邻居接触周长>0.5（经常发生在家具上）
    for (int current_room_index = 0; current_room_index < rooms.size();) {
        Room &current_room = rooms[current_room_index];
        bool merge_rooms = false;
        size_t merge_index = 0;

        const int max_border_neighbor_id = current_room.getNeighborWithLargestCommonBorder();
        if ((double) current_room.getNeighborStatistics()[max_border_neighbor_id] / current_room.getPerimeter() > 0.4) {
            // 与共享最多相邻像素的邻居合并
            merge_rooms = determineRoomIndexFromRoomID(rooms, max_border_neighbor_id, merge_index);
        }

        if (merge_rooms) {
            mergeRoomPair(rooms, merge_index, current_room_index, map_to_merge_rooms, map_resolution_from_subscription);
            current_room_index = 0;
        } else
            current_room_index++;
    }

    if (DEBUG_DISPLAYS_SHOW) {
        cv::imshow("merge rooms furniture result", map_to_merge_rooms);
        cv::waitKey();
    }
}

void AbstractVoronoiSegmentation::formatRooms(cv::Mat &map_to_merge_rooms, std::vector<Room> &rooms,
                                              double map_resolution_from_subscription) {
    for (auto it = rooms.begin(); it != rooms.end();) {
        if (it->getNeighborIDs().empty()) {
            rooms.erase(it++);
        } else {
            it++;
        }
    }

    standardMergeRoom(map_to_merge_rooms, rooms, map_resolution_from_subscription, 0);
}

void AbstractVoronoiSegmentation::standardMergeRoom(cv::Mat &map_to_merge_rooms, std::vector<Room> &rooms,
                                                    double map_resolution_from_subscription, double sign_area) {
    if (rooms.size() < 12)
        return;

    bool merge_rooms = false;
    size_t merge_index = 0;
    int current_room_index = 0;

    double lastArea = 1000000;
    for (int i = 0; i < rooms.size(); ++i) {
        auto room = rooms[i];

        if (room.getArea() > sign_area) {
            if (room.getArea() < lastArea) {
                lastArea = room.getArea();
                current_room_index = i;
            }
        }
    }

    auto current_room = rooms[current_room_index];
    auto neighborStatistics = current_room.getNeighborStatistics();
    auto neighborIDs = current_room.getNeighborIDs();
    if (!neighborIDs.empty()) {
        const int max_border_neighbor_id = current_room.getNeighborWithLargestCommonBorder();

        merge_rooms = determineRoomIndexFromRoomID(rooms, max_border_neighbor_id, merge_index);
    }

    if (merge_rooms) {
        mergeRoomPair(rooms, merge_index, current_room_index, map_to_merge_rooms, map_resolution_from_subscription);

        standardMergeRoom(map_to_merge_rooms, rooms, map_resolution_from_subscription, lastArea);
    } else {
        standardMergeRoom(map_to_merge_rooms, rooms, map_resolution_from_subscription, lastArea);
    }

}
