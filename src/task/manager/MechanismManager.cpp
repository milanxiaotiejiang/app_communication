//
// Created by admin1 on 22-11-25.
//

#include "task/manager/MechanismManager.h"
#include "manager/PublishInnerManager.h"
#include "task/subscribe/zoo_inner_status.h"
#include "glog/logging.h"

void MechanismManager::resetWorkStatus() {
    LOG(INFO) << "MechanismManager : 收起清洁机构.";
    std_msgs::Int16 sweep_status;
//    if (ZooInnerStatus::instance().getSweepStatus() != 0 && ZooInnerStatus::instance().getSweepStatus() != -1) {
    sweep_status.data = 0;
    PublishInnerManager::instance().publishSweepMode(sweep_status);
//    }

    std_msgs::Int16 mop_status;
//    if (ZooInnerStatus::instance().getMopStatus() != 0 && ZooInnerStatus::instance().getMopStatus() != -1) {
    mop_status.data = 0;
    PublishInnerManager::instance().publishMopMode(mop_status);
//    }

    std_msgs::Int16 vacuum_status;
//    if (ZooInnerStatus::instance().getVacuumStatus() != 0 && ZooInnerStatus::instance().getVacuumStatus() != -1) {
    vacuum_status.data = 0;
    PublishInnerManager::instance().publishVacuumMode(vacuum_status);
//    }

    std_msgs::Int16 push_status;
//    if (ZooInnerStatus::instance().getPushStatus() != 0 && ZooInnerStatus::instance().getPushStatus() != -1) {
    push_status.data = 0;
    PublishInnerManager::instance().publishPushMode(push_status);
//    }

    std_msgs::Int16 aromatherapy_status;
//    if (ZooInnerStatus::instance().getAromStatus() != 0 && ZooInnerStatus::instance().getAromStatus() != -1) {
    aromatherapy_status.data = 0;
    PublishInnerManager::instance().publishAromStatus(aromatherapy_status);
//    }

    std_msgs::Int16 disinfect_status;
}

void MechanismManager::controlWorkStatus(const WorkStatus &workStatus) {
    LOG(INFO) << "MechanismManager : 打开清洁机构.";
    //扫
    std_msgs::Int16 sweep_status;
    if (workStatus.getSweepStatus() >= 0 && workStatus.getSweepStatus() <= 2) {
        if (workStatus.getSweepStatus() != ZooInnerStatus::instance().getSweepStatus()) {
            sweep_status.data = ((int16_t) workStatus.getSweepStatus());
            PublishInnerManager::instance().publishSweepMode(sweep_status);
        }
    }

    //拖
    std_msgs::Int16 mop_status;
    if (workStatus.getMopStatus() >= 0 && workStatus.getMopStatus() <= 2) {
        if (workStatus.getMopStatus() != ZooInnerStatus::instance().getMopStatus()) {
            mop_status.data = ((int16_t) workStatus.getMopStatus());
            PublishInnerManager::instance().publishMopMode(mop_status);
        }
    }

    //吸
    std_msgs::Int16 vacuum_status;
    if (workStatus.getVacuumStatus() >= 0 && workStatus.getVacuumStatus() <= 2) {
        if (workStatus.getVacuumStatus() != ZooInnerStatus::instance().getVacuumStatus()) {
            vacuum_status.data = ((int16_t) workStatus.getVacuumStatus());
            PublishInnerManager::instance().publishVacuumMode(vacuum_status);
        }
    }

    //尘推
    std_msgs::Int16 push_status;
    if (workStatus.getPushStatus() >= 0 && workStatus.getPushStatus() <= 2) {
        if (workStatus.getPushStatus() != ZooInnerStatus::instance().getPushStatus()) {
            push_status.data = ((int16_t) workStatus.getPushStatus());
            if (push_status.data == 1) {
                push_status.data = 2;
            }
            PublishInnerManager::instance().publishPushMode(push_status);
        }
    }

    //香薰
    std_msgs::Int16 aromatherapy_status;
    if (workStatus.getAromatherapyStatus() >= 0 && workStatus.getAromatherapyStatus() <= 3) {
        if (workStatus.getAromatherapyStatus() != ZooInnerStatus::instance().getAromStatus()) {
            aromatherapy_status.data = ((int16_t) workStatus.getAromatherapyStatus());
            PublishInnerManager::instance().publishAromStatus(aromatherapy_status);
        }
    }

    std_msgs::Int16 disinfect_status;
}

void MechanismManager::forceControlWorkStatus(const WorkStatus &workStatus) {
    LOG(INFO) << "MechanismManager : 强制打开清洁机构.";
    //扫
    std_msgs::Int16 sweep_status;
    sweep_status.data = ((int16_t) workStatus.getSweepStatus());
    PublishInnerManager::instance().publishSweepMode(sweep_status);

    //拖
    std_msgs::Int16 mop_status;
    mop_status.data = ((int16_t) workStatus.getMopStatus());
    PublishInnerManager::instance().publishMopMode(mop_status);

    //吸
    std_msgs::Int16 vacuum_status;
    vacuum_status.data = ((int16_t) workStatus.getVacuumStatus());
    PublishInnerManager::instance().publishVacuumMode(vacuum_status);

    //尘推
    std_msgs::Int16 push_status;
    push_status.data = ((int16_t) workStatus.getPushStatus());
    if (push_status.data == 1) {
        push_status.data = 2;
    }
    PublishInnerManager::instance().publishPushMode(push_status);

    //香薰
    std_msgs::Int16 aromatherapy_status;
    aromatherapy_status.data = ((int16_t) workStatus.getAromatherapyStatus());
    PublishInnerManager::instance().publishAromStatus(aromatherapy_status);
    std_msgs::Int16 disinfect_status;
}

void MechanismManager::enterManualControl() {
    std_msgs::Int8 map_start;
    map_start.data = 2;
    PublishInnerManager::instance().publishKnobTask(map_start);
}

void MechanismManager::quitManualControl() {
    std_msgs::Int8 map_start;
    map_start.data = 0;
    PublishInnerManager::instance().publishKnobTask(map_start);
}

void MechanismManager::openHotWind() {
    LOG(INFO) << "MechanismManager : 开启热风烘干.";
    std_msgs::Int16 msg;
    msg.data = 1;
    PublishInnerManager::instance().publishSelfClean(msg);
}

void MechanismManager::closeHotWind() {
    LOG(INFO) << "MechanismManager : 关闭热风烘干.";
    std_msgs::Int16 msg;
    msg.data = 0;
    PublishInnerManager::instance().publishSelfClean(msg);
}
