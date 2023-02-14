//
// Created by admin1 on 22-11-25.
//

#include "task/subscribe/ZooRobotStatusSubscribe.h"
#include "task/subscribe/zoo_inner_status.h"
#include "task/subscribe/async_machine.h"
#include "model/ShowWorkStatus.h"
#include "model/VersionSubscribe.h"
#include "manager/PublishOutManager.h"
#include "task/manager/SwitchModePublish.h"
#include "task/manager/NativeSystemManager.h"
#include "clean_history/CleanHistoryCenter.h"
#include "manager/InternalEventPubManager.h"

const int WORK_STATUS_VERSION = 1;
const int KNOB_STATUS_VERSION = 1;

ZooRobotStatusSubscribe::ZooRobotStatusSubscribe(ros::NodeHandle handle)
        : handle(handle) {
    isFirstSwitchMode = true;
    sub_robot_status_ = handle.subscribe("/robot_status_inner", 1, &ZooRobotStatusSubscribe::subscribeCallback, this);
    sub_motor_error_ = handle.subscribe("/mrrobot/push_error", 10, &ZooRobotStatusSubscribe::motorErrorCallback, this);
    sub_hls_error_ = handle.subscribe("/mrrobot/hls_error_", 10, &ZooRobotStatusSubscribe::hlsErrorCallback, this);
    sub_laser_error_ = handle.subscribe("/lidar/restart", 10, &ZooRobotStatusSubscribe::laserErrorCallback, this);
}

ZooRobotStatusSubscribe::~ZooRobotStatusSubscribe() {

}

void ZooRobotStatusSubscribe::subscribeCallback(const zoo_bringup::robot_status &robot_status) {
    auto sweep_status = robot_status.sweep_status;
    auto mop_status = robot_status.mop_status;
    auto vacuum_status = robot_status.vacuum_status;
    auto push_status = robot_status.push_status;
    auto self_clean_status = robot_status.self_clean_status;
    auto RSOC = robot_status.RSOC;
    auto tank_status = robot_status.tank_status;
    auto urgency_stop_status = robot_status.urgency_stop_status;
    auto drawer_status = robot_status.drawer_status;
    auto is_charging = robot_status.is_charging;
    auto arom_status = robot_status.arom_status;
    auto knob_available = robot_status.knob_available;
    auto knob_task = robot_status.knob_task;
    auto clean_water_level = robot_status.clean_water_level;
    auto dirty_water_level = robot_status.dirty_water_level * 100;


    ZooInnerStatus::instance().setSweepStatus(sweep_status);
    ZooInnerStatus::instance().setMopStatus(mop_status);
    ZooInnerStatus::instance().setPushStatus(push_status);
    ZooInnerStatus::instance().setSelfCleanStatus(self_clean_status);
    ZooInnerStatus::instance().setVacuumStatus(vacuum_status);
    ZooInnerStatus::instance().setRsoc(RSOC);
    ZooInnerStatus::instance().setTankStatus(tank_status);
    ZooInnerStatus::instance().setUrgencyStopStatus(urgency_stop_status);
    ZooInnerStatus::instance().setDrawerStatus(drawer_status);
    ZooInnerStatus::instance().setIsCharging(is_charging);
    ZooInnerStatus::instance().setAromStatus(arom_status);
    ZooInnerStatus::instance().setKnobAvailable(knob_available);
    ZooInnerStatus::instance().setKnobTask(knob_task);
    ZooInnerStatus::instance().setCleanWaterLevel(clean_water_level);
    ZooInnerStatus::instance().setDirtyWaterLevel(dirty_water_level);

    NativeSystemManager::instance().urgencyStop(urgency_stop_status);

    if (is_charging && urgency_stop_status) {
        NativeSystemManager::instance().urgencyStopAndCharge();
    }

    if (!is_charging && RSOC < 10) {
        NativeSystemManager::instance().lowBatteryToBackBase();
    }


    //如果此时湿拖托头下放
    if (ZooInnerStatus::instance().getMopStatus() == 1) {
        //清水箱空或者污水箱满
        int mode = 0;
        if (clean_water_level == 0) {
            mode += 1;
        } else if (dirty_water_level == 100) {
            mode += 2;
        }
        if (mode > 0) {
            NativeSystemManager::instance().waterLevelToBackBase(mode);
        }
    }

    //如果此时开启了扫吸模式
    if (ZooInnerStatus::instance().getVacuumStatus() == 1) {
        //污水箱满
        int mode = 0;
        if (dirty_water_level == 100) {
            mode += 2;
        }
        if (mode > 0) {
            NativeSystemManager::instance().waterLevelToBackBase(mode);
        }
    }

    //int vacuum_status = 0;
    int disinfect_status = 0;
    WorkStatus workStatus(sweep_status, mop_status, vacuum_status,
                          push_status, arom_status, disinfect_status);


    int machineCode = AsyncMachine::instance().getMachineCode();
    if (machineCode != last_machine_code_) {
        internal_event::InternalEventPubManager::get_instance()->workStatusUpdate(machineCode);
    }
    last_machine_code_ = machineCode;
    std::string machineMessage = AsyncMachine::instance().getMachineMessage(machineCode);
    long current_execute_time = clean_history_db::CleanHistoryCenter::instance().getCurrentCleanTime();
    auto status = ShowWorkStatus(ZooInnerStatus::instance().getRsoc(), ZooInnerStatus::instance().getCleanWaterLevel(),
                                 ZooInnerStatus::instance().getDirtyWaterLevel(),
                                 workStatus,
                                 machineMessage, machineCode,
                                 ZooInnerStatus::instance().getUrgencyStopStatus(),
                                 current_execute_time,
                                 ZooInnerStatus::instance().getIsCharging(),
                                 ZooInnerStatus::instance().getAromStatus());
    VersionSubscribe<ShowWorkStatus> statusResponse(1, status);
    PublishOutManager::instance().getPubOut()->publishStatus(statusResponse);

    isFirstSwitchMode = ZooInnerStatus::instance().getIsFirstSwitchMode();

    if (isFirstSwitchMode && is_charging) {
        SwitchModePublish::instance().publish();
        ZooInnerStatus::instance().setIsFirstSwitchMode(false);
    }
    isFirstSwitchMode = false;
}

void ZooRobotStatusSubscribe::pubMaterial() const {
    MaterialDuration soft_brush(SOFT_BRUSH_EXPECTED_DURATION, 1000);
    MaterialDuration carpet_brush(CARPET_BRUSH_EXPECTED_DURATION, 1001);
    MaterialDuration push_brush(PUSH_BRUSH_EXPECTED_DURATION, 1002);
    MaterialDuration fan_filter(FAN_FILTER_EXPECTED_DURATION, 1003);
    MaterialStatus materialStatus(soft_brush, carpet_brush, push_brush, fan_filter);
    //回复，带参数，包括分配的id
    VersionSubscribe<MaterialStatus> materialResponse(MATERIAL_STATUS_VERSION, materialStatus);
    PublishOutManager::instance().getPubOut()->publishMaterialStatus(materialResponse);
}

void ZooRobotStatusSubscribe::pubKnob(const zoo_bringup::robot_status &robot_status) const {
    bool knob_available = robot_status.knob_available;
    auto knob_task = robot_status.knob_task;
    KnobStatus knobStatus;
    knobStatus.setIsAvailable(knob_available);
    VersionSubscribe<KnobStatus> knobResponse(WORK_STATUS_VERSION, knobStatus);
    PublishOutManager::instance().getPubOut()->publishKnob(knobResponse);
}

//电机堵转
void ZooRobotStatusSubscribe::motorErrorCallback(const std_msgs::Int32ConstPtr &motor_error) {
    NativeSystemManager::instance().motorErrorEvent(motor_error->data);
}

//雷达故障
void ZooRobotStatusSubscribe::laserErrorCallback(const std_msgs::StringConstPtr &laser_error) {
    NativeSystemManager::instance().laserErrorEvent(laser_error->data);
}


//底盘电机失能
void ZooRobotStatusSubscribe::hlsErrorCallback(const std_msgs::Int32ConstPtr &hls_error) {
  NativeSystemManager::instance().hlsErrorEvent(hls_error->data);
}