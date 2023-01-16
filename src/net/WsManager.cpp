//
// Created by zt on 2022/3/8.
//

#include "net/WsManager.h"

using namespace code_machina;
using namespace std;

using json = nlohmann::json;

typedef websocketpp::client<websocketpp::config::asio_client> client;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// 消息指针
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

code_machina::BlockingCollection<string> dataCollection;

enum ConnectStatus {
    CONNECTED, CONNECTING, DISCONNECTED, RECONNECT
};

void connectFail();

atomic<bool> isAuth(false);

ConnectStatus mStatus = DISCONNECTED;

class WsDataSendThread : public CThread {
private:
    client *pc;
    websocketpp::connection_hdl hdl;
public:
    WsDataSendThread(client *pc, const websocketpp::connection_hdl &hdl) : pc(pc),
                                                                           hdl(hdl) {}

    void *run() override {
        std::cout << "WsDataSendThread : " << syscall(SYS_gettid) << std::endl;

        //keep
//        pc->send(hdl, "1", websocketpp::frame::opcode::text);

        string data;

        while (!dataCollection.is_completed()) {
            auto status = dataCollection.take(data);
            if (status == BlockingCollectionStatus::Ok) {
                pc->send(hdl, data, websocketpp::frame::opcode::text);
                pc->get_alog().write(websocketpp::log::alevel::app, data);
            }
        }
    }
};

// 打开连接时的回调
void on_open(client *c, websocketpp::connection_hdl hdl) {
    std::cout << "open ... " << std::endl;
    mStatus = CONNECTED;

    RegisterOperation operation("666");
    RequestModel<RegisterOperation> model("register", "", operation);
    json basicJson = model;
    std::string msg = basicJson.dump();

    websocketpp::lib::error_code ec;
    // 发送文本消息
    c->send(hdl, msg, websocketpp::frame::opcode::text, ec);
    c->get_alog().write(websocketpp::log::alevel::app, "on_open: " + msg);
    if (ec) {
        isAuth = false;
        std::cout << "Echo failed because: " << ec.message() << std::endl;
    } else {
        WsDataSendThread *pDataSendThread = new WsDataSendThread(c, hdl);
        pDataSendThread->start();
        pDataSendThread->detach();
    }
}

// 连接失败时的回调
void on_fail(client *c, websocketpp::connection_hdl hdl) {
    cout << "on_fail" << endl;
    c->get_alog().write(websocketpp::log::alevel::app, "Connection Failed");
    connectFail();
}

// 接收到服务器发来的WebSocket消息后的回调
void on_message(client *c, websocketpp::connection_hdl hdl, message_ptr msg, PubInner pubInner) {
    cout << "on_message : " << msg->get_payload() << endl;
    c->get_alog().write(websocketpp::log::alevel::app, "on_message : " + msg->get_payload());
    string payload = msg->get_payload();
    json jdecode = json::parse(payload);
    auto entrance = jdecode.get<RequestModel<RequestData>>();
    string op = entrance.getOp();
    string topic = entrance.getTopic();
    if (op == "register") {
        isAuth = true;
    } else {
        string data = entrance.getMsg().data;
//        pubInner.publishWebMessage(data);
    }
}

// 关闭连接时的回调
void on_close(client *c, websocketpp::connection_hdl hdl) {
    cout << "on_close : " << endl;
    connectFail();
}

class WsConnectThread : public CThread {
private:
    PubInner pubInner;
public:
    WsConnectThread(const PubInner &pubInner) : pubInner(pubInner) {}

    void *run() override {

        std::cout << "WsConnectThread : " << syscall(SYS_gettid) << std::endl;

        client echo_client;

        // 调整日志策略
        echo_client.clear_access_channels(websocketpp::log::alevel::frame_header);
        echo_client.clear_access_channels(websocketpp::log::alevel::frame_payload);

        std::string uri = "ws://192.168.1.175:8888";

        try {
            // 初始化ASIO ASIO
            echo_client.init_asio();

            // 注册回调函数
            echo_client.set_open_handler(std::bind(&on_open, &echo_client, ::_1));
            echo_client.set_fail_handler(std::bind(&on_fail, &echo_client, ::_1));
            echo_client.set_message_handler(std::bind(&on_message, &echo_client, ::_1, ::_2, pubInner));
            echo_client.set_close_handler(std::bind(&on_close, &echo_client, ::_1));

            // 在事件循环启动前创建一个连接对象
            websocketpp::lib::error_code ec;
            client::connection_ptr con = echo_client.get_connection(uri, ec);
            if (ec) {
                std::cout << "could not create connection because: " << ec.message() << std::endl;
            } else {
                echo_client.connect(con);
                con->get_handle(); // 连接句柄，发送消息时必须要传入

                // 启动事件循环（ASIO的io_service），当前线程阻塞
                echo_client.run();
            }
        } catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
        } catch (websocketpp::lib::error_code e) {
            std::cout << e.message() << std::endl;
        } catch (...) {
            std::cout << "other exception" << std::endl;
        }
        connectFail();
    }
};

void connectFail() {
    isAuth = false;
    mStatus = DISCONNECTED;
}

void WsManager::startWebSocket(const PubInner &inner) {
    switch (mStatus) {
        case CONNECTED:
            ROS_INFO("已连接");
            break;
        case CONNECTING:
            ROS_INFO("正在连接");
            break;
        case RECONNECT:
        case DISCONNECTED:
            mStatus = CONNECTING;
            WsConnectThread *pConnectThread = new WsConnectThread(inner);
            pConnectThread->start();
            pConnectThread->detach();
            break;
    }
}

void WsManager::sendData(const string &data) {
    if (mStatus == CONNECTED) {

        dataCollection.add(data);
    } else {
        ROS_INFO("其他原因导致发送失败");
    }

}
