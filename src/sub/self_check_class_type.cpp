/*
    文件名：self_check_class_type.cpp
    作者：Tony Zhao
    描述：本文件为定义自检主线类SelfCheckService中的各个成员函数以及自检消息类SelfCheckStatus的各个成员函数而设立。
    最终定稿日期：2022.2.22
*/
#include "error_log/Error_log.h"
#include "glog/logging.h"
#include "sub/self_check.h"
#include "task/subscribe/async_machine.h"

SelfCheckSubscribe::SelfCheckSubscribe(ros::NodeHandle handle, PubInner pubInner, PubOut pubOut)
        : handle(handle), pubInner(std::move(pubInner)), pubOut(std::move(pubOut)) {
    //camera
    int camera_num = 2;
    for (int i = 1; i <= camera_num; i++) {
        std::string camera_string = "camera";
        camera_string.append(std::to_string(i));
        ros::NodeHandle camera_nh;
        boost::shared_ptr<Camera> camera(new Camera(camera_nh, i));
        cameras_.push_back(camera);
    }


    //laser
    ros::NodeHandle laser_nh;
    boost::shared_ptr<RplidarLaserScan> laser_scan(new RplidarLaserScan(laser_nh));
    laser_scan_ = laser_scan;

    //imu
    ros::NodeHandle imu_nh;
    boost::shared_ptr<IMU> imu(new IMU(imu_nh));
    imu_ = imu;

    //tracked_pose
    ros::NodeHandle pose_nh;
    boost::shared_ptr<TrackedPose> tracked_pose(new TrackedPose(pose_nh));
    tracked_pose_ = tracked_pose;

    //odom
    ros::NodeHandle odom_nh;
    boost::shared_ptr<Odom> odom(new Odom(odom_nh));
    odom_ = odom;

    //bms
    ros::NodeHandle bms_nh;
    boost::shared_ptr<BMS> bms(new BMS(bms_nh));
    bms_ = bms;

    //bump
    ros::NodeHandle bump_nh;
    boost::shared_ptr<Bump> bump(new Bump(bump_nh));
    bump_ = bump;

    //ultraSonic
    ros::NodeHandle ultra_sonic_nh;
    boost::shared_ptr<UltraSonic> ultra_sonic(new UltraSonic(ultra_sonic_nh));
    ultraSonic_ = ultra_sonic;

    ros::NodeHandle nh;
    move_base_error_sub_ = nh.subscribe("/move_base/error_code", 20, &SelfCheckSubscribe::moveBaseErrorCB, this);
    back_charge_error_sub_ = nh.subscribe("/back_charge/error_code", 20, &SelfCheckSubscribe::backChargeErrorCB, this);


    ThreadCreate();
}

SelfCheckSubscribe::~SelfCheckSubscribe() {
}

void SelfCheckSubscribe::ThreadCreate() {
    std::thread SelfCheckProcessThread(&SelfCheckSubscribe::ThreadHandle, this);
    SelfCheckProcessThread.detach();
}

void SelfCheckSubscribe::ThreadHandle() {
    ros::Rate r(1.0);
    while (ros::ok()) {
//        std::cout << " wheel_odom:" << odom_->isValid()
//                  << " bms:" << bms_->isValid()
//                  << " tracked_pose:<< " << tracked_pose_->isTrackedPoseValid() << " and "
//                  << tracked_pose_->isBiasDetectValid() << "  >>"
//                  << "camera1: " << cameras_[0]->isValid()
//                  << " camera2: " << cameras_[1]->isValid()
//                  << " bump:<<" << " 0:" << bump_->is_bump_0_valid() << " 1:" << bump_->is_bump_1_valid() << " 2:"
//                  << bump_->is_bump_2_valid() << " 3:" << bump_->is_bump_3_valid() << ">>"
//                  << " ultrasonic:<<" << " 1:" << ultraSonic_->is_ultra_1_valid() << " 2:"
//                  << ultraSonic_->is_ultra_2_valid() << ">>"
//                  << " imu:" << imu_->isValid()
//                  << " laser_scan:" << laser_scan_->isValid() << std::endl;
        checkEnable();
        if (cameras_[0]->checkEnabled()) {
            //更新后判断是否需要发送
            cameras_[0]->isValid();
            cameras_[1]->isValid();
            if (cameras_[0]->publishFlag()) {
              pubError(CAMERA2_NO_DATA); // down inu
              cameras_[0]->resetPublish();
            }
            if (cameras_[1]->publishFlag()) {
                pubError(CAMERA1_NO_DATA); // up inu
                cameras_[1]->resetPublish();
            }
        }
        //imu
        imu_->isValid();
        if (imu_->publishFlag()){
            pubError(IMU_NO_DATA);
            imu_->resetPublish();
        }
        //odom
        if (odom_->publishFlag()) {
            pubError(ODOM_NO_DATA);
            odom_->resetPublish();
        }
        //检查超声传感器自检功能是否使能
        if (ultraSonic_->check_enabled()) {
            if (ultraSonic_->is_ultra_1_need_publish()) {
                pubError(ULTRASONIC1_ABNORMAL_OVER_30_SECOND);
            }
            if (ultraSonic_->is_ultra_2_need_publish()) {
                pubError(ULTRASONIC2_ABNORMAL_OVER_30_SECOND);
            }
        }
        if (bump_->is_bump_0_need_publish()) {
            pubError(BUMP1_ABNORMAL_OVER_30_SECOND);
            bump_->reset_bump_0_publish_flag();
        }
        if (bump_->is_bump_1_need_publish()) {
            pubError(BUMP2_ABNORMAL_OVER_30_SECOND);
            bump_->reset_bump_1_publish_flag();
        }
        if (bump_->is_bump_2_need_publish()) {
            pubError(BUMP3_ABNORMAL_OVER_30_SECOND);
            bump_->reset_bump_2_publish_flag();
        }
        if (bump_->is_bump_3_need_publish()) {
            pubError(BUMP4_ABNORMAL_OVER_30_SECOND);
            bump_->reset_bump_3_publish_flag();
        }
        if (bms_->publishFlag()) {
            pubError(BMS_HOP);
            bms_->resetPublish();
        }
        if (!tracked_pose_->isBiasDetectValid()) {
            tracked_pose_->resetBiasDetectValid();
            pubError(BIAS_DETECTED);
        }
        if (tracked_pose_->publishFlag()) {
            pubError(TRACKED_POSE_HOP);
            tracked_pose_->resetPublish();
        }

        r.sleep();
    }
}

void SelfCheckSubscribe::moveBaseErrorCB(const std_msgs::Int32ConstPtr &msg) {
    int error_msg = msg->data;
    SelfCheckErrorType error_type;
    switch (error_msg) {
        case 3000:
            error_type = GLOBAL_PLANNER_CREATE_FAILED;
            break;
        case 3001:
            error_type = LOCAL_PLANNER_CREATE_FAILED;
            break;
        case 3002:
            error_type = ILLEGAL_QUATERNION;
            break;
        case 3003:
            error_type = GLOBAL_COST_MAP_GET_FAILED;
            break;
            //太多了不上报了
            // case 3004:
            //     error_type = GET_CURRENT_POSE_FAILED;
            //     break;
            // case 3005:
            //     error_type = GLOBAL_PLAN_FAILED;
            //     break;
        case 3006:
            error_type = SET_LOCAL_PLAN_FAILED;
            break;
            // case 3007:
            //     error_type = LOCAL_CONTROL_FAILED;
            // break;
        case 3010:
            error_type = PLANNING_RECOVERY_FAILED_AND_SKIP;
            break;
        case 3011:
            error_type = LOCAL_CONTROL_FAILED_AND_SKIP;
            break;
        case 3012:
            error_type = OSCILLATION_RECOVERY_FAILED_AND_SKIP;
            break;
        case 3013:
            error_type = SONAR_RECOVERY_FAILED_AND_SKIP;
            break;
        case 3014:
            error_type = PROHIBITION_RECOVERY_FAILED_AND_SKIP;
            break;
        case 3015:
            error_type = SONAR_RECOVERY_TIME_OUT;
            break;
        default:
            return;
    }
    pubError(error_type);


}

void SelfCheckSubscribe::backChargeErrorCB(const std_msgs::StringConstPtr &msg) {
    std::string error_msg = msg->data;
    SelfCheckErrorType error_type;
    if (error_msg == "back_charge_5001") {
        error_type = NOT_AT_BASE_POINT;
    } else if (error_msg == "back_charge_5010") {
        error_type = OUT_STATION_ERROR;
    }
    pubError(error_type);
}

void SelfCheckSubscribe::checkEnable() {
    int machine_code = AsyncMachine::instance().getMachineCode();
    //超声只在工作时检验
    if (machine_code == 10001 || machine_code == 10005 ||
        machine_code == 10008 || machine_code == 10009) {//只在清洁中，转场中，回充中，低电量回充中报
        ultraSonic_->set_enabled(true);
    } else {
        ultraSonic_->set_enabled(false);
    }
    if (!camera_check_enable_) {
        handle.getParam("/node_controller/start_finish", camera_check_enable_);
        if (camera_check_enable_) {
            for (auto &item: cameras_) {
                item->setEnabled(true);
            }
        }
    }
}
