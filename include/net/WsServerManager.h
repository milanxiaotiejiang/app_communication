//
// Created by mi on 2022/4/12.
//

#ifndef APP_COMMUNICATION_WSSERVERMANAGER_H
#define APP_COMMUNICATION_WSSERVERMANAGER_H

#include "unordered_map"
#include "vector"
#include <iostream>

#include "nlohmann/json.hpp"

#include "net/base/RequestData.h"
#include "net/base/RequestModel.h"

#include "future/BlockingCollection.h"
#include "sys/syscall.h"
#include "future/CThread.h"

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/logger/syslog.hpp>
#include <websocketpp/server.hpp>

#include "model/NetModel.h"
#include "net/ros/RosBasic.h"
#include "nav_msgs/Odometry.h"
#include <nav_msgs/OccupancyGrid.h>
#include <utility>

#include "glog/logging.h"
#include "net/MessageBusManager.h"

using namespace std;
using namespace code_machina;

using json = nlohmann::json;

const string APP_JSON = "/app_json";
const string APP_COMMUNICATION = "/app_communication";

const string MAP_APP = "/map_app";
const string ODOM_APP = "/odom_app";
const string ROBOT_STATUS = "/robot_status";
const string MATERIAL_STATUS = "/material_status";
const string NOTICE_APP = "/notice_app";
const string TASK_POINT = "/task_point";
const string CHECK_APP = "/check_app";
const string KNOB_APP = "/knob_app";
const string INTERNAL_EVENT = "/robot_internal_event";
const string ALARM_EVENT = "/alarm_event";
const string RESPONSE = "/response";
const string RESPONSE_JSON = "/response_json";

const string MESSAGE_BUS_TOPIC = "message_bus_topic";

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
};

class WsServerManager {
public:
    static auto &instance() {
        static WsServerManager obj;
        return obj;
    }

    static void startWebSocket();

    static void stopWebSocket();

    static void sendData(const std::string &data);

    static void setMapApp(const nav_msgs::OccupancyGrid &occupancyGrid);

    static void setOdomApp(const nav_msgs::OdometryConstPtr &odomPtr);

    static void sendRequestData(const string &key, const std::string &data);

    static void sendMessageBusTopic(const string &string);
};


#endif//APP_COMMUNICATION_WSSERVERMANAGER_H
