//
// Created by io on 23-12-11.
//

#include <Eigen/Geometry>
#include <cmath>
#include "manager/elevator_control.h"
#include "glog/logging.h"
#include "simulation.h"
#include "catch2/catch.hpp"
#include "task/manager/NodeWorkModeManager.h"
#include "BaseThrowable.h"
#include "task/point_planner.h"
#include "future/node/node_control.h"
#include "db/segmentation_data_base.h"
#include "leave/map_control.h"
#include "segmentation/map_attribute.h"
#include "schedule/schedule_manager_singleton.h"
#include "manager/PublishInnerManager.h"
#include "exploration/ExplorationCenter.h"
#include "alignment/ele_protocol.h"
#include "future/timer_call.h"
#include <iostream>
#include <cstdint>

#define SLEEP_TIME 10
#define MOVING_DISTANCE 1.6
#define INEXPLICABLE_MAGIC_NUMBER 0.00556789

bool ElevatorControlManager::hasSerialPortAccess(const std::string &portName) {
    // 尝试以读写方式打开串口设备文件
    std::fstream serialPort;
    serialPort.open(portName, std::ios::in | std::ios::out);

    // 检查是否成功打开
    if (serialPort.is_open()) {
        serialPort.close();
        return true;
    }
    return false;
}

void ElevatorControlManager::publishCmd(double x, double z) const {
    geometry_msgs::Twist move_cmd;
    move_cmd.linear.x = x;
    move_cmd.linear.y = 0.0;
    move_cmd.angular.z = z;
    publisherCmdVel.publish(move_cmd);
}

void ElevatorControlManager::interruptAccessElevators() {
    mainInterrupt = true;
    controlCmd = ControlCmd::NONE;
}

void ElevatorControlManager::recordSensorData() {
    old_x = odom_x;
    old_y = odom_y;
    old_yaw = imu_yaw;
}

void ElevatorControlManager::subscribeOdomCallback(const nav_msgs::Odometry &odometry) {
    odom_x = odometry.pose.pose.position.x;
    odom_y = odometry.pose.pose.position.y;
//    odom_yaw = tf::getYaw(odometry.pose.pose.orientation);
}

void ElevatorControlManager::subscribeImuCallback(const sensor_msgs::Imu &imu) {
    tf::Quaternion q(imu.orientation.x, imu.orientation.y, imu.orientation.z, imu.orientation.w);
    tf::Matrix3x3 m(q);
    double roll, pitch, yaw;
    m.getRPY(roll, pitch, yaw);
    imu_yaw = yaw;

//    LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager imu_yaw ： " << (imu_yaw * 180.0 / M_PI)
//                                 << " , roll : " << roll
//                                 << " , pitch : " << pitch;
}

void ElevatorControlManager::elevatorManagerSubscribeCallback(const std_msgs::Int32 &flag) {
    try {//elevator_manager
        if (flag.data == 0) {
            exitElevator();
        } else if (flag.data == 1) {
            enterElevator();
        } else if (flag.data == 2) {
            ttSendLightUpTargetFloor();
        } else if (flag.data == 3) {
            ttSendDelayedDoorClosing();
        } else if (flag.data == 4) {
            ttOpenQueryFloor();
        } else if (flag.data == 5) {
            ttCloseQueryFloor();
        } else if (flag.data == 10) {
            imitateArrivedCount = 100;
        } else if (flag.data == 100) {
            if (!NodeWorkModeManager::instance().enterWorkMode(2)) {
                throw app::exception(make_error_code(error::mode_switching_is_not_supported));
            }
        }

        //['0x7f 0xf7 0x18 0x29 0x16 0x27 0x11 0x22 0x33 0x44 0x55 0x66 0xd 0x60 0x1 0x1 0x2 0x3 0x4 0x5 0x6 0x7 0x8 0x9 0x10 0x11 0x12 0x9']
        //  0x7f 0xf7 0x18 0x29 0x16 0x27 0x11 0x22 0x33 0x44 0x55 0x66 0xd 0x60 0x1 0x1 0x2 0x3 0x4 0x5 0x6 0x7 0x8 0x9 0x10 0x11 0x12 0x9
    } catch (app::exception const &e) {
        LOG_IF(ERROR, DEBUG_ELEVATOR) << e.what();
    } catch (const std::exception &e) {
        LOG_IF(ERROR, DEBUG_ELEVATOR) << e.what();
    } catch (...) {
        LOG_IF(ERROR, DEBUG_ELEVATOR) << "MessageStrategy other start exception";
    }
}

//void ElevatorControlManager::rotate180() {
//    ros::Rate rate(10); // 10Hz
//    double initial_yaw = imu_yaw;
//    double current_yaw = imu_yaw;
//    while (std::abs(current_yaw - initial_yaw) < M_PI) {
//        geometry_msgs::Twist move_cmd;
//        move_cmd.angular.z = 0.5; // Adjust this value to control the rotation speed
//        publisherCmdVel.publish(move_cmd);
//        current_yaw = imu_yaw; // Update current_yaw with the latest imu_yaw value
//        rate.sleep();
//    }
//    // Stop rotation
//    geometry_msgs::Twist move_cmd;
//    move_cmd.angular.z = 0.0;
//    publisherCmdVel.publish(move_cmd);
//}

void ElevatorControlManager::movement_controls_func(ControlCommand command) {
    interruptAccessElevators();
    recordSensorData();

    std::string print_str;
    if (command == ControlCommand::ENTER_ELEVATOR) {
        print_str = "进电梯";
    } else if (command == ControlCommand::EXIT_ELEVATOR) {
        print_str = "出电梯";
    }

    sendDelayedDoorClosing();

    std::chrono::steady_clock::time_point last_send_time = std::chrono::steady_clock::now();

    while (mainInterrupt) {

        int append_sleep_time = 0;

//        printElevator();

        switch (controlCmd) {
            case ControlCmd::NONE:
                publishCmd();
                controlCmd = ControlCmd::MOVE;
                LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager pre " << print_str << " 前进 ... ";
                break;
            case ControlCmd::MOVE: {
                double distance_x = sqrt(pow(abs(odom_x - old_x), 2) + pow(abs(odom_y - old_y), 2));
//                double distance_x = std::abs(odom_x - old_x);
                if (distance_x < MOVING_DISTANCE - SLEEP_TIME * INEXPLICABLE_MAGIC_NUMBER) {// 0.00556789
                    publishCmd(0.2, 0);
                } else {
                    if (command == ControlCommand::EXIT_ELEVATOR) {
                        LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager pre " << print_str << " 转向 ... ";
                        controlCmd = ControlCmd::REACH;
                    } else {
                        LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager pre " << print_str << " 转向 ... ";
                        controlCmd = ControlCmd::ROTATE;
                    }
                }
                break;
            }
            case ControlCmd::ROTATE: {
                auto old_angle = old_yaw * 180.0 / M_PI;
                auto curr_angle = imu_yaw * 180.0 / M_PI;
                auto angle_difference = curr_angle - old_angle;
                if (angle_difference < 0) {
                    angle_difference += 360;
                }
                if (angle_difference > 180) {
                    angle_difference = 360 - angle_difference;
                }
                bool normal_rotate = true;
                if (controlCmd == ControlCmd::ROTATE && angle_difference > 90) {
                    normal_rotate = angle_difference - last_angle > -(SLEEP_TIME * INEXPLICABLE_MAGIC_NUMBER);
                    if (!normal_rotate) {
                        angle_difference = 188;
                    }
                }
                if (angle_difference < 180 - 100 * SLEEP_TIME * INEXPLICABLE_MAGIC_NUMBER) {// 0.00556789
                    publishCmd(0, 0.4);
                } else {
                    LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager normal_rotate ： " << normal_rotate;
                    if (!normal_rotate) {
                        publishCmd(0, -0.2);
                    }
                    controlCmd = ControlCmd::REACH;
                }
                last_angle = angle_difference;
                break;
            }
            case ControlCmd::REACH: {
                publishCmd(0, 0);
                mainInterrupt = false;

                double distance_x = sqrt(pow(abs(odom_x - old_x), 2) + pow(abs(odom_y - old_y), 2));
                auto old_angle = old_yaw * 180.0 / M_PI;
                auto curr_angle = imu_yaw * 180.0 / M_PI;
                auto angle_difference = curr_angle - old_angle;
                if (angle_difference < 0) {
                    angle_difference += 360;
                }
                if (angle_difference > 180) {
                    angle_difference = 360 - angle_difference;
                }

                LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager over ： " << !mainInterrupt
                                             << "， * ： " << (distance_x - MOVING_DISTANCE)
                                             << "， * ： " << (180 - angle_difference);

                if (command == ControlCommand::ENTER_ELEVATOR) {
                    isConfirmEntry = true;
                    wait_entry_cv.notify_one();
                } else if (command == ControlCommand::EXIT_ELEVATOR) {
                    isConfirmExit = true;
                    wait_exit_cv.notify_one();
                }
                break;
            }

        }

        if (std::chrono::steady_clock::now() - last_send_time > std::chrono::seconds(MAXIMUM_DELAY_TIME - 1)) {
            sendDelayedDoorClosing();
            last_send_time = std::chrono::steady_clock::now();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME + append_sleep_time));
    }

}


void ElevatorControlManager::turn_controls_func() {
    mainInterrupt = true;
    controlCmd = ControlCmd::ROTATE;
    recordSensorData();

    while (mainInterrupt) {

        switch (controlCmd) {
            case ControlCmd::NONE:
                break;
            case ControlCmd::MOVE:
                break;
            case ControlCmd::ROTATE: {
                auto old_angle = old_yaw * 180.0 / M_PI;
                auto curr_angle = imu_yaw * 180.0 / M_PI;
                auto angle_difference = curr_angle - old_angle;
                if (angle_difference < 0) {
                    angle_difference += 360;
                }
                if (angle_difference > 180) {
                    angle_difference = 360 - angle_difference;
                }
                bool normal_rotate = true;
                if (controlCmd == ControlCmd::ROTATE && angle_difference > 90) {
                    normal_rotate = angle_difference - last_angle > -(SLEEP_TIME * INEXPLICABLE_MAGIC_NUMBER);
                    if (!normal_rotate) {
                        angle_difference = 188;
                    }
                }
                if (angle_difference < 180 - 100 * SLEEP_TIME * INEXPLICABLE_MAGIC_NUMBER) {// 0.00556789
                    publishCmd(0, 0.8);
                } else {
                    LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager normal_rotate ： " << normal_rotate;
                    if (!normal_rotate) {
                        publishCmd(0, -0.4);
                    }
                    controlCmd = ControlCmd::REACH;
                }
                last_angle = angle_difference;
                break;
            }
            case ControlCmd::REACH: {
                publishCmd(0, 0);
                mainInterrupt = false;

                auto old_angle = old_yaw * 180.0 / M_PI;
                auto curr_angle = imu_yaw * 180.0 / M_PI;
                auto angle_difference = curr_angle - old_angle;
                if (angle_difference < 0) {
                    angle_difference += 360;
                }
                if (angle_difference > 180) {
                    angle_difference = 360 - angle_difference;
                }

                LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager over ： " << !mainInterrupt
                                             << "， * ： " << (180 - angle_difference);

                setPlanCmd(true, "turn_controls_func");
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
    }

}

[[noreturn]] void ElevatorControlManager::serial_send_thread_func(boost::asio::serial_port &serial) {
    // 上次发送消息的时间点
    std::chrono::steady_clock::time_point last_send_time = std::chrono::steady_clock::now();

    while (true) {
        std::unique_lock<std::mutex> lock(queue_mutex);
        // 等待直到消息队列非空
        queue_cond.wait(lock, [this] {
            return !message_queue.empty();
        });

        auto now = std::chrono::steady_clock::now();
        // 确保两次消息发送之间至少有500毫秒的间隔
        if (now - last_send_time < std::chrono::milliseconds(THE_INTERVAL_BETWEEN_TWO_MESSAGES)) {
            lock.unlock();
//            LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager sleep " << " ";
            std::this_thread::sleep_for(
                    std::chrono::milliseconds(THE_INTERVAL_BETWEEN_TWO_MESSAGES) - (now - last_send_time));
            lock.lock();
        }

        auto message = message_queue.front();
        message_queue.pop();
        lock.unlock();

        bool isSend = true;
        // ID 处于等待响应的列表中，必须要等待响应
        if (inMessageEnum(message.id)) {
            message.waiting_response = true;
        }
        if (message.waiting_response) {
            if (inMessageEnum(message.id)) {
                // 检查消息是否已经在等待响应列表中
                auto iterator = sent_messages.find(message.id);
                if (iterator != sent_messages.end()) {
                    // 如果消息已存在，则调用回调函数并传递空响应
                    message.callback(EleProtocol::errorEleProtocol(), LadderControlError::FREQUENTLY);
                    isSend = false;
                }
            } else {
                // ID 不存在于枚举中，返回错误
                message.callback(EleProtocol::errorEleProtocol(), LadderControlError::ERROR);
                isSend = false;
            }
        }

        if (isSend) {
            try {
                // 发送消息
                //['0x7f 0xf7 0x17 0x29 0x01 0x00 0x11 0x22 0x33 0x44 0x55 0x66 0xc 0x61 0x1 0x2 0x3 0x4 0x5 0x6 0x7 0x8 0x9 0x10 0x11 0x12 0x37']
                //  0x7f 0xf7 0x17 0x29 0x80 0xd9 0x11 0x22 0x33 0x44 0x55 0x66 0xc 0x61 0x1 0x2 0x3 0x4 0x5 0x6 0x7 0x8 0x9 0x10 0x11 0x12 0x6f
                if (SERIAL_PORT_SEND_PRINT) {
                    std::cout << "write  ";
                    for (auto byte: message.data) {
                        std::cout << "0x" << std::hex << static_cast<int>(byte) << " ";
                    }
                    std::cout << std::endl;

                    LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager write ： " << "0x"
                                                 << std::hex << static_cast<int>(message.data[13]) << "  ";
                }

                boost::asio::write(serial, boost::asio::buffer(message.data));

//                uint8_t cmd = message.data[13];
//                if (cmd == CMD_DELAYED_DOOR_CLOSING) {
//                    std::cout << "write  ";
//                    for (auto byte: message.data) {
//                        std::cout << "0x" << std::hex << static_cast<int>(byte) << " ";
//                    }
//                    std::cout << std::endl;
//                }

                last_send_time = std::chrono::steady_clock::now();

                if (message.waiting_response) {
                    // 如果需要等待响应，则将消息及其回调函数添加到等待响应列表中
                    std::lock_guard<std::mutex> map_lock(map_mutex);
                    sent_messages[message.id] = std::make_pair(last_send_time, message.callback);
                }
            } catch (const std::exception &e) {
                LOG_IF(ERROR, DEBUG_ELEVATOR) << e.what();
            } catch (...) {
                LOG_IF(ERROR, DEBUG_ELEVATOR) << "write error .";
            }
        }

    }
}

[[noreturn]] void ElevatorControlManager::serial_receive_thread_func(boost::asio::serial_port &serial) {

//    std::vector<char> buf(128);

    boost::system::error_code ec;

    std::vector<unsigned char> buffer;

    while (true) {
        // 1. 实际为单字节读取，有bug
//        size_t len = serial.read_some(boost::asio::buffer(buf), ec);
        // 2. boost::asio::read 从 serial 串口读取数据，直到填满 buf 缓冲区或发生错误。也有bug，粘包现象严重
//        size_t len = boost::asio::read(serial, boost::asio::buffer(buf), ec);
        // 3. 读取指定数量的字节：boost::asio::read 函数可以指定需要读取的字节量。这在您知道预期要接收的数据量时非常有用。
//        size_t bytes_to_read = 128; // 假设我们想读取 64 个字节
//        std::vector<char> buf(bytes_to_read);
//        size_t len = boost::asio::read(serial, boost::asio::buffer(buf), boost::asio::transfer_exactly(bytes_to_read), ec);
        // 4. 使用 Completion Condition：Completion Condition 允许您指定一个更复杂的完成条件。例如，您可以设置一个函数，该函数决定是否已经读取了足够的数据。
//        size_t my_completion_condition(const boost::system::error_code& ec, std::size_t bytes_transferred) {
//            // 根据错误码和已传输的字节数决定是否完成读取
//            // 返回 0 表示完成，返回更大的数表示还需要读取更多字节
//        }
//        size_t len = boost::asio::read(serial, boost::asio::buffer(buf), my_completion_condition, ec);

//        if (ec) {
//            LOG_IF(ERROR, DEBUG_ELEVATOR) << "Read error: " << ec.message();
//            break;
//        }
//
//        for (auto byte: buf) {
//            std::cout << "0x" << std::hex << static_cast<int>(byte) << " ";
//        }
//        std::cout << std::endl;

        std::vector<unsigned char> temp_buf(128);
        size_t len = serial.read_some(boost::asio::buffer(temp_buf), ec);
        if (ec) {
            buffer.clear();
            LOG_IF(ERROR, DEBUG_ELEVATOR) << "Read error: " << ec.message();
            break;
        }

        if (len <= 0) {
            continue;
        }

        // 累积数据到 buffer
        buffer.insert(buffer.end(), temp_buf.begin(), temp_buf.begin() + len);

        // 检查是否接收到新的消息头
        if (buffer[0] != 0x7f) {
            // 消息头不正确，清除缓冲区
            buffer.clear();
            continue;
        }

        // 检查缓冲区长度是否足够，最低标准是 15 字节
        if (buffer.size() < 15) {
            // 消息至少需要有头部（2）、长度字节（1）、flag（1）、addr（2）、mac（6）、len（1）、cmd（1）、check（1）
            continue;
        }

        // 获取消息长度
        size_t temp_length = static_cast<unsigned char>(buffer[2]);
        if (buffer.size() < temp_length + 4) { // 加 4 是因为包括 Header、Self、Check 字节
            continue; // 缓冲区长度不足
        }

        // 提取各个字段
        size_t dataLengthSize = static_cast<unsigned char>(buffer[12]);

        std::vector<uint8_t> header(buffer.begin(), buffer.begin() + 2);
        uint8_t length = buffer[2];
        uint8_t flag = buffer[3];
        std::vector<uint8_t> address(buffer.begin() + 4, buffer.begin() + 6);
        std::vector<uint8_t> mac(buffer.begin() + 6, buffer.begin() + 12);
        uint8_t dataLength = buffer[12];
        uint8_t cmd = buffer[13];
        std::vector<uint8_t> data(buffer.begin() + 14, buffer.begin() + 14 + dataLengthSize);
        uint8_t checksum = buffer.back();
        bool check = EleProtocol::checkCalculateChecksum(length, flag, address, mac, dataLength, cmd, data, checksum);

        if (SERIAL_PORT_ACCEPT_PRINT) {
            std::cout << "read  " << check << "  ";
            for (auto byte: buffer) {
                std::cout << "0x" << std::hex << static_cast<int>(byte) << " ";
            }
            std::cout << std::endl;

            if (cmd == CMD_DELAYED_DOOR_CLOSING) {
                std::cout << "read  " << check << "  ";
                for (auto byte: buffer) {
                    std::cout << "0x" << std::hex << static_cast<int>(byte) << " ";
                }
                std::cout << std::endl;
            }
        }

        if (!check) {
            // 校验失败，清除缓冲区
            buffer.clear();
            continue;
        }

        std::vector<uint8_t> received_data{buffer.begin(), buffer.begin() + length + 4};
        if (MessageFactory::charInMessageIdEnum(cmd)) {
            int receivedMessageId = MessageFactory::charToMessageId(cmd);

            // 处理接收到的消息
            std::lock_guard<std::mutex> map_lock(map_mutex);
            auto it = sent_messages.find(receivedMessageId);
            if (it != sent_messages.end()) {
                // 调用回调函数，表示消息已正常接收
                EleProtocol eleProtocol(header, length, flag, address, mac, dataLength, cmd, data, checksum);
                LOG(INFO) << eleProtocol;
                if (it->second.second != nullptr)
                    it->second.second(eleProtocol, LadderControlError::NORMAL);
                sent_messages.erase(it);
            }

            // 检查超时的消息
            auto now = std::chrono::steady_clock::now();
            for (auto it = sent_messages.begin(); it != sent_messages.end();) {
                if (now - it->second.first > std::chrono::seconds(1)) {
                    // 调用回调函数，表示消息超时
                    it->second.second(EleProtocol::errorEleProtocol(), LadderControlError::TIMEOUT);
                    it = sent_messages.erase(it);
                } else {
                    ++it;
                }
            }
        } else {
            // 处理其他类型的消息，直接给成员变量即可
            if (cmd == CMD_QUERY_FLOOR_WHERE_LOCATED) {

                if (dataLengthSize >= 1) {
                    recentlyFloor = static_cast<unsigned char>(data[0]);//某个楼层
                }
                if (dataLengthSize >= 4) {
                    recentlyElevatorStatus = static_cast<unsigned char>(data[1]);//电梯状态
                }
                recentlyDataLengthSize = dataLengthSize;

                if (SERIAL_PORT_ACCEPT_PRINT) {
                    int cFloor = EleStatus::floorRule(recentlyFloor);
                    auto status = EleStatus::parseElevatorStatus(recentlyElevatorStatus);
                    LOG_IF(INFO, DEBUG_ELEVATOR)
                                    << "ElevatorControlManager 电梯状态"
                                    << "  dataSize : " << recentlyDataLengthSize
                                    << "  floor : " << cFloor
                                    << "  doorState : " << ElevatorStatus::printDoorState(status.doorState)
                                    << "  lastDirection : " << ElevatorStatus::printLastDirection(status.lastDirection)
                                    << "  availability : " << ElevatorStatus::printAvailability(status.availability)
                                    << "  nextDirection : " << ElevatorStatus::printNextDirection(status.nextDirection)
                                    << " ... ";
                }

//                if (dataLengthSize >= 16) {
//                    // 预留(2byte)
//                    std::vector<uint8_t> residenceTime(data.begin() + 4, data.begin() + 6);//楼层停留时间
//                    size_t openTime = static_cast<unsigned char>(data[6]);//开门时间
//                    std::vector<uint8_t> historicalFloors(data.begin() + 7, data.begin() + 12);//历史楼层
//                    // 预留(4byte)
//                }

            } else if (cmd == CMD_LIGHT_UP_TARGET_FLOOR) {
//                EleProtocol eleProtocol(header, length, flag, address, mac, dataLength, cmd, data, checksum);
//                LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager read ： " << eleProtocol << "  ";
            } else if (cmd == CMD_DELAYED_DOOR_CLOSING) {
//                EleProtocol eleProtocol(header, length, flag, address, mac, dataLength, cmd, data, checksum);
//                LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager read ： " << eleProtocol << "  ";
            }

        }

        // 清除处理过的消息
        buffer.erase(buffer.begin(), buffer.begin() + length + 4);
    }
}

void ElevatorControlManager::light_up_thread_func() {
    while (true) {
        std::unique_lock<std::mutex> lock(light_mutex);
        // 等待直到消息队列非空
        light_cond.wait(lock, [this] {
            return inLight;
        });
//        LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager pre 点亮楼层 " << lightFloor << " ... ";
        sendSimpleLightUpTargetFloor(lightFloor);
        std::this_thread::sleep_for(std::chrono::milliseconds(THE_TIME_INTERVAL_FOR_CONTINUOUSLY_LIGHTING_UP_FLOORS));
    }
}

void ElevatorControlManager::query_floor_thread_func() {
    while (true) {
        std::unique_lock<std::mutex> lock(query_mutex);
        // 等待直到消息队列非空
        query_cond.wait(lock, [this] {
            return inquiry;
        });
//        LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager pre 楼层查询 ... ";
        sendAsyncMessage(EleProtocol(CMD_QUERY_FLOOR_WHERE_LOCATED, mElevatorAddress));
        std::this_thread::sleep_for(std::chrono::milliseconds(THE_TIME_INTERVAL_FOR_CONTINUOUS_FLOOR_QUERIES));
    }
}

void ElevatorControlManager::arrive_floor_thread_func() {
    while (true) {
        std::unique_lock<std::mutex> lock(arrive_mutex);
        // 等待直到消息队列非空
        arrive_cond.wait(lock, [this] {
            return unseal;
        });
        std::this_thread::sleep_for(std::chrono::milliseconds(THE_TIME_INTERVAL_FOR_CONTINUOUS_FLOOR_DETERMINATION));

        int cFloor = EleStatus::floorRule(recentlyFloor);
        auto status = EleStatus::parseElevatorStatus(recentlyElevatorStatus);
        LOG_IF(INFO, DEBUG_ELEVATOR)
                        << "ElevatorControlManager 电梯状态"
                        << "  dataSize : " << recentlyDataLengthSize
                        << "  floor : " << cFloor
                        //                        << "  doorState : " << ElevatorStatus::printDoorState(status.doorState)
                        //                        << "  lastDirection : " << ElevatorStatus::printLastDirection(status.lastDirection)
                        //                        << "  availability : " << ElevatorStatus::printAvailability(status.availability)
                        //                        << "  nextDirection : " << ElevatorStatus::printNextDirection(status.nextDirection)
                        << " ... ";

        if (mElevatorCallback != nullptr)
            mElevatorCallback(cFloor, static_cast<int>(status.doorState), static_cast<int>(status.lastDirection),
                              static_cast<int>(status.availability), static_cast<int>(status.nextDirection));

        bool isArrived = false;
        if (recentlyDataLengthSize != 0) {
            if (mTargetFloor == cFloor) {
                if (JUMP_ELEVATOR_STATUS_DOOR_STATE) {
                    isArrived = true;
                } else {
                    if (recentlyDataLengthSize == 1) {
                        isArrived = true;
                    } else if (recentlyDataLengthSize == 4) {
                        if (status.doorState == ElevatorStatus::DoorState::Open) {
                            isArrived = true;
                        }
                    }
                }
            }
        }

        if (imitateArrivedCount > 10) {
            LOG_IF(INFO, DEBUG_ELEVATOR)
                            << "ElevatorControlManager pre 模拟已经到达 " << mTargetFloor << " 层 ... ";
            isArrived = true;
        }

        if (isArrived) {

            closeLightUp();
            waitDelayClosingDoor();
            recentlyDataLengthSize = 0;
            unseal = false;
            mElevatorArrived = true;
            wait_from_cv.notify_one();
            wait_to_cv.notify_one();
        }
    }
}

[[noreturn]] void ElevatorControlManager::elevator_pre_thread_func() {

    while (true) {

        std::unique_lock<std::mutex> lk(pre_mutex_);
        pre_condition_variable_.wait(lk, [this] {
            return preState != ElevatorPreState::PRE_NONE;
        });
        if (preState == ElevatorPreState::PRE_CIRCULATION) {
            doPreCirculation();
        } else if (preState == ElevatorPreState::PRE_ELEVATOR_IN) {
            doPreElevatorIn();
        } else if (preState == ElevatorPreState::PRE_SWITCH_MAP) {
            doPreSwitchMap();
        } else if (preState == ElevatorPreState::PRE_ELEVATOR_OUT) {
            doPreElevatorOut();
        } else if (preState == ElevatorPreState::PRE_OVER) {
            LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager pre over " << !preError << " ... ";
            planElevatorRelatedInterrupt = false;
            callbackElevatorPre(!preError);
        }

        preState = ElevatorPreState::PRE_NONE;
    }

}

[[noreturn]] void ElevatorControlManager::elevator_post_thread_func() {

    while (true) {

        std::unique_lock<std::mutex> lk(post_mutex_);
        post_condition_variable_.wait(lk, [this] {
            return postState != ElevatorPostState::POST_NONE;
        });
        if (postState == ElevatorPostState::POST_CIRCULATION) {
            doPostCirculation();
        } else if (postState == ElevatorPostState::POST_ELEVATOR_IN) {
            doPostElevatorIn();
        } else if (postState == ElevatorPostState::POST_SWITCH_MAP) {
            doPostSwitchMap();
        } else if (postState == ElevatorPostState::POST_ELEVATOR_OUT) {
            doPostElevatorOut();
        } else if (postState == ElevatorPostState::POST_OVER) {
            LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager post over " << !postError << " ... ";
            callbackElevatorPost(!postError);
        }

        postState = ElevatorPostState::POST_NONE;
    }
}

void ElevatorControlManager::doPreCirculation() {
    pool_.execute([this]() {
        try {
            if (preCirculationBlock.plannerPoints.empty())
                throw std::runtime_error("preCirculationBlock plannerPoints is empty ...");
            if (preAdjustmentFrequency == 0) {
                LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager pre 开始移动到电梯点位 ... ";
                PointPlanner::instance().goToPath(preCirculationBlock);
            } else if (preAdjustmentFrequency < MAXIMUM_NUMBER_OF_RETRY_ATTEMPTS_FOR_ERRORS_TO_THE_ELEVATOR) {
                LOG_IF(INFO, DEBUG_ELEVATOR)
                                << "ElevatorControlManager pre 开始调整电梯点位，"
                                << "总次数为 " << (MAXIMUM_NUMBER_OF_RETRY_ATTEMPTS_FOR_ERRORS_TO_THE_ELEVATOR - 1)
                                << " ，当前次数为 " << preAdjustmentFrequency
                                << " 次 ... ";
                auto point = preCirculationBlock.plannerPoints[0];
                point.core_move = true;
                PointPlanner::instance().goToPoint(point);
            }
            preAdjustmentFrequency++;
        } catch (app::exception const &e) {
            LOG_IF(ERROR, DEBUG_ELEVATOR) << e.what();
            goPreError();
        } catch (const std::exception &e) {
            LOG_IF(ERROR, DEBUG_ELEVATOR) << e.what();
            goPreError();
        } catch (...) {
            LOG_IF(ERROR, DEBUG_ELEVATOR) << "doPreCirculation other exception";
            goPreError();
        }
    });
}

void ElevatorControlManager::doPreElevatorIn() {
    pool_.execute([this]() {
        try {
            if (preElevatorInBlock.plannerPoints.empty())
                throw std::runtime_error("preElevatorInBlock plannerPoints is empty ...");
            auto point = preElevatorInBlock.plannerPoints[0];
            std::pair<int, int> &floorPair = point.targetFloorPair;
            auto fromFloor = floorPair.first;
            auto toFloor = floorPair.second;

            auto outPoint = preElevatorInBlock.plannerPoints[1];
            auto inPoint = preElevatorInBlock.plannerPoints[2];

            takeElevatorIn(fromFloor, toFloor, preElevatorInBlock.plannerPoints[1],
                           preElevatorInBlock.plannerPoints[2]);

            {
                std::unique_lock<std::mutex> lk(pre_mutex_);
                preState = ElevatorPreState::PRE_SWITCH_MAP;
            }
            pre_condition_variable_.notify_one();

        } catch (app::exception const &e) {
            LOG_IF(ERROR, DEBUG_ELEVATOR) << e.what();
            goPreError();
        } catch (const std::exception &e) {
            LOG_IF(ERROR, DEBUG_ELEVATOR) << e.what();
            goPreError();
        } catch (...) {
            LOG_IF(ERROR, DEBUG_ELEVATOR) << "doPreElevatorIn other exception";
            goPreError();
        }
    });
}

void ElevatorControlManager::doPreSwitchMap() {
    pool_.execute([this]() {
        try {
            if (preSwitchMapBlock.plannerPoints.empty())
                throw std::runtime_error("preSwitchMapBlock plannerPoints is empty ...");

            auto mapPoint = preSwitchMapBlock.plannerPoints[0];
            auto mapPair = mapPoint.targetMapIdPair;
            auto fromMapId = mapPair.first;
            auto toMapId = mapPair.second;

            LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager doPreSwitchMap 开始切换地图 "
                                         << " fromMapId : " << fromMapId
                                         << " toMapId : " << toMapId
                                         << "... ";
            if (Environment::instance().isRealEnvironment) {
                if (fromMapId != toMapId) {
                    switchMapsInWorkMode(fromMapId, toMapId);
                    poseEstimate(mapPoint);
                }
            }

            {
                std::unique_lock<std::mutex> lk(pre_mutex_);
                preState = ElevatorPreState::PRE_ELEVATOR_OUT;
            }
            pre_condition_variable_.notify_one();
        } catch (app::exception const &e) {
            LOG_IF(ERROR, DEBUG_ELEVATOR) << e.what();
            goPreError();
        } catch (const std::exception &e) {
            LOG_IF(ERROR, DEBUG_ELEVATOR) << e.what();
            goPreError();
        } catch (...) {
            LOG_IF(ERROR, DEBUG_ELEVATOR) << "doPreSwitchMap other exception";
            goPreError();
        }
    });
}

void ElevatorControlManager::doPreElevatorOut() {
    pool_.execute([this]() {
        try {
            if (preElevatorOutBlock.plannerPoints.empty())
                throw std::runtime_error("preElevatorOutBlock plannerPoints is empty ...");
            auto point = preElevatorOutBlock.plannerPoints[0];
            std::pair<int, int> &floorPair = point.targetFloorPair;
            auto fromFloor = floorPair.first;
            auto toFloor = floorPair.second;

            takeElevatorOut(fromFloor, toFloor, preElevatorOutBlock.plannerPoints[1],
                            preElevatorOutBlock.plannerPoints[2]);

            {
                std::unique_lock<std::mutex> lk(pre_mutex_);
                preState = ElevatorPreState::PRE_OVER;
            }
            pre_condition_variable_.notify_one();
        } catch (app::exception const &e) {
            LOG_IF(ERROR, DEBUG_ELEVATOR) << e.what();
            goPreError();
        } catch (const std::exception &e) {
            LOG_IF(ERROR, DEBUG_ELEVATOR) << e.what();
            goPreError();
        } catch (...) {
            LOG_IF(ERROR, DEBUG_ELEVATOR) << "doPreElevatorOut other exception";
            goPreError();
        }
    });
}

void ElevatorControlManager::goPreError() {
    closeQueryFloor();
    closeWaitingArrive();

    {
        std::unique_lock<std::mutex> lk(pre_mutex_);
        preError = true;
        preState = ElevatorPreState::PRE_OVER;
    }
    pre_condition_variable_.notify_one();
}

void ElevatorControlManager::doPostCirculation() {
    pool_.execute([this]() {
        try {
            if (postCirculationBlock.plannerPoints.empty())
                throw std::runtime_error("postCirculationBlock plannerPoints is empty ...");
            if (postAdjustmentFrequency == 0) {
                LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager post 开始移动到电梯点位 ... ";
                PointPlanner::instance().goToPath(postCirculationBlock);
            } else if (postAdjustmentFrequency < MAXIMUM_NUMBER_OF_RETRY_ATTEMPTS_FOR_ERRORS_TO_THE_ELEVATOR) {
                LOG_IF(INFO, DEBUG_ELEVATOR)
                                << "ElevatorControlManager post 开始调整电梯点位，"
                                << "总次数为 " << (MAXIMUM_NUMBER_OF_RETRY_ATTEMPTS_FOR_ERRORS_TO_THE_ELEVATOR - 1)
                                << " ，当前次数为 " << preAdjustmentFrequency
                                << " 次 ... ";
                auto point = postCirculationBlock.plannerPoints[0];
                point.core_move = true;
                PointPlanner::instance().goToPoint(point);
            }
            postAdjustmentFrequency++;
        } catch (app::exception const &e) {
            LOG_IF(ERROR, DEBUG_ELEVATOR) << e.what();
            goPostError();
        } catch (const std::exception &e) {
            LOG_IF(ERROR, DEBUG_ELEVATOR) << e.what();
            goPostError();
        } catch (...) {
            LOG_IF(ERROR, DEBUG_ELEVATOR) << "doPostCirculation other exception";
            goPostError();
        }
    });
}

void ElevatorControlManager::doPostElevatorIn() {
    pool_.execute([this]() {
        try {
            if (postElevatorInBlock.plannerPoints.empty())
                throw std::runtime_error("postElevatorInBlock plannerPoints is empty ...");
            auto point = postElevatorInBlock.plannerPoints[0];
            std::pair<int, int> &floorPair = point.targetFloorPair;
            auto fromFloor = floorPair.first;
            auto toFloor = floorPair.second;

            takeElevatorIn(fromFloor, toFloor, postElevatorInBlock.plannerPoints[1],
                           postElevatorInBlock.plannerPoints[2]);

            {
                std::unique_lock<std::mutex> lk(post_mutex_);
                postState = ElevatorPostState::POST_SWITCH_MAP;
            }
            post_condition_variable_.notify_one();
        } catch (app::exception const &e) {
            LOG_IF(ERROR, DEBUG_ELEVATOR) << e.what();
            goPostError();
        } catch (const std::exception &e) {
            LOG_IF(ERROR, DEBUG_ELEVATOR) << e.what();
            goPostError();
        } catch (...) {
            LOG_IF(ERROR, DEBUG_ELEVATOR) << "doPostElevatorIn other exception";
            goPostError();
        }
    });
}

void ElevatorControlManager::doPostSwitchMap() {
    pool_.execute([this]() {
        try {
            if (postSwitchMapBlock.plannerPoints.empty())
                throw std::runtime_error("postSwitchMapBlock plannerPoints is empty ...");

            auto mapPoint = postSwitchMapBlock.plannerPoints[0];
            auto mapPair = mapPoint.targetMapIdPair;
            auto fromMapId = mapPair.first;
            auto toMapId = mapPair.second;

            LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager doPostSwitchMap 开始切换地图 "
                                         << "fromMapId : " << fromMapId
                                         << "toMapId : " << toMapId
                                         << "... ";

            if (Environment::instance().isRealEnvironment) {
                if (fromMapId != toMapId) {
                    switchMapsInWorkMode(fromMapId, toMapId);
                    poseEstimate(mapPoint);
                }
            }

            {
                std::unique_lock<std::mutex> lk(post_mutex_);
                postState = ElevatorPostState::POST_ELEVATOR_OUT;
            }
            post_condition_variable_.notify_one();
        } catch (app::exception const &e) {
            LOG_IF(ERROR, DEBUG_ELEVATOR) << e.what();
            goPostError();
        } catch (const std::exception &e) {
            LOG_IF(ERROR, DEBUG_ELEVATOR) << e.what();
            goPostError();
        } catch (...) {
            LOG_IF(ERROR, DEBUG_ELEVATOR) << "doPostSwitchMap other exception";
            goPostError();
        }
    });
}

void ElevatorControlManager::doPostElevatorOut() {
    pool_.execute([this]() {
        try {
            if (postElevatorOutBlock.plannerPoints.empty())
                throw std::runtime_error("postElevatorOutBlock plannerPoints is empty ...");
            auto point = postElevatorOutBlock.plannerPoints[0];
            std::pair<int, int> &floorPair = point.targetFloorPair;
            auto fromFloor = floorPair.first;
            auto toFloor = floorPair.second;

            takeElevatorOut(fromFloor, toFloor, postElevatorOutBlock.plannerPoints[1],
                            postElevatorOutBlock.plannerPoints[2]);

            {
                std::unique_lock<std::mutex> lk(post_mutex_);
                postState = ElevatorPostState::POST_OVER;
            }
            post_condition_variable_.notify_one();
        } catch (app::exception const &e) {
            LOG_IF(ERROR, DEBUG_ELEVATOR) << e.what();
            goPostError();
        } catch (const std::exception &e) {
            LOG_IF(ERROR, DEBUG_ELEVATOR) << e.what();
            goPostError();
        } catch (...) {
            LOG_IF(ERROR, DEBUG_ELEVATOR) << "doPostElevatorOut other exception";
            goPostError();
        }
    });
}

void ElevatorControlManager::goPostError() {
    {
        std::unique_lock<std::mutex> lk(post_mutex_);
        postError = true;
        postState = ElevatorPostState::POST_OVER;
    }
    post_condition_variable_.notify_one();
}

/**
 1. 查询所在楼层 0x61
 2. 呼叫电梯到 fromFloor 层 0x60
 3. 等待电梯到达 fromFloor
    3.x 自动开门（一条多次）
 4. 进电梯
    4.1 延迟关闭电梯
 5. 呼叫电梯到 toFloor 层 0x60
 6. 等待电梯到达 toFloor
 7. 出电梯
    4.1 延迟关闭电梯
 */
void ElevatorControlManager::takeElevatorIn(int fromFloor, int toFloor, const RealPoint &fromOutPoint,
                                            const RealPoint &fromInPoint) {
    closeLightUp();
    closeQueryFloor();
    closeWaitingArrive();

    LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager 乘坐电梯从 " << fromFloor << " 到 " << toFloor << " 层 ...";

    // 1
    LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager 开启楼层点亮功能 " << fromFloor << " ... ";
    openLightUp(fromFloor);
    openWaitingArrive(fromFloor);

    // 2
    LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager 开启楼层查询功能 ... ";
    openQueryFloor();

    // 3
    std::unique_lock<std::mutex> from_lock(wait_from_mutex);
    if (!wait_from_cv.wait_for(from_lock, std::chrono::milliseconds(MAXIMUM_WAITING_TIME_FOR_ELEVATOR),
                               [this] { return mElevatorArrived; }))
        throw std::runtime_error("takeElevatorIn wait_from_mutex timeout ...");
    closeWaitingArrive();
    LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager 电梯已经到达 " << fromFloor << " 层 ... ";

    // 4
    LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager 执行进入电梯逻辑 ... ";
    isConfirmEntry = false;
    enterElevator(fromInPoint);
    std::unique_lock<std::mutex> entry_lock(wait_entry_mutex);
    if (!wait_entry_cv.wait_for(entry_lock,
                                std::chrono::milliseconds(MAXIMUM_TIME_FOR_ENTERING_AND_EXITING_THE_ELEVATOR),
                                [this] { return isConfirmEntry; }))
        throw std::runtime_error("takeElevatorIn wait_entry_mutex timeout ...");
    if (!isElevatorEntryResult)
        throw std::runtime_error("takeElevatorIn wait_entry_mutex error ...");
    closeWaitingArrive();
    LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager 已进入电梯 ... ";

    // 5
    LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager 开启楼层点亮功能 " << toFloor << " ... ";
    openLightUp(toFloor);
}

void ElevatorControlManager::takeElevatorOut(int fromFloor, int toFloor, const RealPoint &toOutPoint,
                                             const RealPoint &toInPoint) {
    openWaitingArrive(toFloor);

    // 6
    std::unique_lock<std::mutex> to_lock(wait_to_mutex);
    if (!wait_to_cv.wait_for(to_lock, std::chrono::milliseconds(MAXIMUM_WAITING_TIME_FOR_ELEVATOR),
                             [this] { return mElevatorArrived; }))
        throw std::runtime_error("takeElevatorOut wait_to_mutex timeout ...");
    LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager 电梯已经到达 " << toFloor << " 层 ... ";

    for (int i = 0; i < MAXIMUM_NUMBER_OF_ENTERING_THE_ELEVATOR; i++) {
        sendDelayedDoorClosing();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    // 7
    LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager 执行离开电梯逻辑 ... ";
    isConfirmExit = false;
    exitElevator(toOutPoint);
    std::unique_lock<std::mutex> exit_lock(wait_exit_mutex);
    if (!wait_exit_cv.wait_for(exit_lock, std::chrono::milliseconds(MAXIMUM_TIME_FOR_ENTERING_AND_EXITING_THE_ELEVATOR),
                               [this] { return isConfirmExit; }))
        throw std::runtime_error("takeElevatorOut wait_exit_mutex timeout ...");
    if (!isElevatorExitResult)
        throw std::runtime_error("takeElevatorOut wait_exit_mutex error ...");
    LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager 已离开电梯 ... ";

    closeQueryFloor();
    closeWaitingArrive();
}

void ElevatorControlManager::switchMapsInWorkMode(const std::string &fromMapId, const std::string &toMapId) {

    MapPo oldMap = SegmentationDataBase::instance().getDbMap();
    if (oldMap.id != fromMapId)
        throw std::runtime_error("from map data error ， oldMap ： " + oldMap.id
                                 + "， fromMapId ： " + fromMapId + " ...");


    const std::vector<MapPo> &allMap = SegmentationDataBase::instance().loadAllMap();
    bool isFind = false;
    for (const auto &item: allMap) {
        if (item.id == toMapId) {
            isFind = true;
            break;
        }
    }
    if (!isFind)
        throw std::runtime_error("not find toMapId : " + toMapId + " ...");

    // 确保文件存在
    if (!MapControl::instance().checkMapInformation(toMapId))
        throw std::runtime_error("not exist toMapId : " + toMapId + " ...");

    if (Environment::instance().isRealEnvironment) {
        // 备份之前的地图
        MapControl::instance().backupAndRetrieve(fromMapId);
        // 改变为新地图信息
        SegmentationDataBase::instance().changeMap(toMapId);
        SegmentationDataBase::instance().loadMainMap();
        // 加载新资源
        MapControl::instance().loadInformation(toMapId);
        // 重新加载基站信息
        MapAttributeSingleton::instance().loadStation();
        // 更新内存中禁区
        MapAttributeSingleton::instance().resetProhibition();
        MapAttributeSingleton::instance().loadVirtualWall();
        MapAttributeSingleton::instance().loadPenaltyZone();
        // 更新内存中定时任务
        ScheduleManagerSingleton::instance().trigger_task_update();
        // 重新规划牛耕田算法的全覆盖
        ExplorationCenter::instance().repaintCoveragePath();


        int step = NodeControl::instance().restoreWork();
        if (step != 2)
            throw std::runtime_error("map switching failed toMapId : " + toMapId + " ...");

        MapControl::instance().changeMapServer();
        // 发布给 move_base 最新的禁行区域
        PublishInnerManager::instance().publishResetProhibition();
    } else {
        // 改变为新地图信息
        SegmentationDataBase::instance().changeMap(toMapId);
        SegmentationDataBase::instance().loadMainMap();
    }

}

void ElevatorControlManager::sendLightUpTargetFloor(int floor, const MessageSuccessCallback &successCallback) {
    LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager pre 点亮楼层 " << floor << " ... ";
//    EleProtocol eleProtocol(CMD_LIGHT_UP_TARGET_FLOOR, mElevatorAddress);
//    std::vector<uint8_t> data;
//    data.push_back(EleStatus::reverseFloorRule(floor));
//    eleProtocol.setData(data);
//    eleProtocol.setAddress(mElevatorAddress);
//    sendSyncMessage(MessageFactory::charToMessageId(CMD_LIGHT_UP_TARGET_FLOOR), eleProtocol.getProtocol(),
//                    successCallback,
//                    [this](LadderControlError) {
//                        LOG_IF(ERROR, DEBUG_ELEVATOR) << "sendLightUpTargetFloor error ...";
//                        goPreError();
//                    });
}

void ElevatorControlManager::sendSimpleLightUpTargetFloor(int floor) {
    EleProtocol eleProtocol(CMD_LIGHT_UP_TARGET_FLOOR, mElevatorAddress);
    std::vector<uint8_t> data;
    data.push_back(EleStatus::reverseFloorRule(floor));
    eleProtocol.setData(data);
    eleProtocol.setAddress(mElevatorAddress);
    sendAsyncMessage(eleProtocol);
}

void ElevatorControlManager::sendDelayedDoorClosing() {
//    LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager 延迟关门 ... ";
    EleProtocol eleProtocol(CMD_DELAYED_DOOR_CLOSING, mElevatorAddress);
    std::vector<uint8_t> data;
    data.push_back(MAXIMUM_DELAY_TIME);
    eleProtocol.setData(data);
    eleProtocol.setAddress(mElevatorAddress);
    sendAsyncMessage(eleProtocol);
    // 崩溃
//    sendSyncMessage(MessageFactory::charToMessageId(CMD_DELAYED_DOOR_CLOSING), eleProtocol.getProtocol(),
//                    [](const EleProtocol &ele) {
//                        LOG_IF(ERROR, DEBUG_ELEVATOR) << "sendDelayedDoorClosing " << ele << " ...";
//                    },
//                    [this](LadderControlError) {
//                        LOG_IF(ERROR, DEBUG_ELEVATOR) << "sendDelayedDoorClosing error ...";
//                        goPreError();
//                    });
}

void ElevatorControlManager::sendSyncMessage(ElevatorControlManager::MessageIdEnum messageId,
                                             const std::vector<uint8_t> &data,
                                             const MessageSuccessCallback &successCallback,
                                             const MessageFailCallback &failCallback) {
//    MessageCallback messageCallback = [&successCallback, &failCallback](const EleProtocol &response,
//                                                                        LadderControlError error) {
//        switch (error) {
//            case LadderControlError::NORMAL:
//                // "Message normal.";
//                std::cout << "Message normal. " << response << std::endl;
//                successCallback(response);
//                break;
//            case LadderControlError::TIMEOUT:
//                // "Message timeout.";
//                if (SERIAL_PORT_PRINT)
//                    std::cout << "Message timeout." << std::endl;
//                failCallback(error);
//                break;
//            case LadderControlError::FREQUENTLY:
//                // "Message frequently.";
//                if (SERIAL_PORT_PRINT)
//                    std::cout << "Message frequently." << std::endl;
//                failCallback(error);
//                break;
//            case LadderControlError::ERROR:
//                // "Message other.";
//                if (SERIAL_PORT_PRINT)
//                    std::cout << "Message other." << std::endl;
//                failCallback(error);
//                break;
//            default:
//                // "Received response: ";
//                if (SERIAL_PORT_PRINT)
//                    std::cout << "Received response: " << std::endl;
//                failCallback(error);
//                break;
//        }
//    };
//
//    auto message = MessageFactory::createSyncMessage(messageId, data, messageCallback);
//
//    message_queue.push(message);
//    queue_cond.notify_one();
}

void ElevatorControlManager::sendAsyncMessage(EleProtocol eleProtocol) {
    std::vector<uint8_t> data = eleProtocol.getProtocol();
    auto message = MessageFactory::createAsyncMessage(data);
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        message_queue.push(message);
        queue_cond.notify_one();
    }
}

void ElevatorControlManager::openLightUp(int floor) {
    lightFloor = floor;
    inLight = true;
    light_cond.notify_one();
}

void ElevatorControlManager::closeLightUp() {
    inLight = false;
    light_cond.notify_one();
}

void ElevatorControlManager::openQueryFloor() {
    inquiry = true;
    query_cond.notify_one();
}

void ElevatorControlManager::closeQueryFloor() {
    inquiry = false;
    query_cond.notify_one();
}

void ElevatorControlManager::openWaitingArrive(int targetFloor) {
    recentlyDataLengthSize = 0;
    mTargetFloor = targetFloor;
    mElevatorArrived = false;
    imitateArrivedCount = 0;
    unseal = true;
    arrive_cond.notify_one();
}

void ElevatorControlManager::closeWaitingArrive() {
    mElevatorArrived = false;
    imitateArrivedCount = 0;
    unseal = false;
    arrive_cond.notify_one();
}

void ElevatorControlManager::poseEstimate(const RealPoint &realPoint) {
    LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager poseEstimate 重新定位 " << "... ";

    LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager poseEstimate " <<
                                 "  realPoint.realPosition.x : " << realPoint.realPosition.x <<
                                 "  realPoint.realPosition.y : " << realPoint.realPosition.y <<
                                 "  realPoint.realPosition.z : " << realPoint.realPosition.z <<
                                 "  realPoint.realOrientation.x : " << realPoint.realOrientation.x <<
                                 "  realPoint.realOrientation.y : " << realPoint.realOrientation.y <<
                                 "  realPoint.realOrientation.z : " << realPoint.realOrientation.z <<
                                 "  realPoint.realOrientation.w : " << realPoint.realOrientation.w <<
                                 "... ";
    geometry_msgs::PoseWithCovarianceStamped original_pose;
    original_pose.header.frame_id = "map";
    original_pose.header.stamp = ros::Time::now();
    original_pose.pose.pose.position.x = realPoint.realPosition.x;
    original_pose.pose.pose.position.y = realPoint.realPosition.y;
    original_pose.pose.pose.position.z = realPoint.realPosition.z;
    original_pose.pose.pose.orientation.x = realPoint.realOrientation.x;
    original_pose.pose.pose.orientation.y = realPoint.realOrientation.y;
    original_pose.pose.pose.orientation.z = realPoint.realOrientation.z;
    original_pose.pose.pose.orientation.w = realPoint.realOrientation.w;

//    // 将四元数转换为tf::Quaternion
//    tf::Quaternion original_orientation;
//    tf::quaternionMsgToTF(original_pose.pose.pose.orientation, original_orientation);
//
//    // 创建一个表示180度旋转的四元数（绕Z轴）
//    tf::Quaternion rotation;
//    rotation.setRPY(0, 0, M_PI); // 绕Z轴旋转180度
//
//    // 将原始方向和旋转相结合
//    tf::Quaternion new_orientation = original_orientation * rotation;
//
//    // 将新方向转换回geometry_msgs::Quaternion
//    geometry_msgs::Quaternion new_orientation_msg;
//    tf::quaternionTFToMsg(new_orientation, new_orientation_msg);
//
//    // 更新原始pose消息
//    original_pose.pose.pose.orientation = new_orientation_msg;

    publisherPose.publish(original_pose);
}

void ElevatorControlManager::setPlanCmd(bool arrive, const std::string &tag) {
    if (arrive) {
        if (enterPlanCmd == PLAN_NONE) {
            enterPlanCmd = PLAN_CORE_MOVE_SUCCESS;
        } else if (enterPlanCmd == PLAN_MOVE_BASE_ING) {
            enterPlanCmd = PLAN_MOVE_BASE_SUCCESS;
        } else {
            LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager " << tag << " " << arrive << " ... ";
        }
    } else {
        if (enterPlanCmd == PLAN_NONE) {
            enterPlanCmd = PLAN_CORE_MOVE_ERROR;
        } else if (enterPlanCmd == PLAN_MOVE_BASE_ING) {
            enterPlanCmd = PLAN_MOVE_BASE_ERROR;
        } else {
            LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager " << tag << " " << arrive << " ... ";
        }
    }

    if (arrive) {
        if (exitPlanCmd == PLAN_NONE) {
            exitPlanCmd = PLAN_CORE_MOVE_SUCCESS;
        } else if (exitPlanCmd == PLAN_MOVE_BASE_ING) {
            exitPlanCmd = PLAN_MOVE_BASE_SUCCESS;
        } else {
            LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager " << tag << " " << arrive << " ... ";
        }
    } else {
        if (exitPlanCmd == PLAN_NONE) {
            exitPlanCmd = PLAN_CORE_MOVE_ERROR;
        } else if (exitPlanCmd == PLAN_MOVE_BASE_ING) {
            exitPlanCmd = PLAN_MOVE_BASE_ERROR;
        } else {
            LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager " << tag << " " << arrive << " ... ";
        }
    }
}

void ElevatorControlManager::waitDelayClosingDoor() {
    for (int i = 0; i < MAXIMUM_NUMBER_OF_ENTERING_THE_ELEVATOR; i++) {
        sendDelayedDoorClosing();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void ElevatorControlManager::initialize(ros::NodeHandle handle) {
    interruptAccessElevators();
    pool_.setNumOfThreads(4);

    preState = ElevatorPreState::PRE_NONE;
    postState = ElevatorPostState::POST_NONE;

    subscriberOdom = handle.subscribe("/odom", 10, &ElevatorControlManager::subscribeOdomCallback, this);
    subscriberImu = handle.subscribe(Environment::instance().isRealEnvironment ? "/imu/data" : "/imu",
                                     10, &ElevatorControlManager::subscribeImuCallback, this);
    publisherCmdVel = handle.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
    publisherPose = handle.advertise<geometry_msgs::PoseWithCovarianceStamped>("/initialpose", 10);

    elevator_pre_thread = std::thread(&ElevatorControlManager::elevator_pre_thread_func, this);
    elevator_pre_thread.detach();
    elevator_post_thread = std::thread(&ElevatorControlManager::elevator_post_thread_func, this);
    elevator_post_thread.detach();


    std::string portName = Environment::instance().isRealEnvironment ? "/dev/elevator" : "/dev/ttyUSB0";

    try {

        if (!hasSerialPortAccess(portName)) {
            LOG_IF(ERROR, DEBUG_ELEVATOR) << "Error: No access to " << portName;
            LOG_IF(ERROR, DEBUG_ELEVATOR) << "Please check the permissions for the serial port.";
            LOG_IF(ERROR, DEBUG_ELEVATOR)
                            << "You may need to run this program as root or add your user to the dialout group (on Linux).";
            return;
        }

        boostSerial = new boost::asio::serial_port(boostIo, portName);
        //波特率（Baud Rate）:波特率是指每秒传输的比特（位）数。它是衡量串口通信速度的标准指标。设置波特率要确保与连接的设备匹配，否则可能会导致数据传输错误。
        boostSerial->set_option(boost::asio::serial_port_base::baud_rate(115200));
//        //字符大小（Character Size）:字符大小指的是串口通信中每个数据字节的位数。最常见的设置是 8 位，但有些系统或设备可能使用 7 位或其他大小。
//        boostSerial->set_option(boost::asio::serial_port_base::character_size(8));
//        //奇偶校验（Parity）:奇偶校验是一种错误检测机制，它可以是无（none）、奇数（odd）或偶数（even）。无奇偶校验意味着不进行错误检测。
//        boostSerial->set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
//        //停止位（Stop Bits）:停止位用于标识每个字符的结束。常见的设置有 1 个停止位和 2 个停止位。
//        boostSerial->set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
//        //流控制（Flow Control）:流控制用于管理数据传输的速率，以防止接收方的缓冲区溢出。流控制可以是硬件控制（hardware）、软件控制（software）或无（none）。
//        boostSerial->set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));


        serial_sender_thread = std::thread(&ElevatorControlManager::serial_send_thread_func, this,
                                           std::ref(*boostSerial));
        serial_receiver_thread = std::thread(&ElevatorControlManager::serial_receive_thread_func, this,
                                             std::ref(*boostSerial));
        light_up_thread = std::thread(&ElevatorControlManager::light_up_thread_func, this);
        query_floor_thread = std::thread(&ElevatorControlManager::query_floor_thread_func, this);
        arrive_floor_thread = std::thread(&ElevatorControlManager::arrive_floor_thread_func, this);

        serial_sender_thread.detach();
        serial_receiver_thread.detach();
        light_up_thread.detach();
        query_floor_thread.detach();
        arrive_floor_thread.detach();
    } catch (const std::exception &e) {
        LOG_IF(ERROR, DEBUG_ELEVATOR) << e.what();
    } catch (...) {
        LOG_IF(ERROR, DEBUG_ELEVATOR) << "open dev error .";
    }

    subscriberElevatorManager = handle.subscribe("/elevator_manager", 1,
                                                 &ElevatorControlManager::elevatorManagerSubscribeCallback,
                                                 this);

}

void ElevatorControlManager::setBuildElevatorAddress(int elevatorAddress) {
    mElevatorAddress = elevatorAddress;
}

void ElevatorControlManager::setElevatorCallback(ElevatorControlManager::ElevatorCallback callback) {
    ElevatorControlManager::mElevatorCallback = callback;
}

void ElevatorControlManager::setCallbackElevatorPre(const std::function<void(bool)> &callbackElevatorPre) {
    ElevatorControlManager::callbackElevatorPre = callbackElevatorPre;
}

void ElevatorControlManager::setCallbackElevatorPost(const std::function<void(bool)> &callbackElevatorPost) {
    ElevatorControlManager::callbackElevatorPost = callbackElevatorPost;
}

void ElevatorControlManager::printElevator() {
    double distance_x = sqrt(pow(abs(odom_x - old_x), 2) + pow(abs(odom_y - old_y), 2));
    double difference_yaw = imu_yaw - old_yaw;

    auto old_angle = old_yaw * 180.0 / M_PI;
    auto curr_angle = imu_yaw * 180.0 / M_PI;

    auto angle_difference = curr_angle - old_angle;
    if (angle_difference < 0) {
        angle_difference += 360;
    }
    if (angle_difference > 180) {
        angle_difference = 360 - angle_difference;
    }

    LOG_IF(INFO, DEBUG_ELEVATOR) << "distance_x ： " << distance_x
                                 << "， old_yaw ： " << old_yaw
                                 << "， imu_yaw ： " << imu_yaw
                                 << "， difference_yaw ： " << difference_yaw
                                 << "， old_angle ： " << old_angle
                                 << "， curr_angle ： " << curr_angle
                                 << "， angle_difference ： " << angle_difference;
}

void ElevatorControlManager::enterElevator() {
    LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager enterElevator 开始执行进电梯逻辑 ... ";
    pool_.execute([this] {
        movement_controls_func(ControlCommand::ENTER_ELEVATOR);
    });
}

void ElevatorControlManager::exitElevator() {
    LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager exitElevator 开始执行出电梯逻辑 ... ";
    pool_.execute([this] {
        movement_controls_func(ControlCommand::EXIT_ELEVATOR);
    });
}

void ElevatorControlManager::enterElevator(const RealPoint &point) {
    LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager enterElevator 开始执行进电梯逻辑 ... ";

    isElevatorEntryResult = false;
    planElevatorRelatedInterrupt = true;
    enterPlanCmd = PlanCmd::PLAN_NONE;
    enterPlanRetryCount = 0;

    RealPoint coreMovePoint;
//    realPoint.id = point.id;
//    realPoint.blockId = point.blockId;
    coreMovePoint.realPosition = point.realPosition;
    coreMovePoint.realOrientation = point.realOrientation;
//    realPoint.cmcMode = point.cmcMode;
//    realPoint.timeout = point.timeout;
//    realPoint.currentStep = point.currentStep;
    coreMovePoint.core_move = true;
//    realPoint.gateControl = point.gateControl;
//    realPoint.gate_uuid = point.gate_uuid;
//    realPoint.gate_factory_id = point.gate_factory_id;
//    realPoint.elevatorControl = point.elevatorControl;
//    realPoint.targetFloorPair = point.targetFloorPair;
//    realPoint.targetMapIdPair = point.targetMapIdPair;

    // 先用 core_move 进入电梯
    PointPlanner::instance().goToPoint(coreMovePoint);

//    LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager coreMovePoint " <<
//                                 " " << coreMovePoint.realPosition <<
//                                 " " << coreMovePoint.realOrientation <<
//                                 " ... ";

    pool_.execute([this, &point, &coreMovePoint] {
        sendDelayedDoorClosing();
        std::chrono::steady_clock::time_point last_send_time = std::chrono::steady_clock::now();
        while (planElevatorRelatedInterrupt) {
            int append_sleep_time = 0;

            switch (enterPlanCmd) {
                case PlanCmd::PLAN_NONE:

                    break;
                case PlanCmd::PLAN_CORE_MOVE_ERROR:
                    enterPlanCmd = PlanCmd::PLAN_AGAIN;// 失败直接进入重试逻辑
                    break;
                case PlanCmd::PLAN_CORE_MOVE_SUCCESS:// 再用 move_base 进行转向等二次调整
//                    LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager moveBasePoint " <<
//                                                 " " << point.realPosition <<
//                                                 " " << point.realOrientation <<
//                                                 " ... ";
//                    PointPlanner::instance().goToPoint(point, true);
//                    enterPlanCmd = PlanCmd::PLAN_MOVE_BASE_ING;

                    pool_.execute([this] {
                        turn_controls_func();
                    });
                    enterPlanCmd = PlanCmd::PLAN_MOVE_BASE_ING;
                    break;
                case PlanCmd::PLAN_MOVE_BASE_ING:

                    break;
                case PlanCmd::PLAN_MOVE_BASE_ERROR:
                    enterPlanCmd = PlanCmd::PLAN_AGAIN;// 失败直接进入重试逻辑
                    break;
                case PlanCmd::PLAN_MOVE_BASE_SUCCESS:// 进入电梯成功
                    LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager enterElevator 进电梯成功 ... ";
                    planElevatorRelatedInterrupt = false;
                    isElevatorEntryResult = true;
                    isConfirmEntry = true;
                    wait_entry_cv.notify_one();
                    break;
                case PlanCmd::PLAN_AGAIN:
                    if (enterPlanRetryCount > MAXIMUM_NUMBER_OF_RETRIES_FOR_ELEVATOR_LOGIC_ERRORS) {
                        LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager enterElevator 进电梯出错 ... ";
                        planElevatorRelatedInterrupt = false;
                        isElevatorEntryResult = false;
                        isConfirmEntry = true;
                        wait_entry_cv.notify_one();
                    } else {
//                        LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager enterElevator retry ... " <<
//                                                     " " << coreMovePoint.realPosition <<
//                                                     " " << coreMovePoint.realOrientation <<
//                                                     " ... ";;
                        PointPlanner::instance().goToPoint(coreMovePoint, true);
                        enterPlanCmd = PlanCmd::PLAN_NONE;
                    }
                    enterPlanRetryCount++;
                    break;
            }

            if (std::chrono::steady_clock::now() - last_send_time > std::chrono::seconds(MAXIMUM_DELAY_TIME - 1)) {
                sendDelayedDoorClosing();
                last_send_time = std::chrono::steady_clock::now();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME + append_sleep_time));
        }
    });
}

void ElevatorControlManager::exitElevator(const RealPoint &point) {
    LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager exitElevator 开始执行出电梯逻辑 ... ";

    isElevatorExitResult = false;
    planElevatorRelatedInterrupt = true;
    exitPlanCmd = PlanCmd::PLAN_NONE;
    exitPlanRetryCount = 0;

//    PointPlanner::instance().goToPoint(point, true);
//
//    LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager moveBasePoint " <<
//                                 " " << point.realPosition <<
//                                 " " << point.realOrientation <<
//                                 " ... ";

    RealPoint coreMovePoint;
//    realPoint.id = point.id;
//    realPoint.blockId = point.blockId;
    coreMovePoint.realPosition = point.realPosition;
    coreMovePoint.realOrientation = point.realOrientation;
//    realPoint.cmcMode = point.cmcMode;
//    realPoint.timeout = point.timeout;
//    realPoint.currentStep = point.currentStep;
    coreMovePoint.core_move = true;
//    realPoint.gateControl = point.gateControl;
//    realPoint.gate_uuid = point.gate_uuid;
//    realPoint.gate_factory_id = point.gate_factory_id;
//    realPoint.elevatorControl = point.elevatorControl;
//    realPoint.targetFloorPair = point.targetFloorPair;
//    realPoint.targetMapIdPair = point.targetMapIdPair;

    // 先用 core_move 出电梯
    PointPlanner::instance().goToPoint(coreMovePoint);

    LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager coreMovePoint " <<
                                 " " << coreMovePoint.realPosition <<
                                 " " << coreMovePoint.realOrientation <<
                                 " ... ";

    pool_.execute([this, &point] {
        sendDelayedDoorClosing();
        std::chrono::steady_clock::time_point last_send_time = std::chrono::steady_clock::now();
        while (planElevatorRelatedInterrupt) {
            int append_sleep_time = 0;

            switch (exitPlanCmd) {
                case PlanCmd::PLAN_NONE:

                    break;
                case PlanCmd::PLAN_CORE_MOVE_ERROR:
                    exitPlanCmd = PlanCmd::PLAN_AGAIN;
                    break;
                case PlanCmd::PLAN_CORE_MOVE_SUCCESS:
                    LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager exitElevator 出电梯成功 ... ";
                    planElevatorRelatedInterrupt = false;
                    isElevatorExitResult = true;
                    isConfirmExit = true;
                    wait_exit_cv.notify_one();
                    break;
                case PlanCmd::PLAN_MOVE_BASE_ING:

                    break;
                case PlanCmd::PLAN_MOVE_BASE_ERROR:
                    exitPlanCmd = PlanCmd::PLAN_AGAIN;
                    break;
                case PlanCmd::PLAN_MOVE_BASE_SUCCESS:
                    LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager exitElevator 出电梯成功 ... ";
                    planElevatorRelatedInterrupt = false;
                    isElevatorExitResult = true;
                    isConfirmExit = true;
                    wait_exit_cv.notify_one();
                    break;
                case PlanCmd::PLAN_AGAIN:
                    if (exitPlanRetryCount > MAXIMUM_NUMBER_OF_RETRIES_FOR_ELEVATOR_LOGIC_ERRORS) {
                        LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager exitElevator 出电梯出错 ... ";
                        planElevatorRelatedInterrupt = false;
                        isElevatorExitResult = false;
                        isConfirmEntry = true;
                        wait_entry_cv.notify_one();
                    } else {
                        PointPlanner::instance().goToPoint(point);
                        exitPlanCmd = PlanCmd::PLAN_MOVE_BASE_ING;
                    }
                    exitPlanRetryCount++;
                    break;
            }

            if (std::chrono::steady_clock::now() - last_send_time > std::chrono::seconds(MAXIMUM_DELAY_TIME - 1)) {
                sendDelayedDoorClosing();
                last_send_time = std::chrono::steady_clock::now();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME + append_sleep_time));
        }
    });
}


void ElevatorControlManager::handlePreFlow(const std::vector<RealBlock> &preFlows) {
    if (preFlows.size() != 4) {
        throw app::exception(make_error_code(error::elevator_pre_flow_error));
    }
    preError = false;
    preCirculationBlock = preFlows[0];
    preElevatorInBlock = preFlows[1];
    preSwitchMapBlock = preFlows[2];
    preElevatorOutBlock = preFlows[3];
    preAdjustmentFrequency = 0;
    preRetryFrequency = 0;

    //todo 逻辑判断，看看执行哪个流程

    {
        std::unique_lock<std::mutex> lk(pre_mutex_);
        preState = ElevatorPreState::PRE_CIRCULATION;
    }
    pre_condition_variable_.notify_one();
}

void ElevatorControlManager::handlePostFlow(const std::vector<RealBlock> &postFlows) {
    if (postFlows.size() != 4) {
        throw app::exception(make_error_code(error::elevator_post_flow_error));
    }
    postError = false;
    postCirculationBlock = postFlows[0];
    postElevatorInBlock = postFlows[1];
    postSwitchMapBlock = postFlows[2];
    postElevatorOutBlock = postFlows[3];
    postAdjustmentFrequency = 0;
    postRetryFrequency = 0;

    //todo 逻辑判断，看看执行哪个流程
    {
        std::unique_lock<std::mutex> lk(post_mutex_);
        postState = ElevatorPostState::POST_CIRCULATION;
    }
    post_condition_variable_.notify_one();
}

void ElevatorControlManager::completePreCirculation(const bool arrive) {
    if (planElevatorRelatedInterrupt) {

        setPlanCmd(arrive, "completePreCirculation");

    } else {
        LOG_IF(INFO, DEBUG_ELEVATOR)
                        << "ElevatorControlManager pre 移动或调整电梯点位结果 " << arrive << "... ";
        if (arrive) {
            if (preAdjustmentFrequency < MAXIMUM_NUMBER_OF_RETRY_ATTEMPTS_FOR_ERRORS_TO_THE_ELEVATOR) {
                {
                    std::unique_lock<std::mutex> lk(pre_mutex_);
                    preState = ElevatorPreState::PRE_CIRCULATION;
                }
                pre_condition_variable_.notify_one();
            } else {
                {
                    std::unique_lock<std::mutex> lk(pre_mutex_);
                    preState = ElevatorPreState::PRE_ELEVATOR_IN;
                }
                pre_condition_variable_.notify_one();
            }


        } else {
            if (preRetryFrequency < MAXIMUM_NUMBER_OF_RETRY_ATTEMPTS_FOR_ERRORS_TO_THE_ELEVATOR) {
                preAdjustmentFrequency = 0;
                {
                    std::unique_lock<std::mutex> lk(pre_mutex_);
                    preState = ElevatorPreState::PRE_CIRCULATION;
                }
                pre_condition_variable_.notify_one();
            } else {
                goPreError();
            }
            preRetryFrequency++;
        }
    }
}


void ElevatorControlManager::completePostCirculation(const bool arrive) {
    if (planElevatorRelatedInterrupt) {

        setPlanCmd(arrive, "completePostCirculation");

    } else {
        LOG_IF(INFO, DEBUG_ELEVATOR)
                        << "ElevatorControlManager completePostCirculation 移动或调整电梯点位有完成 " << arrive
                        << "... ";
        if (arrive) {
            if (postAdjustmentFrequency < MAXIMUM_NUMBER_OF_RETRY_ATTEMPTS_FOR_ERRORS_TO_THE_ELEVATOR) {
                {
                    std::unique_lock<std::mutex> lk(post_mutex_);
                    postState = ElevatorPostState::POST_CIRCULATION;
                }
                post_condition_variable_.notify_one();
            } else {
                {
                    std::unique_lock<std::mutex> lk(post_mutex_);
                    postState = ElevatorPostState::POST_ELEVATOR_IN;
                }
                post_condition_variable_.notify_one();
            }
        } else {
            if (postRetryFrequency < MAXIMUM_NUMBER_OF_RETRY_ATTEMPTS_FOR_ERRORS_TO_THE_ELEVATOR) {
                postAdjustmentFrequency = 0;
                {
                    std::unique_lock<std::mutex> lk(post_mutex_);
                    postState = ElevatorPostState::POST_CIRCULATION;
                }
                post_condition_variable_.notify_one();
            } else {
                goPostError();
            }
            postRetryFrequency++;
        }
    }
}

void ElevatorControlManager::ttSendLightUpTargetFloor() {
    setBuildElevatorAddress(1);
    sendLightUpTargetFloor(1, [](const EleProtocol &response) {
        LOG_IF(INFO, DEBUG_ELEVATOR) << "ElevatorControlManager 开启楼层判断逻辑，楼层为 " << 1 << " ... ";
    });
}

void ElevatorControlManager::ttSendDelayedDoorClosing() {
    setBuildElevatorAddress(1);
    sendDelayedDoorClosing();
}

void ElevatorControlManager::ttOpenQueryFloor() {
    setBuildElevatorAddress(1);
    openQueryFloor();
}

void ElevatorControlManager::ttCloseQueryFloor() {
    setBuildElevatorAddress(1);
    closeQueryFloor();
}
