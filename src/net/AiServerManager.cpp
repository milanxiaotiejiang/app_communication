//
// Created by Looper on 2023/4/10.
//

#include "net/AiServerManager.h"

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/logger/syslog.hpp>
#include <websocketpp/server.hpp>

#include "nlohmann/json.hpp"

#include "net/kill_port.h"

#include "glog/logging.h"

#include "future/CThread.h"

#include <iostream>
#include "vector"

#include <boost/bind.hpp>

#include "manager/PublishInnerManager.h"
#include "ai_msgs/MultiRectangles.h"
#include "ai_msgs/Rectangle.h"

using json = nlohmann::json;

typedef websocketpp::server<websocketpp::config::asio> server;

//{"num_results":1,"results":[{"conf":0.73,"obj_id":1,"x_max":415,"x_min":359,"y_max":322,"y_min":268}]}

struct AiResult {
    float conf;
    int obj_id;
    int x_max;
    int x_min;
    int y_max;
    int y_min;

    friend void to_json(json &j, const AiResult &b) {
        j = json{
                {"conf",   b.conf},
                {"obj_id", b.obj_id},
                {"x_max",  b.x_max},
                {"x_min",  b.x_min},
                {"y_max",  b.y_max},
                {"y_min",  b.y_min},
        };
    }

    friend void from_json(const json &j, AiResult &b) {
        j.at("conf").get_to(b.conf);
        j.at("obj_id").get_to(b.obj_id);
        j.at("x_max").get_to(b.x_max);
        j.at("x_min").get_to(b.x_min);
        j.at("y_max").get_to(b.y_max);
        j.at("y_min").get_to(b.y_min);
    }
};

struct AiData {
    int num_results;
    std::vector<AiResult> results;

    friend void to_json(json &j, const AiData &b) {
        j = json{
                {"num_results", b.num_results},
                {"results",     b.results},
        };
    }

    friend void from_json(const json &j, AiData &b) {
        j.at("num_results").get_to(b.num_results);
        j.at("results").get_to(b.results);
    }
};


void ai_fail(server *s, websocketpp::connection_hdl hdl) {
    server::connection_ptr con = s->get_con_from_hdl(hdl);
    LOG(WARNING) << "Fail handler: " << con->get_ec() << " " << con->get_ec().message();
}

void ai_close(websocketpp::connection_hdl hdl) {
    LOG(WARNING) << "Close handler";
}

void ai_open(server *s, websocketpp::connection_hdl hdl) {
    LOG(WARNING) << "Open handler" << std::endl;

}

// Define a callback to handle incoming messages
void ai_message(server *s, const websocketpp::connection_hdl &hdl, const server::message_ptr &msg) {
    std::cout << "on_message called with hdl: " << hdl.lock().get()
              << " and message: " << msg->get_payload()
              << " and " << msg->get_opcode()
              << std::endl;
    try {
        json jDecode = json::parse(msg->get_payload());
        AiData aiData = jDecode.get<AiData>();

        ai_msgs::MultiRectangles detect_results;
        for (int i = 0; i < aiData.num_results; i++) {
            AiResult aiResult = aiData.results[i];
            ai_msgs::Rectangle rect;
            rect.x = aiResult.x_min;
            rect.y = aiResult.y_min;
            rect.width = aiResult.x_max - aiResult.x_min;
            rect.height = aiResult.y_max - aiResult.y_min;
            detect_results.rectangles.push_back(rect);
        }
        PublishInnerManager::instance().pubDetection(detect_results);
    } catch (const std::exception &e) {
        std::cout << "Error parsing JSON message: " << e.what() << std::endl;
    }
}

class AiServerThread : public CThread {
private:
    server echo_server;

public:
    void *run() override {
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
            echo_server.set_message_handler(bind(&ai_message, &echo_server, ::_1, ::_2));

            //设置连接失败时的回调函数
            echo_server.set_fail_handler(bind(&ai_fail, &echo_server, ::_1));
            //设置关闭连接时的回调函数
            echo_server.set_close_handler(&ai_close);
            //设置打开连接时的回调函数
            echo_server.set_open_handler(bind(&ai_open, &echo_server, ::_1));

            //监听端口号
            echo_server.listen(9095);
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

    void stopThread() {

    }
};

AiServerThread *aiServerThread;

void AiServerManager::startWebSocket() {
    std::string pid = get_pid_using_port(9095);
    if (!pid.empty()) {
        LOG(INFO) << "进程 pid 为 " << pid << " 占用 9095 端口 ！！";
        kill_process(pid);
    }

    aiServerThread = new AiServerThread();
    aiServerThread->start();
    aiServerThread->detach();
}

void AiServerManager::stopWebSocket() {
    aiServerThread->stopThread();
    aiServerThread->stop();
}
