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
#include <tf/transform_listener.h>

void DeliveryControlManager::initialize(ros::NodeHandle nh) {

    tag_test_sub_ = nh.subscribe("/tag_test", 10, &DeliveryControlManager::tagTestCallback, this);

    tag_sub_ = nh.subscribe("/tag_detections", 10, &DeliveryControlManager::tagDetectionsCallback, this);
    if (Environment::instance().isRealEnvironment) {
        odom_sub_ = nh.subscribe("/odom_app", 10, &DeliveryControlManager::odomCallback, this);
    } else {
        odom_sub_ = nh.subscribe("/odom", 10, &DeliveryControlManager::odomCallback, this);
    }

    cmd_vel_pub_ = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);

    pose_tag_pub_ = nh.advertise<geometry_msgs::PoseStamped>("/tag_to_odom/goal", 1);
    pose_final_pub_ = nh.advertise<geometry_msgs::PoseStamped>("/final_to_odom/goal", 1);

    std::thread point_circulation_thread(&DeliveryControlManager::point_circulation_thread_func, this);
    point_circulation_thread.detach();

    std::thread move_towards_tag_thread(&DeliveryControlManager::move_towards_tag_thread_func, this);
    move_towards_tag_thread.detach();

    move_timer_ = nh.createTimer(ros::Duration(0.1), &DeliveryControlManager::move_timer_fun, this, false, false);
}

void DeliveryControlManager::convertPose(const geometry_msgs::PoseStamped &input_pose,
                                         geometry_msgs::PoseStamped &output_pose) {

    const auto &position = input_pose.pose.position;
    const auto &orientation = input_pose.pose.orientation;

    // 正常检测出来点位
    tf::Vector3 avg_position(position.z, position.x, position.y);
    tf::Quaternion avg_orientation(orientation.x, orientation.y, orientation.z, orientation.w);
    tf::Transform avg_transform(avg_orientation, avg_position);

    // 旋转
    tf::Quaternion rotation(current_odom_.pose.pose.orientation.x,
                            current_odom_.pose.pose.orientation.y,
                            current_odom_.pose.pose.orientation.z,
                            current_odom_.pose.pose.orientation.w);
    // rotation.setRPY(-M_PI / 2, 0, 0);
    tf::Transform rotation_transform(rotation, tf::Vector3(0.0, 0.0, 0.0));

    tf::Transform transform = rotation_transform * avg_transform;

    output_pose.header = input_pose.header;
    output_pose.pose.position.x = input_pose.pose.position.z;
    output_pose.pose.position.y = -input_pose.pose.position.x;
    output_pose.pose.position.z = -input_pose.pose.position.y;
    output_pose.pose.orientation.x = transform.getRotation().x();
    output_pose.pose.orientation.y = transform.getRotation().y();
    output_pose.pose.orientation.z = transform.getRotation().z();
    output_pose.pose.orientation.w = transform.getRotation().w();
}

bool DeliveryControlManager::transformPose(const geometry_msgs::PoseStamped &input_pose,
                                           geometry_msgs::PoseStamped &output_pose) {
    static tf::TransformListener listener;

    try {
        // 等待tf变换可用
        listener.waitForTransform("map", input_pose.header.frame_id,
                                  input_pose.header.stamp, ros::Duration(1.0));

        // 执行变换
        listener.transformPose("map", input_pose, output_pose);
        return true;
    } catch (tf::TransformException &ex) {
        ROS_ERROR("%s", ex.what());
        return false;
    }
}

tf::Transform DeliveryControlManager::adjustToHorizontal(const tf::Transform &tag_transform) {
    // 获取当前的旋转四元数
    tf::Quaternion current_orientation = tag_transform.getRotation();

    // 将四元数转换为欧拉角
    double roll, pitch, yaw;
    tf::Matrix3x3(current_orientation).getRPY(roll, pitch, yaw);

    // 创建一个新的四元数，只保留偏航角和滚转角，将俯仰角设置为0
    tf::Quaternion horizontal_orientation;
    horizontal_orientation.setRPY(roll, 0, yaw);

    // 创建一个新的变换，保留原始位置，更新旋转部分
    tf::Transform horizontal_transform(horizontal_orientation, tag_transform.getOrigin());

    return horizontal_transform;
}


tf::Transform
DeliveryControlManager::calculateTransform(const tf::Transform &avg_tag_to_base) {
    tf::Quaternion z_rotation;
    z_rotation.setRPY(0, M_PI / 2, 0);  // 绕Z轴旋转90度

    // 创建旋转变换
    tf::Transform z_rotation_transform(z_rotation, tf::Vector3(0.0, 0.0, 0.0));

    // 更新传入的变换
    return avg_tag_to_base * z_rotation_transform;  // 注意乘法的顺序，它决定了旋转的应用顺序
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

tf::Transform
DeliveryControlManager::calculatePositionForwardFromTag(const tf::Transform &tag_to_odom, double forward_distance) {
    // 获取标签朝向的单位向量
    tf::Vector3 forward_direction = tf::quatRotate(tag_to_odom.getRotation(), tf::Vector3(1.0, 0.0, 0.0));
    forward_direction.normalize();

    // 创建一个向前移动指定距离的变换
    tf::Transform forward_transform;
    forward_transform.setOrigin(forward_direction * forward_distance);  // 使用标签的朝向
    forward_transform.setRotation(tf::Quaternion(0, 0, 0, 1));  // 保持原朝向不变

    // 应用变换
    tf::Transform final_position = tag_to_odom * forward_transform;

    return final_position;
}

void DeliveryControlManager::publishTagPosition(const tf::Transform &tag_to_odom) {
    geometry_msgs::PoseStamped pose;
    pose.header.frame_id = "map";
    pose.header.stamp = ros::Time::now();
    pose.pose.position.x = tag_to_odom.getOrigin().getX();
    pose.pose.position.y = tag_to_odom.getOrigin().getY();
    pose.pose.position.z = tag_to_odom.getOrigin().getZ();
    pose.pose.orientation.x = tag_to_odom.getRotation().x();
    pose.pose.orientation.y = tag_to_odom.getRotation().y();
    pose.pose.orientation.z = tag_to_odom.getRotation().z();
    pose.pose.orientation.w = tag_to_odom.getRotation().w();
    pose_tag_pub_.publish(pose);
}

void DeliveryControlManager::publishFinalPosition(const tf::Transform &tag_to_odom) {
    geometry_msgs::PoseStamped pose;
    pose.header.frame_id = "map";
    pose.header.stamp = ros::Time::now();
    pose.pose.position.x = tag_to_odom.getOrigin().getX();
    pose.pose.position.y = tag_to_odom.getOrigin().getY();
    pose.pose.position.z = tag_to_odom.getOrigin().getZ();
    pose.pose.orientation.x = tag_to_odom.getRotation().x();
    pose.pose.orientation.y = tag_to_odom.getRotation().y();
    pose.pose.orientation.z = tag_to_odom.getRotation().z();
    pose.pose.orientation.w = tag_to_odom.getRotation().w();
    pose_final_pub_.publish(pose);
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
    PointPlanner::instance().goToPoint(realPoint, true);
}

void DeliveryControlManager::tagTestCallback(const std_msgs::Int32 &flag) {
    for (const auto &detection: aprilTagDetectionArray.detections) {

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

        if (detection.id.size() == 1) {

            geometry_msgs::PoseStamped input_pose;
            input_pose.header.frame_id = detection.pose.header.frame_id;
            input_pose.pose.position.x = detection.pose.pose.pose.position.x;
            input_pose.pose.position.y = detection.pose.pose.pose.position.y;
            input_pose.pose.position.z = detection.pose.pose.pose.position.z;
            input_pose.pose.orientation.x = detection.pose.pose.pose.orientation.x;
            input_pose.pose.orientation.y = detection.pose.pose.pose.orientation.y;
            input_pose.pose.orientation.z = detection.pose.pose.pose.orientation.z;
            input_pose.pose.orientation.w = detection.pose.pose.pose.orientation.w;

            geometry_msgs::PoseStamped output_pose;
//            convertPose(input_pose, output_pose);
            transformPose(input_pose, output_pose);

            tf::Transform avg_tag_to_base;
            avg_tag_to_base.setOrigin(tf::Vector3(output_pose.pose.position.x,
                                                  output_pose.pose.position.y,
                                                  output_pose.pose.position.z));
            avg_tag_to_base.setRotation(tf::Quaternion(output_pose.pose.orientation.x,
                                                       output_pose.pose.orientation.y,
                                                       output_pose.pose.orientation.z,
                                                       output_pose.pose.orientation.w));

//            tf::Transform tag_to_odom = calculateTagToOdomTransform(avg_tag_to_base);
//            tf::Transform final_position = calculatePositionForwardFromTag(tag_to_odom, 0.5);

            tf::Transform tag_to_odom = calculateTransform(avg_tag_to_base);
            tf::Transform horizontal_transform = adjustToHorizontal(tag_to_odom);

            tf::Transform final_position = calculatePositionForwardFromTag(horizontal_transform, 0.2);

            LOG(INFO) << "Detected tag ID: " << detection.id[0];
            LOG(INFO) << "tag position - x: " << output_pose.pose.position.x << ", y: " << output_pose.pose.position.y
                      << ", z: " << output_pose.pose.position.z;
            LOG(INFO) << "detection position - x: " << avg_tag_to_base.getOrigin().getX()
                      << ", y: " << avg_tag_to_base.getOrigin().getY() << ", z: " << avg_tag_to_base.getOrigin().getZ()
                      << ", o: " << avg_tag_to_base.getRotation().getX();
            LOG(INFO) << "current_odom_ position - x: " << current_odom_.pose.pose.position.x
                      << ", y: " << current_odom_.pose.pose.position.y
                      << ", z: " << current_odom_.pose.pose.position.z;
            LOG(INFO) << "Tag position in odom - x: " << tag_to_odom.getOrigin().getX()
                      << ", y: " << tag_to_odom.getOrigin().getY()
                      << ", z: " << tag_to_odom.getOrigin().getZ()
                      << ", o: " << tag_to_odom.getRotation().getX();
            LOG(INFO) << "Final position - x: " << final_position.getOrigin().getX() << ", y: "
                      << final_position.getOrigin().getY() << ", z: " << final_position.getOrigin().getZ();

            publishTagPosition(tag_to_odom);
            publishFinalPosition(horizontal_transform);
        }
    }
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

        bool single_loop_result = false;

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

                geometry_msgs::PoseStamped input_pose;
                input_pose.header.frame_id = detection.pose.header.frame_id;
                input_pose.pose.position.x = detection.pose.pose.pose.position.x;
                input_pose.pose.position.y = detection.pose.pose.pose.position.y;
                input_pose.pose.position.z = detection.pose.pose.pose.position.z;
                input_pose.pose.orientation.x = detection.pose.pose.pose.orientation.x;
                input_pose.pose.orientation.y = detection.pose.pose.pose.orientation.y;
                input_pose.pose.orientation.z = detection.pose.pose.pose.orientation.z;
                input_pose.pose.orientation.w = detection.pose.pose.pose.orientation.w;

                geometry_msgs::PoseStamped output_pose;
//                convertPose(input_pose, output_pose);
                transformPose(input_pose, output_pose);

                tf::Transform avg_tag_to_base;
                avg_tag_to_base.setOrigin(tf::Vector3(output_pose.pose.position.x,
                                                      output_pose.pose.position.y,
                                                      output_pose.pose.position.z));
                avg_tag_to_base.setRotation(tf::Quaternion(output_pose.pose.orientation.x,
                                                           output_pose.pose.orientation.y,
                                                           output_pose.pose.orientation.z,
                                                           output_pose.pose.orientation.w));

                recent_detections_.push_back(avg_tag_to_base);
                continuous_detection_count_++;

                single_loop_result = true;

                if (continuous_detection_count_ >= 20) {
                    has_target_tag = true;
                    break;
                }

            }

        }

        if (!single_loop_result) {
            // 如果检测到非目标标签ID，重置累计的数据
            recent_detections_.clear();
            continuous_detection_count_ = 0;
        }

        if (has_target_tag) {

            LOG(INFO) << "Detected target tag, no need to do anything."
                      << "  record_detection_count_ : " << tag_detection_count_
                      << "  continuous_detection_count_ : " << continuous_detection_count_;

            // 正常检测出来点位
//            tf::Vector3 avg_position(0, 0, 0);
//            tf::Quaternion avg_orientation(0, 0, 0, 0);
//            // 只计算最后10次的平均值
//            int start_index = std::max(0, int(recent_detections_.size()) - 10);  // 确保从最后10个开始
//
//            for (int i = start_index; i < recent_detections_.size(); ++i) {
//                avg_position += recent_detections_[i].getOrigin();
//                avg_orientation += recent_detections_[i].getRotation();
//            }
//            avg_position /= 10;  // 只平均最后10个检测
//            avg_orientation.normalize();

//             tf::Transform avg_tag_to_base;
//             avg_tag_to_base.setOrigin(avg_position);
//             avg_tag_to_base.setRotation(avg_orientation);


            auto last_transform = recent_detections_[recent_detections_.size() - 1];

//            tf::Transform tag_to_odom = calculateTagToOdomTransform(last_transform);
//            tf::Transform final_position = calculatePositionForwardFromTag(tag_to_odom, 0.5);


            tf::Transform tag_to_odom = calculateTransform(last_transform);
            tf::Transform horizontal_transform = adjustToHorizontal(tag_to_odom);

            tf::Transform final_position = calculatePositionForwardFromTag(horizontal_transform, 0.2);

            publishTagPosition(tag_to_odom);
            publishFinalPosition(horizontal_transform);

            moveToTag(horizontal_transform);

            record_detection_ = false;

        } else {
            tag_detection_count_++;
            if (tag_detection_count_ > 50) {
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
                          << tag_detection_count_
                          << "  continuous_detection_count_ : " << continuous_detection_count_;
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
                    rectilinearMove(0.0);

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

                rectilinearMove(0.0);
                PublishInnerManager::instance().pubLiftControl(true);
                std::this_thread::sleep_for(std::chrono::seconds(10));
                rectilinearMove(-0.3);
            } else if (cmd == 1) {
                LOG_IF(INFO, DEBUG_DELIVERY) << "4. 放下并后退 ... ";

                PublishInnerManager::instance().pubLiftControl(false);
                std::this_thread::sleep_for(std::chrono::seconds(10));
                rectilinearMove(-0.3);
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