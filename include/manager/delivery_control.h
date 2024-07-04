//
// Created by noodles on 24-7-2.
//

#ifndef APP_COMMUNICATION_DELIVERY_CONTROL_H
#define APP_COMMUNICATION_DELIVERY_CONTROL_H

#include <geometry_msgs/Twist.h>
#include <apriltag_ros/AprilTagDetectionArray.h>

#include <vector>
#include <ros/node_handle.h>
#include "task/RealBlock.h"

#include <mutex>
#include <condition_variable>
#include <nav_msgs/Odometry.h>
#include "future/thread_pool.h"

class DeliveryControlManager {
private:
    DeliveryControlManager() = default;

    DeliveryControlManager(DeliveryControlManager &) = delete;

    DeliveryControlManager &operator=(const DeliveryControlManager &) = delete;

public:
    ~DeliveryControlManager() {
    };

    static auto &instance() {
        static DeliveryControlManager obj;
        return obj;
    }


    enum DeliveryError {
        NoDeliveryError,
        DeliveryMoveError,
        DeliveryCalibrationError,
        DeliveryDistinguishError,
        DeliveryDeliveryError,
    };

    using DeliveryFailCallback = std::function<void(DeliveryError error)>;

private:

    enum DeliveryState {
        NONE,
        TAKE,
        MOVE,               //移动
        CALIBRATION,        //校准
        DISTINGUISH,        //识别
        DELIVERY,           //配送
        OVER                //完成
    };

    enum ArriveState {
        ArriveMove,
        ArriveCalibration,
        ArriveDistinguish,
    };

//    enum TagDetectionState {
//        TagDetectionStateNone,
//        TagDetectionStateDetected,    // none -> into 从初始状态到已经检测到对应的tag时的状态
//        TagDetectionStateNotDetected,     // none -> out 从初始状态到未检测到对应的tag时的状态
//        TagDetectionStateReached,    // into -> over 从已经检测到对应的tag到已经到达对应的tag时的状态
//    };


    ros::Subscriber tag_sub_;
    ros::Subscriber odom_sub_;

    ros::Publisher cmd_vel_pub_;
    ros::Publisher up_pub_;

    RealBlock realBlock;
    std::deque<RealPoint> plannerQueue;
    RealPoint currentPoint;

    async::ThreadPool pool_;

    std::mutex point_mutex_;
    std::condition_variable point_condition_variable_;
    std::atomic<DeliveryState> deliveryState;

//    std::mutex move_mutex_;
//    std::condition_variable move_condition_variable_;
//    bool move_triggered_;

    DeliveryError deliveryError;
    ArriveState arriveState;

    bool record_detection_;                                 // 是否开启接受消息

//    apriltag_ros::AprilTagDetection current_detection_;
//    bool detection_received_;


    nav_msgs::Odometry current_odom_;
    bool odom_received_;

//    TagDetectionState tagDetectionState = TagDetectionStateNone;

//    double start_x_;
//    double start_y_;

    ros::Timer move_timer_;

    DeliveryFailCallback mDeliveryFailCallback;

    void tagDetectionsCallback(const apriltag_ros::AprilTagDetectionArray::ConstPtr &msg);

    void odomCallback(const nav_msgs::Odometry::ConstPtr &msg);

    void move_timer_fun(const ros::TimerEvent &event);

    [[noreturn]] void point_circulation_thread_func();

    [[noreturn]] void move_towards_tag_thread_func();

    void goError(DeliveryError error);

    void doTake();

    void doMove();

    void doCalibration();

    void doDistinguish();

    void doDelivery();

    void doOver();

public:
    void initialize(ros::NodeHandle handle);

    void setDeliveryCallback(DeliveryFailCallback callback);

    void completeCirculation(bool arrive);

    void handleFlow(const RealBlock &block);

};


#endif //APP_COMMUNICATION_DELIVERY_CONTROL_H
