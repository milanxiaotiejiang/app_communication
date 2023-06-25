//
// Created by Looper on 2022/10/13.
//

#include <std_msgs/String.h>
#include "segmentation/map_attribute.h"
#include "db/segmentation_data_base.h"
#include "manager/PublishInnerManager.h"
#include "BaseThrowable.h"
#include "leave/cartographer_node.h"
#include "simulation.h"

/**
 * map_origin_pose.position (0,0) 为显示地图的左下角，即 starting_position_pose.x 越大，机器人越靠右；starting_position_pose.y 越大，机器人越考上
 */
void MapAttribute::setRobotPositionPose(geometry_msgs::Pose2D positionPose) {
    this->starting_position_pose = positionPose;
}

cv::Point MapAttribute::getRobotPositionPoint(const cv::Mat &room_map) const {
    auto cols = room_map.cols;//width
    auto rows = room_map.rows;//height
    cv::Point starting_position;
    starting_position.x =
            cols - (starting_position_pose.y - map_origin_pose.position.x) / map_resolution_from_subscription;
    starting_position.y =
            rows - (starting_position_pose.x - map_origin_pose.position.y) / map_resolution_from_subscription;
    LOG_IF(INFO, DEBUG_SEGMENTATION)
    << "current robot position (" << starting_position.x << ", " << starting_position.y << ")";
    return starting_position;
}

void MapAttribute::loadStation() {
    if (access(path::map_yaml_path().c_str(), F_OK) != 0) {//存在
        return;
    }
    YAML::Node config = YAML::LoadFile(path::map_yaml_path());
    const YAML::Node &originNode = config["origin"];
    if (!originNode.IsDefined()) {
        return;
    }
    if (originNode.size() != 3) {
        return;
    }

    map_origin_pose.position.x = originNode[1].as<double>();
    map_origin_pose.position.y = originNode[0].as<double>();
    map_origin_pose.position.z = originNode[2].as<double>();

    map_origin.x = map_origin_pose.position.x;
    map_origin.y = map_origin_pose.position.y;

    initialize_finish = true;
}

void MapAttribute::resetProhibition() {
    virtualWallList.clear();
    penaltyZoneList.clear();
}

void MapAttribute::loadVirtualWall() {
    if (access(path::prohibition_areas_path().c_str(), F_OK) != 0) {//存在
        return;
    }

    YAML::Node config = YAML::LoadFile(path::prohibition_areas_path());
    const YAML::Node &prohibitionNode = config["prohibition_areas"];
    if (!prohibitionNode.IsDefined()) {
        return;
    }
    auto size = prohibitionNode.size();
    for (int i = 0; i < size; i++) {
        const YAML::Node &childNode = prohibitionNode[i];

        if (childNode.size() == VIRTUAL_WALL_DUS_COUNT) {//虚拟墙
            handleProhibition(virtualWallList, childNode, VIRTUAL_WALL_DUS_COUNT);
        }
    }
}

void MapAttribute::loadPenaltyZone() {
    if (access(path::prohibition_areas_path().c_str(), F_OK) != 0) {//存在
        return;
    }

    YAML::Node config = YAML::LoadFile(path::prohibition_areas_path());
    const YAML::Node &prohibitionNode = config["prohibition_areas"];
    if (!prohibitionNode.IsDefined()) {
        return;
    }
    auto size = prohibitionNode.size();
    for (int i = 0; i < size; i++) {
        const YAML::Node &childNode = prohibitionNode[i];

        if (childNode.size() == PENALTY_ZONE_DUS_COUNT) {//禁区
            handleProhibition(penaltyZoneList, childNode, PENALTY_ZONE_DUS_COUNT);
        }
    }
}

void MapAttribute::loadPlanParam() {
    std::string &map_id = SegmentationDataBase::instance().getDbMap().id;
    auto planPo = SegmentationDataBase::instance().getDbPlan(map_id);
    if (planPo.map_id.empty()) {
        loadDefaultPlanParam();
    }
    auto planPo2 = SegmentationDataBase::instance().getDbPlan(map_id);
    if (planPo2.version < 3) {
        SegmentationDataBase::instance().removePlanParam(map_id);
        loadDefaultPlanParam();
    }
}

void MapAttribute::loadDefaultPlanParam() {
    std::string &map_id = SegmentationDataBase::instance().getDbMap().id;
    SegmentationDataBase::instance().setPlanParam(map_id,
                                                  robot_radius_,
                                                  map_correction_closing_neighborhood_size_,
                                                  grid_obstacle_offset_,
                                                  path_eps_,
                                                  min_cell_area_,
                                                  max_deviation_from_track_,
                                                  range_near_base_station_,
                                                  room_area_factor_lower_limit_,
                                                  room_area_factor_upper_limit_,
                                                  neighborhood_index_,
                                                  max_iterations_,
                                                  min_critical_point_distance_factor_,
                                                  max_area_for_merging_,
                                                  distance_from_obstacles_,
                                                  number_extension_,
                                                  multiple_contour_spacing_,
                                                  random_number_generation_ratio_,
                                                  boundary_min_area_);
}

void
MapAttribute::handleProhibition(std::vector<std::vector<Point>> &list, const YAML::Node &node, int dusCount) const {
    std::vector<Point> pointList;
    for (int j = 0; j < node.size(); j++) {
        const YAML::Node &childNode = node[j];
        if (childNode.size() == 2) {
            Point point(atof(childNode[0].as<std::string>().c_str()),
                        atof(childNode[1].as<std::string>().c_str())
            );
            pointList.push_back(point);
        }
    }

    if (pointList.size() == dusCount) {
        list.push_back(pointList);
    }
}

cv::Point MapAttribute::rosPoint2MapPoint(const cv::Mat &room_map, const Point &point) const {
    double rows = room_map.rows * map_resolution_from_subscription;
    double cols = room_map.cols * map_resolution_from_subscription;
    double x = cols - (point.getY() - map_origin_pose.position.x);
    double y = rows - (point.getX() - map_origin_pose.position.y);
    cv::Point position;
    position.x = x / map_resolution_from_subscription;
    position.y = y / map_resolution_from_subscription;
    return position;
}

cv::Point MapAttribute::rosPoint2MapPoint(double rows, double cols, const Point &point) const {
    double x = cols - (point.getY() - map_origin_pose.position.x);
    double y = rows - (point.getX() - map_origin_pose.position.y);
    cv::Point position;
    position.x = x / map_resolution_from_subscription;
    position.y = y / map_resolution_from_subscription;
    return position;
}

bool MapAttribute::isCreatingMap() const {
    return creating_map;
}

bool MapAttribute::saveMap() {
    //防止二次进入
    if (creating_map) {
        throw app::exception(make_error_code(error::in_creating_map));
    }
    creating_map = true;

    //发送建图保存指令
    CartographerPublisher::instance().publishSaveMap();

    //加锁
    std::unique_lock<std::mutex> lck(wait_mutex);
    if (wait_cv.wait_for(lck, std::chrono::seconds(20)) == std::cv_status::timeout) {
        //timeout
        creating_map = false;
        return false;
    } else {
        creating_map = false;
        return true;
    }
}

void MapAttribute::notifySaveMap() {
    wait_cv.notify_all();
}
