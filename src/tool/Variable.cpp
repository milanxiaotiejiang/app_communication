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

const geometry_msgs::Pose &Variable::getCurrentPose() const {
    return current_pose;
}

void Variable::setCurrentPose(const geometry_msgs::Pose &currentPose) {
    current_pose = currentPose;
}