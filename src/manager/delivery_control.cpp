//
// Created by noodles on 24-7-2.
//

#include <tbb/compat/thread>
#include <utility>
#include <tf/LinearMath/Transform.h>
#include <tf/transform_datatypes.h>
#include <std_msgs/Int16.h>
#include "manager/delivery_control.h"
#include "simulation.h"
#include "BaseThrowable.h"
#include "task/point_planner.h"
#include "manager/PublishInnerManager.h"

void DeliveryControlManager::initialize(ros::NodeHandle nh) {

    tag_sub_ = nh.subscribe("/tag_detections", 10, &DeliveryControlManager::tagDetectionsCallback, this);
    if (Environment::instance().isRealEnvironment) {
        odom_sub_ = nh.subscribe("/odom_app", 10, &DeliveryControlManager::odomCallback, this);
    } else {
        odom_sub_ = nh.subscribe("/odom", 10, &DeliveryControlManager::odomCallback, this);
    }

    cmd_vel_pub_ = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);

    pose_pub_ = nh.advertise<geometry_msgs::PoseStamped>("/tag_to_odom/goal", 1);

    std::thread point_circulation_thread(&DeliveryControlManager::point_circulation_thread_func, this);
    point_circulation_thread.detach();

    std::thread move_towards_tag_thread(&DeliveryControlManager::move_towards_tag_thread_func, this);
    move_towards_tag_thread.detach();

    move_timer_ = nh.createTimer(ros::Duration(0.1), &DeliveryControlManager::move_timer_fun, this, false, false);
}

tf::Transform
DeliveryControlManager::calculateTransform(const tf::Vector3 &avg_position, const tf::Quaternion &avg_orientation) {
    // 相机到基座的固定变换
    tf::Transform camera_to_base(tf::Quaternion(0, 0, 0, 1), tf::Vector3(0.4, 0.0, 0.3));

    // 从平均位置和方向创建标签到相机的变换
    tf::Transform tag_to_camera(avg_orientation, avg_position);

    // 计算标签到基座的最终变换
    return camera_to_base * tag_to_camera;
}

tf::Transform DeliveryControlManager::calculateTagToOdomTransform(const tf::Transform &tag_to_base) {
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
    return base_to_odom * tag_to_base;
}

void DeliveryControlManager::publishTagPosition(const tf::Transform &tag_to_odom) {
    geometry_msgs::PoseStamped pose;
    pose.header.frame_id = "odom";
    pose.header.stamp = ros::Time::now();
    pose.pose.position.x = tag_to_odom.getOrigin().getX();
    pose.pose.position.y = tag_to_odom.getOrigin().getY();
    pose.pose.position.z = tag_to_odom.getOrigin().getZ();
    pose.pose.orientation.x = tag_to_odom.getRotation().x();
    pose.pose.orientation.y = tag_to_odom.getRotation().y();
    pose.pose.orientation.z = tag_to_odom.getRotation().z();
    pose.pose.orientation.w = tag_to_odom.getRotation().w();
    pose_pub_.publish(pose);
}

void DeliveryControlManager::moveToTag(const tf::Transform &tag_to_odom) {
    RealPoint realPoint;
    realPoint.realPosition.x = tag_to_odom.getOrigin().getX();
    realPoint.realPosition.y = tag_to_odom.getOrigin().getY();
    realPoint.realPosition.z = tag_to_odom.getOrigin().getZ();
    realPoint.realOrientation.x = tag_to_odom.getRotation().x();
    realPoint.realOrientation.y = tag_to_odom.getRotation().y();
    realPoint.realOrientation.z = tag_to_odom.getRotation().z();
    realPoint.realOrientation.w = tag_to_odom.getRotation().w();
    realPoint.core_move = true;

    arriveState = ArriveState::ArriveDistinguish;
    PointPlanner::instance().goToPoint(realPoint);
}

void DeliveryControlManager::tagDetectionsCallback(
        const apriltag_ros::AprilTagDetectionArray::ConstPtr &msg) {
    aprilTagDetectionArray = *msg;
    april_tag_detection_received_ = true;

    // 等待检测标志位开启
    if (record_detection_) {

        int target_tag_id_ = currentPoint.deliveryVo.getTag();
        bool has_target_tag = false;

        // LOG(INFO) << "Detected tag ID: " << target_tag_id_;

        for (const auto &detection: msg->detections) {

            // for (size_t j = 0; j < detection.id.size(); ++j) {
            //     int tag_id = detection.id[j];
            //     double tag_size = detection.size[j];
            //     const auto &position = detection.pose.pose.pose.position;
            //     const auto &orientation = detection.pose.pose.pose.orientation;

            //     LOG(INFO) << "Detected tag ID: " << tag_id;
            //     LOG(INFO) << "Tag size: " << tag_size;
            //     LOG(INFO) << "Tag position: [x: " << position.x << ", y: " << position.y << ", z: " << position.z
            //               << "]";
            //     LOG(INFO) << "Tag orientation: [x: " << orientation.x << ", y: " << orientation.y << ", z: "
            //               << orientation.z << ", w: " << orientation.w << "]";
            // }

            // 单个目标才能使用
            if (detection.id.size() == 1 && detection.id[0] == target_tag_id_) {

                const auto &position = detection.pose.pose.pose.position;
                const auto &orientation = detection.pose.pose.pose.orientation;

                // 相机相对于机器人的固定变换，位于机器人前方0.4米，高度0.3米
                tf::Transform camera_to_base;
                camera_to_base.setOrigin(tf::Vector3(0.4, 0.0, 0.3));
                camera_to_base.setRotation(tf::Quaternion(0, 0, 0, 1));
                // 二维码相对于相机的变换
                tf::Transform tag_to_camera(tf::Quaternion(orientation.x, orientation.y, orientation.z, orientation.w),
                                            tf::Vector3(position.x, position.y, position.z));

                recent_detections_.push_back(tag_to_camera);
                continuous_detection_count_++;

                if (continuous_detection_count_ >= 30) {
                    has_target_tag = true;
                    break;
                }

            } else {
                // 如果检测到非目标标签ID，重置累计的数据
                recent_detections_.clear();
                continuous_detection_count_ = 0;
            }

        }

        if (has_target_tag) {

            LOG(INFO) << "Detected target tag, no need to do anything."
                      << "  record_detection_count_ : " << tag_detection_count_
                      << "  continuous_detection_count_ : " << continuous_detection_count_;

            // 正常检测出来点位
            tf::Vector3 avg_position(0, 0, 0);
            tf::Quaternion avg_orientation(0, 0, 0, 0);

            // 只计算最后10次的平均值
            int start_index = std::max(0, int(recent_detections_.size()) - 10);  // 确保从最后10个开始

            for (int i = start_index; i < recent_detections_.size(); ++i) {
                avg_position += recent_detections_[i].getOrigin();
                avg_orientation += recent_detections_[i].getRotation();
            }
            avg_position /= 10;  // 只平均最后10个检测
            avg_orientation.normalize();

            tf::Transform avg_tag_to_base = calculateTransform(avg_position, avg_orientation);

            tf::Transform tag_to_odom = calculateTagToOdomTransform(avg_tag_to_base);


            LOG(INFO) << "detection position - x: " << avg_tag_to_base.getOrigin().getX()
                      << ", y: " << avg_tag_to_base.getOrigin().getY() << ", z: " << avg_tag_to_base.getOrigin().getZ();
            LOG(INFO) << "Tag position in odom - x: " << tag_to_odom.getOrigin().getX()
                      << ", y: " << tag_to_odom.getOrigin().getY()
                      << ", z: " << tag_to_odom.getOrigin().getZ();
            LOG(INFO) << "current_odom_ position - x: " << current_odom_.pose.pose.position.x
                      << ", y: " << current_odom_.pose.pose.position.y
                      << ", z: " << current_odom_.pose.pose.position.z;

            publishTagPosition(tag_to_odom);

            moveToTag(tag_to_odom);

            record_detection_ = false;

        } else {
            tag_detection_count_++;
            if (tag_detection_count_ > 100) {
                // 在一定的阈值内未检测到目标点位，认为检测失败，直接到下一个点位
                LOG(INFO) << "Detected target tag failed, record_detection_count_ : " << tag_detection_count_;
                record_detection_ = false;
                {
                    std::unique_lock<std::mutex> lk(point_mutex_);
                    deliveryState = DeliveryState::OVER;
                }
                point_condition_variable_.notify_one();
            } else {
                // 继续再次重试
                LOG(INFO) << "Detected target tag failed, continue to retry. record_detection_count_ : "
                          << tag_detection_count_;
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
    if (plannerQueue.empty()) {
        LOG_IF(INFO, DEBUG_DELIVERY) << "DeliveryControlManager doTake ... plannerQueue is empty";
        return;
    }

    LOG_IF(INFO, DEBUG_DELIVERY) << "----------------------- start ... ";
    currentPoint = plannerQueue.front();
    plannerQueue.pop_front();

    deliveryState = DeliveryState::MOVE;
}

void DeliveryControlManager::doMove() {
    pool_.execute([this]() {
        try {
            LOG_IF(INFO, DEBUG_DELIVERY) << "1. move_base 初步移动 ... ";

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
            LOG_IF(INFO, DEBUG_DELIVERY) << "2. core_move 二次微调 ... ";

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
            if (!odom_received_) {
                LOG_IF(ERROR, DEBUG_DELIVERY) << "DeliveryControlManager odom_received_ is false";
                goError(DeliveryError::DeliveryDistinguishError);
                return;
            }
            {
                std::unique_lock<std::mutex> lk(move_mutex_);
                move_triggered_ = true;
                move_condition_variable_.notify_one();
            }

            int cmd = currentPoint.deliveryVo.getCmd();

            if (cmd == 0) {
                if (Environment::instance().isRealEnvironment) {
                    LOG_IF(INFO, DEBUG_DELIVERY) << "3. april_tag 再次定位 ... ";
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    // 开启检测
                    tagDetectionState = TagDetectionStateNone;
                    tag_detection_count_ = 0;

                    recent_detections_.clear();
                    continuous_detection_count_ = 0;

                    record_detection_ = true;
                } else {
                    LOG_IF(INFO, DEBUG_DELIVERY) << "3. 跳过 april_tag 定位 ... ";
                    // 跳过检测
                    rectilinearMove(0.2);

                    {
                        std::unique_lock<std::mutex> lk(point_mutex_);
                        deliveryState = DeliveryState::DELIVERY;
                    }
                    point_condition_variable_.notify_one();
                }
            } else if (cmd == 1) {
                LOG_IF(INFO, DEBUG_DELIVERY) << "3. 跳过 april_tag 进入下个步骤 ... ";

                {
                    std::unique_lock<std::mutex> lk(point_mutex_);
                    deliveryState = DeliveryState::DELIVERY;
                }
                point_condition_variable_.notify_one();
            } else {
                throw app::exception("未用到的 cmd");
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
            int cmd = currentPoint.deliveryVo.getCmd();

            if (cmd == 0) {
                LOG_IF(INFO, DEBUG_DELIVERY) << "4. 抬升并后退 ... ";

                PublishInnerManager::instance().pubLiftControl(true);
                std::this_thread::sleep_for(std::chrono::seconds(5));
                rectilinearMove(-0.2);
            } else if (cmd == 1) {
                LOG_IF(INFO, DEBUG_DELIVERY) << "4. 放下并后退 ... ";

                PublishInnerManager::instance().pubLiftControl(false);
                std::this_thread::sleep_for(std::chrono::seconds(5));
                rectilinearMove(-0.2);
            } else {
                throw app::exception("未用到的 cmd");
            }


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
    LOG_IF(INFO, DEBUG_DELIVERY) << "5. 单元结束 ... ";

    if (plannerQueue.empty()) {
        deliveryState = DeliveryState::NONE;

        mDeliveryFailCallback(deliveryError);
    } else {
        deliveryState = DeliveryState::MOVE;

        LOG_IF(INFO, DEBUG_DELIVERY) << "----------------------- start ... ";
        currentPoint = plannerQueue.front();
        plannerQueue.pop_front();
    }
}

/**
 *
 * @param backward_speed 负值表示后退
 */
void DeliveryControlManager::rectilinearMove(double backward_speed) const {
//    if (backward_speed > 0) {
//        LOG_IF(INFO, DEBUG_DELIVERY) << "DeliveryControlManager rectilinearMove 向前移动 0.5 m ... ";
//    } else if (backward_speed < 0) {
//        LOG_IF(INFO, DEBUG_DELIVERY) << "DeliveryControlManager rectilinearMove 后退移动 0.5 m ... ";
//    }

    // 目标距离，单位：米
    double target_distance = 0.5;

    // 发布速度消息的频率，单位：赫兹
    double rate = 10.0;
    ros::Rate loop_rate(rate);

    // 总共需要运行的时间，单位：秒
    double duration = target_distance / std::abs(backward_speed);

    // 运行循环的次数
    int iterations = duration * rate;

    for (int i = 0; i < iterations; ++i) {
        geometry_msgs::Twist cmd_vel_msg;
        cmd_vel_msg.linear.x = backward_speed;
        cmd_vel_pub_.publish(cmd_vel_msg);
        loop_rate.sleep();
    }

    // 停止移动
    geometry_msgs::Twist cmd_vel_msg;
    cmd_vel_msg.linear.x = 0;
    cmd_vel_pub_.publish(cmd_vel_msg);
}

void DeliveryControlManager::setDeliveryCallback(DeliveryControlManager::DeliveryFailCallback callback) {
    DeliveryControlManager::mDeliveryFailCallback = std::move(callback);
}

void DeliveryControlManager::completeCirculation(bool arrive) {

    if (arrive) {

        if (arriveState == ArriveState::ArriveMove) {
            LOG_IF(INFO, DEBUG_DELIVERY) << "1.1 result " << arrive << " ... ";
            {
                std::unique_lock<std::mutex> lk(point_mutex_);
                deliveryState = DeliveryState::CALIBRATION;
            }
            point_condition_variable_.notify_one();
        } else if (arriveState == ArriveState::ArriveCalibration) {
            LOG_IF(INFO, DEBUG_DELIVERY) << "2.1 result " << arrive << " ... ";
            {
                std::unique_lock<std::mutex> lk(point_mutex_);
                deliveryState = DeliveryState::DISTINGUISH;
            }
            point_condition_variable_.notify_one();

        } else if (arriveState == ArriveState::ArriveDistinguish) {
            LOG_IF(INFO, DEBUG_DELIVERY) << "3.1 result " << arrive << " ... ";
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

    for (const auto &point: block.plannerPoints) {
        plannerQueue.push_back(point);
    }

    {
        std::unique_lock<std::mutex> lk(point_mutex_);
        deliveryState = DeliveryState::TAKE;
    }
    point_condition_variable_.notify_one();
}