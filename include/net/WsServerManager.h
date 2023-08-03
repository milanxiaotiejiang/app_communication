//
// Created by mi on 2022/4/12.
//

#ifndef APP_COMMUNICATION_WSSERVERMANAGER_H
#define APP_COMMUNICATION_WSSERVERMANAGER_H

#include "string"

#include "nav_msgs/Odometry.h"
#include <nav_msgs/OccupancyGrid.h>

const std::string APP_JSON = "/app_json";
const std::string APP_COMMUNICATION = "/app_communication";

const std::string MAP_APP = "/map_app";
const std::string GZIP_MAP_APP = "/gzip_map_app";
const std::string ODOM_APP = "/odom_app";
const std::string ROBOT_STATUS = "/robot_status";
const std::string NOTICE_APP = "/notice_app";
const std::string TASK_POINT = "/task_point";
const std::string CHECK_APP = "/check_app";
const std::string KNOB_APP = "/knob_app";
const std::string INTERNAL_EVENT = "/robot_internal_event";
const std::string ALARM_EVENT = "/alarm_event";
const std::string RESPONSE = "/response";
const std::string RESPONSE_JSON = "/response_json";

const std::string SENSOR_CHECK = "/sensor_check";
const std::string APP_MRROBOT_UL_SENSOR1 = "/app/mrrobot/ul_sensor1";
const std::string APP_MRROBOT_UL_SENSOR2 = "/app/mrrobot/ul_sensor2";
const std::string APP_MRROBOT_UL_SENSOR3 = "/app/mrrobot/ul_sensor3";
const std::string APP_MRROBOT_UL_SENSOR4 = "/app/mrrobot/ul_sensor4";
const std::string APP_MRROBOT_LS_FRONT_LEFT = "/app/mrrobot/ls_front_left";
const std::string APP_MRROBOT_LS_FRONT_RIGHT = "/app/mrrobot/ls_front_right";
const std::string APP_1_DEPTH_DEPTH2PC = "/app/1/depth/depth2pc";
const std::string APP_2_DEPTH_DEPTH2PC = "/app/2/depth/depth2pc";
const std::string APP_SCAN_RAW = "/app/scan_raw";
const std::string APP_WHEEL_ODOM = "/app/wheel_odom";
const std::string APP_MRROBOT_ON_LADDER = "/app/mrrobot/on_ladder";
const std::string APP_HANDSFREE_IMU = "/app/handsfree/imu";
const std::string APP_MRROBOT_BUMP_SENSOR = "/app/mrrobot/bump_sensor";
const std::string APP_MRROBOT_CARPET_DETECT = "/app/mrrobot/carpet_detect";


const std::string MESSAGE_BUS_TOPIC = "message_bus_topic";

const int NET_MODEL_MAP = 0;
const int NET_MODEL_ODOM = 1;

namespace APP_JSON_VERSION {
    enum {
        V1,
        V2
    };
}
namespace APP_COMMUNICATION_VERSION {
    enum {
        V1,
        V2
    };
}

class WsServerManager {
private:
    WsServerManager() = default;

    WsServerManager(WsServerManager &) = delete;

    WsServerManager &operator=(const WsServerManager &) = delete;

public:
    ~WsServerManager() = default;

public:
    static auto &instance() {
        static WsServerManager obj;
        return obj;
    }

    static void startWebSocket();

    static void stopWebSocket();

    static void sendData(const std::string &data);

    static void setMapApp(const nav_msgs::OccupancyGrid &occupancyGrid);

    static void setMapApp2(const nav_msgs::OccupancyGrid &occupancyGrid);

    static void setOdomApp(const nav_msgs::OdometryConstPtr &odomPtr);

    static void sendRequestData(const std::string &key, const std::string &data);

    static void sendMessageBusTopic(const std::string &string);
};


#endif//APP_COMMUNICATION_WSSERVERMANAGER_H
