//
// Created by Looper on 2022/9/15.
//

#include "segmentation/VoronoiSegmentation.h"
#include "segmentation/contains.h"
#include "segmentation/wavefront_region_growing.h"

VoronoiSegmentation::VoronoiSegmentation() {

}

void VoronoiSegmentation::segmentMap(const cv::Mat &map_to_be_labeled, cv::Mat &segmented_map, std::vector<Room> &rooms,
                                     double map_resolution_from_subscription, double room_area_factor_lower_limit,
                                     double room_area_factor_upper_limit, int neighborhood_index, int max_iterations,
                                     double min_critical_point_distance_factor, double max_area_for_merging) {

    //*********************I. Calculate and draw the Voronoi-Diagram in the given map*****************
    cv::Mat voronoi_map = map_to_be_labeled.clone();
    createVoronoiGraph(voronoi_map);

    //***************************II. extract the possible candidates for critical Points****************************
    std::set<cv::Point, cv_Point_comp> node_points;
    pruneVoronoiGraph(voronoi_map, node_points);

    cv::Mat distance_map;
    cv::distanceTransform(map_to_be_labeled, distance_map, CV_DIST_L2, 5);

    cv::convertScaleAbs(distance_map, distance_map);

    std::vector<cv::Point> critical_points;
    for (int v = 0; v < voronoi_map.rows; v++) {
        for (int u = 0; u < voronoi_map.cols; u++) {
            if (voronoi_map.at<unsigned char>(v, u) == 127) {
                int eps = neighborhood_index / (int) distance_map.at<unsigned char>(v, u); //310
                int loopcounter = 0;
                std::vector<cv::Point> temporary_points;
                std::set<cv::Point, cv_Point_comp> neighbor_points;
                int neighbor_count = 0;
                neighbor_points.insert(cv::Point(u, v));
                do {
                    loopcounter++;
                    for (std::set<cv::Point, cv_Point_comp>::iterator it_neighbor_points = neighbor_points.begin();
                         it_neighbor_points != neighbor_points.end(); it_neighbor_points++) {
                        for (int row_counter = -1; row_counter <= 1; row_counter++) {
                            for (int column_counter = -1; column_counter <= 1; column_counter++) {
                                if (row_counter == 0 && column_counter == 0)
                                    continue;

                                const cv::Point &current_neighbor_point = *it_neighbor_points;
                                const int nu = current_neighbor_point.x + column_counter;
                                const int nv = current_neighbor_point.y + row_counter;
                                if (nv >= 0 && nu >= 0 && nv < voronoi_map.rows && nu < voronoi_map.cols &&
                                    voronoi_map.at<unsigned char>(nv, nu) == 127 &&
                                    neighbor_points.find(cv::Point(nu, nv)) == neighbor_points.end()) {
                                    neighbor_count++;
                                    temporary_points.push_back(cv::Point(nu, nv));
                                }
                            }
                        }
                    }
                    for (int temporary_point_index = 0;
                         temporary_point_index < temporary_points.size(); temporary_point_index++) {
                        neighbor_points.insert(temporary_points[temporary_point_index]);
                        voronoi_map.at<unsigned char>(temporary_points[temporary_point_index].y,
                                                      temporary_points[temporary_point_index].x) = 255;
                        voronoi_map.at<unsigned char>(v, u) = 255;
                    }
                } while (neighbor_count <= eps && loopcounter < max_iterations);
                cv::Point current_critical_point = cv::Point(u, v);
                for (std::set<cv::Point, cv_Point_comp>::iterator it_neighbor_points = neighbor_points.begin();
                     it_neighbor_points != neighbor_points.end(); it_neighbor_points++) {
                    if (distance_map.at<unsigned char>(it_neighbor_points->y, it_neighbor_points->x) <
                        distance_map.at<unsigned char>(current_critical_point.y, current_critical_point.x)) {
                        current_critical_point = cv::Point(*it_neighbor_points);
                    }
                }
                critical_points.push_back(current_critical_point);
            }
        }
    }

    //*************III. draw the critical lines from every found critical Point to its two closest zero-pixel****************
    map_to_be_labeled.convertTo(segmented_map, CV_32SC1, 256, 0); // rescale to 32 int, 255 --> 255*256 = 65280

    // 1. Get the points of the contour, which are the possible closest points for a critical point
    //clone the map to extract the contours, because after using OpenCV find-/drawContours
    //the map will be different from the original one
    cv::Mat temporary_map_to_extract_the_contours = segmented_map.clone();
    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(temporary_map_to_extract_the_contours, contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);

    // 2. Get the basis-points for each critical-point
    std::vector<cv::Point> basis_points_1, basis_points_2;
    std::vector<double> length_of_critical_line;
    std::vector<double> angles; //每个临界点的基线之间的角度
    for (int critical_point_index = 0; critical_point_index < critical_points.size(); critical_point_index++) {
        //设置基点的初始点和值，以便可以进行距离比较
        cv::Point basis_point_1 = contours[0][0];
        cv::Point basis_point_2 = contours[0][1];
        //从当前临界点到轮廓点的第一个向量的初始值及其距离
        const cv::Point &critical_point = critical_points[critical_point_index];
        double vector_x_1 = critical_point.x - contours[0][0].x;
        double vector_y_1 = critical_point.y - contours[0][0].y;
        double distance_basis_1 = std::sqrt(vector_x_1 * vector_x_1 + vector_y_1 * vector_y_1);
        //从当前临界点到轮廓点的第二个向量的初始值及其距离
        double vector_x_2 = critical_point.x - contours[0][1].x;
        double vector_y_2 = critical_point.y - contours[0][1].y;
        double distance_basis_2 = std::sqrt(vector_x_2 * vector_x_2 + vector_y_2 * vector_y_2);

        //查找第一个基点
        int basis_vector_1_x, basis_vector_2_x, basis_vector_1_y, basis_vector_2_y;
        for (int c = 0; c < contours.size(); c++) {
            for (int p = 0; p < contours[c].size(); p++) {
                //计算从临界点到轮廓上点的欧几里得距离
                const double vector_x = contours[c][p].x - critical_point.x;
                const double vector_y = contours[c][p].y - critical_point.y;
                const double current_distance = std::sqrt(vector_x * vector_x + vector_y * vector_y);
                //如果距离较小，请将其与保存的距离进行比较
                if (current_distance < distance_basis_1) {
                    distance_basis_1 = current_distance;
                    basis_point_1 = contours[c][p];
                    basis_vector_1_x = vector_x;
                    basis_vector_1_y = vector_y;
                }
            }
        }
        //查找第二个基本点
        for (int c = 0; c < contours.size(); c++) {
            for (int p = 0; p < contours[c].size(); p++) {
                //计算从临界点到轮廓上点的欧几里得距离
                const double vector_x = contours[c][p].x - critical_point.x;
                const double vector_y = contours[c][p].y - critical_point.y;
                const double current_distance = std::sqrt(vector_x * vector_x + vector_y * vector_y);
                //计算当前轮廓点和第一个基点之间的距离，以确保它们彼此不太接近
                const double vector_x_basis = basis_point_1.x - contours[c][p].x;
                const double vector_y_basis = basis_point_1.y - contours[c][p].y;
                const double basis_distance = std::sqrt(
                        vector_x_basis * vector_x_basis + vector_y_basis * vector_y_basis);
                if (current_distance > distance_basis_1 && current_distance < distance_basis_2 &&
                    basis_distance > (double) distance_map.at<unsigned char>(critical_point.y, critical_point.x)) {
                    distance_basis_2 = current_distance;
                    basis_point_2 = contours[c][p];
                    basis_vector_2_x = vector_x;
                    basis_vector_2_y = vector_y;
                }
            }
        }
        double current_angle = std::acos((basis_vector_1_x * basis_vector_2_x + basis_vector_1_y * basis_vector_2_y) /
                                         (distance_basis_1 * distance_basis_2)) * 180.0 / PI;

        //保存关键线及其计算值
        basis_points_1.push_back(basis_point_1);
        basis_points_2.push_back(basis_point_2);
        length_of_critical_line.push_back(distance_basis_1 + distance_basis_2);
        angles.push_back(current_angle);
    }

    //3. Check which critical points should be used for the segmentation. This is done by checking the points that are
    //   in a specified distance to each other and take the point with the largest calculated angle, because larger angles
    //   correspond to a separation across the room, which is more useful
    for (int first_critical_point = 0; first_critical_point < critical_points.size(); first_critical_point++) {
        bool draw = true;
        for (int second_critical_point = 0; second_critical_point < critical_points.size(); second_critical_point++) {
            if (second_critical_point != first_critical_point) {
                const double vector_x =
                        critical_points[second_critical_point].x - critical_points[first_critical_point].x;
                const double vector_y =
                        critical_points[second_critical_point].y - critical_points[first_critical_point].y;
                const double critical_point_distance = std::sqrt(vector_x * vector_x + vector_y * vector_y);
                if (critical_point_distance < ((int) distance_map.at<unsigned char>(
                        critical_points[first_critical_point].y, critical_points[first_critical_point].x) *
                                               min_critical_point_distance_factor)) {
                    if (angles[first_critical_point] < angles[second_critical_point]) {
                        draw = false;
                    }
                    if (angles[first_critical_point] == angles[second_critical_point] &&
                        length_of_critical_line[first_critical_point] >
                        length_of_critical_line[second_critical_point] &&
                        (length_of_critical_line[second_critical_point] > 3 ||
                         first_critical_point > second_critical_point)) {
                        draw = false;
                    }
                }
            }
        }

        //4. draw critical-lines if angle of point is larger than the other
        if (draw) {
            cv::line(voronoi_map, critical_points[first_critical_point], basis_points_1[first_critical_point],
                     cv::Scalar(0), 2);
            cv::line(voronoi_map, critical_points[first_critical_point], basis_points_2[first_critical_point],
                     cv::Scalar(0), 2);
        }
    }

    //***********************Find the Contours seperated from the critcal lines and fill them with color******************
    std::vector<cv::Scalar> already_used_colors;
    std::vector<cv::Vec4i> hierarchy;

    //1. Erode map one time, so small gaps are closed
    cv::findContours(voronoi_map, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
    for (int current_contour = 0; current_contour < contours.size(); current_contour++) {
        if (hierarchy[current_contour][3] == -1) {
            double room_area = map_resolution_from_subscription * map_resolution_from_subscription *
                               cv::contourArea(contours[current_contour]);
            if (room_area >= room_area_factor_lower_limit && room_area <= room_area_factor_upper_limit) {
                //2. Draw the region with a random color into the map if it is large/small enough
                bool drawn = false;
                int loop_counter = 0;
                do {
                    loop_counter++;
                    int random_number = rand() % 52224 + 13056;
                    cv::Scalar fill_colour(random_number);
                    if (!contains(already_used_colors, fill_colour) || loop_counter > 1000) {
                        cv::drawContours(segmented_map, contours, current_contour, fill_colour, 1);
                        already_used_colors.push_back(fill_colour);
                        Room current_room(random_number);
                        for (int point = 0; point < contours[current_contour].size(); point++) {
                            current_room.insertMemberPoint(cv::Point(contours[current_contour][point]),
                                                           map_resolution_from_subscription);
                        }
                        rooms.push_back(current_room);
                        drawn = true;
                    }
                } while (!drawn);
            }
        }
    }

    wavefrontRegionGrowing(segmented_map);

    //3.fill the last white areas with the surrounding color
    mergeRooms(segmented_map, rooms, map_resolution_from_subscription, max_area_for_merging);


    //4.Format and merge to prevent problems such as too many rooms after map segmentation
    formatRooms(segmented_map, rooms, map_resolution_from_subscription);

}
