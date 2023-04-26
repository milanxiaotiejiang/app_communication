//
// Created by mi on 2022/4/12.
//

#include "net/WsServerManager.h"

#include "vector"
#include <iostream>
#include "sys/syscall.h"

#include "nlohmann/json.hpp"

#include "model/NetModel.h"
#include "net/base/RequestData.h"
#include "net/base/RequestModel.h"
#include "net/ros/RosBasic.h"
#include "net/ros/Twist.h"
#include "net/poly/Queue.hpp"
#include "net/kill_port.h"

#include "future/BlockingCollection.h"
#include "future/CThread.h"

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/logger/syslog.hpp>
#include <websocketpp/server.hpp>

#include <std_msgs/String.h>

#include "glog/logging.h"

#include "net/MessageBusManager.h"

#include "simulation.h"
#include "manager/PublishOutManager.h"
#include "manager/PublishInnerManager.h"

#include <opencv2/opencv.hpp>

#include "tool/map_compress.h"

//#include "tool/ZLibString.hpp"

using namespace code_machina;

using json = nlohmann::json;

////////////////////////////////////////////////////////////////////////////////

struct Ask {
    websocketpp::connection_hdl hdl;
    map<string, bool> subMap;
    string osVersion = "";
    string osSystem = "";
    string osModel = "";
    string osSource = "";

    friend ostream &operator<<(ostream &os, const Ask &ask) {
        std_msgs::String mapStr;
        for (const auto &item: ask.subMap) {
            mapStr.data.append(item.first);
            mapStr.data.append(" ");
            mapStr.data.append(to_string(item.second));
            mapStr.data.append(" | ");
        }
        os << "hdl: " << ask.hdl.lock().get() << " subMap: " << mapStr;
        return os;
    }
};


vector<websocketpp::connection_hdl> list;
map<void *, Ask> mMap;
std::mutex askMutex;

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::lib::bind;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

// pull out the type of messages sent by our config
typedef server::message_ptr message_ptr;

code_machina::BlockingCollection<string> serverCollection;
code_machina::BlockingCollection<NetModel> serverDataCollection;

PolyM::Queue transformQueue;

void wsServerSend(server *server, websocketpp::connection_hdl hdl, std::string const &payload, std::string tag) {
    try {
        server->send(hdl, payload, websocketpp::frame::opcode::text);
//        server->get_alog().write(websocketpp::log::alevel::app, data);
    } catch (const std::exception &e) {
        LOG(ERROR) << "WsServerManager : " << tag << " " << e.what();
    } catch (...) {
        LOG(ERROR) << "WsServerManager " << tag << " other start exception";
    }
}

bool validate(server *s, const websocketpp::connection_hdl &hdl) {
    //sleep(6);
    return true;
}

void on_http(server *s, websocketpp::connection_hdl hdl) {
    server::connection_ptr con = s->get_con_from_hdl(hdl);

    std::string res = con->get_request_body();
    auto path = con->get_resource();//"/nebula/sys/randomImage/5585"
    string host = con->get_host();//"192.168.2.87"
    uint16_t port = con->get_port();//9090
    string reason = con->get_local_close_reason();
    string proxy = con->get_proxy();
    auto request = con->get_request();//post uri
    string requestBody = con->get_request_body();//"{\"captcha\":\"\",\"checkKey\":\"5585\",\"password\":\"demo@123\",\"username\":\"demo\"}"
    auto resource = con->get_resource();//"/nebula/sys/login"
    string responseMsg = con->get_response_msg();//"[::ffff:192.168.2.83]:42898"
    string remoteEndPoint = con->get_remote_endpoint();
    string uri = request.get_uri();//"/nebula/sys/login"
    string method = request.get_method();//"POST"
    string body = request.get_body();//"{\"captcha\":\"\",\"checkKey\":\"5585\",\"password\":\"demo@123\",\"username\":\"demo\"}"
    string version = request.get_version();
    auto headers = request.get_headers();

    std::stringstream ss;
    ss << "got HTTP request with " << res.size() << " bytes of body data.";

    con->set_body(ss.str());
    con->set_status(websocketpp::http::status_code::ok);
}

void on_fail(server *s, websocketpp::connection_hdl hdl) {
    server::connection_ptr con = s->get_con_from_hdl(hdl);
    {
        std::unique_lock<std::mutex> lock(askMutex);
        mMap.clear();
    }

    LOG(WARNING) << "Fail handler: " << con->get_ec() << " " << con->get_ec().message();
}

void on_close(websocketpp::connection_hdl hdl) {
    LOG(WARNING) << "Close handler";
    {
        std::unique_lock<std::mutex> lock(askMutex);
        mMap.erase(hdl.lock().get());
    }
}

void on_open(server *s, websocketpp::connection_hdl hdl) {
    LOG(WARNING) << "Open handler" << std::endl;

    auto con = s->get_con_from_hdl(hdl);
    auto path = con->get_resource();
    string host = con->get_host();
    uint16_t port = con->get_port();
    string origin = con->get_origin();
    string reason = con->get_local_close_reason();
    string proxy = con->get_proxy();
    auto request = con->get_request();
    string requestBody = con->get_request_body();
    auto resource = con->get_resource();
    string responseMsg = con->get_response_msg();
    string remoteEndPoint = con->get_remote_endpoint();
    string uri = request.get_uri();
    string method = request.get_method();
    string body = request.get_body();
    string version = request.get_version();
    auto headers = request.get_headers();
    string osVersion = headers["os-version"];
    string osSystem = headers["os-system"];
    string osModel = headers["os-model"];
    string osSource = headers["os-source"];
    LOG(INFO) << "Connected to remote : " << remoteEndPoint
              << " , osVersion : " + osVersion + " , osSystem : " + osSystem +
                 " , osModel : " + osModel + " , osSource : " + osSource;

    Ask ask = Ask();
    ask.hdl = hdl;
    ask.osVersion = osVersion;
    ask.osSystem = osSystem;
    ask.osModel = osModel;
    ask.osSource = osSource;
    ask.subMap[MAP_APP] = false;
    ask.subMap[ODOM_APP] = false;
    ask.subMap[ROBOT_STATUS] = false;
    ask.subMap[NOTICE_APP] = false;
    ask.subMap[TASK_POINT] = false;
    ask.subMap[CHECK_APP] = false;
    ask.subMap[KNOB_APP] = false;
    ask.subMap[ALARM_EVENT] = false;
    ask.subMap[RESPONSE] = false;
    ask.subMap[RESPONSE_JSON] = false;
    {
        std::unique_lock<std::mutex> lock(askMutex);
        mMap[hdl.lock().get()] = ask;
    }
}

// Define a callback to handle incoming messages
void on_message(server *s, const websocketpp::connection_hdl &hdl, message_ptr msg) {
    //    std::cout << "on_message called with hdl: " << hdl.lock().get()
    //              << " and message: " << msg->get_payload()
    //              << " and " << msg->get_opcode()
    //              << std::endl;

    auto con = s->get_con_from_hdl(hdl);
    string remoteEndPoint = con->get_remote_endpoint();
    string payload = msg->get_payload();

    string header = msg->get_header();
    string data = msg->get_extension_data();
    string raw = msg->get_raw_payload();

//    LOG(INFO) << "on_message remote : " << remoteEndPoint << " , payload : " << payload;
    {
        std::unique_lock<std::mutex> lock(askMutex);
        if (mMap.find(hdl.lock().get()) != mMap.end()) {
            Ask *ask = &mMap[hdl.lock().get()];
            string osSystem = ask->osSystem;
            string osVersion = ask->osVersion;

            AcceptRequestModel entrance;
            try {
                json jDecode = json::parse(payload);
                entrance = jDecode.get<AcceptRequestModel>();
                string op = entrance.getOp();
                string topic = entrance.getTopic();

                if (op == "subscribe") {
                    ask->subMap[topic] = true;
                } else if (op == "publish") {
                    if (topic == APP_JSON) {
                        transformQueue.put(PolyM::DataMsg<std::string>(1, payload));
                    } else if (topic == APP_COMMUNICATION) {
                        auto data = jDecode.get<RequestModel<RequestData>>();
                        std_msgs::String result;
                        result.data.append(data.getMsg().data);
                        PublishOutManager::instance().publishAppCommunication(result);
                    } else if (topic == "/cmd_val") {
                        auto data = jDecode.get<RequestModel<MyTwist>>();
                        auto myTwist = data.getMsg();
                        geometry_msgs::Twist twist;
                        twist.linear.x = myTwist.linear.x;
                        twist.linear.y = myTwist.linear.y;
                        twist.linear.z = myTwist.linear.z;
                        twist.angular.x = myTwist.angular.x;
                        twist.angular.y = myTwist.angular.y;
                        twist.angular.z = myTwist.angular.z;
                        PublishInnerManager::instance().publishVelocity(twist);
                    }
                }
            } catch (...) {
                LOG(ERROR) << "json parse exception";
            }
        } else {
//            LOG(WARNING) << "on_message remote : " << remoteEndPoint << " no find .. ";
        }
    }

    //    for (const auto &item: mMap) {
    //        std::cout << "on_message mMap: " << item.second << std::endl;
    //    }
}

class WsServerMapThread : public CThread {
private:
    websocketpp::server<websocketpp::config::asio> *server;

public:
    explicit WsServerMapThread(websocketpp::server<websocketpp::config::asio> *server) : server(server) {}

    void *run() override {
        LOG(INFO) << "WsServerMapThread : " << syscall(SYS_gettid);
        NetModel netModel;
        while (!serverDataCollection.is_completed()) {
            auto status = serverDataCollection.take(netModel);
            if (status == BlockingCollectionStatus::Ok) {
                {
                    std::unique_lock<std::mutex> lock(askMutex);
                    for (const auto &item: mMap) {
                        wsServerSend(server, item.second.hdl, netModel.value, "WsServerMapThread");
                    }
                }
            }
        }
    }
};

class WsServerSubThread : public CThread {
private:
    websocketpp::server<websocketpp::config::asio> *server;
    map<string, string> dataMap;
    string mapData;

public:
    explicit WsServerSubThread(websocketpp::server<websocketpp::config::asio> *server) : server(server) {
        dataMap[ODOM_APP] = "";
        dataMap[ROBOT_STATUS] = "";
        dataMap[NOTICE_APP] = "";
        dataMap[TASK_POINT] = "";
        dataMap[CHECK_APP] = "";
        dataMap[KNOB_APP] = "";
        dataMap[ALARM_EVENT] = "";
        dataMap[RESPONSE] = "";
        dataMap[RESPONSE_JSON] = "";
    }

    void setMapApp(const string &data) {
        mapData = data;
    }

    void setOdomApp(const std_msgs::String &data) {
        dataMap[ODOM_APP] = data.data;
    }

//    void sendRequestData(const string &key, const std_msgs::String &data) {
//        RequestData requestData;
//        requestData.data = data.data;
//
//        RequestModel<RequestData> requestModel;
//        requestModel.setOp("publish");
//        requestModel.setTopic(key);
//        requestModel.setMsg(requestData);
//
//        json jsonResult = requestModel;
//        dataMap[key] = jsonResult.dump();
//    }

    void sendRequestData(const string &key, const std::string &value) {
        dataMap[key] = value;
    }

    void *run() override {
        LOG(INFO) << "WsServerSubThread : " << syscall(SYS_gettid);

        while (ros::ok()) {
            sleep(2);
            {
                std::unique_lock<std::mutex> lock(askMutex);
                for (const auto &ask: mMap) {
                    auto hdl = ask.second.hdl;
                    auto subMap = ask.second.subMap;
                    for (const auto &item: subMap) {
                        string key = item.first;
                        bool send = item.second;
                        if (send) {
                            if (key == MAP_APP) {
                                if (!mapData.empty()) {
                                    wsServerSend(server, ask.second.hdl, mapData, key);
                                    dataMap[key] = "";
                                }
                            }
                            if (key == NOTICE_APP) {
                                auto realData = dataMap[key];
                                if (!realData.empty()) {
                                    wsServerSend(server, ask.second.hdl, realData, key);
                                    dataMap[key] = "";
                                }
                            } else if (key == ALARM_EVENT) {
                                auto realData = dataMap[key];
                                if (!realData.empty()) {
                                    wsServerSend(server, ask.second.hdl, realData, key);
                                    dataMap[key] = "";
                                }
                            } else {
                                auto realData = dataMap[key];
                                if (!realData.empty()) {
                                    wsServerSend(server, ask.second.hdl, realData, key);
//                                dataMap[key] = "";
                                }
                            }
                        }
                    }
                    //                    server->get_alog().write(websocketpp::log::alevel::app, data);
                }
            }
        }
    }
};

class WsServerDataThread : public CThread {
private:
    websocketpp::server<websocketpp::config::asio> *server;

public:
    explicit WsServerDataThread(websocketpp::server<websocketpp::config::asio> *server) : server(server) {}

    void *run() override {
        LOG(INFO) << "WsServerDataThread : " << syscall(SYS_gettid);
        string data;
        while (!serverCollection.is_completed()) {
            auto status = serverCollection.take(data);
            if (status == BlockingCollectionStatus::Ok) {
                {
                    std::unique_lock<std::mutex> lock(askMutex);
                    for (const auto &item: mMap) {
                        wsServerSend(server, item.second.hdl, data, "WsServerDataThread");
                    }
                }
            }
        }
    }
};

/**
 * WebSocket++ 包括两种主要的对象类型：断点和连接
 * 端点创建并启动新的连接，并维护这些连接的默认设置。端点还管理任何共享的网络资源。
 * 一旦启动了连接，端点和连接之间就不存在链接，端点将所有默认设置复制到新连接中。更改端点上的默认设置只会影响未来的连接
 * 连接存储特定于每个WebSocket会话的消息
 * 连接不维护指向其关联端点的链接
 * 端点不维护未完成连接的列表。如果你的应用程序需要遍历所有的连接，则需要维护它们自身的列表
 *
 * WebSocket++ 端点是通过结合端点角色和端点配置来构建的
 * 有两种不同类型的端点角色：
 * WebSocket会话中的客户端角色
 * WebSocket会话中的服务端角色
 *
 * 术语：端点配置
 * webSocket++端点有一组配置，可以在编译时通过 config template 参数进行配置
 * 配置是 struct 类型的，它包含了用于生成具有特定属性的端点的类型和静态变量
 * 根据使用的配置，端点将具有不同的可用方法，并且可能具有其他第三方依赖项
 *
 * endpoint角色接受一个名为 config 的模板参数，该参数用于在编译时配置 endpoint 的行为
 *
 * */
class WsServerThread : public CThread {
private:
    WsServerDataThread *wsServerDataThread;
    WsServerSubThread *wsServerSubThread;
    WsServerMapThread *wsServerMapThread;

    server echo_server;

public:
    WsServerThread() {}

    void *run() override {
        LOG(INFO) << "WsServerThread : " << syscall(SYS_gettid);

        try {
            //设置日志级别
            //            echo_server.set_access_channels(websocketpp::log::alevel::all);
            echo_server.set_access_channels(websocketpp::log::alevel::fail);
            echo_server.set_error_channels(websocketpp::log::elevel::all);
            //屏蔽某个级别的日志
            echo_server.clear_access_channels(websocketpp::log::alevel::all);
            //禁用所有（替代方法）
            //            echo_server.set_access_channels(websocketpp::log::alevel::none);

            // Initialize ASIO
            echo_server.init_asio();
            echo_server.set_reuse_addr(true);

            //设置收到消息时的回调函数
            echo_server.set_message_handler(bind(&on_message, &echo_server, ::_1, ::_2));

            echo_server.set_http_handler(bind(&on_http, &echo_server, ::_1));
            //设置连接失败时的回调函数
            echo_server.set_fail_handler(bind(&on_fail, &echo_server, ::_1));
            //设置关闭连接时的回调函数
            echo_server.set_close_handler(&on_close);
            //设置打开连接时的回调函数
            echo_server.set_open_handler(bind(&on_open, &echo_server, ::_1));

            echo_server.set_validate_handler(bind(&validate, &echo_server, ::_1));

            wsServerDataThread = new WsServerDataThread(&echo_server);
            wsServerDataThread->start();
            wsServerDataThread->detach();
            wsServerSubThread = new WsServerSubThread(&echo_server);
            wsServerSubThread->start();
            wsServerSubThread->detach();
            wsServerMapThread = new WsServerMapThread(&echo_server);
            wsServerMapThread->start();
            wsServerMapThread->detach();

            //监听端口号
            echo_server.listen(9090);
            //排队连接接受操作
            echo_server.start_accept();
            //启动 Asio 运行循环
            echo_server.run();
        } catch (websocketpp::exception const &e) {
            LOG(ERROR) << e.what();
        } catch (const std::exception &e) {
            LOG(ERROR) << e.what();
        } catch (...) {
            LOG(ERROR) << "other start exception";
        }
    }

    WsServerDataThread *getWsServerDataThread() const {
        return wsServerDataThread;
    }

    WsServerSubThread *getWsServerSubThread() const {
        return wsServerSubThread;
    }

    void stopThread() {
        wsServerDataThread->stop();
        wsServerSubThread->stop();
        wsServerMapThread->stop();
        {
            std::unique_lock<std::mutex> lock(askMutex);
            for (const auto &item: mMap) {
                websocketpp::lib::error_code ec;
                echo_server.close(item.second.hdl, websocketpp::close::status::going_away, "", ec);
                if (ec) {
                    LOG(INFO) << " Error closing connection " << ec.message();
                }
            }
        }
        LOG(ERROR) << "echo_server stop 9090";
        echo_server.stop();
    }
};

WsServerThread *wsServerThread;

void messageBusTopic(const string &message) {
    LOG(INFO) << "messageBusTopic : " << message;
}

void WsServerManager::startWebSocket() {

    std::string pid = get_pid_using_port(9090);
    if (!pid.empty()) {
        LOG(INFO) << "进程 pid 为 " << pid << " 占用 9090 端口 ！！";
        kill_process(pid);
    }

    //    MessageBusManager::get_instance()->getMessageBus()->attach(
    //            [](const string message) {
    //                LOG(INFO) << "messageBusTopic : " << message;
    //            }, MESSAGE_BUS_TOPIC);

    wsServerThread = new WsServerThread();
    wsServerThread->start();
    wsServerThread->detach();

    auto funTransformBuffer = [](PolyM::Queue &q) {
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            auto m = q.get();
            auto &dm = dynamic_cast<PolyM::DataMsg<std::string> &>(*m);
            auto payload = dm.getPayload();
            std_msgs::String result;
            result.data.append(payload);
            PublishOutManager::instance().publishAppJson(APP_JSON_VERSION::V1, result);
        }
    };
    std::thread tTransformBuffer(funTransformBuffer, std::ref(transformQueue));
    tTransformBuffer.detach();
}

void WsServerManager::stopWebSocket() {
    wsServerThread->stopThread();
    wsServerThread->stop();
}

void WsServerManager::sendData(const std::string &data) {
    serverCollection.add(data);
}

void WsServerManager::setMapApp(const nav_msgs::OccupancyGrid &occupancyGrid) {
    if (occupancyGrid.data.size() == 0) {
        return;
    }
    if (occupancyGrid.data.size() != occupancyGrid.info.width * occupancyGrid.info.height) {
        return;
    }
    RosOrientation orientation(occupancyGrid.info.origin.orientation.w,
                               occupancyGrid.info.origin.orientation.x,
                               occupancyGrid.info.origin.orientation.y,
                               occupancyGrid.info.origin.orientation.z);
    RosPosition position(occupancyGrid.info.origin.position.x,
                         occupancyGrid.info.origin.position.y,
                         occupancyGrid.info.origin.position.z);
    RosOrigin origin(orientation, position);

    RosMapLoadTime mapLoadTime(occupancyGrid.info.map_load_time.nsec, occupancyGrid.info.map_load_time.sec);

    RosStamp stamp(occupancyGrid.header.stamp.nsec, occupancyGrid.header.stamp.sec);
    RosHeader header(occupancyGrid.header.frame_id, occupancyGrid.header.seq, stamp);

    RosInfo info(occupancyGrid.info.width, occupancyGrid.info.height, occupancyGrid.info.resolution,
                 mapLoadTime, origin);

    auto occupancyList = occupancyGrid.data;

    cv::Mat mat(occupancyGrid.info.height, occupancyGrid.info.width, CV_8U);
    int index = 0;
    for (int y = 0; y < mat.rows; y++) {
        for (int x = 0; x < mat.cols; x++) {
            mat.at<unsigned char>(y, x) = occupancyList[index];
            index++;
        }
    }
//    cv::imshow("ds", mat);
//    cv::waitKey();
    cv::normalize(mat, mat, 0, 255, cv::NORM_MINMAX);

    /*
     * 调试 log ，误删
     * -1 34518
     * 0 9913
     * 100 1346
     *
     * 255 34518
     * 0 9913
     * 100 1346
     */
//    std::map<int, int> map1;
//    for (const auto &item: occupancyList) {
//        if (map1.count(item)) {
//            auto value = map1[item];
//            auto next = value + 1;
//            map1[item] = next;
//        } else {
//            map1[item] = 0;
//        }
//    }
//    LOG(ERROR) << "********************************************* 1";
//    for (const auto &item: map1) {
//        LOG(ERROR) << item.first << " " << item.second;
//    }

//    std::map<int, int> matMap;
//    for (int y = 0; y < mat.rows; y++) {
//        for (int x = 0; x < mat.cols; x++) {
//            if (matMap.count(mat.at<unsigned char>(y, x))) {
//                auto value = matMap[mat.at<unsigned char>(y, x)];
//                auto next = value + 1;
//                matMap[mat.at<unsigned char>(y, x)] = next;
//            } else {
//                matMap[mat.at<unsigned char>(y, x)] = 0;
//            }
//        }
//    }
//    LOG(ERROR) << "********************************************* 2";
//    for (const auto &item: matMap) {
//        LOG(ERROR) << item.first << " " << item.second;
//    }

//    for (const auto &item: occupancyList) {
//        mapDataList.push_back(item);
//    }
    std::vector<int> mapDataList = mat2Vector(mat);

//    std::set<int> sets;
//    for (const auto &item: mapDataList) {
//        sets.insert(item);
//    }
//    LOG(ERROR) << "********************************************* 3";
//    for (const auto &item: sets) {
//        LOG(ERROR) << item << " ";
//    }

    vector<int> dataList = compressValueQuantity(mapDataList);

    RosMap map(dataList, header, info);

    RequestModel<RosMap> requestModel;
    requestModel.setOp("publish");
    requestModel.setTopic(MAP_APP);
    requestModel.setMsg(map);

    json jsonResult = requestModel;
    NetModel netModel(NET_MODEL_MAP, jsonResult.dump());
    serverDataCollection.add(netModel);
}

void WsServerManager::setOdomApp(const nav_msgs::OdometryConstPtr &odomPtr) {
    RosStamp stamp(odomPtr->header.stamp.nsec, odomPtr->header.stamp.sec);
    RosHeader header(odomPtr->header.frame_id, odomPtr->header.seq, stamp);

    RosOrientation orientation(odomPtr->pose.pose.orientation.w,
                               odomPtr->pose.pose.orientation.x,
                               odomPtr->pose.pose.orientation.y,
                               odomPtr->pose.pose.orientation.z);
    RosPosition position(odomPtr->pose.pose.position.x,
                         odomPtr->pose.pose.position.y,
                         odomPtr->pose.pose.position.z);
    RosOrigin origin(orientation, position);
    vector<double> poseCovariance;
    for (const auto &item: odomPtr->pose.covariance) {
        poseCovariance.push_back(item);
    }
    RosPose pose(origin, poseCovariance);

    RosAngular angular(odomPtr->twist.twist.angular.x,
                       odomPtr->twist.twist.angular.y,
                       odomPtr->twist.twist.angular.z);
    RosLinear linear(odomPtr->twist.twist.linear.x,
                     odomPtr->twist.twist.linear.y,
                     odomPtr->twist.twist.linear.z);
    RosTwistX twistX(angular, linear);
    vector<double> twistCovariance;
    for (const auto &item: odomPtr->twist.covariance) {
        twistCovariance.push_back(item);
    }
    RosTwist twist(twistX, twistCovariance);

    RosOdom odom(odomPtr->child_frame_id, header, pose, twist);

    RequestModel<RosOdom> requestModel;
    requestModel.setOp("publish");
    requestModel.setTopic(ODOM_APP);
    requestModel.setMsg(odom);

    json jsonResult = requestModel;
    NetModel netModel(NET_MODEL_ODOM, jsonResult.dump());
    serverDataCollection.add(netModel);
}

void WsServerManager::sendRequestData(const string &key, const std::string &data) {
    wsServerThread->getWsServerSubThread()->sendRequestData(key, data);
}

void WsServerManager::sendMessageBusTopic(const string &message) {
    LOG(INFO) << "sendMessageBusTopic : " << message;
    MessageBusManager::instance().getMessageBus()->sendReq<void, string>(
            message.data(), MESSAGE_BUS_TOPIC);
}
