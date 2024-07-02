//
// Created by noodles on 24-7-2.
//

#include <tbb/compat/thread>
#include <utility>
#include "manager/delivery_control.h"
#include "simulation.h"
#include "BaseThrowable.h"
#include "task/point_planner.h"

void DeliveryControlManager::initialize(ros::NodeHandle nh) {

    tag_sub_ = nh.subscribe("/tag_detections", 10, &DeliveryControlManager::tagDetectionsCallback, this);
    odom_sub_ = nh.subscribe("/odom_app", 10, &DeliveryControlManager::odomCallback, this);

    cmd_vel_pub_ = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
    up_pub_ = nh.advertise<std_msgs::Int32>("/up_pub_", 1);

    std::thread point_circulation_thread(&DeliveryControlManager::point_circulation_thread_func, this);

    std::thread move_towards_tag_thread(&DeliveryControlManager::move_towards_tag_thread_func, this);

    point_circulation_thread.detach();
    move_towards_tag_thread.detach();

//    move_timer_ = nh.createTimer(ros::Duration(0.1), &DeliveryControlManager::moveAccordingToTagCallback, this);
}

void DeliveryControlManager::tagDetectionsCallback(
        const apriltag_ros::AprilTagDetectionArray::ConstPtr &msg) {

    for (const auto &detection: msg->detections) {
        if (std::find(detection.id.begin(), detection.id.end(), target_tag_id_) != detection.id.end()) {
            current_detection_ = detection;
            detection_received_ = true;
            target_reached_ = false;
            return;
        }
    }
    detection_received_ = false;
}

void DeliveryControlManager::odomCallback(const nav_msgs::Odometry::ConstPtr &msg) {
    current_odom_ = *msg;
    odom_received_ = true;
}

void DeliveryControlManager::moveAccordingToTagCallback(const ros::TimerEvent &event) {
    if (!detection_received_ || !odom_received_) {
        return;
    }

    doMoveAccordingToTag();
}

bool DeliveryControlManager::moveAccordingToTag() {
    if (Environment::instance().isRealEnvironment) {

        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        doMoveAccordingToTag();

        return target_reached_;
    } else {
        static int call_count = 0;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        call_count++;
        LOG_IF(INFO, DEBUG_DELIVERY) << "Executing moveAccordingToTag, call count: " << call_count;


        if (call_count >= 5) {
            call_count = 0; // 重置计数器
            return true;
        }
        return false;
    }

}

void DeliveryControlManager::doMoveAccordingToTag() {
    if (!detection_received_ || !odom_received_) {
        return;
    }

    const auto &position = current_detection_.pose.pose.pose.position;
    double distance = std::sqrt(position.x * position.x + position.y * position.y + position.z * position.z);
    double angle_to_tag = std::atan2(position.y, position.x);

    geometry_msgs::Twist twist;

    // 如果距离大于目标距离，前进
    if (distance > target_distance_) {
        twist.linear.x = linear_speed_;
        twist.angular.z = angular_speed_ * angle_to_tag;
    } else {
        twist.linear.x = 0;
        twist.angular.z = 0;
        detection_received_ = false;  // 重置检测接收状态
        target_reached_ = true;  // 标记到达目标位置
    }

    cmd_vel_pub_.publish(twist);

    // 打印位置和距离信息
    LOG(INFO) << "Current Distance: " << distance << ", Target Distance: " << target_distance_;
    LOG(INFO) << "Position - x: " << position.x << ", y: " << position.y << ", z: " << position.z;

    if (target_reached_) {
        LOG(INFO) << "Target reached!";
    }
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
    while (true) {
        std::unique_lock<std::mutex> lock(move_mutex_);
        move_condition_variable_.wait(lock, [this] { return move_triggered_; });

        while (move_triggered_) {
            lock.unlock();
            bool result = moveAccordingToTag();
            lock.lock();

            if (result) {
                move_triggered_ = false;
                {
                    std::unique_lock<std::mutex> lk(point_mutex_);
                    deliveryState = DeliveryState::DELIVERY;
                }
                point_condition_variable_.notify_one();
            }
        }

    }
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
    LOG_IF(INFO, DEBUG_DELIVERY) << "DeliveryControlManager doTake ... ";

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
            LOG_IF(INFO, DEBUG_DELIVERY) << "DeliveryControlManager doMove ... ";

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
            LOG_IF(INFO, DEBUG_DELIVERY) << "DeliveryControlManager doCalibration ... ";

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
            LOG_IF(INFO, DEBUG_DELIVERY) << "DeliveryControlManager doDistinguish ... ";

            {
                std::unique_lock<std::mutex> lk(move_mutex_);
                move_triggered_ = true;
                move_condition_variable_.notify_one();
            }

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
            LOG_IF(INFO, DEBUG_DELIVERY) << "DeliveryControlManager doDelivery ... ";

            up_pub_.publish(std_msgs::Int32());
            std::this_thread::sleep_for(std::chrono::seconds(1));

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
    LOG_IF(INFO, DEBUG_DELIVERY) << "DeliveryControlManager doOver ... ";

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

        }
    } else {
        LOG_IF(ERROR, DEBUG_DELIVERY) << "DeliveryControlManager completeCirculation fail ... ";
        goError(DeliveryError::DeliveryMoveError);
    }

}

void DeliveryControlManager::handleFlow(const RealBlock &block) {
    realBlock = block;
    for (const auto &point: block.plannerPoints) {
        plannerQueue.push_back(point);
    }

    {
        std::unique_lock<std::mutex> lk(point_mutex_);
        deliveryState = DeliveryState::TAKE;
    }
    point_condition_variable_.notify_one();
}