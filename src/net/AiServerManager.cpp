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

using json = nlohmann::json;

typedef websocketpp::server<websocketpp::config::asio> server;


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
