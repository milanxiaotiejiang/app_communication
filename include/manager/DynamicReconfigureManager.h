//
// Created by admin1 on 2023/1/4.
//

#ifndef APP_COMMUNICATION_DYNAMICRECONFIGUREMANAGER_H
#define APP_COMMUNICATION_DYNAMICRECONFIGUREMANAGER_H

#include "nlohmann/json.hpp"
#include "simulation.h"
#include <dynamic_reconfigure/Reconfigure.h>
#include <ros/ros.h>

class DynamicReconfigureManager {
public:
    DynamicReconfigureManager();

    void set_inflation_radius(double radius);

    void set_goal_layer(bool enabled);

    void ClientInitialize();

private:
    ros::NodeHandle private_nh_;
    ros::NodeHandle global_nh_, goal_layer_nh_;
    ros::NodeHandle local_nh_;
    ros::ServiceClient global_dynamic_reconfigure_service_, goal_layer_dynamic_reconfigure_service_;
    ros::ServiceClient local_dynamic_reconfigure_service_;
    ros::Publisher front_obstacle_pub_;
    std::string inflation_radius_param_name;
    double old_inflation_radius;
    bool old_goal_layer_enabled;
};


#endif //APP_COMMUNICATION_DYNAMICRECONFIGUREMANAGER_H
