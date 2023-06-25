//
// Created by Looper on 2023/4/11.
//

#include "leave/cartographer_node.h"
#include "exploration/ExplorationCenter.h"
#include "future/node/node_control.h"
#include "future/timer_call.h"
#include "simulation.h"
#include "back_charge_msgs/sensor_status.h"
#include "back_charge_msgs/ready_check.h"
#include "back_charge_msgs/start_localization.h"
#include "back_charge_msgs/stop_localization.h"
#include "db/segmentation_data_base.h"
#include "segmentation/SegmentationCenter.h"
#include "leave/map_control.h"

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
        MapControl::instance().backupMap(SegmentationDataBase::instance().getDbMap().id, false);
        MapAttribute::instance().loadStation();
        SegmentationCenter::instance().resetSegmentation();
        ExplorationCenter::instance().repaintCoveragePath();
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

void CartographerServiceClient::initialize(ros::NodeHandle handle) {
    sensor_status = handle.serviceClient<back_charge_msgs::sensor_status>("sensor_status");
    ready_check = handle.serviceClient<back_charge_msgs::ready_check>("ready_check");
    start_localization = handle.serviceClient<back_charge_msgs::start_localization>("start_localization");
    stop_localization = handle.serviceClient<back_charge_msgs::stop_localization>("stop_localization");
}

bool CartographerServiceClient::callSensorStatus() {
    if (!Environment::instance().isRealEnvironment) {
        return true;
    }
    back_charge_msgs::sensor_status srv;
    bool result = sensor_status.call(srv);
    if (result) {
        bool hlsStatus = srv.response.hls_status;//电机失能
        bool imuStatus = srv.response.imu_status;//imu
        bool laserStatus = srv.response.laser_status;//激光雷达
        bool localizationStatus = srv.response.localization_status;//定位
        LOG_IF(INFO, DEBUG_NODE) << "callSensorStatus  hlsStatus : " << hlsStatus
                  << " , imuStatus : " << imuStatus
                  << " , laserStatus : " << laserStatus
                  << " , localizationStatus : " << localizationStatus;
    } else {
        LOG(ERROR) << "Failed to call service sensor_status ...";
    }
    return result;
}

bool CartographerServiceClient::callReadyCheck() {
    if (!Environment::instance().isRealEnvironment) {
        return true;
    }
    back_charge_msgs::ready_check srv;
    bool result = ready_check.call(srv);
    if (result) {
        bool hlsStatus = srv.response.hls_status;//电机失能
        bool imuStatus = srv.response.imu_status;//imu
        bool laserStatus = srv.response.laser_status;//激光雷达
        bool bumpTriggeredStatus = srv.response.bump_triggered;//后碰撞
        LOG_IF(INFO, DEBUG_NODE) << "callReadyCheck  hlsStatus : " << hlsStatus
                  << " , imuStatus : " << imuStatus
                  << " , laserStatus : " << laserStatus
                  << " , bumpTriggeredStatus : " << bumpTriggeredStatus;
        return hlsStatus && imuStatus && laserStatus && !bumpTriggeredStatus;
    } else {
        LOG(ERROR) << "Failed to call service ready_check ...";
        return result;
    }
}

bool CartographerServiceClient::callStartLocalization() {
    if (!Environment::instance().isRealEnvironment) {
        return true;
    }
    back_charge_msgs::start_localization srv;
    bool result = start_localization.call(srv);
    if (result) {
        bool tfValid = srv.response.tf_valid;
        LOG_IF(INFO, DEBUG_NODE) << "callStartLocalization  tfValid : " << tfValid;
        return tfValid;
    } else {
        LOG(ERROR) << "Failed to call service start_localization ...";
        return result;
    }
}

bool CartographerServiceClient::callStopLocalization() {
    if (!Environment::instance().isRealEnvironment) {
        return true;
    }
    back_charge_msgs::stop_localization srv;
    bool result = stop_localization.call(srv);
    if (result) {
        bool tfValid = srv.response.tf_valid;
        LOG_IF(INFO, DEBUG_NODE) << "callStopLocalization  tfValid : " << tfValid;
        return !tfValid;
    } else {
        LOG(ERROR) << "Failed to call service stop_localization ...";
        return result;
    }
}
