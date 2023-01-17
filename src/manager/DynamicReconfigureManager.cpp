//
// Created by admin1 on 2023/1/4.
//

#include "manager/DynamicReconfigureManager.h"

DynamicReconfigureManager::DynamicReconfigureManager() {
    ClientInitialize();

}

void DynamicReconfigureManager::set_inflation_radius(double radius) {
    dynamic_reconfigure::Reconfigure inflation_radius_reconfigure;
    dynamic_reconfigure::Reconfigure inflation_radius_reconfigure_res;
    dynamic_reconfigure::DoubleParameter new_inflation_radius;
    new_inflation_radius.name = "inflation_radius";
    new_inflation_radius.value = radius;
    inflation_radius_reconfigure.request.config.doubles.push_back(
            new_inflation_radius);
    try {
        global_dynamic_reconfigure_service_.call(inflation_radius_reconfigure);
    }
    catch (...) {
        ROS_ERROR("Something went wrong in the service call to global "
                  "dynamic_reconfigure");
    }
    try {
        local_dynamic_reconfigure_service_.call(inflation_radius_reconfigure);
    }
    catch (...) {
        ROS_ERROR("Something went wrong in the service call to local "
                  "dynamic_reconfigure");
    }
}

void DynamicReconfigureManager::set_goal_layer(bool enabled) {
    dynamic_reconfigure::Reconfigure enabled_reconfigure;
    dynamic_reconfigure::Reconfigure enabled_res;
    dynamic_reconfigure::BoolParameter new_enabled;
    new_enabled.name = "enabled";
    new_enabled.value = enabled;
    enabled_reconfigure.request.config.bools.push_back(
            new_enabled);
    try {
        goal_layer_dynamic_reconfigure_service_.call(enabled_reconfigure);
    }
    catch (...) {
        ROS_ERROR("Something went wrong in the service call to goal_layer "
                  "dynamic_reconfigure");
    }
}

void DynamicReconfigureManager::ClientInitialize() {
    //用于动态参数修改
    private_nh_.param("inflation_radius_param_name", inflation_radius_param_name,
                      std::string("inflation_radius"));

    std::string global_namespace;
    std::string local_namespace;

    private_nh_.param("global_namespace", global_namespace,
                      std::string("global_costmap/inflation_layer"));
    private_nh_.param("local_namespace", local_namespace,
                      std::string("local_costmap/inflation_layer"));

    global_nh_ = ros::NodeHandle("/move_base/" + global_namespace);
    local_nh_ = ros::NodeHandle("/move_base/" + local_namespace);
    goal_layer_nh_ = ros::NodeHandle("/move_base/global_costmap/goal_layer");
    std::cout << "/move_base/" + global_namespace << std::endl;
    std::cout << "/move_base/" + local_namespace << std::endl;
    LOG(INFO) << "Waiting for /move_base/global_costmap/inflation_layer/set_parameters server to start.";
    ros::service::waitForService("/move_base/global_costmap/inflation_layer/set_parameters");
    global_dynamic_reconfigure_service_ =
            global_nh_.serviceClient<dynamic_reconfigure::Reconfigure>(
                    "set_parameters", true);
    LOG(INFO) << "/move_base/global_costmap/inflation_layer/set_parameters server to start.";
    LOG(INFO) << "Waiting for /move_base/global_costmap/inflation_layer/set_parameters server to start.";
    ros::service::waitForService("/move_base/local_costmap/inflation_layer/set_parameters");
    local_dynamic_reconfigure_service_ =
            local_nh_.serviceClient<dynamic_reconfigure::Reconfigure>(
                    "set_parameters", true);
    LOG(INFO) << "/move_base/local_costmap/inflation_layer/set_parameters server  start.";
    ros::service::waitForService("/move_base/global_costmap/goal_layer/set_parameters");
    goal_layer_dynamic_reconfigure_service_ =
            goal_layer_nh_.serviceClient<dynamic_reconfigure::Reconfigure>(
                    "set_parameters", true);
    if (!global_nh_.getParam(inflation_radius_param_name, old_inflation_radius)) {
        ROS_ERROR("The global_costmap %s, does not have the parameter %s",
                  global_nh_.getNamespace().c_str(),
                  inflation_radius_param_name.c_str());
    }
    if (!local_nh_.getParam(inflation_radius_param_name, old_inflation_radius)) {
        ROS_ERROR("The local_costmap %s, does not have the parameter %s",
                  local_nh_.getNamespace().c_str(),
                  inflation_radius_param_name.c_str());
    }
    if (!goal_layer_nh_.getParam("enabled", old_goal_layer_enabled)) {
        ROS_ERROR("The local_costmap %s, does not have the parameter enabled ",
                  goal_layer_nh_.getNamespace().c_str());
    }
    ///////////////////////////////////////////////////////////////////////////////
}
