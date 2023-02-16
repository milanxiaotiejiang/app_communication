//
// Created by Looper on 2022/9/30.
//

#include "exploration/ExplorationCenter.h"
#include "exploration/handle_exploration_display.h"
#include <ros/time.h>
#include <ros/package.h>
#include <opencv2/opencv.hpp>
#include "nav_msgs/Path.h"
#include "BaseThrowable.h"
#include "exploration/tsp/tsp_solver_defines.h"
#include "exploration/energy_functional_explorator.h"
#include "exploration/boustrophedon_explorator.h"
#include "exploration/tsp/genetic_TSP.h"
#include "exploration/tsp/nearest_neighbor_TSP.h"
#include <Eigen/Dense>
#include <eigen_conversions/eigen_msg.h>
#include "exploration/line.h"
#include "segmentation/SegmentationCenter.h"
#include "db/segmentation_data_base.h"
#include "exploration/infinitely_near_boundary.h"
#include "simulation.h"

static bool DISPLAY_TRAJECTORY = false;
static bool DISPLAY_TRAJECTORY_EFFECT = false;

void ExplorationCenter::initialize(ros::NodeHandle handle) {
    ros::Time::init();

    poseSubscribe = new OdomSubscribe(handle);

    path_pub_ = handle.advertise<nav_msgs::Path>("exploration_coverage_path", 2);

    initialize_finish = true;

    //testing
//    geometry_msgs::Pose2D pose2D;
//    pose2D.x = 0.0;
//    pose2D.y = 0.0;
//    MapAttribute::instance().setRobotPositionPose(pose2D);

//    cv::Mat segmented_map;
//    std::vector<Room> rooms;
//    SegmentationCenter::instance().storage2Memory(segmented_map, rooms);

    std::vector<geometry_msgs::Pose2D> exploration_path;
    std::vector<cv::Point> point_path;

    //1
//    const cv::Mat &map = SegmentationCenter::instance().choiceOneRoom(segmented_map, rooms, 60400);
//    ExplorationCenter::instance().generatePlanningPath(map, false, BOUSTROPHEDON_EXPLORER_MODE,
//                                                       false, cv::Point(0, 0),
//                                                       exploration_path, point_path);

    //2
//    const cv::Mat &map = SegmentationCenter::instance().generateMat();
//    generatePlanningSegmentationPath(map, segmented_map, rooms, BOUSTROPHEDON_EXPLORER_MODE,
//                                     exploration_path, point_path);

    //3
    if (DISPLAY_TRAJECTORY_EFFECT) {
//        const cv::Mat &map = SegmentationCenter::instance().generateMat();
//        generatePlanningPath(map, ExplorationModel::FULL, BOUSTROPHEDON_EXPLORER_MODE, true, cv::Point(0, 0),
//                             exploration_path, point_path);
    }

    //4
    if (DISPLAY_TRAJECTORY_EFFECT) {
        const cv::Mat &map = SegmentationCenter::instance().generateMat();
        infinitelyNearBoundary(map, exploration_path, point_path);
    }

//    pathPublish(exploration_path);
}

void ExplorationCenter::uninstall() {
    delete poseSubscribe;
}

void ExplorationCenter::infinitelyNearBoundary(const cv::Mat &room_map,
                                               std::vector<geometry_msgs::Pose2D> &pose_path,
                                               std::vector<cv::Point> &point_path) {
    if (!initialize_finish) {
        throw app::exception(make_error_code(error::exploration_initialize_fail));
    }
    if (MapAttribute::instance().isCreatingMap()) {
        throw app::exception(make_error_code(error::in_creating_map));
    }

    cv::Mat original_map = room_map.clone();
    cv::Mat map = room_map.clone();

    cv::Point2d map_origin = MapAttribute::instance().getMapOrigin();
    const cv::Point &stationPoint = MapAttribute::instance().rosPoint2MapPoint(map, Point(0, 0));

    //禁区虚拟墙
    cv::Mat prohibition_image = prohibitionMat(map);
    cv::Mat andMat;
    cv::bitwise_and(map, prohibition_image, andMat);
    cv::bitwise_xor(map, andMat, map);

    auto plan = SegmentationDataBase::instance().getDbPlan(SegmentationDataBase::instance().getDbMap().id);
    double grid_spacing_in_meter = plan.robot_radius * std::sqrt(2);//0.565685 网格正方形的边长
    double grid_spacing_in_pixel = grid_spacing_in_meter / map_resolution_from_subscription;
    int half_grid_spacing_as_int_ = (int) std::floor(0.5 * grid_spacing_in_pixel);

    int distance_from_obstacles = plan.distance_from_obstacles > 0 ? plan.distance_from_obstacles : 1;
    int number_extension = plan.number_extension > 0 ? plan.number_extension : 1;
    int multiple_contour_spacing = plan.multiple_contour_spacing > 0 ? plan.multiple_contour_spacing : 1;
    int random_number_generation_ratio = plan.random_number_generation_ratio;
    int boundary_min_area = plan.boundary_min_area;
    LOG(INFO) << "(infinitely near boundary) distance_from_obstacles: " << distance_from_obstacles;
    LOG(INFO) << "(infinitely near boundary) number_extension: " << number_extension;
    LOG(INFO) << "(infinitely near boundary) multiple_contour_spacing: " << multiple_contour_spacing;

    morphologicalEdging(map, plan.map_correction_closing_neighborhood_size);

    drawBaseStation(map, stationPoint, grid_spacing_in_pixel + plan.range_near_base_station);

    cv::Mat latelyMap = findClosestPointRoom(map, stationPoint);

    if (!removeUnconnectedRoomParts(latelyMap)) {
        LOG(ERROR)
                << "RoomExplorationServer::infinitelyNearBoundary: Warning: the requested room is too small for generating exploration trajectories.";
        throw app::exception(make_error_code(error::exploration_room_is_too_small));
    }

    if (!detectionTooSmallRoom(latelyMap, half_grid_spacing_as_int_ + plan.grid_obstacle_offset)) {
        throw app::exception(make_error_code(error::room_has_too_small_room));
    }

    int start_time = ros::Time::now().sec;

    InfinitelyNearBoundary infinitelyNearBoundary;
    infinitelyNearBoundary.getExplorationPath(original_map, latelyMap, pose_path, point_path,
                                              map_resolution_from_subscription,
                                              stationPoint, map_origin,
                                              plan.robot_radius,
                                              number_extension,
                                              distance_from_obstacles,
                                              multiple_contour_spacing,
                                              random_number_generation_ratio,
                                              boundary_min_area
    );

    int end_time = ros::Time::now().sec;
    std::cout << "cost infinitelyNearBoundary : " << end_time - start_time << " s " << std::endl;

    if (pose_path.empty()) {
        throw app::exception(make_error_code(error::exploration_path_planning_failed));
    }

    if (DISPLAY_TRAJECTORY || DISPLAY_TRAJECTORY_EFFECT)
        planning_pose_path_display(room_map, map_origin, pose_path, 1, "planning_pose_path_display");

//    pose2CVPoint(room_map, point_path, pose_path, map_origin);
//    if (DISPLAY_TRAJECTORY)
//        planning_point_path_display(room_map, point_path, 1, "planning_point_path_display");

}

void
ExplorationCenter::generatePlanningPath(const cv::Mat &room_map, ExplorationModel model,
                                        std::vector<geometry_msgs::Pose2D> &exploration_path,
                                        std::vector<cv::Point> &point_path) {
    generatePlanningPath(room_map, model, BOUSTROPHEDON_EXPLORER_MODE, false, cv::Point(0, 0),
                         exploration_path, point_path);
}

void ExplorationCenter::generatePlanningPath(const cv::Mat &room_map, ExplorationModel model, int explorer_mode,
                                             bool ordain_start, const cv::Point &start_position,
                                             std::vector<geometry_msgs::Pose2D> &exploration_path,
                                             std::vector<cv::Point> &point_path) {
    if (!initialize_finish) {
        throw app::exception(make_error_code(error::exploration_initialize_fail));
    }

    cv::Mat map = room_map.clone();


    cv::Point2d map_origin = MapAttribute::instance().getMapOrigin();
    const cv::Point &stationPoint = MapAttribute::instance().rosPoint2MapPoint(map, Point(0, 0));

    cv::Point robotPosition = MapAttribute::instance().getRobotPositionPoint(room_map);
    if (!DISPLAY_TRAJECTORY_EFFECT) {
        if (ordain_start) {
            robotPosition.x = start_position.x;
            robotPosition.y = start_position.y;
        }
    }


    //禁区虚拟墙
    cv::Mat prohibition_image = prohibitionMat(map);
    cv::Mat andMat;
    cv::bitwise_and(map, prohibition_image, andMat);
    cv::bitwise_xor(map, andMat, map);


    LOG(INFO) << "map-size: " << map.rows << "x" << map.cols;
    LOG(INFO) << "map-resolution: " << map_resolution_from_subscription << " m/cell";
    LOG(INFO) << "map-origin: " << map_origin << " m";
    LOG(INFO) << "starting-point: (" << robotPosition << " px";


    auto plan = SegmentationDataBase::instance().getDbPlan(SegmentationDataBase::instance().getDbMap().id);
    LOG(INFO) << "robot-radius: " << plan.robot_radius << " m   ("
              << (plan.robot_radius / map_resolution_from_subscription)
              << " px)";

    int area_px = 0;
    for (int v = 0; v < map.rows; ++v)
        for (int u = 0; u < map.cols; ++u)
            if (map.at<uchar>(v, u) >= 250)
                area_px++;
    LOG(INFO) << "### room area = "
              << area_px * map_resolution_from_subscription * map_resolution_from_subscription
              << " m^2";


    //Minimum area of one cell for the boustrophedon explorator. 拆分各段分割地图后的面积最小值（16）
    double min_cell_area_ = std::max(area_px / 1000.0, plan.min_cell_area);
    //Minimal distance between two points on the generated path [pixel]. 覆盖路径中两点之间的最小距离，单位像素 px，例如 20，20 * 0.05 = 1m（8）
    double path_eps_ = plan.path_eps;//1.0;//std::max(std::min(map.rows, map.cols) / 100.0, 8.0);
    //Allows to displace the grid by more than the standard half_grid_size from obstacles [m].（0.1）
    double grid_obstacle_offset_ = plan.grid_obstacle_offset;//0.0;
    //Maximal allowed shift off the ideal boustrophedon track for avoiding obstacles on track, in [pixel]. For negative values max_deviation_from_track is automatically set to grid_spacing.（-1）
    int max_deviation_from_track_ = plan.max_deviation_from_track;//-1;
    LOG(INFO) << "min_cell_area_ : " << min_cell_area_ << " , path_eps_ : " << path_eps_;
    LOG(INFO) << "planning mode: planning coverage path with robot's footprint";

    double grid_spacing_in_meter = plan.robot_radius * std::sqrt(2);//网格正方形的边长
    double grid_spacing_in_pixel = grid_spacing_in_meter / map_resolution_from_subscription;
    LOG(INFO) << "grid size: " << grid_spacing_in_meter << " m   (" << grid_spacing_in_pixel << " px)";
    int half_grid_spacing_as_int_ = (int) std::floor(0.5 * grid_spacing_in_pixel);
    int map_prohibition_expand_size_ = (int) std::floor(grid_spacing_in_pixel);

    cv::Mat generate_map = loadGenerateMap((int) std::floor(grid_spacing_in_pixel));


    if (model == ExplorationModel::FULL) {
        if (!baseStationAvailable(map, stationPoint)) {
            LOG(ERROR)
                    << "RoomExplorationServer::exploreRoom: Warning: Obstacles around the base station.";
            throw app::exception(make_error_code(error::exploration_obstacles_around_the_base_station));
        }
        cv::erode(map, map, cv::Mat(), cv::Point(-1, -1), map_prohibition_expand_size_);

        morphologicalEdging(map, plan.map_correction_closing_neighborhood_size);
    } else if (model == ExplorationModel::SUB) {
        cv::Mat temp;
        cv::bitwise_xor(map, generate_map, temp);
        cv::bitwise_and(map, temp, temp);
        cv::bitwise_xor(map, temp, map);

        morphologicalEdging(map, plan.map_correction_closing_neighborhood_size);
    } else if (model == ExplorationModel::RECT) {
        min_cell_area_ = 0;
        cv::dilate(map, map, cv::Mat(), cv::Point(-1, -1), half_grid_spacing_as_int_ + grid_obstacle_offset_);
        cv::Mat temp;
        cv::bitwise_xor(map, generate_map, temp);
        cv::bitwise_and(map, temp, temp);
        cv::bitwise_xor(map, temp, map);
    }

    drawBaseStation(map, stationPoint, grid_spacing_in_pixel + plan.range_near_base_station);
    findBaseNearReachable(map, robotPosition, (int) (grid_spacing_in_pixel * 2 + plan.range_near_base_station));

    cv::Mat latelyMap;
    if (model == ExplorationModel::FULL) {
        latelyMap = findClosestPointRoom(map, stationPoint);
    } else {
        latelyMap = map;
    }

    if (!removeUnconnectedRoomParts(latelyMap)) {
        LOG(ERROR)
                << "RoomExplorationServer::exploreRoom: Warning: the requested room is too small for generating exploration trajectories.";
        throw app::exception(make_error_code(error::exploration_room_is_too_small));
    }

    if (!detectionTooSmallRoom(latelyMap, half_grid_spacing_as_int_ + grid_obstacle_offset_)) {
        throw app::exception(make_error_code(error::room_has_too_small_room));
    }

    int start_time = ros::Time::now().sec;

    if (explorer_mode == BOUSTROPHEDON_EXPLORER_MODE) {
        BoustrophedonExplorer boustrophedon_explorer;
        boustrophedon_explorer.getExplorationPath(latelyMap, exploration_path, map_resolution_from_subscription,
                                                  robotPosition, map_origin,
                                                  grid_spacing_in_pixel, grid_obstacle_offset_,
                                                  path_eps_, min_cell_area_, max_deviation_from_track_,
                                                  TSP_GENETIC);
    } else {
        EnergyFunctionalExplorator energy_functional_explorer;
        energy_functional_explorer.getExplorationPath(latelyMap, exploration_path, map_resolution_from_subscription,
                                                      robotPosition, map_origin, grid_spacing_in_pixel);
    }


    int end_time = ros::Time::now().sec;
    std::cout << "cost getExplorationPath : " << end_time - start_time << " s " << std::endl;

    if (exploration_path.empty()) {
        throw app::exception(make_error_code(error::exploration_path_planning_failed));
    }

    optimizePathColumn(exploration_path);

    if (DISPLAY_TRAJECTORY || DISPLAY_TRAJECTORY_EFFECT)
        planning_pose_path_display(room_map, map_origin, exploration_path, 1, "planning_pose_path_display");

    pose2CVPoint(room_map, point_path, exploration_path, map_origin);
    if (DISPLAY_TRAJECTORY)
        planning_point_path_display(room_map, point_path, 1, "planning_point_path_display");

}

void ExplorationCenter::generatePlanningSegmentationPath(const cv::Mat &room_map, cv::Mat segmented_map,
                                                         std::vector<Room> rooms, int explorer_mode,
                                                         std::vector<geometry_msgs::Pose2D> &exploration_path,
                                                         std::vector<cv::Point> &point_path) {

    if (!initialize_finish) {
        throw app::exception(make_error_code(error::exploration_initialize_fail));
    }

    cv::Mat map = SegmentationCenter::instance().generateMat();
    auto robotPosition = MapAttribute::instance().getRobotPositionPoint(map);

    std::vector<cv::Point> polygon_centers;
    std::vector<GeneralizedPolygon> cell_polygons;
    cv::Mat generalized_map = cv::Mat::zeros(room_map.rows, room_map.cols, CV_8UC1);

    for (auto &room: rooms) {
        auto members = room.getMembers();
        GeneralizedPolygon current_cell(members, map_resolution_from_subscription);
        cell_polygons.push_back(current_cell);
        polygon_centers.emplace_back(current_cell.getCenter());
//        polygon_centers.emplace_back(room.getCenter().x, room.getCenter().y);

        cv::Mat room_mat = cv::Mat::zeros(room_map.rows, room_map.cols, CV_8UC1);
        cv::drawContours(room_mat, std::vector<std::vector<cv::Point> >(1, members), -1, cv::Scalar(255), CV_FILLED);

        if (DISPLAY_TRAJECTORY)
            center_point_display(room_mat, cv::Point(current_cell.getCenter().x, current_cell.getCenter().y), 3,
                                 "center_point_display");
        cv::bitwise_or(generalized_map, room_mat, generalized_map);
    }

    int start_cell_index = 0;
    for (std::vector<GeneralizedPolygon>::iterator cell = cell_polygons.begin(); cell != cell_polygons.end(); ++cell)
        if (cv::pointPolygonTest(cell->getVertices(), robotPosition, false) >= 0)
            start_cell_index = cell - cell_polygons.begin();

    std::vector<int> optimal_order;
//    NearestNeighborTSPSolver tsp_solver;
    GeneticTSPSolver tsp_solver;
    optimal_order = tsp_solver.solveGeneticTSP(generalized_map, polygon_centers, 1, 0.0,
                                               map_resolution_from_subscription, start_cell_index, 0);

    std::map<int, std::vector<geometry_msgs::Pose2D>> each_pose_map;
    std::map<int, std::vector<cv::Point>> each_point_map;
    std::map<int, std::vector<std::vector<cv::Point>>> each_contour_map;
    cv::Point start_position;
    bool hasError = false;
    for (int i = 0; i < optimal_order.size(); ++i) {
        int pos = optimal_order[i];
        auto room = rooms[pos];
        auto members = room.getMembers();
        auto one_map = SegmentationCenter::instance().choiceOneRoom(segmented_map, rooms, room.getID());

        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(one_map, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
        each_contour_map.insert(std::make_pair(pos, contours));

        std::vector<geometry_msgs::Pose2D> child_exploration_path;
        std::vector<cv::Point> child_point_path;
        try {
            ExplorationCenter::instance().generatePlanningPath(one_map, ExplorationModel::SUB, explorer_mode,
                                                               i != 0, start_position,
                                                               child_exploration_path, child_point_path);
            if (DISPLAY_TRAJECTORY)
                planning_point_path_display(map, child_point_path, 2, "planning_point_path_display");

            each_pose_map.insert(std::make_pair(pos, child_exploration_path));
            each_point_map.insert(std::make_pair(pos, child_point_path));

            start_position = child_point_path[child_point_path.size() - 1];
        } catch (app::exception const &e) {
            LOG(ERROR) << e.what();
            if (e.code().value() != error::exploration_room_is_too_small &&
                e.code().value() != error::room_has_too_small_room) {
                hasError = true;
                break;
            }
        }

    }
    if (hasError) {
        throw app::exception(make_error_code(error::exploration_segmentation_planning_fail));
    }

    if (DISPLAY_TRAJECTORY)
        planning_point_segmentation_path_display(map, optimal_order, polygon_centers,
                                                 each_point_map, each_contour_map, 3, "segmentation_path");

//    save_planning_point_segmentation_path(map, segmented_map, robotPosition, optimal_order, polygon_centers,
//                                          each_point_map, each_contour_map);

    for (int pos: optimal_order) {
        std::vector<geometry_msgs::Pose2D> &each_post_path = each_pose_map[pos];
        std::vector<cv::Point> &each_point_path = each_point_map[pos];
        for (const auto &item: each_post_path) {
            exploration_path.push_back(item);
        }
        for (const auto &item: each_point_path) {
            point_path.push_back(item);
        }
    }

    cv::Point2d map_origin = MapAttribute::instance().getMapOrigin();
    if (DISPLAY_TRAJECTORY)
        planning_pose_path_display(room_map, map_origin, exploration_path, 2, "planning_pose_path_display");

    if (DISPLAY_TRAJECTORY)
        planning_point_path_display(room_map, point_path, 2, "planning_point_path_display");
}

void ExplorationCenter::pathPublish(const std::vector<geometry_msgs::Pose2D> &exploration_path) const {
    std::vector<geometry_msgs::PoseStamped> exploration_path_pose_stamped(exploration_path.size());
    std_msgs::Header header;
    header.stamp = ros::Time::now();
    header.frame_id = "/map";
    for (size_t i = 0; i < exploration_path.size(); ++i) {
        exploration_path_pose_stamped[i].header = header;
        exploration_path_pose_stamped[i].header.seq = i;
        exploration_path_pose_stamped[i].pose.position.x = exploration_path[i].y;
        exploration_path_pose_stamped[i].pose.position.y = exploration_path[i].x;
        exploration_path_pose_stamped[i].pose.position.z = 0.;
        Eigen::Quaterniond quaternion;
        quaternion = Eigen::AngleAxisd((double) exploration_path[i].theta, Eigen::Vector3d::UnitZ());
        tf::quaternionEigenToMsg(quaternion, exploration_path_pose_stamped[i].pose.orientation);
    }

    nav_msgs::Path coverage_path;
    coverage_path.header.frame_id = "map";
    coverage_path.header.stamp = ros::Time::now();
    coverage_path.poses = exploration_path_pose_stamped;
    path_pub_.publish(coverage_path);
}

bool ExplorationCenter::baseStationAvailable(cv::Mat &room_map, const cv::Point &point) {
    bool isAvailable = false;
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(room_map, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    for (auto &contour: contours) {
        double d = cv::pointPolygonTest(contour, point, false);
        if (d >= 0) {
            isAvailable = true;
            break;
        }
    }
    return isAvailable;
}

/**
 * 移除距离 point 点远的区域
 * 当多个区域不相连时，找里机器人或者基站最近的区域（应该为包含此点的区域）
 * @param room_map
 * @return
 */
cv::Mat ExplorationCenter::findClosestPointRoom(cv::Mat &room_map, const cv::Point &point) {
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(room_map, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    cv::Mat image = cv::Mat::zeros(room_map.rows, room_map.cols, CV_8UC1);
    double distance = -100000;
    for (int i = 0; i < contours.size(); ++i) {

        double d = cv::pointPolygonTest(contours[i], point, true);
        if (d > distance) {
            distance = d;
            cv::drawContours(image, contours, i, cv::Scalar(255), CV_FILLED);
        }
    }
    cv::Mat result;
    cv::bitwise_and(room_map, image, result);
    return result;
}

/**
 * 保留下面积最大的一个连通区域
 * @param room_map
 * @return
 */
bool ExplorationCenter::removeUnconnectedRoomParts(cv::Mat &room_map) {
    cv::Mat room_map_int(room_map.rows, room_map.cols, CV_32SC1);
    for (int v = 0; v < room_map.rows; ++v) {
        for (int u = 0; u < room_map.cols; ++u) {
            if (room_map.at<uchar>(v, u) == 255)
                room_map_int.at<int32_t>(v, u) = -100;
            else
                room_map_int.at<int32_t>(v, u) = 0;
        }
    }

    std::map<int, int> area_to_label_map;
    int label = 1;
    for (int v = 0; v < room_map_int.rows; ++v) {
        for (int u = 0; u < room_map_int.cols; ++u) {
            if (room_map_int.at<int32_t>(v, u) == -100) {
                const int area = cv::floodFill(room_map_int, cv::Point(u, v),
                                               cv::Scalar(label), 0, 0, 0,
                                               8 | cv::FLOODFILL_FIXED_RANGE);
                area_to_label_map[area] = label;
                ++label;
            }
        }
    }
    if (area_to_label_map.empty())
        return false;

    const int label_of_biggest_room = area_to_label_map.rbegin()->second;
    LOG(INFO) << "最大房间标签 = " << label_of_biggest_room;
    for (int v = 0; v < room_map.rows; ++v)
        for (int u = 0; u < room_map.cols; ++u)
            if (room_map_int.at<int32_t>(v, u) != label_of_biggest_room)
                room_map.at<uchar>(v, u) = 0;

    return true;
}

cv::Mat ExplorationCenter::prohibitionMat(const cv::Mat &room_map) const {
    cv::Mat prohibition_image = cv::Mat::zeros(room_map.rows, room_map.cols, CV_8UC1);

    auto penaltyZoneList = MapAttribute::instance().getPenaltyZoneList();
    std::vector<std::vector<cv::Point>> polygon_array;
    for (int i = 0; i < penaltyZoneList.size(); ++i) {
        std::vector<cv::Point> cvPoints;
        auto vector = penaltyZoneList[i];
        for (int j = 0; j < vector.size(); ++j) {
            const cv::Point &point = MapAttribute::instance().rosPoint2MapPoint(prohibition_image, vector[j]);
            cvPoints.push_back(point);
        }
        polygon_array.push_back(cvPoints);
    }
    cv::fillPoly(prohibition_image, polygon_array, cv::Scalar(255));

    auto virtualWallList = MapAttribute::instance().getVirtualWallList();
    for (const auto &vector: virtualWallList) {
        if (vector.size() == 2) {
            const cv::Point &pointStart = MapAttribute::instance().rosPoint2MapPoint(prohibition_image, vector[0]);
            const cv::Point &pointEnd = MapAttribute::instance().rosPoint2MapPoint(prohibition_image, vector[1]);
            cv::line(prohibition_image, pointStart, pointEnd, cv::Scalar(255));
        }
    }
//    cv::dilate(prohibition_image, prohibition_image, cv::Mat(), cv::Point(-1, -1),
//               map_prohibition_expand_size_);
    return prohibition_image;
}

void ExplorationCenter::morphologicalEdging(cv::Mat &room_map, int map_correction_closing_neighborhood_size) const {
    cv::Mat temp;
    cv::erode(room_map, temp, cv::Mat(), cv::Point(-1, -1), map_correction_closing_neighborhood_size);
    cv::dilate(temp, room_map, cv::Mat(), cv::Point(-1, -1), map_correction_closing_neighborhood_size);
}

void ExplorationCenter::drawBaseStation(cv::Mat &room_map, const cv::Point &stationPoint, int radius) const {
    cv::rectangle(room_map, cv::Point(stationPoint.x - radius, stationPoint.y - radius),
                  cv::Point(stationPoint.x + radius, stationPoint.y + radius),
                  cv::Scalar(0), CV_FILLED);
}

void ExplorationCenter::pose2CVPoint(const cv::Mat &room_map, std::vector<cv::Point> &pointList,
                                     const std::vector<geometry_msgs::Pose2D> &poseList,
                                     const cv::Point2d &map_origin) {
    if (poseList.empty()) {
        return;
    }
    int cols = room_map.cols;
    int rows = room_map.rows;
    for (const auto &item: poseList) {
        cv::Point point(cols - (item.x - map_origin.x) / map_resolution_from_subscription,
                        rows - (item.y - map_origin.y) / map_resolution_from_subscription);
        pointList.push_back(point);
    }
}

void ExplorationCenter::cvPoint2Pose(const cv::Mat &room_map, std::vector<geometry_msgs::Pose2D> &postList,
                                     const std::vector<cv::Point> &pointList, const cv::Point2d &map_origin) {
    if (pointList.empty()) {
        return;
    }
    int cols = room_map.cols;
    int rows = room_map.rows;
    for (const auto &item: pointList) {
        geometry_msgs::Pose2D pose;
        pose.x = (cols - item.x) * map_resolution_from_subscription + map_origin.x;
        pose.y = (cols - item.y) * map_resolution_from_subscription + map_origin.y;
        postList.push_back(pose);
    }
}

void ExplorationCenter::cacheRoomCoverage(const RoomCoverage &coverage) {
    coverageCache.put(coverage.getCoverageId(), coverage);
}

RoomCoverage ExplorationCenter::findRoomCoverage(const std::string &coverageId, bool latest) {
    if (coverageCache.size() <= 0) {
        throw std::range_error("No cached Room Coverage Path ...");
    }
    if (coverageCache.exists(coverageId)) {
        return coverageCache.get(coverageId);
    }

    if (latest) {
        if (!Environment::instance().room_coverage_uuid.empty()) {
            if (coverageCache.exists(Environment::instance().room_coverage_uuid)) {
                return coverageCache.get(Environment::instance().room_coverage_uuid);
            }
        }
    }

    throw std::range_error("There is no find key : " + coverageId + " in coverageCache");
}

cv::Mat ExplorationCenter::loadGenerateMap(int grid_spacing_in_pixel) {
    auto generate_map = SegmentationCenter::instance().generateMat();
    cv::Mat prohibition_image = prohibitionMat(generate_map);
    cv::Mat andMat;
    cv::bitwise_and(generate_map, prohibition_image, andMat);
    cv::bitwise_xor(generate_map, andMat, generate_map);
    cv::erode(generate_map, generate_map, cv::Mat(), cv::Point(-1, -1), grid_spacing_in_pixel);
    return generate_map;
}

void ExplorationCenter::optimizePathColumn(std::vector<geometry_msgs::Pose2D> &vector) {
    if (vector.size() < 3) {
        return;
    }
    std::vector<geometry_msgs::Pose2D> optimize;
    optimize.push_back(vector[0]);
    geometry_msgs::Pose2D last = vector[0];
    for (int i = 1; i < vector.size() - 1; ++i) {
        if (!conversion::one_line(last, vector[i], vector[i + 1])) {
            if (sqrt(pow(vector[i].x - vector[i + 1].x, 2) + pow(vector[i + 1].y - vector[i].y, 2)) < 1.0) {
                last = vector[i];
                optimize.push_back(vector[i]);
            }
        }
    }
    optimize.push_back(vector[vector.size() - 1]);

    vector.clear();
    for (const auto &item: optimize) {
        vector.push_back(item);
    }
}

bool ExplorationCenter::detectionTooSmallRoom(const cv::Mat &map, int iterations) const {
    cv::Mat compute_map = map.clone();
    cv::erode(compute_map, compute_map, cv::Mat(), cv::Point(-1, -1), iterations);

    int count = 0;
    for (int v = 0; v < compute_map.rows; ++v) {
        for (int u = 0; u < compute_map.cols; ++u) {
            if (compute_map.at<uchar>(v, u) == 255)
                count++;
        }
    }
    return count != 0;
}

cv::Point &ExplorationCenter::findBaseNearReachable(cv::Mat &map, cv::Point &reachablePoint, int range) {
    int origin_x = reachablePoint.x;
    int origin_y = reachablePoint.y;

    if (map.at<unsigned char>(reachablePoint.y, reachablePoint.x) != 255) {
        LOG(INFO) << "ExplorationCenter : Find available points near the base station";
        for (int row = -range; row <= range; row++) {
            if (map.at<unsigned char>(reachablePoint.y, reachablePoint.x) == 255)
                break;
            for (int col = -range; col <= range; col++) {
                if (map.at<unsigned char>(origin_y + row, origin_x + col) == 255) {
                    reachablePoint.x = origin_x + col;
                    reachablePoint.y = origin_y + row;
                    break;
                }
            }
        }
    } else {
        LOG(INFO) << "InfinitelyNearBoundary : The location of the base station can ensure the arrival ...";
    }
}
