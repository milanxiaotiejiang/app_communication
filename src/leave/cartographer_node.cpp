//
// Created by Looper on 2023/4/11.
//

#include "leave/cartographer_node.h"
#include "exploration/ExplorationCenter.h"
#include "future/node/node_control.h"
#include "future/timer_call.h"
#include "simulation.h"

void CartographerPublisher::initialize(ros::NodeHandle handle) {
    save_map = handle.advertise<std_msgs::Int32>("/save_map", 1);
    update_map = handle.advertise<std_msgs::Int32>("/update_map", 1);
    start_carto_mapping = handle.advertise<std_msgs::Int32>("/start_carto_mapping", 1);
    start_carto_localization = handle.advertise<std_msgs::Int32>("/start_carto_localization", 1);
    shutdown_carto = handle.advertise<std_msgs::Int32>("/shutdown_carto", 1);
    restart_carto = handle.advertise<std_msgs::Int32>("/restart_carto", 1);
    initial_pose = handle.advertise<geometry_msgs::PoseWithCovarianceStamped>("/initial_pose", 1);
    clear_current_pose = handle.advertise<std_msgs::Int32>("/clear_current_pose", 1);

    control_move_base = handle.advertise<std_msgs::Int32>("/control_move_base", 1);
}

void CartographerPublisher::publishSaveMap() const {
    std_msgs::Int32 message;
    message.data = 1;
    save_map.publish(message);
}

void CartographerPublisher::publishUpdateMap() const {
    std_msgs::Int32 message;
    message.data = 1;
    update_map.publish(message);
    if (!Environment::instance().isRealEnvironment) {
        async::TimerCall::instance().baseLoop()->scheduleLater(std::chrono::seconds(10), []() {
            CartographerSubscribe::instance().coverResult();
        });
    }
}

void CartographerPublisher::publishStartCartoMapping() const {
    std_msgs::Int32 message;
    message.data = 1;
    start_carto_mapping.publish(message);
}

void CartographerPublisher::publishStartCartoLocalization() const {
    std_msgs::Int32 message;
    message.data = 1;
    start_carto_localization.publish(message);
}

void CartographerPublisher::publishShutdownCarto() const {
    std_msgs::Int32 message;
    message.data = 1;
    shutdown_carto.publish(message);
}

void CartographerPublisher::publishRestartCarto() const {
    std_msgs::Int32 message;
    message.data = 1;
    restart_carto.publish(message);
}

void CartographerPublisher::publishInitialPose(const geometry_msgs::PoseWithCovarianceStamped &pose) const {
    initial_pose.publish(pose);
}

void CartographerPublisher::publishClearCurrentPose() const {
    std_msgs::Int32 message;
    message.data = 1;
    clear_current_pose.publish(message);
}

void CartographerPublisher::publishControlMoveBase(bool open) const {
    if (open) {
        std_msgs::Int32 message;
        message.data = 1;
        control_move_base.publish(message);
    } else {
        std_msgs::Int32 message;
        message.data = 0;
        control_move_base.publish(message);
    }
}

void CartographerSubscribe::initialize(ros::NodeHandle handle) {
    update_finish = handle.subscribe("/update_finish", 1, &CartographerSubscribe::updateFinishCallback, this);
    build_map_finish = handle.subscribe("/build_map_finish", 1, &CartographerSubscribe::buildMapFinishCallback, this);
    carto_heart_beat = handle.subscribe("/carto_heart_beat", 1, &CartographerSubscribe::cartoHeartBeatCallback, this);
    current_cartographer_pose = handle.subscribe("/current_cartographer_pose", 1,
                                                 &CartographerSubscribe::currentCartographerPoseCallback, this);
}

void CartographerSubscribe::updateFinishCallback(const std_msgs::Int32 &carto_result) {
    if (carto_result.data == 1) {
        ExplorationCenter::instance().repaintCoveragePath(true);
        CartographerSubscribe::instance().coverResult();
    }
}

void CartographerSubscribe::buildMapFinishCallback(const std_msgs::Int32 &carto_result) {
    if (carto_result.data == 1) {
        MapAttribute::instance().notifySaveMap();
    }
}

void CartographerSubscribe::cartoHeartBeatCallback(const std_msgs::Int32 &carto_result) {
    if (carto_result.data == 1) {
        if (NodeControl::instance().heart_beat > 10000)
            NodeControl::instance().heart_beat = 0;
        NodeControl::instance().heart_beat++;
    }
}

void
CartographerSubscribe::currentCartographerPoseCallback(const geometry_msgs::PoseWithCovarianceStamped &carto_result) {

}

void CartographerSubscribe::coverResult() {
    asyncTaskCall->executeCover();
}
