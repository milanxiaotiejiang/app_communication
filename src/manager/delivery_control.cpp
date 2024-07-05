//
// Created by noodles on 24-7-2.
//

#include <tbb/compat/thread>
#include <utility>
#include <tf/LinearMath/Transform.h>
#include <tf/transform_datatypes.h>
#include "manager/delivery_control.h"
#include "simulation.h"
#include "BaseThrowable.h"
#include "task/point_planner.h"

void DeliveryControlManager::initialize(ros::NodeHandle nh) {

    tag_sub_ = nh.subscribe("/tag_detections", 10, &DeliveryControlManager::tagDetectionsCallback, this);
    if (Environment::instance().isRealEnvironment) {
        odom_sub_ = nh.subscribe("/odom_app", 10, &DeliveryControlManager::odomCallback, this);
    } else {
        odom_sub_ = nh.subscribe("/odom", 10, &DeliveryControlManager::odomCallback, this);
    }

    cmd_vel_pub_ = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
    up_pub_ = nh.advertise<std_msgs::Int32>("/up_pub_", 1);

    std::thread point_circulation_thread(&DeliveryControlManager::point_circulation_thread_func, this);
    point_circulation_thread.detach();

    std::thread move_towards_tag_thread(&DeliveryControlManager::move_towards_tag_thread_func, this);
    move_towards_tag_thread.detach();

    move_timer_ = nh.createTimer(ros::Duration(0.1), &DeliveryControlManager::move_timer_fun, this, false, false);
}

void DeliveryControlManager::tagDetectionsCallback(
        const apriltag_ros::AprilTagDetectionArray::ConstPtr &msg) {
    aprilTagDetectionArray = *msg;
    april_tag_detection_received_ = true;

    // 等待检测标志位开启
    if (record_detection_) {

        int target_tag_id_ = currentPoint.deliveryVo.getTag();
        bool has_target_tag = false;

        for (const auto &detection: msg->detections) {

            for (size_t j = 0; j < detection.id.size(); ++j) {
                int tag_id = detection.id[j];
                double tag_size = detection.size[j];
                const auto &position = detection.pose.pose.pose.position;
                const auto &orientation = detection.pose.pose.pose.orientation;

                LOG(INFO) << "Detected tag ID: " << tag_id;
                LOG(INFO) << "Tag size: " << tag_size;
                LOG(INFO) << "Tag position: [x: " << position.x << ", y: " << position.y << ", z: " << position.z
                          << "]";
                LOG(INFO) << "Tag orientation: [x: " << orientation.x << ", y: " << orientation.y << ", z: "
                          << orientation.z << ", w: " << orientation.w << "]";
            }

            // 单个目标才能使用
            if (detection.id.size() == 1) {
                auto detected_tag_id = detection.id[0];

                if (target_tag_id_ == detected_tag_id) {
//                    current_detection_ = detection;

                    const auto &position = detection.pose.pose.pose.position;
                    const auto &orientation = detection.pose.pose.pose.orientation;

                    // 相机相对于机器人的固定变换，位于机器人前方0.1米，高度0.2米
                    tf::Transform camera_to_base;
                    camera_to_base.setOrigin(tf::Vector3(0.1, 0.0, 0.2));
                    camera_to_base.setRotation(tf::Quaternion(0, 0, 0, 1));

                    // 二维码相对于相机的变换
                    tf::Transform tag_to_camera;
                    tag_to_camera.setOrigin(tf::Vector3(position.x, position.y, position.z));
                    tag_to_camera.setRotation(
                            tf::Quaternion(orientation.x, orientation.y, orientation.z, orientation.w));

                    // 计算二维码相对于机器人的变换
                    tf::Transform tag_to_base = camera_to_base * tag_to_camera;

                    // 获取机器人的当前里程计信息
                    tf::Transform base_to_odom;
                    base_to_odom.setOrigin(tf::Vector3(current_odom_.pose.pose.position.x,
                                                       current_odom_.pose.pose.position.y,
                                                       current_odom_.pose.pose.position.z));
                    base_to_odom.setRotation(tf::Quaternion(current_odom_.pose.pose.orientation.x,
                                                            current_odom_.pose.pose.orientation.y,
                                                            current_odom_.pose.pose.orientation.z,
                                                            current_odom_.pose.pose.orientation.w));

                    // 计算二维码相对于全局坐标系（例如里程计坐标系）的变换
                    tf::Transform tag_to_odom = base_to_odom * tag_to_base;

                    LOG(INFO) << "Detected tag ID: " << detected_tag_id;
                    LOG(INFO) << "Tag position in odom - x: " << tag_to_odom.getOrigin().x()
                              << ", y: " << tag_to_odom.getOrigin().y()
                              << ", z: " << tag_to_odom.getOrigin().z();

                    RealPoint realPoint;
                    realPoint.realPosition.x = tag_to_odom.getOrigin().x();
                    realPoint.realPosition.y = tag_to_odom.getOrigin().y();
                    realPoint.realPosition.z = tag_to_odom.getOrigin().z();
                    realPoint.realOrientation.x = tag_to_odom.getRotation().x();
                    realPoint.realOrientation.y = tag_to_odom.getRotation().y();
                    realPoint.realOrientation.z = tag_to_odom.getRotation().z();
                    realPoint.realOrientation.w = tag_to_odom.getRotation().w();
                    realPoint.core_move = true;


                    PointPlanner::instance().goToPoint(realPoint);

                    has_target_tag = true;
                }
            }

        }

        if (has_target_tag) {
            // 正常检测出来点位，已经发给move_base了，这里无需做任何事情

            LOG(INFO) << "Detected target tag, no need to do anything. record_detection_count_ : "
                      << record_detection_count_;
        } else {
            record_detection_count_++;
            if (record_detection_count_ > 50) {
                // 在一定的阈值内未检测到目标点位，认为检测失败，直接到下一个点位
                LOG(INFO) << "Detected target tag failed, record_detection_count_ : " << record_detection_count_;
                record_detection_ = false;
                {
                    std::unique_lock<std::mutex> lk(point_mutex_);
                    deliveryState = DeliveryState::OVER;
                }
                point_condition_variable_.notify_one();
            } else {
                // 继续再次重试
                LOG(INFO) << "Detected target tag failed, continue to retry. record_detection_count_ : "
                          << record_detection_count_;
            }
        }

    }

}

void DeliveryControlManager::odomCallback(const nav_msgs::Odometry::ConstPtr &msg) {
    current_odom_ = *msg;
    odom_received_ = true;
}

void DeliveryControlManager::move_timer_fun(const ros::TimerEvent &event) {

}

void DeliveryControlManager::point_circulation_thread_func() {

    while (true) {

        std::unique_lock<std::mutex> lk(point_mutex_);
        point_condition_variable_.wait(lk, [this] {
            return deliveryState != DeliveryState::NONE;
        });

        bool reprocess = false;

        if (deliveryState == DeliveryState::TAKE) {
            doTake();
            reprocess = true;
        } else if (deliveryState == DeliveryState::MOVE) {
            doMove();
        } else if (deliveryState == DeliveryState::CALIBRATION) {
            doCalibration();
        } else if (deliveryState == DeliveryState::DISTINGUISH) {
            doDistinguish();
        } else if (deliveryState == DeliveryState::DELIVERY) {
            doDelivery();
        } else if (deliveryState == DeliveryState::OVER) {
            doOver();
            reprocess = true;
        }

        if (!reprocess)
            deliveryState = DeliveryState::NONE;
    }
}

void DeliveryControlManager::move_towards_tag_thread_func() {
//    while (true) {
//        std::unique_lock<std::mutex> lock(move_mutex_);
//        move_condition_variable_.wait(lock, [this] { return move_triggered_; });
//        move_triggered_ = false;
//
//        // todo
//    }
}


void DeliveryControlManager::goError(DeliveryControlManager::DeliveryError error) {
    {
        std::unique_lock<std::mutex> lk(point_mutex_);
        this->deliveryError = error;
        deliveryState = DeliveryState::OVER;
    }
    point_condition_variable_.notify_one();
}

void DeliveryControlManager::doTake() {
    LOG_IF(INFO, DEBUG_DELIVERY) << "DeliveryControlManager 取出队列头部第一个点 ... ";

    if (plannerQueue.empty()) {
        LOG_IF(INFO, DEBUG_DELIVERY) << "DeliveryControlManager doTake ... plannerQueue is empty";
        return;
    }

    currentPoint = plannerQueue.front();
    plannerQueue.pop_front();

    deliveryState = DeliveryState::MOVE;
}

void DeliveryControlManager::doMove() {
    pool_.execute([this]() {
        try {
            LOG_IF(INFO, DEBUG_DELIVERY) << "DeliveryControlManager move_base 初步移动 ... ";

            arriveState = ArriveState::ArriveMove;
            currentPoint.core_move = false;
            PointPlanner::instance().goToPoint(currentPoint, false);

        } catch (app::exception const &e) {
            LOG_IF(ERROR, DEBUG_DELIVERY) << e.what();
            goError(DeliveryError::DeliveryMoveError);
        } catch (const std::exception &e) {
            LOG_IF(ERROR, DEBUG_DELIVERY) << e.what();
            goError(DeliveryMoveError);
        } catch (...) {
            LOG_IF(ERROR, DEBUG_DELIVERY) << "doMove other exception";
            goError(DeliveryMoveError);
        }
    });
}

void DeliveryControlManager::doCalibration() {
    pool_.execute([this]() {
        try {
            LOG_IF(INFO, DEBUG_DELIVERY) << "DeliveryControlManager core_move 二次微调 ... ";

            arriveState = ArriveState::ArriveCalibration;
            currentPoint.core_move = true;
            PointPlanner::instance().goToPoint(currentPoint, false);

        } catch (app::exception const &e) {
            LOG_IF(ERROR, DEBUG_DELIVERY) << e.what();
            goError(DeliveryError::DeliveryCalibrationError);
        } catch (const std::exception &e) {
            LOG_IF(ERROR, DEBUG_DELIVERY) << e.what();
            goError(DeliveryCalibrationError);
        } catch (...) {
            LOG_IF(ERROR, DEBUG_DELIVERY) << "doCalibration other exception";
            goError(DeliveryCalibrationError);
        }
    });
}

void DeliveryControlManager::doDistinguish() {
    pool_.execute([this]() {
        try {
            LOG_IF(INFO, DEBUG_DELIVERY) << "DeliveryControlManager april_tag 再次定位 ... ";

            if (odom_received_) {
                LOG_IF(ERROR, DEBUG_DELIVERY) << "DeliveryControlManager odom_received_ is false";
                goError(DeliveryError::DeliveryDistinguishError);
                return;
            }
            {
                std::unique_lock<std::mutex> lk(move_mutex_);
                move_triggered_ = true;
                move_condition_variable_.notify_one();
            }

            // 开启检测
            tagDetectionState = TagDetectionStateNone;
            record_detection_ = true;
            record_detection_count_ = 0;

        } catch (app::exception const &e) {
            LOG_IF(ERROR, DEBUG_DELIVERY) << e.what();
            goError(DeliveryError::DeliveryDistinguishError);
        } catch (const std::exception &e) {
            LOG_IF(ERROR, DEBUG_DELIVERY) << e.what();
            goError(DeliveryDistinguishError);
        } catch (...) {
            LOG_IF(ERROR, DEBUG_DELIVERY) << "doDistinguish other exception";
            goError(DeliveryDistinguishError);
        }
    });
}

void DeliveryControlManager::doDelivery() {
    pool_.execute([this]() {
        try {
            LOG_IF(INFO, DEBUG_DELIVERY) << "DeliveryControlManager 抬升 ... ";

            up_pub_.publish(std_msgs::Int32());
            std::this_thread::sleep_for(std::chrono::seconds(5));

            {
                std::unique_lock<std::mutex> lk(point_mutex_);
                deliveryState = DeliveryState::OVER;
            }
            point_condition_variable_.notify_one();

        } catch (app::exception const &e) {
            LOG_IF(ERROR, DEBUG_DELIVERY) << e.what();
            goError(DeliveryError::DeliveryDeliveryError);
        } catch (const std::exception &e) {
            LOG_IF(ERROR, DEBUG_DELIVERY) << e.what();
            goError(DeliveryDeliveryError);
        } catch (...) {
            LOG_IF(ERROR, DEBUG_DELIVERY) << "doDelivery other exception";
            goError(DeliveryDeliveryError);
        }
    });
}

void DeliveryControlManager::doOver() {
    LOG_IF(INFO, DEBUG_DELIVERY) << "DeliveryControlManager 配送结束 ... ";

    if (plannerQueue.empty()) {
        deliveryState = DeliveryState::NONE;

        mDeliveryFailCallback(deliveryError);
    } else {
        deliveryState = DeliveryState::MOVE;

        currentPoint = plannerQueue.front();
        plannerQueue.pop_front();
    }
}

void DeliveryControlManager::setDeliveryCallback(DeliveryControlManager::DeliveryFailCallback callback) {
    DeliveryControlManager::mDeliveryFailCallback = std::move(callback);
}

void DeliveryControlManager::completeCirculation(bool arrive) {

    if (arrive) {
        LOG_IF(INFO, DEBUG_DELIVERY) << "DeliveryControlManager completeCirculation " << arrive << " ... ";

        if (arriveState == ArriveState::ArriveMove) {
            {
                std::unique_lock<std::mutex> lk(point_mutex_);
                deliveryState = DeliveryState::CALIBRATION;
            }
            point_condition_variable_.notify_one();
        } else if (arriveState == ArriveState::ArriveCalibration) {
            {
                std::unique_lock<std::mutex> lk(point_mutex_);
                deliveryState = DeliveryState::DISTINGUISH;
            }
            point_condition_variable_.notify_one();

        } else if (arriveState == ArriveState::ArriveDistinguish) {
            {
                std::unique_lock<std::mutex> lk(point_mutex_);
                deliveryState = DeliveryState::DELIVERY;
            }
            point_condition_variable_.notify_one();
        }
    } else {
        LOG_IF(ERROR, DEBUG_DELIVERY) << "DeliveryControlManager completeCirculation fail ... ";
        goError(DeliveryError::DeliveryMoveError);
    }

}

void DeliveryControlManager::handleFlow(const RealBlock &block) {

    for (int i = 0; i < block.plannerPoints.size(); ++i) {
        auto point = block.plannerPoints[i];
        point.deliveryVo.setTag(2);
        point.deliveryVo.setCmd(2);
        plannerQueue.push_back(point);
    }

    {
        std::unique_lock<std::mutex> lk(point_mutex_);
        deliveryState = DeliveryState::TAKE;
    }
    point_condition_variable_.notify_one();
}