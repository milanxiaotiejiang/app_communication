//
// Created by Looper on 2022/9/15.
//

#include "segmentation/handle_segmentation_display.h"
#include "model/Point.h"
#include "segmentation/map_attribute.h"
#include <cv_bridge/cv_bridge.h>

void calculation_center_point(cv::Mat &segmented_map, std::vector<Room> &rooms,
                              double robot_radius, double map_resolution) {

    // get the min/max-values and the room-centers
    // compute room label codebook
    std::map<int, size_t> label_vector_index_codebook; // maps each room label to a position in the rooms vector
    size_t vector_index = 0;
    for (int v = 0; v < segmented_map.rows; ++v) {
        for (int u = 0; u < segmented_map.cols; ++u) {
            const int label = segmented_map.at<int>(v, u);
            if (label > 0 && label < 65280) {// do not count walls/obstacles or free space as label
                if (label_vector_index_codebook.find(label) == label_vector_index_codebook.end()) {
                    label_vector_index_codebook[label] = vector_index;
                    vector_index++;
                }
            }
        }
    }
    //min/max y/x-values vector for each room. Initialized with extreme values
    std::vector<int> min_x_value_of_the_room(label_vector_index_codebook.size(), 100000000);
    std::vector<int> max_x_value_of_the_room(label_vector_index_codebook.size(), 0);
    std::vector<int> min_y_value_of_the_room(label_vector_index_codebook.size(), 100000000);
    std::vector<int> max_y_value_of_the_room(label_vector_index_codebook.size(), 0);
    //vector of the central Point for each room, initially filled with Points out of the map
    std::vector<int> room_centers_x_values(label_vector_index_codebook.size(), -1);
    std::vector<int> room_centers_y_values(label_vector_index_codebook.size(), -1);
    //***********************Find min/max x and y coordinate and center of each found room********************
    //check y/x-value for every Pixel and make the larger/smaller value to the current value of the room
    for (int y = 0; y < segmented_map.rows; ++y) {
        for (int x = 0; x < segmented_map.cols; ++x) {
            const int label = segmented_map.at<int>(y, x);
            if (label > 0 && label < 65280) //if Pixel is white or black it is no room --> doesn't need to be checked
            {
                const int index = label_vector_index_codebook[label];
                min_x_value_of_the_room[index] = std::min(x, min_x_value_of_the_room[index]);
                max_x_value_of_the_room[index] = std::max(x, max_x_value_of_the_room[index]);
                max_y_value_of_the_room[index] = std::max(y, max_y_value_of_the_room[index]);
                min_y_value_of_the_room[index] = std::min(y, min_y_value_of_the_room[index]);
            }
        }
    }
    //get centers for each room
//	for (size_t idx = 0; idx < room_centers_x_values.size(); ++idx)
//	{
//		if (max_x_value_of_the_room[idx] != 0 && max_y_value_of_the_room[idx] != 0 && min_x_value_of_the_room[idx] != 100000000 && min_y_value_of_the_room[idx] != 100000000)
//		{
//			room_centers_x_values[idx] = (min_x_value_of_the_room[idx] + max_x_value_of_the_room[idx]) / 2;
//			room_centers_y_values[idx] = (min_y_value_of_the_room[idx] + max_y_value_of_the_room[idx]) / 2;
//			cv::circle(segmented_map, cv::Point(room_centers_x_values[idx], room_centers_y_values[idx]), 2, cv::Scalar(200*256), CV_FILLED);
//		}
//	}
    // use distance transform and mean shift to find good room centers that are reachable by the robot
    // first check whether a robot radius shall be applied to obstacles in order to exclude room center points that are not reachable by the robot
    cv::Mat segmented_map_copy = segmented_map;
    // stores for each pixel whether a path to another rooms exists for a robot of size robot_radius
    cv::Mat connection_to_other_rooms = cv::Mat::zeros(segmented_map.rows, segmented_map.cols, CV_8UC1);
    if (robot_radius > 0.0) {
        // consider robot radius for exclusion of non-reachable points
        segmented_map_copy = segmented_map.clone();
        cv::Mat map_8u, eroded_map;
        segmented_map_copy.convertTo(map_8u, CV_8UC1, 1., 0.);
        int number_of_erosions = (robot_radius / map_resolution);
        cv::erode(map_8u, eroded_map, cv::Mat(), cv::Point(-1, -1), number_of_erosions);
        for (int v = 0; v < segmented_map_copy.rows; ++v)
            for (int u = 0; u < segmented_map_copy.cols; ++u)
                if (eroded_map.at<uchar>(v, u) == 0)
                    segmented_map_copy.at<int>(v, u) = 0;

        // compute connectivity of remaining accessible room cells to other rooms
        bool stop = false;
        while (!stop) {
            stop = true;
            for (int v = 1; v < segmented_map_copy.rows - 1; ++v) {
                for (int u = 1; u < segmented_map_copy.cols - 1; ++u) {
                    // skip already identified cells
                    if (connection_to_other_rooms.at<uchar>(v, u) != 0)
                        continue;

                    // only consider cells labeled as a room
                    const int label = segmented_map_copy.at<int>(v, u);
                    if (label <= 0 || label >= 65280)
                        continue;

                    for (int dv = -1; dv <= 1; ++dv) {
                        for (int du = -1; du <= 1; ++du) {
                            if (dv == 0 && du == 0)
                                continue;
                            const int neighbor_label = segmented_map_copy.at<int>(v + dv, u + du);
                            if (neighbor_label > 0 && neighbor_label < 65280 && (neighbor_label != label ||
                                                                                 (neighbor_label == label &&
                                                                                  connection_to_other_rooms.at<uchar>(
                                                                                          v + dv, u + du) == 255))) {
                                // either the room cell has a direct border to a different room or the room cell has a neighbor from the same room label with a connecting path to another room
                                connection_to_other_rooms.at<uchar>(v, u) = 255;
                                stop = false;
                            }
                        }
                    }
                }
            }
        }
    }
    // compute the room centers
    MeanShift2D ms;
    for (std::map<int, size_t>::iterator it = label_vector_index_codebook.begin();
         it != label_vector_index_codebook.end(); ++it) {
        int trial = 1;    // use robot_radius to avoid room centers that are not accessible by a robot with a given radius
        if (robot_radius <= 0.)
            trial = 2;

        for (; trial <= 2; ++trial) {
            // compute distance transform for each room on the room cells that have some connection to another room (trial 1) or just on all cells of that room (trial 2)
            const int label = it->first;
            int number_room_pixels = 0;
            cv::Mat room = cv::Mat::zeros(segmented_map_copy.rows, segmented_map_copy.cols, CV_8UC1);
            for (int v = 0; v < segmented_map_copy.rows; ++v)
                for (int u = 0; u < segmented_map_copy.cols; ++u)
                    if (segmented_map_copy.at<int>(v, u) == label &&
                        (trial == 2 || connection_to_other_rooms.at<uchar>(v, u) == 255)) {
                        room.at<uchar>(v, u) = 255;
                        ++number_room_pixels;
                    }
            if (number_room_pixels == 0)
                continue;
            cv::Mat distance_map; //variable for the distance-transformed map, type: CV_32FC1
            cv::distanceTransform(room, distance_map, CV_DIST_L2, 5);
            // find point set with largest distance to obstacles
            double min_val = 0., max_val = 0.;
            cv::minMaxLoc(distance_map, &min_val, &max_val);
            std::vector<cv::Vec2d> room_cells;
            for (int v = 0; v < distance_map.rows; ++v)
                for (int u = 0; u < distance_map.cols; ++u)
                    if (distance_map.at<float>(v, u) > max_val * 0.95f)
                        room_cells.push_back(cv::Vec2d(u, v));
            if (room_cells.size() == 0)
                continue;
            // use meanshift to find the modes in that set
            cv::Vec2d room_center = ms.findRoomCenter(room, room_cells, map_resolution);
            const int index = it->second;
            room_centers_x_values[index] = room_center[0];
            room_centers_y_values[index] = room_center[1];

            if (room_cells.size() > 0)
                break;
        }
    }

    // convert the segmented map into an indexed map which labels the segments with consecutive numbers (instead of arbitrary unordered labels in segmented map)
    cv::Mat indexed_map = segmented_map.clone();
    for (int y = 0; y < segmented_map.rows; ++y) {
        for (int x = 0; x < segmented_map.cols; ++x) {
            const int label = segmented_map.at<int>(y, x);
            if (label > 0 && label < 65280)
                indexed_map.at<int>(y, x) =
                        label_vector_index_codebook[label] + 1;//start value from 1 --> 0 is reserved for obstacles
        }
    }

    // colorize the segmented map with the indices of the room_center vector
    cv::Mat color_segmented_map = indexed_map.clone();
    color_segmented_map.convertTo(color_segmented_map, CV_8U);
    cv::cvtColor(color_segmented_map, color_segmented_map, CV_GRAY2BGR);
    for (size_t i = 1; i <= room_centers_x_values.size(); ++i) {
        //choose random color for each room
        const cv::Vec3b color((rand() % 250) + 1, (rand() % 250) + 1, (rand() % 250) + 1);
        for (size_t v = 0; v < indexed_map.rows; ++v)
            for (size_t u = 0; u < indexed_map.cols; ++u)
                if (indexed_map.at<int>(v, u) == i)
                    color_segmented_map.at<cv::Vec3b>(v, u) = color;
    }
//		cv::Mat disp = segmented_map.clone();
    for (size_t index = 0; index < room_centers_x_values.size(); ++index)
        cv::circle(color_segmented_map, cv::Point(room_centers_x_values[index], room_centers_y_values[index]), 2,
                   cv::Scalar(256), CV_FILLED);


//    room_information[i].room_center.x = room_centers_x_values[i];
//    room_information[i].room_center.y = room_centers_y_values[i];
//    std::vector<int> room_centers_x_values(label_vector_index_codebook.size(), -1);
//    std::vector<int> room_centers_y_values(label_vector_index_codebook.size(), -1);
}

void single_display(const cv::Mat &roomMat, Room &room, const std::string &winname) {
    cv::Mat showMat = cv::Mat::zeros(roomMat.rows, roomMat.cols, CV_8UC3);
    int blue = (rand() % 250) + 2;
    int green = (rand() % 250) + 2;
    int red = (rand() % 250) + 2;
    auto members = room.getMembers();
    for (const auto &item: members) {
        showMat.at<cv::Vec3b>(item)[0] = blue;
        showMat.at<cv::Vec3b>(item)[1] = green;
        showMat.at<cv::Vec3b>(item)[2] = red;
    }
    cv::imshow(winname, showMat);
    cv::waitKey();
}

void ordinary_display(const cv::Mat &segmented_map, const std::vector<Room> &rooms, const std::string &winname) {

    cv::Mat colour_segmented_map = segmented_map.clone();
    colour_segmented_map.convertTo(colour_segmented_map, CV_8UC3);
    cv::cvtColor(colour_segmented_map, colour_segmented_map, CV_GRAY2BGR);
    for (const auto &item: rooms) {
        //choose random color for each room
        int blue = (rand() % 250) + 1;
        int green = (rand() % 250) + 1;
        int red = (rand() % 250) + 1;
        for (size_t u = 0; u < segmented_map.rows; ++u) {
            for (size_t v = 0; v < segmented_map.cols; ++v) {
                if (segmented_map.at<int>(u, v) == item.getID()) {
                    colour_segmented_map.at<cv::Vec3b>(u, v)[0] = blue;
                    colour_segmented_map.at<cv::Vec3b>(u, v)[1] = green;
                    colour_segmented_map.at<cv::Vec3b>(u, v)[2] = red;
                }
            }
        }
    }
    //draw the room centers into the map
    for (size_t i = 0; i < rooms.size(); ++i) {
        auto room = rooms[i];
        cv::Point current_center(room.getCenter().x,
                                 room.getCenter().y);
        cv::circle(colour_segmented_map, current_center, 2, CV_RGB(0, 0, 255), CV_FILLED);
    }

    cv::imshow(winname, colour_segmented_map);
    cv::waitKey();
}


void whole_display(const cv::Mat &segmented_map, const std::vector<Room> &rooms, const std::string &winname) {
    cv::Mat roomMat = cv::Mat::zeros(segmented_map.rows, segmented_map.cols, CV_8UC3);
    for (int i = 0; i < rooms.size(); ++i) {
        int blue = (rand() % 250) + 2;
        int green = (rand() % 250) + 2;
        int red = (rand() % 250) + 2;

        auto current_room = rooms[i];
        std::map<int, int, std::greater<int> > neighbor_room_statistics_inverse;    // common border length, room_id
        current_room.getNeighborStatisticsInverse(neighbor_room_statistics_inverse);
        auto statistics = current_room.getNeighborStatistics();

        auto members = current_room.getMembers();
        for (const auto &item: members) {
            roomMat.at<cv::Vec3b>(item)[0] = blue;
            roomMat.at<cv::Vec3b>(item)[1] = green;
            roomMat.at<cv::Vec3b>(item)[2] = red;
        }

        auto center = current_room.getCenter();
        auto neighborIDs = current_room.getNeighborIDs();
        int id = current_room.getID();

        std::string idStr;
        std::string neighborStr;
        cv::Point point1(center.x - 10, center.y - 10);
        cv::Point point2(center.x - 10, center.y + 10);

        idStr.append(std::to_string(i) + "." + std::to_string(id));
        cv::putText(roomMat, idStr, point1, cv::FONT_HERSHEY_TRIPLEX, 0.4,
                    cv::Scalar(255, 200, 200), 1, CV_AA);

        for (const auto &item: neighborIDs) {
            neighborStr.append(std::to_string(item) + ",");
        }
        cv::putText(roomMat, neighborStr, point2, cv::FONT_HERSHEY_TRIPLEX, 0.3,
                    cv::Scalar(255, 200, 200), 1, CV_AA);
    }
    cv::imshow(winname, roomMat);
    cv::waitKey();

//    auto cloneMat = roomMat.clone();
//    cv::resize(cloneMat, cloneMat, cv::Size(), 0.5, 0.5, cv::INTER_LINEAR);
//    cv::imshow(winname, cloneMat);
//    cv::waitKey();
}

void whole_display(const cv::Mat &segmented_map, const std::vector<Room> &rooms, cv::Point pl, cv::Point pr,
                   const std::string &winname) {
    cv::Mat roomMat = cv::Mat::zeros(segmented_map.rows, segmented_map.cols, CV_8UC3);
    for (int i = 0; i < rooms.size(); ++i) {
        int blue = (rand() % 250) + 2;
        int green = (rand() % 250) + 2;
        int red = (rand() % 250) + 2;

        auto current_room = rooms[i];
        std::map<int, int, std::greater<int> > neighbor_room_statistics_inverse;    // common border length, room_id
        current_room.getNeighborStatisticsInverse(neighbor_room_statistics_inverse);
        auto statistics = current_room.getNeighborStatistics();

        auto members = current_room.getMembers();
        for (const auto &item: members) {
            roomMat.at<cv::Vec3b>(item)[0] = blue;
            roomMat.at<cv::Vec3b>(item)[1] = green;
            roomMat.at<cv::Vec3b>(item)[2] = red;
        }

        auto center = current_room.getCenter();
        auto neighborIDs = current_room.getNeighborIDs();
        int id = current_room.getID();

        std::string idStr;
        std::string neighborStr;
        cv::Point point1(center.x - 10, center.y - 10);
        cv::Point point2(center.x - 10, center.y + 10);

        idStr.append(std::to_string(i) + "." + std::to_string(id));
        cv::putText(roomMat, idStr, point1, cv::FONT_HERSHEY_TRIPLEX, 0.4,
                    cv::Scalar(255, 200, 200), 1, CV_AA);

        for (const auto &item: neighborIDs) {
            neighborStr.append(std::to_string(item) + ",");
        }
        cv::putText(roomMat, neighborStr, point2, cv::FONT_HERSHEY_TRIPLEX, 0.3,
                    cv::Scalar(255, 200, 200), 1, CV_AA);
    }

    cv::circle(roomMat, pl, 5, cv::Scalar(0, 200, 0), CV_FILLED);
    cv::circle(roomMat, pr, 5, cv::Scalar(200, 0, 0), CV_FILLED);

    cv::imshow(winname, roomMat);
    cv::waitKey();

//    auto cloneMat = roomMat.clone();
//    cv::resize(cloneMat, cloneMat, cv::Size(), 0.5, 0.5, cv::INTER_LINEAR);
//    cv::imshow(winname, cloneMat);
//    cv::waitKey();
}

void whole_display(const cv::Mat &segmented_map, std::vector<Room> &rooms, const std::vector<Gate> &gateList,
                   std::map<std::pair<int, int>, std::pair<Gate, bool>> planMap, float resize,
                   const std::string &winname) {

    cv::Mat roomMat = cv::Mat::zeros(segmented_map.rows, segmented_map.cols, CV_8UC3);
    for (auto &room: rooms) {

        int blue = (rand() % 250) + 2;
        int green = (rand() % 250) + 2;
        int red = (rand() % 250) + 2;

        std::vector<cv::Point> members = room.getMembers();
        for (const auto &member: members) {
//                roomMat.at<int>(member) = room.getID();
            roomMat.at<cv::Vec3b>(member)[0] = blue;
            roomMat.at<cv::Vec3b>(member)[1] = green;
            roomMat.at<cv::Vec3b>(member)[2] = red;
        }
    }

    cv::resize(roomMat, roomMat, cv::Size(), resize, resize, cv::INTER_LINEAR);
    for (auto &room: rooms) {
        auto center = room.getCenter();
        cv::putText(roomMat, "   " + std::to_string(room.getID()), center * resize, cv::FONT_HERSHEY_TRIPLEX, 0.4,
                    cv::Scalar(255, 200, 200), 1, CV_AA);

        cv::circle(roomMat, center * resize, 4, cv::Scalar(255, 200, 200), CV_FILLED);
    }

    for (const auto &gate: gateList) {

        int blue = (rand() % 250) + 2;
        int green = (rand() % 250) + 2;
        int red = (rand() % 250) + 2;
        auto scalar = cv::Scalar(blue, green, red);

        cv::Point ps(gate.start_x, gate.start_y);
        cv::Point pe(gate.end_x, gate.end_y);

        cv::line(roomMat, ps * resize, pe * resize, scalar, 4, cv::LINE_8);

        Point gateLeftPoint(gate.left_position_x, gate.left_position_y);
        Point gateRightPoint(gate.right_position_x, gate.right_position_y);

        auto cvGateLeftPoint = MapAttributeSingleton::instance().rosPoint2MapPoint(segmented_map.rows,
                                                                                   segmented_map.cols,
                                                                                   gateLeftPoint);
        auto cvGateRightPoint = MapAttributeSingleton::instance().rosPoint2MapPoint(segmented_map.rows,
                                                                                    segmented_map.cols,
                                                                                    gateRightPoint);

        cv::circle(roomMat, cvGateLeftPoint * resize, 4, scalar, CV_FILLED);
        cv::putText(roomMat, " L", cvGateLeftPoint * resize, cv::FONT_HERSHEY_TRIPLEX, 0.4,
                    cv::Scalar(255, 200, 200), 1, CV_AA);

        cv::circle(roomMat, cvGateRightPoint * resize, 2, scalar, CV_FILLED);
        cv::putText(roomMat, " R", cvGateRightPoint * resize, cv::FONT_HERSHEY_TRIPLEX, 0.4,
                    cv::Scalar(255, 200, 200), 1, CV_AA);

        cv::Point midPoint((ps.x + pe.x) / 2, (ps.y + pe.y) / 2);
        cv::putText(roomMat, std::to_string(gate.id), midPoint * resize, cv::FONT_HERSHEY_TRIPLEX, 0.4,
                    cv::Scalar(255, 200, 200), 1, CV_AA);

    }
    cv::imshow(winname, roomMat);
    cv::waitKey();

    for (const auto &plan: planMap) {

        auto directionMat = roomMat.clone();

        std::pair<int, int> region = plan.first;
        std::pair<Gate, bool> gatePoint = plan.second;

        int originRegionId = region.first;
        int finishRegionId = region.second;

        Room roomStart(0);
        Room roomEnd(0);
        for (const auto &room: rooms) {
            if (room.getID() == originRegionId) {
                roomStart = room;
            } else if (room.getID() == finishRegionId) {
                roomEnd = room;
            }
        }

        if (roomStart.getID() == 0 || roomEnd.getID() == 0) {
            throw std::runtime_error("no find room .");
        }


        Gate &gate = gatePoint.first;
        bool direction = gatePoint.second;

        cv::putText(directionMat, std::to_string(originRegionId) + " -> " + std::to_string(finishRegionId),
                    cv::Point(0, 50), cv::FONT_HERSHEY_TRIPLEX, 0.4, cv::Scalar(255, 200, 200), 1, CV_AA);

        Point gateLeftPoint(gate.left_position_x, gate.left_position_y);
        Point gateRightPoint(gate.right_position_x, gate.right_position_y);

        auto cvGateLeftPoint = MapAttributeSingleton::instance().rosPoint2MapPoint(segmented_map.rows,
                                                                                   segmented_map.cols,
                                                                                   gateLeftPoint);
        auto cvGateRightPoint = MapAttributeSingleton::instance().rosPoint2MapPoint(segmented_map.rows,
                                                                                    segmented_map.cols,
                                                                                    gateRightPoint);

        cv::Point directionStart;
        cv::Point directionEnd;
        if (direction) {
            directionStart = cvGateLeftPoint;
            directionEnd = cvGateRightPoint;
        } else {
            directionStart = cvGateRightPoint;
            directionEnd = cvGateLeftPoint;
        }

        cv::line(directionMat, roomStart.getCenter() * resize, directionStart * resize,
                 cv::Scalar(0, 0, 0), 1, cv::LINE_8);
        cv::line(directionMat, directionStart * resize, directionEnd * resize,
                 cv::Scalar(0, 0, 0), 1, cv::LINE_8);
        cv::line(directionMat, directionEnd * resize, roomEnd.getCenter() * resize,
                 cv::Scalar(0, 0, 0), 1, cv::LINE_8);

        cv::putText(directionMat, " S",
                    roomStart.getCenter() * resize, cv::FONT_HERSHEY_TRIPLEX, 0.4,
                    cv::Scalar(255, 200, 200), 1, CV_AA);
        cv::putText(directionMat, " E",
                    roomEnd.getCenter() * resize, cv::FONT_HERSHEY_TRIPLEX, 0.4,
                    cv::Scalar(255, 200, 200), 1, CV_AA);

        cv::imshow(std::to_string(originRegionId) + " -> " + std::to_string(finishRegionId), directionMat);
        cv::waitKey();
    }
}
