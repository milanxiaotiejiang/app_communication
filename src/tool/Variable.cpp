//
// Created by lijiang on 2021/12/23.
//

#include "tool/Variable.h"


const nav_msgs::OccupancyGrid &Variable::getMapApp() const {
    return map_App;
}

void Variable::setMapApp(const nav_msgs::OccupancyGrid &mapApp) {
    map_App = mapApp;
}

const nav_msgs::OccupancyGrid &Variable::getGridMapApp() const {
    return grid_map_App;
}

void Variable::setGridMapApp(const nav_msgs::OccupancyGrid &mapApp) {
    grid_map_App = mapApp;
}

int Variable::getPlannerType() const {
    return planner_type;
}

void Variable::setPlannerType(int plannerType) {
    planner_type = plannerType;
}

bool Variable::isFlagIsStartTeachMode() const {
    return flag_is_start_teach_mode;
}

void Variable::setFlagIsStartTeachMode(bool flagIsStartTeachMode) {
    flag_is_start_teach_mode = flagIsStartTeachMode;
}

const geometry_msgs::Pose &Variable::getCurrentPose() const {
    return current_pose;
}

void Variable::setCurrentPose(const geometry_msgs::Pose &currentPose) {
    current_pose = currentPose;
}

bool Variable::isScanFlag() const {
    return scan_flag;
}

void Variable::setScanFlag(bool scanFlag) {
    scan_flag = scanFlag;
}
