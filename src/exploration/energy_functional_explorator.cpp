//
// Created by Looper on 2022/10/19.
//

#include "exploration/energy_functional_explorator.h"
#include "exploration/room_rotator.h"
#include "glog/logging.h"
#include "exploration/grid.h"

static bool DISPLAY_TRAJECTORY = false;

EnergyFunctionalExplorator::EnergyFunctionalExplorator() {

}

double EnergyFunctionalExplorator::E(const EnergyExploratorNode &location,
                                     const EnergyExploratorNode &neighbor,
                                     const double cell_size_in_pixel,
                                     const double previous_travel_angle) {
    float energy_functional = 0.0;

    cv::Point diff = neighbor.center_ - location.center_;
    energy_functional += cv::norm(diff) / cell_size_in_pixel;

    const double travel_angle_to_node = std::atan2(diff.y, diff.x);
    double diff_angle = travel_angle_to_node - previous_travel_angle;
    while (diff_angle < -PI)
        diff_angle += 2 * PI;
    while (diff_angle > PI)
        diff_angle -= 2 * PI;
    energy_functional += std::abs(diff_angle) * PI_2_INV;

    int visited_neighbors = 0;
    for (std::vector<EnergyExploratorNode *>::const_iterator n = neighbor.neighbors_.begin();
         n != neighbor.neighbors_.end(); ++n)
        if ((*n)->obstacle_ == false && (*n)->visited_ == true)
            ++visited_neighbors;
    energy_functional += 4. - 0.5 * visited_neighbors;

    int wall_points = 0;
    for (std::vector<EnergyExploratorNode *>::const_iterator n = neighbor.neighbors_.begin();
         n != neighbor.neighbors_.end(); ++n)
        if ((*n)->obstacle_ == true)
            ++wall_points;
    energy_functional += 0.72 - 0.09 * wall_points;

    return energy_functional;
}

void
EnergyFunctionalExplorator::getExplorationPath(const cv::Mat &room_map, std::vector<geometry_msgs::Pose2D> &pose_path,
                                               const float map_resolution, const cv::Point starting_position,
                                               const cv::Point2d map_origin, const double grid_spacing_in_pixel) {

    LOG(INFO) << "Planning the boustrophedon path trough the room.";

    const int grid_spacing_as_int = (int) std::floor(grid_spacing_in_pixel);
    const int half_grid_spacing_as_int = (int) std::floor(0.5 * grid_spacing_in_pixel);

    cv::Mat R;
    cv::Rect bbox;
    cv::Mat rotated_room_map;
    RoomRotator room_rotation;
    room_rotation.computeRoomRotationMatrix(room_map, R, bbox, map_resolution);
    room_rotation.rotateRoom(room_map, rotated_room_map, R, bbox);

    if (DISPLAY_TRAJECTORY) {
        cv::imshow("rotated_room_map", rotated_room_map);
        cv::waitKey();
    }

    cv::Point min_room(1000000, 1000000), max_room(0, 0);
    for (int v = 0; v < rotated_room_map.rows; ++v) {
        for (int u = 0; u < rotated_room_map.cols; ++u) {
            if (rotated_room_map.at<uchar>(v, u) == 255) {
                min_room.x = std::min(min_room.x, u);
                min_room.y = std::min(min_room.y, v);
                max_room.x = std::max(max_room.x, u);
                max_room.y = std::max(max_room.y, v);
            }
        }
    }

    cv::Mat inflated_rotated_room_map;
    cv::erode(rotated_room_map, inflated_rotated_room_map, cv::Mat(), cv::Point(-1, -1), half_grid_spacing_as_int);
    if (DISPLAY_TRAJECTORY) {
        cv::imshow("inflated_rotated_room_map", inflated_rotated_room_map);
        cv::waitKey();
    }

    std::vector<std::vector<EnergyExploratorNode> > nodes;
    int number_of_nodes = 0;
    for (int y = min_room.y + half_grid_spacing_as_int; y < max_room.y; y += grid_spacing_as_int) {
        std::vector<EnergyExploratorNode> current_row;
        for (int x = min_room.x + half_grid_spacing_as_int; x < max_room.x; x += grid_spacing_as_int) {
            EnergyExploratorNode current_node;
            current_node.center_ = cv::Point(x, y);
            if (GridGenerator::completeCellTest(inflated_rotated_room_map,
                                                current_node.center_,
                                                grid_spacing_as_int)
                    ) {
                current_node.obstacle_ = false;
                current_node.visited_ = false;
                ++number_of_nodes;
            } else {
                current_node.obstacle_ = true;
                current_node.visited_ = true;
                ++number_of_nodes;
            }
            current_row.push_back(current_node);
        }

        nodes.push_back(current_row);
    }
    LOG(INFO) << "found " << number_of_nodes << " nodes";


    EnergyExploratorNode *first_accessible_node = 0;
    std::vector<EnergyExploratorNode *> corner_nodes;
    for (size_t row = 0; row < nodes.size(); ++row) {
        for (size_t column = 0; column < nodes[row].size(); ++column) {
            std::vector<EnergyExploratorNode *> &current_neighbors = nodes[row][column].neighbors_;
            for (int dy = -1; dy <= 1; ++dy) {
                if (row + dy < 0 || row + dy >= nodes.size())
                    continue;

                if (column > 0)
                    current_neighbors.push_back(&nodes[row + dy][column - 1]);

                if (dy != 0)
                    current_neighbors.push_back(&nodes[row + dy][column]);

                if (column < nodes[row].size() - 1)
                    current_neighbors.push_back(&nodes[row + dy][column + 1]);
            }

            int non_obstacle_neighbors = nodes[row][column].countNonObstacleNeighbors();
            if (non_obstacle_neighbors <= 3 && !nodes[row][column].obstacle_)
                corner_nodes.push_back(&nodes[row][column]);

            if (first_accessible_node == 0 && !nodes[row][column].obstacle_)
                first_accessible_node = &nodes[row][column];
        }
    }

    LOG(INFO) << "found neighbors, corners: " << corner_nodes.size();
    if (first_accessible_node == 0) {
        LOG(ERROR) << "Warning: there are no accessible points in this room.";
        return;
    }

    if (DISPLAY_TRAJECTORY) {
        cv::Mat test_map = rotated_room_map.clone();
        for (size_t i = 0; i < nodes.size(); ++i)
            for (size_t j = 0; j < nodes[i].size(); ++j)
                if (nodes[i][j].obstacle_ == false)
                    cv::circle(test_map, nodes[i][j].center_, 2, cv::Scalar(127), CV_FILLED);
        cv::imshow("grid", test_map);
        cv::waitKey();
//        for (size_t i = 0; i < nodes.size(); ++i) {
//            for (size_t j = 0; j < nodes[i].size(); ++j) {
//                cv::Mat test_map = rotated_room_map.clone();
//
//                std::vector<EnergyExploratorNode *> neighbors = nodes[i][j].neighbors_;
//                for (std::vector<EnergyExploratorNode *>::iterator n = neighbors.begin(); n != neighbors.end(); ++n)
//                    cv::circle(test_map, (*n)->center_, 2, cv::Scalar(127), CV_FILLED);
//
//                cv::imshow("neighbors", test_map);
//                cv::waitKey();
//            }
//        }
    }


    std::vector<cv::Point> starting_point_vector(1, starting_position);
    cv::transform(starting_point_vector, starting_point_vector, R);
    const cv::Point rotated_starting_point = starting_point_vector[0];

    EnergyExploratorNode *start_node = first_accessible_node;
    double min_distance = 1e10;
    for (std::vector<EnergyExploratorNode *>::iterator corner = corner_nodes.begin();
         corner != corner_nodes.end(); ++corner) {
        cv::Point diff = (*corner)->center_ - rotated_starting_point;
        double current_distance = diff.x * diff.x + diff.y * diff.y;
        if (current_distance <= min_distance) {
            start_node = *corner;
            min_distance = current_distance;
        }
    }
    LOG(INFO) << "start node: " << start_node->center_;

    std::vector<cv::Point2f> fov_coverage_path;
    fov_coverage_path.push_back(cv::Point2f(start_node->center_.x, start_node->center_.y));
    start_node->visited_ = true;

    EnergyExploratorNode *last_node = start_node;
    double previous_travel_angle = 0;
    for (std::vector<EnergyExploratorNode *>::iterator neighbor = last_node->neighbors_.begin();
         neighbor != last_node->neighbors_.end(); ++neighbor) {
        if (!(*neighbor)->obstacle_ && (*neighbor)->center_.y == start_node->center_.y &&
            (*neighbor)->center_.x > start_node->center_.x) {
            previous_travel_angle = 0;
            break;
        }
        if (!(*neighbor)->obstacle_ && (*neighbor)->center_.y == start_node->center_.y &&
            (*neighbor)->center_.x < start_node->center_.x) {
            previous_travel_angle = PI;
            break;
        }
        if (!(*neighbor)->obstacle_ && (*neighbor)->center_.y < start_node->center_.y &&
            (*neighbor)->center_.x == start_node->center_.x) {
            previous_travel_angle = -0.5 * PI;
        }
        if (!(*neighbor)->obstacle_ && (*neighbor)->center_.y > start_node->center_.y &&
            (*neighbor)->center_.x == start_node->center_.x) {
            previous_travel_angle = 0.5 * PI;
        }
    }

    int count = 0;
    do {
        std::vector<EnergyExploratorNode *> not_visited_neighbors;
        for (std::vector<EnergyExploratorNode *>::iterator neighbor = last_node->neighbors_.begin();
             neighbor != last_node->neighbors_.end(); ++neighbor)
            if (!(*neighbor)->obstacle_ && !(*neighbor)->visited_)
                not_visited_neighbors.push_back(*neighbor);

        double min_energy = 1e10;
        EnergyExploratorNode *next_node = 0;
        if (not_visited_neighbors.size() > 0) {
            // find best neighbor
            for (std::vector<EnergyExploratorNode *>::iterator candidate = not_visited_neighbors.begin();
                 candidate != not_visited_neighbors.end(); ++candidate) {
                const double current_energy = E(*last_node, **candidate, grid_spacing_in_pixel, previous_travel_angle);
                if (current_energy < min_energy) {
                    min_energy = current_energy;
                    next_node = *candidate;
                }
            }
        } else {
            for (size_t row = 0; row < nodes.size(); ++row) {
                for (size_t col = 0; col < nodes[row].size(); ++col) {
                    if (!nodes[row][col].obstacle_ && !nodes[row][col].visited_) {
                        const double current_energy = E(*last_node, nodes[row][col],
                                                        grid_spacing_in_pixel, previous_travel_angle);
                        if (current_energy < min_energy) {
                            min_energy = current_energy;
                            next_node = &nodes[row][col];
                        }
                    }
                }
            }
            if (next_node == nullptr)
                break;
        }
        previous_travel_angle = std::atan2(next_node->center_.y - last_node->center_.y,
                                           next_node->center_.x - last_node->center_.x);
        fov_coverage_path.push_back(cv::Point2f(next_node->center_.x, next_node->center_.y));
        next_node->visited_ = true;

        last_node = next_node;
        if (count % 200 == 0) {
            sleep(1);
        }
        count++;
    } while (true);

    std::vector<geometry_msgs::Pose2D> fov_poses;

    room_rotation.transformPathBackToOriginalRotation(fov_coverage_path, fov_poses, R);

    for (std::vector<geometry_msgs::Pose2D>::iterator pose = fov_poses.begin(); pose != fov_poses.end(); ++pose) {
        geometry_msgs::Pose2D current_pose;
        current_pose.x = (((room_map.cols - pose->x) * map_resolution) + map_origin.x);
        current_pose.y = (((room_map.rows - pose->y) * map_resolution) + map_origin.y);
        current_pose.theta = pose->theta;
        pose_path.push_back(current_pose);
    }
}
