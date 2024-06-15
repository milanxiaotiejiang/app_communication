
#include "task/subscribe/FlagResultSubscribe.h"
#include "task/manager/StationManager.h"


FlagOutSubscribe::FlagOutSubscribe(ros::NodeHandle handle) {
    sub_flag_out_ = handle.subscribe("/result_flag_out", 1, &FlagOutSubscribe::subscribeCallback, this);
}

void FlagOutSubscribe::subscribeCallback(const std_msgs::Int32 &flag_result) {
    auto flag = flag_result.data;
    StationManager::instance().stationOutResult(flag);
}


FlagInSubscribe::FlagInSubscribe(ros::NodeHandle handle) {
    sub_flag_in_ = handle.subscribe("/result_flag_in", 1, &FlagInSubscribe::subscribeCallback, this);
}

void FlagInSubscribe::subscribeCallback(const std_msgs::Int32 &flag_result) {
    auto flag = flag_result.data;
    StationManager::instance().stationInResult(flag);
}