//
// Created by io on 23-12-11.
//

#ifndef APP_COMMUNICATION_ELEVATOR_CONTROL_H
#define APP_COMMUNICATION_ELEVATOR_CONTROL_H

////两消息之间的间隔
//#define the_interval_between_two_messages 300
////移动到电梯点重试次数
//#define maximum_number_of_retry_attempts_for_errors_to_the_elevator 2
////进出电梯错误重试次数
//#define maximum_number_of_retries_for_elevator_logic_errors 4
////进入电梯前的调整频率
//#define maximum_number_of_entering_the_elevator 1
////点亮楼层
//#define the_time_interval_for_continuously_lighting_up_floors 3000
////查询楼层
//#define the_time_interval_for_continuous_floor_queries 1000
////判断楼层
//#define the_time_interval_for_continuous_floor_determination 1000
////等待电梯
//#define maximum_waiting_time_for_elevator 60 * 10 * 1000
////进出电梯
//#define maximum_time_for_entering_and_exiting_the_elevator 60 * 2 * 1000
////转向
//#define entering_inner_steering_speed 0.4
//
//
//#define serial_port_send_print false
//#define serial_port_accept_print false
//#define jump_elevator_status_door_state true
//
//#define maximum_delay_time 9


#define SLEEP_TIME 10
#define MOVING_DISTANCE 1.6
#define INEXPLICABLE_MAGIC_NUMBER 0.00556789

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
#include "exploration/A_star_pathplanner.h"

#include <boost/asio.hpp>
#include <queue>
#include <utility>
#include <ostream>
#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_listener.h>
#include <costmap_2d/costmap_2d_ros.h>
#include <sensor_msgs/LaserScan.h>
#include <tf/transform_listener.h>

class Voice {
private:
    int order{};
public:
    Voice() {}

    explicit Voice(int order) : order(order) {}

    friend void to_json(json &j, const Voice &vo) {
        j = json{
                {"order", vo.order},
        };
    }

    friend void from_json(const json &j, Voice &vo) {
        j.at("order").get_to(vo.order);
    }
};

const unsigned char CMD_LIGHT_UP_TARGET_FLOOR = 0x60;//MessageIdEnum::LIGHTING_UP_THE_ELEVATOR
const unsigned char CMD_QUERY_FLOOR_WHERE_LOCATED = 0x61;
const unsigned char CMD_DELAYED_DOOR_CLOSING = 0x62;//MessageIdEnum::DELAYED_DOOR_CLOSING
const unsigned char CMD_AUTOMATIC_DOOR_OPENING = 0x66;//MessageIdEnum::AUTOMATIC_DOOR_OPENING

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

    enum PlanCmd {
        PLAN_NONE,
        PLAN_CORE_MOVE_ERROR,
        PLAN_CORE_MOVE_SUCCESS,
        PLAN_MOVE_BASE_ING,
        PLAN_MOVE_BASE_ERROR,
        PLAN_MOVE_BASE_SUCCESS,
        PLAN_AGAIN
    };

    enum ElevatorPreState {
        PRE_NONE,
        PRE_CIRCULATION,
        PRE_ELEVATOR_IN,
        PRE_SWITCH_MAP,
        PRE_ELEVATOR_OUT,
        PRE_OVER,
    };

    enum ElevatorPostState {
        POST_NONE,
        POST_CIRCULATION,
        POST_ELEVATOR_IN,
        POST_SWITCH_MAP,
        POST_ELEVATOR_OUT,
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
                    return false;
                case CMD_DELAYED_DOOR_CLOSING:
                    return false;
                case CMD_AUTOMATIC_DOOR_OPENING:
                    return true;
                default:
                    return false;
            }
        }
    };

    struct ConventionRetryMechanism {
        int preAdjustmentFrequency;
        int preRetryFrequency;
        int postAdjustmentFrequency;
        int postRetryFrequency;

        friend std::ostream &
        operator<<(std::ostream &os, const ElevatorControlManager::ConventionRetryMechanism &mechanism) {
            os << "pre 调整频率: " << mechanism.preAdjustmentFrequency
               << " pre 重试频率: " << mechanism.preRetryFrequency
               << " post 调整频率: " << mechanism.postAdjustmentFrequency
               << " post 重试频率: " << mechanism.postRetryFrequency;
            return os;
        }

        void reset() {
            preAdjustmentFrequency = 0;
            preRetryFrequency = 0;
            postAdjustmentFrequency = 0;
            postRetryFrequency = 0;
        }
    };

    struct ErrorRetryMechanism {
        int preCirculationErrorRetryCount;//梯控前期-到电梯外点位错误
        int preElevatorInErrorRetryCount;//梯控前期-进入电梯错误
        int preSwitchMapErrorRetryCount;//梯控前期-切换地图错误
        int preElevatorOutErrorRetryCount;//梯控前期-出电梯错误
        int postCirculationErrorRetryCount;//梯控后期-到电梯外点位错误
        int postElevatorInErrorRetryCount;//梯控后期-进入电梯错误
        int postSwitchMapErrorRetryCount;//梯控后期-切换地图错误
        int postElevatorOutErrorRetryCount;//梯控后期-出电梯错误

        friend std::ostream &
        operator<<(std::ostream &os, const ElevatorControlManager::ErrorRetryMechanism &mechanism) {
            os << "pre 到电梯外点位错误 RetryCount: " << mechanism.preCirculationErrorRetryCount
               << " pre 进入电梯错误 RetryCount: " << mechanism.preElevatorInErrorRetryCount
               << " pre 切换地图错误 RetryCount: " << mechanism.preSwitchMapErrorRetryCount
               << " pre 出电梯错误 RetryCount: " << mechanism.preElevatorOutErrorRetryCount
               << " post 到电梯外点位错误 RetryCount: " << mechanism.postCirculationErrorRetryCount
               << " post 进入电梯错误 RetryCount: " << mechanism.postElevatorInErrorRetryCount
               << " post 切换地图错误 RetryCount: " << mechanism.postSwitchMapErrorRetryCount
               << " post 出电梯错误 RetryCount: " << mechanism.postElevatorOutErrorRetryCount;
            return os;
        }

        void reset() {
            preCirculationErrorRetryCount = 0;
            preElevatorInErrorRetryCount = 0;
            preSwitchMapErrorRetryCount = 0;
            preElevatorOutErrorRetryCount = 0;
            postCirculationErrorRetryCount = 0;
            postElevatorInErrorRetryCount = 0;
            postSwitchMapErrorRetryCount = 0;
            postElevatorOutErrorRetryCount = 0;
        }
    };

    struct ElevatorSensor {
        double odom_x;
        double odom_y;
//        double odom_yaw;
        double imu_yaw;
    };

    struct ElevatorLastSensor {
        double old_x;
        double old_y;
        double old_yaw;
    };

public:

    enum ElevatorError {
        NoElevatorError,
        PreCirculationError,//梯控前期-到电梯外点位错误
        PreElevatorInError,//梯控前期-进入电梯错误
        PreSwitchMapError,//梯控前期-切换地图错误
        PreElevatorOutError,//梯控前期-出电梯错误
        PostCirculationError,//梯控后期-到电梯外点位错误
        PostElevatorInError,//梯控后期-进入电梯错误
        PostSwitchMapError,//梯控后期-切换地图错误
        PostElevatorOutError,//梯控后期-出电梯错误
    };

    typedef void (*ElevatorCallback)(int floor, int doorState, int lastDirection, int availability, int nextDirection);

private:
    int mElevatorAddress;

    ros::Subscriber subscriberRawMap;
    ros::Subscriber subscriberLocalMap;
    ros::Subscriber subscriberScan;
    ros::Subscriber subscriberOdom;
    ros::Subscriber subscriberImu;
    ros::Publisher publisherCmdVel;
    ros::Publisher publisherPose;

    ros::Subscriber subscriberElevatorManager;


    async::ThreadPool pool_;

//    std::thread elevator_pre_thread;
//    std::thread elevator_post_thread;

    std::mutex pre_mutex_;
    std::condition_variable pre_condition_variable_;
    std::mutex post_mutex_;
    std::condition_variable post_condition_variable_;

    boost::asio::io_service boostIo;
    boost::asio::serial_port *boostSerial = nullptr;

//    std::thread serial_sender_thread;
//    std::thread serial_receiver_thread;
//    std::thread light_up_thread;
//    std::thread query_floor_thread;
//    std::thread arrive_floor_thread;

    std::queue<Message> message_queue;
    std::mutex queue_mutex;
    std::condition_variable queue_cond;
    std::map<int, std::pair<std::chrono::steady_clock::time_point, MessageCallback>> sent_messages;
    std::mutex map_mutex;

    std::condition_variable light_cond;
    std::mutex light_mutex;
    bool inLight;
    int lightFloor;

    std::condition_variable query_cond;
    std::mutex query_mutex;
    bool inquiry;

    std::condition_variable arrive_cond;
    std::mutex arrive_mutex;
    bool unseal;
    int mTargetFloor;
    bool mElevatorArrived;
    bool mArrivedResult;
    bool mTakeIn;

    ElevatorSensor elevatorSensor;
    ElevatorLastSensor elevatorLastSensor;

    double last_angle;

    std::atomic<bool> mainInterrupt;
    std::atomic<ControlCmd> controlCmd;

    std::atomic<bool> planElevatorRelatedInterrupt;

    std::atomic<PlanCmd> enterPlanCmd;
    std::atomic<int> enterPlanRetryCount;

    std::atomic<PlanCmd> exitPlanCmd;
    std::atomic<int> exitPlanRetryCount;

    bool isConfirmEntry;
    bool isConfirmExit;

    std::atomic<bool> isElevatorEntryResult;
    std::atomic<bool> isElevatorExitResult;

    std::atomic<ElevatorPreState> preState;
    std::atomic<ElevatorPostState> postState;

    RealBlock preCirculationBlock;
    RealBlock preElevatorInBlock;
    RealBlock preSwitchMapBlock;
    RealBlock preElevatorOutBlock;
    RealBlock postCirculationBlock;
    RealBlock postElevatorInBlock;
    RealBlock postSwitchMapBlock;
    RealBlock postElevatorOutBlock;

    std::function<void(ElevatorError)> callbackElevatorPre;
    std::function<void(ElevatorError)> callbackElevatorPost;

//    std::atomic<bool> preError;
//    std::atomic<bool> postError;
    ElevatorError elevatorError;

    int recentlyDataLengthSize;
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

    ElevatorCallback mElevatorCallback;

    std::function<void(bool)> mElevatorMovementCallback;

    bool isUrgencyStop{false};
    bool isGarbage{false};

    ConventionRetryMechanism conventionRetryMechanism;
    ErrorRetryMechanism errorRetryMechanism;

    nav_msgs::OccupancyGrid normalOccupancyGrid;
    nav_msgs::OccupancyGrid localOccupancyGrid;
    sensor_msgs::LaserScan laserScan;

    bool suspendLightUp;

    std::unique_ptr<tf2_ros::Buffer> tfBuffer;
    std::unique_ptr<tf2_ros::TransformListener> tfListener;
    std::shared_ptr<tf::TransformListener> listener_;

    AStarPlanner path_planner_;

    int internalSpatialAnalysisCount;

    int take_in_voice{0};

private:

    bool hasSerialPortAccess(const std::string &portName);

    void publishCmd(double x = 0, double z = 0) const;

    void interruptAccessElevators();

    void recordSensorData();

    void subscribeRawMapCallback(const nav_msgs::OccupancyGrid &msg);

    void subscribeLocalMapCallback(const nav_msgs::OccupancyGrid &msg);

    void subscribeScanCallback(const sensor_msgs::LaserScan &msg);

    void subscribeOdomCallback(const nav_msgs::Odometry &msg);

    void subscribeImuCallback(const sensor_msgs::Imu &msg);

    void elevatorManagerSubscribeCallback(const std_msgs::Int32 &flag);

//    void movement_controls_func(ControlCommand command);

    void turn_controls_func();

    [[noreturn]] void serial_send_thread_func(boost::asio::serial_port &serial);

    [[noreturn]] void serial_receive_thread_func(boost::asio::serial_port &serial);

    [[noreturn]] void light_up_thread_func();

    [[noreturn]] void query_floor_thread_func();

    [[noreturn]] void arrive_floor_thread_func();

    [[noreturn]] void elevator_voice_thread_func();

    void notifyArrived(bool in, bool result);

    [[noreturn]] void elevator_pre_thread_func();

    [[noreturn]] void elevator_post_thread_func();

    void doPreCirculation();

    void doPreElevatorIn();

    void doPreSwitchMap();

    void doPreElevatorOut();

    void goPreError(ElevatorError error);

    void doPostCirculation();

    void doPostElevatorIn();

    void doPostSwitchMap();

    void doPostElevatorOut();

    void goPostError(ElevatorError error);

    void takeElevatorIn(int fromFloor, int toFloor, const RealPoint &fromOutPoint, const RealPoint &fromInPoint);

    void takeElevatorOut(int fromFloor, int toFloor, const RealPoint &toOutPoint, const RealPoint &toInPoint,
                         const RealPoint &relocationPoint);

    void sendLightUpTargetFloor(int floor, const MessageSuccessCallback &successCallback);

    void sendSimpleLightUpTargetFloor(int floor);

    void sendDelayedDoorClosing();

    void sendSyncMessage(MessageIdEnum messageId, const std::vector<uint8_t> &data,
                         const MessageSuccessCallback &successCallback,
                         const MessageFailCallback &failCallback);

    void sendAsyncMessage(EleProtocol eleProtocol);

    void openLightUp(int floor);

    void closeLightUp();

    void openQueryFloor();

    void closeQueryFloor();

    void openWaitingArrive(int targetFloor, bool isTakeIn);

    void closeWaitingArrive();

    void poseEstimate(const RealPoint &realPoint);

    RealPoint rotate180DegreesAroundZ(const RealPoint &point);

    void setPlanCmd(bool arrive, const std::string &tag);

    void waitDelayClosingDoor();

    bool getTransform(const std::string &target_frame, const std::string &source_frame,
                      geometry_msgs::TransformStamped &transform);

    cv::Mat occupancyGridToCvMat(const nav_msgs::OccupancyGrid &occupancyGrid);

    cv::Mat localMapTransformGlobal(const cv::Mat benchmarkMap, const cv::Mat &localMap);

    void showMap(const cv::String &winname, cv::Mat mat);

    cv::Point midpointElevator();

    double averageIntensityForElevatorInside(const cv::Mat &image);

    double averageIntensityForElevatorWay(const cv::Mat &image, cv::Point &midpoint);

    double calculateDistance(const geometry_msgs::Pose &pose1, const geometry_msgs::Pose &pose2);

    cv::Point
    calculateExtendedLineEndPoint(const cv::Point &center, const cv::Point &target, const cv::Size &imageSize);

    bool elevatorInternalInspection();

    bool elevatorInternalInspection2();

    void openTakeInVoice();

    void closeTakeInVoice();

public:
    void initialize(ros::NodeHandle handle);

    void setElevatorCallback(ElevatorCallback callback);

    void setElevatorMovementCallback(const std::function<void(bool)> &callback);

    void setCallbackElevatorPre(const std::function<void(ElevatorError)> &callbackElevatorPre);

    void setCallbackElevatorPost(const std::function<void(ElevatorError)> &callbackElevatorPost);

    void setBuildElevatorAddress(int elevatorAddress);

//    void enterElevator();

//    void exitElevator();

    void enterElevator(const RealPoint &point);

    void exitElevator(const RealPoint &point);

    void printElevator();

    void handlePreFlow(const std::vector<RealBlock> &preFlows);

    void handlePostFlow(const std::vector<RealBlock> &postFlows);

    void completePreCirculation(bool arrive);

    void completePostCirculation(bool arrive);

    void setUrgencyStop(bool isUrgencyStop);

    void setGarbage(bool isGarbage);

    static void switchMapsInWorkMode(const std::string &fromMapId, const std::string &toMapId);

    void ttSendLightUpTargetFloor();

    void ttSendDelayedDoorClosing();

    void ttOpenQueryFloor();

    void ttCloseQueryFloor();
};


#endif //APP_COMMUNICATION_ELEVATOR_CONTROL_H
