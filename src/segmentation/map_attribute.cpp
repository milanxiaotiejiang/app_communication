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
#include "cppfs/FilePath.h"
#include <cppfs/fs.h>
#include <cppfs/FileHandle.h>

/**
 * map_origin_pose.position (0,0) 为显示地图的左下角，即 starting_position_pose.x 越大，机器人越靠右；starting_position_pose.y 越大，机器人越考上
 */
void MapAttributeSingleton::setRobotPositionPose(geometry_msgs::Pose2D positionPose) {
    this->starting_position_pose = positionPose;
}

cv::Point MapAttributeSingleton::getRobotPositionPoint(const cv::Mat &room_map) const {
    auto cols = room_map.cols;//width
    auto rows = room_map.rows;//height
    cv::Point starting_position;
    starting_position.x =
            cols - (starting_position_pose.y - getMapOrigin().x) / map_resolution_from_subscription;
    starting_position.y =
            rows - (starting_position_pose.x - getMapOrigin().y) / map_resolution_from_subscription;
    LOG_IF(INFO, DEBUG_SEGMENTATION)
                    << "current robot position (" << starting_position.x << ", " << starting_position.y << ")";
    return starting_position;
}

cv::Point MapAttributeSingleton::getRobotPositionPoint(int rows, int cols) const {
    cv::Point starting_position;
    starting_position.x =
            cols - (starting_position_pose.y - getMapOrigin().x) / map_resolution_from_subscription;
    starting_position.y =
            rows - (starting_position_pose.x - getMapOrigin().y) / map_resolution_from_subscription;
    LOG_IF(INFO, DEBUG_SEGMENTATION)
                    << "current robot position (" << starting_position.x << ", " << starting_position.y << ")";
    return starting_position;
}

bool MapAttributeSingleton::loadStation() {
    currentMapAttribute.attrPath = path::map_yaml_path();
    return readAnyMapInfo(currentMapAttribute);
}

void MapAttributeSingleton::resetProhibition() {
    virtualWallList.clear();
    penaltyZoneList.clear();
}

void MapAttributeSingleton::loadVirtualWall() {
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

void MapAttributeSingleton::loadPenaltyZone() {
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

void MapAttributeSingleton::loadPlanParam() {
    std::string &map_id = SegmentationDataBase::instance().getDbMap().id;
    auto planPo = SegmentationDataBase::instance().getDbPlan(map_id);
    if (planPo.map_id.empty()) {
        loadDefaultPlanParam();
    }
    auto planPo2 = SegmentationDataBase::instance().getDbPlan(map_id);
    if (planPo2.version < CURRENT_PLAN_VERSION) {
        SegmentationDataBase::instance().removePlanParam(map_id);
        loadDefaultPlanParam();
    }
}

void MapAttributeSingleton::loadDefaultPlanParam() {
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
MapAttributeSingleton::handleProhibition(std::vector<std::vector<Point>> &list, const YAML::Node &node,
                                         int dusCount) const {
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

cv::Point MapAttributeSingleton::rosPoint2MapPoint(const cv::Mat &room_map, const Point &point) const {
    double rows = room_map.rows * map_resolution_from_subscription;
    double cols = room_map.cols * map_resolution_from_subscription;
    double x = cols - (point.getY() - getMapOrigin().x);
    double y = rows - (point.getX() - getMapOrigin().y);
    cv::Point position;
    position.x = x / map_resolution_from_subscription;
    position.y = y / map_resolution_from_subscription;
    return position;
}

cv::Point MapAttributeSingleton::rosPoint2MapPoint(int rows, int cols, const Point &point) const {
    const cv::Point2d &origin = getMapOrigin();
    double x = cols * map_resolution_from_subscription - (point.getY() - getMapOrigin().x);
    double y = rows * map_resolution_from_subscription - (point.getX() - getMapOrigin().y);
    cv::Point position;
    position.x = x / map_resolution_from_subscription;
    position.y = y / map_resolution_from_subscription;
    return position;
}

Point MapAttributeSingleton::mapPoint2RosPoint(int rows, int cols, const cv::Point &point) const {
    const cv::Point2d &origin = getMapOrigin();
    Point p;
    p.setX((rows - point.y) * map_resolution_from_subscription + getMapOrigin().y);
    p.setY((cols - point.x) * map_resolution_from_subscription + getMapOrigin().x);
    return p;
}

bool MapAttributeSingleton::isCreatingMap() const {
    return creating_map;
}

bool MapAttributeSingleton::saveMap() {
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
        LOG(INFO) << "save map timeout";
        creating_map = false;
        return false;
    } else {
        creating_map = false;
        return true;
    }
}

void MapAttributeSingleton::notifySaveMap() {
    wait_cv.notify_all();
}

bool MapAttributeSingleton::readAnyMapInfo(MapAttribute &mapAttribute) {
    std::string yamlPath = mapAttribute.attrPath;

    auto mapAttr = cppfs::FilePath(yamlPath);
    if (!cppfs::fs::open(mapAttr.fullPath()).exists()) {
        return false;
    }

    YAML::Node config = YAML::LoadFile(yamlPath);
    const YAML::Node &originNode = config["origin"];
    if (!originNode.IsDefined()) {
        return false;
    }
    if (originNode.size() != 3) {
        return false;
    }

    const YAML::Node &imageNode = config["image"];
    if (!imageNode.IsDefined()) {
        return false;
    }

    auto mapPath = imageNode.as<std::string>();
    auto mapFile = cppfs::FilePath(mapPath);
    if (!cppfs::fs::open(mapFile.fullPath()).exists()) {
        return false;
    }

    mapAttribute.originPose.position.x = originNode[1].as<double>();
    mapAttribute.originPose.position.y = originNode[0].as<double>();
    mapAttribute.originPose.position.z = originNode[2].as<double>();

    cv::Mat map = cv::imread(mapPath, cv::ImreadModes::IMREAD_GRAYSCALE);
    cv::rotate(map, map, cv::RotateFlags::ROTATE_90_COUNTERCLOCKWISE);

    mapAttribute.mapPath = mapPath;
    mapAttribute.mapRows = map.rows;
    mapAttribute.mapCols = map.cols;

    mapAttribute.originPoint.x = map.cols + mapAttribute.originPose.position.x / map_resolution_from_subscription;
    mapAttribute.originPoint.y = map.rows + mapAttribute.originPose.position.y / map_resolution_from_subscription;

    return true;
}
