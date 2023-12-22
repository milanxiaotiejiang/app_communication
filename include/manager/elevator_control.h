//
// Created by io on 23-12-11.
//

#ifndef APP_COMMUNICATION_ELEVATOR_CONTROL_H
#define APP_COMMUNICATION_ELEVATOR_CONTROL_H

#define MAX_ADJUSTMENT_FREQUENCY 2

#include <ros/ros.h>

#include <nav_msgs/Odometry.h>
#include <sensor_msgs/Imu.h>
#include <geometry_msgs/Twist.h>
#include <tf/transform_datatypes.h>

#include <mutex>
#include <condition_variable>
#include <thread>
#include <std_msgs/Int32.h>
#include "future/thread_pool.h"
#include "task/RealBlock.h"
#include "alignment/ele_protocol.h"

#include <boost/asio.hpp>
#include <queue>
#include <utility>

const unsigned char CMD_LIGHT_UP_TARGET_FLOOR = 0x60;//MessageIdEnum::LIGHTING_UP_THE_ELEVATOR
const unsigned char CMD_QUERY_FLOOR_WHERE_LOCATED = 0x61;
const unsigned char CMD_DELAYED_DOOR_CLOSING = 0x62;//MessageIdEnum::DELAYED_DOOR_CLOSING
const unsigned char CMD_AUTOMATIC_DOOR_OPENING = 0x66;//MessageIdEnum::AUTOMATIC_DOOR_OPENING

#define SERIAL_PORT_PRINT false

#define MAXIMUM_DELAY_TIME 9

class ElevatorControlManager {
private:
    ElevatorControlManager() = default;

    ElevatorControlManager(ElevatorControlManager &) = delete;

    ElevatorControlManager &operator=(const ElevatorControlManager &) = delete;

public:
    ~ElevatorControlManager() {
        delete boostSerial;  // 清理分配的资源
    };

    static auto &instance() {
        static ElevatorControlManager obj;
        return obj;
    }

private:
    enum ControlCommand {
        ENTER_ELEVATOR,
        EXIT_ELEVATOR
    };

    enum ControlCmd {
        NONE,
        MOVE,
        ROTATE,
        REACH
    };

    enum ElevatorPreState {
        PRE_NONE,
        PRE_CIRCULATION,
        PRE_ELEVATOR,
        PRE_SWITCH_MAP,
        PRE_OVER,
    };

    enum ElevatorPostState {
        POST_NONE,
        POST_CIRCULATION,
        POST_ELEVATOR,
        POST_SWITCH_MAP,
        POST_OVER,
    };

    enum MessageIdEnum {
        LIGHTING_UP_THE_ELEVATOR,
        AUTOMATIC_DOOR_OPENING,
        DELAYED_DOOR_CLOSING,
        MESSAGE_MAX
    };

    static bool inMessageEnum(int value) {
        return value >= LIGHTING_UP_THE_ELEVATOR && value < MESSAGE_MAX;
    }

    enum LadderControlError {
        NORMAL,
        TIMEOUT,// 超时
        FREQUENTLY,// 发送频繁
        ERROR,
        WRITE_ERROR
    };

    using MessageCallback = std::function<void(const EleProtocol &response, LadderControlError error)>;
    using MessageSuccessCallback = std::function<void(const EleProtocol &response)>;
    using MessageFailCallback = std::function<void(LadderControlError error)>;

    struct Message {
        int id; // 唯一标识符
        bool waiting_response; // 是否等待响应
        std::vector<uint8_t> data; // 消息内容
        MessageCallback callback; // 回调函数

        // 使用枚举值作为ID的构造函数
        Message(MessageIdEnum id, const std::vector<uint8_t> &data, MessageCallback callback = nullptr)
                : id(id), waiting_response(true), data(data), callback(std::move(callback)) {
            // 如果没有提供回调函数，可以在这里初始化默认回调逻辑
        }

        // 自动生成ID的构造函数
        explicit Message(const std::vector<uint8_t> &data)
                : id(MessageFactory::generateUniqueID()), waiting_response(false), data(data), callback(nullptr) {
            // 自动生成ID，不需要等待响应
        }

    };


    class MessageFactory {
    public:
        // 创建需要等待响应的消息
        static Message
        createSyncMessage(MessageIdEnum id, const std::vector<uint8_t> &data, MessageCallback callback = nullptr) {
            return Message(id, data, std::move(callback));
        }

        // 创建不需要等待响应的消息
        static Message createAsyncMessage(const std::vector<uint8_t> &data) {
            return Message(data);
        }

        // 生成唯一ID的方法
        static int generateUniqueID() {
            static std::atomic<int> uniqueID(MESSAGE_MAX); // 从 MESSAGE_MAX 开始自增
            return uniqueID++;
        }

        static unsigned char messageIdToChar(MessageIdEnum messageId) {
            switch (messageId) {
                case LIGHTING_UP_THE_ELEVATOR:
                    return CMD_LIGHT_UP_TARGET_FLOOR;
                case AUTOMATIC_DOOR_OPENING:
                    return CMD_AUTOMATIC_DOOR_OPENING;
                case DELAYED_DOOR_CLOSING:
                    return CMD_DELAYED_DOOR_CLOSING;
                default:
                    return MESSAGE_MAX + 1; // 或其他默认值
            }
        }

        static MessageIdEnum charToMessageId(unsigned char ch) {
            switch (ch) {
                case CMD_LIGHT_UP_TARGET_FLOOR:
                    return MessageIdEnum::LIGHTING_UP_THE_ELEVATOR;
                case CMD_DELAYED_DOOR_CLOSING:
                    return MessageIdEnum::DELAYED_DOOR_CLOSING;
                case CMD_AUTOMATIC_DOOR_OPENING:
                    return MessageIdEnum::AUTOMATIC_DOOR_OPENING;
                default:
                    throw std::runtime_error("Invalid message id");
            }
        }

        static bool charInMessageIdEnum(unsigned char ch) {
            switch (ch) {
                case CMD_LIGHT_UP_TARGET_FLOOR:
                    return true;
                case CMD_DELAYED_DOOR_CLOSING:
                    return true;
                case CMD_AUTOMATIC_DOOR_OPENING:
                    return true;
                default:
                    return false;
            }
        }
    };


    ros::Subscriber subscriberOdom;
    ros::Subscriber subscriberImu;
    ros::Publisher publisherCmdVel;

    ros::Subscriber subscriberElevatorManager;


    async::ThreadPool pool_;

    std::thread elevator_pre_thread;
    std::thread elevator_post_thread;

    std::mutex pre_mutex_;
    std::condition_variable pre_condition_variable_;
    std::mutex post_mutex_;
    std::condition_variable post_condition_variable_;

    boost::asio::io_service boostIo;
    boost::asio::serial_port *boostSerial = nullptr;

    std::thread serial_sender_thread;
    std::thread serial_receiver_thread;
    std::thread query_floor_thread;
    std::thread arrive_floor_thread;

    std::queue<Message> message_queue;
    std::mutex queue_mutex;
    std::condition_variable queue_cond;
    std::map<int, std::pair<std::chrono::steady_clock::time_point, MessageCallback>> sent_messages;
    std::mutex map_mutex;

    std::condition_variable query_cond;
    std::mutex query_mutex;
    bool inquiry;

    std::condition_variable arrive_cond;
    std::mutex arrive_mutex;
    bool unseal;
    int mTargetFloor;
    bool mElevatorArrived;
    int imitateArrivedCount;

    double odom_x;
//    double odom_yaw;
    double imu_yaw;

    std::atomic<double> old_x;
    std::atomic<double> old_yaw;

    double last_angle;

    std::atomic<bool> mainInterrupt;
    std::atomic<ControlCmd> controlCmd;

    bool isConfirmEntry;
    bool isConfirmExit;

    std::atomic<ElevatorPreState> preState;
    std::atomic<ElevatorPostState> postState;

    RealBlock preCirculationBlock;
    RealBlock preElevatorBlock;
    RealBlock preSwitchMapBlock;
    RealBlock postCirculationBlock;
    RealBlock postElevatorBlock;
    RealBlock postSwitchMapBlock;

    std::function<void(bool)> callbackElevatorPre;
    std::function<void(bool)> callbackElevatorPost;

    std::atomic<bool> preError;
    std::atomic<bool> postError;

    std::atomic<int> preAdjustmentFrequency;
    std::atomic<int> postAdjustmentFrequency;

    uint8_t recentlyFloor;
    uint8_t recentlyElevatorStatus;

    std::mutex wait_from_mutex;
    std::condition_variable wait_from_cv;
    std::mutex wait_entry_mutex;
    std::condition_variable wait_entry_cv;
    std::mutex wait_to_mutex;
    std::condition_variable wait_to_cv;
    std::mutex wait_exit_mutex;
    std::condition_variable wait_exit_cv;

private:
    bool hasSerialPortAccess(const std::string &portName);

    void publishCmd(double x = 0, double z = 0) const;

    void interruptAccessElevators();

    void recordSensorData();

    void subscribeOdomCallback(const nav_msgs::Odometry &msg);

    void subscribeImuCallback(const sensor_msgs::Imu &msg);

    void elevatorManagerSubscribeCallback(const std_msgs::Int32 &flag);

    void movement_controls_func(ControlCommand command);

    [[noreturn]] void serial_send_thread_func(boost::asio::serial_port &serial);

    [[noreturn]] void serial_receive_thread_func(boost::asio::serial_port &serial);

    [[noreturn]] void query_floor_thread_func();

    [[noreturn]] void arrive_floor_thread_func();

    [[noreturn]] void elevator_pre_thread_func();

    [[noreturn]] void elevator_post_thread_func();

    void doPreCirculation();

    void doPreElevator();

    void doPreSwitchMap();

    void goPreError();

    void doPostCirculation();

    void doPostElevator();

    void doPostSwitchMap();

    void goPostError();

    void takeElevator(int fromFloor, int toFloor);

    static void switchMapsInWorkMode(const std::string &fromMapId, const std::string &toMapId);

    void sendLightUpTargetFloor(int floor, const MessageSuccessCallback &successCallback);

    void sendDelayedDoorClosing();

    void sendSyncMessage(MessageIdEnum messageId, const std::vector<uint8_t> &data,
                         const MessageSuccessCallback &successCallback,
                         const MessageFailCallback &failCallback);

    void sendAsyncMessage(EleProtocol eleProtocol);

    void openQueryFloor();

    void closeQueryFloor();

    void openWaitingArrive(int targetFloor);

    void closeWaitingArrive();

public:
    void initialize(ros::NodeHandle handle);

    void setCallbackElevatorPre(const std::function<void(bool)> &callbackElevatorPre);

    void setCallbackElevatorPost(const std::function<void(bool)> &callbackElevatorPost);

    void enterElevator();

    void exitElevator();

    void printElevator();

    void handlePreFlow(const std::vector<RealBlock> &preFlows);

    void handlePostFlow(const std::vector<RealBlock> &postFlows);

    void completePreCirculation(bool arrive);

    void completePostCirculation(bool arrive);
};


#endif //APP_COMMUNICATION_ELEVATOR_CONTROL_H
