//
// Created by admin1 on 22-11-25.
//

#include "task/subscribe/ZooRobotStatusSubscribe.h"
#include "task/subscribe/zoo_inner_status.h"
#include "task/subscribe/async_machine.h"
#include "model/ShowWorkStatus.h"
#include "net/base/VersionSubscribe.h"
#include "manager/PublishOutManager.h"
#include "task/manager/SwitchModePublish.h"
#include "task/manager/NativeSystemManager.h"
#include "clean_history/CleanHistoryCenter.h"
#include "manager/InternalEventPubManager.h"

const int WORK_STATUS_VERSION = 1;
const int KNOB_STATUS_VERSION = 1;

ZooRobotStatusSubscribe::ZooRobotStatusSubscribe(ros::NodeHandle handle)
        : handle(handle) {
//    ZooInnerStatus::instance().setNeedSleep(true);
    sub_motor_error_ = handle.subscribe("/mrrobot/push_error", 10, &ZooRobotStatusSubscribe::motorErrorCallback, this);
    sub_hls_error_ = handle.subscribe("/mrrobot/hls_error", 10, &ZooRobotStatusSubscribe::hlsErrorCallback, this);
    sub_wet_mop_error = handle.subscribe("/mrrobot/wet_mop_error", 10, &ZooRobotStatusSubscribe::wetMopErrorCallback,
                                         this);
    sub_laser_error_ = handle.subscribe("/lidar/restart", 10, &ZooRobotStatusSubscribe::laserErrorCallback, this);

    if (!Environment::instance().isRealEnvironment) {
        std::thread moveBaseThread([]() {
            sleep(5);
            NodeControl::instance().emulate();
            int last_machine_code = 10006;
            long ii = 0;
            while (1) {
                sleep(1);
                NativeSystemManager::instance().urgencyStop(ZooInnerStatus::instance().getUrgencyStopStatus());
                long current_execute_time = clean_history_db::CleanHistoryCenter::instance().getCurrentCleanTime();
                WorkStatus workStatus(0, 0, 0, 0, 0, 0);
                int machineCode = AsyncMachine::instance().getMachineCode();
                if (machineCode != last_machine_code) {
                    internal_event::InternalEventPubManager::get_instance()->workStatusUpdate(machineCode);
                }
                last_machine_code = machineCode;
                std::string machineMessage = AsyncMachine::instance().getMachineMessage(machineCode);
                auto status = ShowWorkStatus(ZooInnerStatus::instance().getRsoc(), 28, 72,
                                             workStatus,
                                             machineMessage, machineCode,
                                             ZooInnerStatus::instance().getUrgencyStopStatus(),
                                             current_execute_time,
                                             ZooInnerStatus::instance().getIsCharging(),
                                             ZooInnerStatus::instance().getAromStatus());
                VersionSubscribe<ShowWorkStatus> statusResponse(1, status);
                PublishOutManager::instance().publishStatus(statusResponse);

                if (ZooInnerStatus::instance().getNeedSleep() && ZooInnerStatus::instance().getIsCharging()) {
                    SwitchModePublish::instance().publish();
                    ZooInnerStatus::instance().setNeedSleep(false);
                }

            }
        });
        moveBaseThread.detach();
    }
}

ZooRobotStatusSubscribe::~ZooRobotStatusSubscribe() {

}

//电机堵转
void ZooRobotStatusSubscribe::motorErrorCallback(const std_msgs::Int32 &motor_error) {
    NativeSystemManager::instance().motorErrorEvent(motor_error.data);
}

//湿拖堵转
void ZooRobotStatusSubscribe::wetMopErrorCallback(const std_msgs::Int32 &motor_error) {
    NativeSystemManager::instance().wetMopErrorEvent(motor_error.data);
}

//底盘电机失能
void ZooRobotStatusSubscribe::hlsErrorCallback(const std_msgs::Int32 &hls_error) {
    NativeSystemManager::instance().hlsErrorEvent(hls_error.data);
}

//雷达故障
void ZooRobotStatusSubscribe::laserErrorCallback(const std_msgs::String &laser_error) {
    NativeSystemManager::instance().laserErrorEvent(laser_error.data);
}