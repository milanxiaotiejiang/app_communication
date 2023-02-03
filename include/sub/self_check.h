/*
    文件名：self_check.h
    作者：Tony Zhao
    描述：本文件为定义自检主线类SelfCheckService以及自检消息类SelfCheckStatus而设立。
    最终定稿日期：2022.2.22
*/
#ifndef SELF_CHECK_H
#define SELF_CHECK_H

#define RAD2DEG(x) ((x) *180. / M_PI)
#define DEG2RAD(dg) ((dg) *M_PI / 180.)


// 引自他方的文件
#include "model/Point.h"
#include "model/SelfCheckErrorType.h"
#include "manager/InternalEventPubManager.h"
#include "nlohmann/json.hpp"
#include "pub/PubInner.h"
#include "pub/PubOut.h"
#include "std_msgs/Int32.h"
#include "ros/ros.h"
#include "sensor_msgs/Image.h"
#include "sensor_msgs/LaserScan.h"
#include "sensor_msgs/PointCloud2.h"
#include "nav_msgs/Odometry.h"
#include "sensor_msgs/Imu.h"
#include "sensor_msgs/Range.h"
#include "std_msgs/String.h"
#include "std_msgs/UInt8MultiArray.h"
#include "std_msgs/Char.h"
#include "std_msgs/Int16.h"
#include "geometry_msgs/PoseStamped.h"


#include <dynamic_reconfigure/Reconfigure.h>
#include <thread>
#include "manager/VersionManager.h"
#include "model/VersionSubscribe.h"
#include "model/SelfCheckParam.h"


static float RANGE_THRESHOLD = 0.15;
static int BATTERY_THRESHOLD = 2;
static double ODOM_THRESHOLD = 0.25;
static double POSE_THRESHOLD = 2.5;

//深度相机自检，只需要检查有没有数据
class Camera {
public:
    Camera(ros::NodeHandle &n, int i) {
        private_nh_ = n;

        index = i;
        if (index == 1) {
            private_nh_.param("image_name", image_name_, std::string("/1/camera/aligned_depth_to_color/image_raw"));
            private_nh_.param("pointcloud_name", pointcloud_name_, std::string("/1/depth/depth2pc"));
        }
        if (index == 2) {
            private_nh_.param("image_name", image_name_, std::string("/2/camera/aligned_depth_to_color/image_raw"));
            private_nh_.param("pointcloud_name", pointcloud_name_, std::string("/2/depth/depth2pc"));
        }

        image_sub_ = private_nh_.subscribe<sensor_msgs::Image>(image_name_, 10, &Camera::ImageCB, this);
        pointcloud_sub_ = private_nh_.subscribe<sensor_msgs::PointCloud2>(pointcloud_name_, 10, &Camera::PointCloudCB,
                                                                          this);
        enabled_ = false;
        valid_ = true;
        last_valid_ = true;
        publish_flag_ = false;
    }

    void ImageCB(const sensor_msgs::ImageConstPtr &depth_msg) {
        last_image_ = depth_msg->header.stamp;
    }

    void PointCloudCB(const sensor_msgs::PointCloud2ConstPtr &pointcloud_msg) {
        last_pointcloud_ = pointcloud_msg->header.stamp;
    }

    bool isValid() {
        ros::Time now = ros::Time::now();
        last_valid_ = valid_;
        valid_ = (now - last_image_ < ros::Duration(5) && now - last_pointcloud_ < ros::Duration(5));
        if(needPublish()){
            setPublish();
        }
        return valid_;
    }

    bool checkEnabled() { return enabled_; }

    //原本是好的变坏了需要发一下
    bool needPublish() { return (last_valid_ && !valid_); }

    void setPublish() { publish_flag_ = true; }

    void resetPublish() { publish_flag_ = false; }

    bool publishFlag() { return publish_flag_; }

    bool setEnabled(bool enabled) {
        enabled_ = enabled;
        //使能时间作为计算是否超时的起点
        last_image_ = ros::Time::now();
        last_pointcloud_ = ros::Time::now();
    }

private:
    ros::NodeHandle private_nh_;
    ros::Subscriber image_sub_;
    ros::Subscriber pointcloud_sub_;
    std::string image_name_;
    std::string pointcloud_name_;
    ros::Time last_image_;
    ros::Time last_pointcloud_;

    bool valid_;
    bool last_valid_;
    bool publish_flag_;

    bool enabled_;

    int index = 0;
};

//激光雷达自检，只需要检查是否有数据
class RplidarLaserScan {
public:
    RplidarLaserScan(ros::NodeHandle &n) {
        private_nh_ = n;
        private_nh_.param("laser_name", laser_name_, std::string("/scan_raw"));
        laser_sub_ = private_nh_.subscribe<sensor_msgs::LaserScan>(laser_name_, 10, &RplidarLaserScan::LaserScanCB,
                                                                   this);
        last_laser_ = ros::Time::now();

        valid_ = true;
        last_valid_ = true;
        publish_flag_ = false;
    }

    void LaserScanCB(const sensor_msgs::LaserScanConstPtr &laser_msg) {
        last_laser_ = ros::Time::now();
    }

    bool isValid() {
        last_valid_ = valid_;
        valid_ = ros::Time::now() - last_laser_ < ros::Duration(5);
        if (needPublish()) {
          setPublish();
        }
        return valid_;
    }

    //原本是好的变坏了需要发一下
    bool needPublish() { return (last_valid_ && !valid_); }

    void setPublish() { publish_flag_ = true; }

    void resetPublish() { publish_flag_ = false; }

    bool publishFlag() { return publish_flag_; }

  private:
    ros::NodeHandle private_nh_;
    ros::Subscriber laser_sub_;
    std::string laser_name_;
    ros::Time last_laser_;
    bool publish_flag_;

    bool valid_;
    bool last_valid_;
};

//imu自检，只需要检查是否有数据
class IMU {
public:
    IMU(ros::NodeHandle &n) {
        private_nh_ = n;
        private_nh_.param("imu_name", imu_name_, std::string("/handsfree/imu"));

        imu_sub_ = private_nh_.subscribe<sensor_msgs::Imu>(imu_name_, 10, &IMU::IMUCB, this);

        last_imu_ = ros::Time::now();

        valid_ = true;
        last_valid_ = true;
        publish_flag_ = false;
    }

    void IMUCB(const sensor_msgs::ImuConstPtr &imu_msg) {
        last_imu_ = ros::Time::now();
    }

    bool isValid() {
        last_valid_ = valid_;
        valid_ = ros::Time::now() - last_imu_ < ros::Duration(5);
        if (needPublish()) {
          setPublish();
        }
        return valid_;
    }

    //原本是好的变坏了需要发一下
    bool needPublish() { return (last_valid_ && !valid_); }

    void setPublish() { publish_flag_ = true; }

    void resetPublish() { publish_flag_ = false; }

    bool publishFlag() { return publish_flag_; }

  private:
    ros::NodeHandle private_nh_;
    ros::Subscriber imu_sub_;
    ros::Subscriber imu_error_pub_;

    std::string imu_name_;
    ros::Time last_imu_;

    bool valid_;
    bool last_valid_;
    bool publish_flag_;
};

class TrackedPose {
public:
    TrackedPose(ros::NodeHandle &n) {
        private_nh_ = n;
        private_nh_.param("tracked_pose_name", tracked_pose_name_, std::string("/tracked_pose"));
        private_nh_.param("bias_detect_name", bias_detect_name_, std::string("/bias_detect"));

        tracked_pose_valid = true;
        tracked_pose_publish_flag_ = false;
        bias_detect_valid = true;

        tracked_pose_sub_ = private_nh_.subscribe<geometry_msgs::PoseStamped>(tracked_pose_name_, 10,
                                                                              &TrackedPose::trackedPoseCB, this);
        bias_detect_sub_ = private_nh_.subscribe<std_msgs::Int16>(bias_detect_name_, 10, &TrackedPose::biasDetectCB,
                                                                  this);

    }

    void trackedPoseCB(const geometry_msgs::PoseStampedConstPtr &msg) {
        double last_tracked_pose_x = last_tracked_pose.pose.position.x;
        double last_tracked_pose_y = last_tracked_pose.pose.position.y;

        double current_tracked_pose_x = msg->pose.position.x;
        double current_tracked_pose_y = msg->pose.position.y;

        //绝对位置两帧之间跳变超过阈值
        if ((abs(last_tracked_pose_x - current_tracked_pose_x) >= POSE_THRESHOLD) ||
            (abs(last_tracked_pose_y - current_tracked_pose_y) >= POSE_THRESHOLD)) {
            if (tracked_pose_valid == true) {
                tracked_pose_valid = false;
                setPublish();
            }
        } else {
            tracked_pose_valid = true;
        }

        //给上一帧赋值
        last_tracked_pose.pose = msg->pose;
    }

    void biasDetectCB(const std_msgs::Int16ConstPtr &msg) {
        if (msg->data == 1) {
            bias_detect_valid = false;
        }
    }

    bool isBiasDetectValid() {
        return bias_detect_valid;
    }

    bool resetBiasDetectValid() {  
        bias_detect_valid = true; 
    }

    bool isTrackedPoseValid() {
        return tracked_pose_valid;
    }

    void setPublish() { tracked_pose_publish_flag_ = true; }

    void resetPublish() { tracked_pose_publish_flag_ = false; }

    bool publishFlag() { return tracked_pose_publish_flag_; }

  private:
    ros::NodeHandle private_nh_;
    ros::Subscriber bias_detect_sub_;
    ros::Subscriber tracked_pose_sub_;

    std::string tracked_pose_name_;
    std::string bias_detect_name_;

    bool tracked_pose_valid;
    bool tracked_pose_publish_flag_;
    bool bias_detect_valid;

    geometry_msgs::PoseStamped last_tracked_pose;

};

//轮式里程计自检，检查里程计是否跳变
class Odom {
public:
    Odom(ros::NodeHandle &n) {
        private_nh_ = n;
        private_nh_.param("odom_name", odom_name_, std::string("/wheel_odom"));
        valid_ = true;
        publish_flag_ = false;
        odom_sub_ = private_nh_.subscribe<nav_msgs::Odometry>(odom_name_, 10, &Odom::OdomCB, this);
        enabled_ = false;
    }

    void OdomCB(const nav_msgs::OdometryConstPtr &odom_msg) {
        //检查是否发生里程计跳变
        double last_odom_pose_x = last_wheel_odom.pose.pose.position.x;
        double last_odom_pose_y = last_wheel_odom.pose.pose.position.y;

        double current_odom_pose_x = odom_msg->pose.pose.position.x;
        double current_odom_pose_y = odom_msg->pose.pose.position.y;
        if(enabled_){
          //里程计两帧之间跳变超过阈值
          if ((abs(last_odom_pose_x - current_odom_pose_x) >= ODOM_THRESHOLD) ||
              (abs(last_odom_pose_y - current_odom_pose_y) >= ODOM_THRESHOLD)) {
            if (valid_ == true) {
              valid_ = false;
              setPublish();
            }
          } else {
            valid_ = true;
          }
        }else{
            valid_ = true;
        }
        last_wheel_odom.pose = odom_msg->pose;
    }

    bool isValid() {
        return valid_;
    }

    void setPublish() { publish_flag_ = true; }

    void resetPublish() { publish_flag_ = false; }

    bool publishFlag() { return publish_flag_; }

    bool setEnabled(bool enable){
        enabled_ = enable;
    }

private:
    ros::NodeHandle private_nh_;
    ros::Subscriber odom_sub_;
    std::string odom_name_;

    bool valid_;
    bool publish_flag_;
    bool enabled_;
    nav_msgs::Odometry last_wheel_odom;
};

class BMS {
public:
    BMS(ros::NodeHandle &n) {
        private_nh_ = n;
        private_nh_.param("battery_name", battery_name_, std::string("/battery_status"));
        battery_valid = true;
        publish_flag_ = false;
        battery_sub_ = private_nh_.subscribe<std_msgs::Char>(battery_name_, 10, &BMS::batteryCB, this);
    }

    void batteryCB(const std_msgs::CharConstPtr &battery_msg) {
        u_char battery = battery_msg->data;
        //check if go on battery jump
        if (abs(last_battery - battery) >= BATTERY_THRESHOLD) {
            if (battery_valid == true) {
                battery_valid = false;
                setPublish();
            }
        } else {
            battery_valid = true;
        }
        last_battery = battery;
    }

    bool isValid() {
        return battery_valid;
    }

    void setPublish() { publish_flag_ = true; }

    void resetPublish() { publish_flag_ = false; }

    bool publishFlag() { return publish_flag_; }

  private:
    ros::NodeHandle private_nh_;
    ros::Subscriber battery_sub_;
    std::string battery_name_;

    u_char last_battery;
    bool publish_flag_;

    bool battery_valid;
};

class Bump {
public:
    Bump(ros::NodeHandle &n) {
        private_nh_ = n;
        private_nh_.param("bump_name", bump_name_, std::string("/mrrobot/bump_sensor"));

        bump_0_valid = bump_1_valid = bump_2_valid = bump_3_valid = true;
        bump_0_publish_flag_ = bump_1_publish_flag_ = bump_2_publish_flag_ =
        bump_3_publish_flag_ = false;
        bump_sensor_trigger_time_0 = bump_sensor_trigger_time_1 = bump_sensor_trigger_time_2 = bump_sensor_trigger_time_3 = ros::Time::now();

        bump_sub_ = private_nh_.subscribe<std_msgs::UInt8MultiArray>(bump_name_, 10, &Bump::bumpCB, this);
    }

    void bumpCB(const std_msgs::UInt8MultiArrayConstPtr &bump_msg) {
        uint temp_bump_0 = bump_msg->data[0];
        uint temp_bump_1 = bump_msg->data[1];
        uint temp_bump_2 = bump_msg->data[2];
        uint temp_bump_3 = bump_msg->data[3];

        if (temp_bump_0 == 0) {
            bump_sensor_trigger_time_0 = ros::Time::now();
            bump_0_valid = true;
        }
        if (temp_bump_1 == 0) {
            bump_sensor_trigger_time_1 = ros::Time::now();
            bump_1_valid = true;
        }
        if (temp_bump_2 == 0) {
            bump_sensor_trigger_time_2 = ros::Time::now();
            bump_2_valid = true;
        }
        if (temp_bump_3 == 0) {
            bump_sensor_trigger_time_3 = ros::Time::now();
            bump_3_valid = true;
        }

        double current_time_sec = ros::Time::now().toSec();

        if (temp_bump_0 == 1) {
            if ((current_time_sec - bump_sensor_trigger_time_0.toSec()) > 30.0) {
                //bump trigger error
                if (bump_0_valid) {
                  bump_0_publish_flag_ = true;
                  bump_0_valid = false;
                
                }
            }
        }

        if (temp_bump_1 == 1) {
            if ((current_time_sec - bump_sensor_trigger_time_1.toSec()) > 30.0) {
                //bump trigger error
                if (bump_1_valid) {
                  bump_1_publish_flag_ = true;
                  bump_1_valid = false;
                }
            }
        }

        if (temp_bump_2 == 1) {
            if ((current_time_sec - bump_sensor_trigger_time_2.toSec()) > 30.0) {
                //bump trigger error
                if (bump_2_valid) {
                  bump_2_publish_flag_ = true;
                  bump_2_valid = false;
                }
            }
        }

        if (temp_bump_3 == 1) {
            if ((current_time_sec - bump_sensor_trigger_time_3.toSec()) > 30.0) {
                //bump trigger error
                if (bump_3_valid) {
                  bump_3_publish_flag_ = true;
                  bump_3_valid = false;
                }
            }
        }
    }

    bool is_bump_0_valid() {
        return bump_0_valid;
    }

    bool is_bump_0_need_publish() { return bump_0_publish_flag_; }

    void reset_bump_0_publish_flag() { bump_0_publish_flag_ = false; }

    bool is_bump_1_valid() {
        return bump_1_valid;
    }

    bool is_bump_1_need_publish() { return bump_1_publish_flag_; }
    void reset_bump_1_publish_flag() { bump_1_publish_flag_ = false; }
    bool is_bump_2_valid() {
        return bump_2_valid;
    }

    bool is_bump_2_need_publish() { return bump_2_publish_flag_; }
    void reset_bump_2_publish_flag() { bump_2_publish_flag_ = false; }
    bool is_bump_3_valid() {
        return bump_3_valid;
    }

    bool is_bump_3_need_publish() { return bump_3_publish_flag_; }
    void reset_bump_3_publish_flag() { bump_3_publish_flag_ = false; }

  private:
    ros::NodeHandle private_nh_;
    ros::Subscriber bump_sub_;

    std::string bump_name_;

    ros::Time bump_sensor_trigger_time_0;
    ros::Time bump_sensor_trigger_time_1;
    ros::Time bump_sensor_trigger_time_2;
    ros::Time bump_sensor_trigger_time_3;

    bool bump_0_valid;
    bool bump_1_valid;
    bool bump_2_valid;
    bool bump_3_valid;

    bool bump_0_publish_flag_;
    bool bump_1_publish_flag_;
    bool bump_2_publish_flag_;
    bool bump_3_publish_flag_;
};

class UltraSonic {
public:
    UltraSonic(ros::NodeHandle &n) {
        private_nh_ = n;
        private_nh_.param("ul_sensor_name_1", ul_sensor_name_1, std::string("/mrrobot/ul_senser1"));
        private_nh_.param("ul_sensor_name_2", ul_sensor_name_2, std::string("/mrrobot/ul_senser2"));

        ul_sensor_trigger_time_1 = ul_sensor_trigger_time_2 = ros::Time::now();

        ultra_1_is_valid = ultra_2_is_valid = true;
        ultra_1_need_publish = ultra_2_need_publish = false;
        ul_sensor_sub_1 = private_nh_.subscribe<sensor_msgs::Range>(ul_sensor_name_1, 10, &UltraSonic::ultrasonicCB_1,
                                                                    this);
        ul_sensor_sub_2 = private_nh_.subscribe<sensor_msgs::Range>(ul_sensor_name_2, 10, &UltraSonic::ultrasonicCB_2,
                                                                    this);
    }

    void ultrasonicCB_1(const sensor_msgs::RangeConstPtr &ul_msg_1) {
        if (enabled_) {
            float temp_range = ul_msg_1->range;
            //        std::cout<<"temp_range1  "<<temp_range<<std::endl;
            if (temp_range >= RANGE_THRESHOLD) {
                ul_sensor_trigger_time_1 = ros::Time::now();
                ultra_1_is_valid = true;
            }

            if (temp_range < RANGE_THRESHOLD) {
                if ((ul_msg_1->header.stamp.toSec() -
                     ul_sensor_trigger_time_1.toSec()) > 30.0) {
                    // ul_sensor_1_error
                    if (ultra_1_is_valid){
                        ultra_1_need_publish = true;
                        ultra_1_is_valid = false;
                    }

                }
            }
        } else {
          ultra_1_is_valid = true;
        }
    }

    void ultrasonicCB_2(const sensor_msgs::RangeConstPtr &ul_msg_2) {
        if (enabled_) {
            float temp_range = ul_msg_2->range;
            if (temp_range >= RANGE_THRESHOLD) {
                ul_sensor_trigger_time_2 = ros::Time::now();
                ultra_2_is_valid = true;
            }

            if (temp_range < RANGE_THRESHOLD) {
                std::cout << (ul_msg_2->header.stamp.toSec() -
                              ul_sensor_trigger_time_2.toSec())
                          << std::endl;
                if ((ul_msg_2->header.stamp.toSec() -
                     ul_sensor_trigger_time_2.toSec()) > 30.0) {
                    // ul_sensor_2_error
                    if (ultra_2_is_valid){
                      ultra_2_need_publish = true;
                      ultra_2_is_valid = false;
                    }

                }
            }
        } else {
          ultra_2_is_valid = true;
        }
    }

    bool is_ultra_1_valid() {
        return ultra_1_is_valid;
    }

    bool is_ultra_1_need_publish() {
         return (ultra_1_need_publish ); 
    }
    void reset_ultra_1_need_publish() { ultra_1_need_publish = false; }

    bool is_ultra_2_valid() {
        return ultra_2_is_valid;
    }

    bool is_ultra_2_need_publish() {
      return (ultra_2_need_publish);
    }

    void reset_ultra_2_need_publish() { ultra_2_need_publish = false; }

    bool check_enabled() {
        return enabled_;
    }

    bool set_enabled(bool enabled) {
        enabled_ = enabled;
    }

private:
    ros::NodeHandle private_nh_;

    ros::Subscriber ul_sensor_sub_1;
    ros::Subscriber ul_sensor_sub_2;

    std::string ul_sensor_name_1;
    std::string ul_sensor_name_2;

    ros::Time ul_sensor_trigger_time_1;
    ros::Time ul_sensor_trigger_time_2;


    bool ultra_1_is_valid;
    bool ultra_2_is_valid;

    bool ultra_1_need_publish;
    bool ultra_2_need_publish;

    bool enabled_;
};

class SelfCheckSubscribe {
    /*
    本类为自检主线程类，创建该类对象即可进行自检。
    */
public:
    SelfCheckSubscribe(ros::NodeHandle handle, PubInner pubinner, PubOut pubout);// 构造函数，与主节点衔接，进入自检主线程。
    ~SelfCheckSubscribe();


private:
    //add new 2212B
    std::vector<boost::shared_ptr<Camera>> cameras_;
    boost::shared_ptr<RplidarLaserScan> laser_scan_;
    boost::shared_ptr<IMU> imu_;
    boost::shared_ptr<TrackedPose> tracked_pose_;
    boost::shared_ptr<Odom> odom_;
    boost::shared_ptr<BMS> bms_;
    boost::shared_ptr<Bump> bump_;
    boost::shared_ptr<UltraSonic> ultraSonic_;
    ros::Subscriber move_base_error_sub_;
    ros::Subscriber back_charge_error_sub_;
    ////////////////////////////////////////////////////////////////////
    ros::NodeHandle handle;
    PubInner pubInner;
    PubOut pubOut;

    bool camera_check_enable_{false};//用于确认银牛是否启动

public:

    void ThreadCreate();                     // 用于创建线程
    void ThreadHandle();                     // 线程中执行的内容，即自检。
    void pubError(SelfCheckErrorType error_type) {
        internal_event::InternalEventPubManager::get_instance()->pubAlarm(error_type);
    }

    //规划模块自检报告回调函数
    void moveBaseErrorCB(const std_msgs::Int32ConstPtr &msg);

    //回充模块自检报告回调函数
    void backChargeErrorCB(const std_msgs::StringConstPtr &msg);

    //检查各传感器是否需要自检
    void checkEnable();
};


#endif