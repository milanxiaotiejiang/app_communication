//
// Created by Looper on 2023/3/17.
//

#ifndef APP_COMMUNICATION_KILL_PORT_H
#define APP_COMMUNICATION_KILL_PORT_H

#include "string"

// 获取指定端口占用的进程 ID
std::string get_pid_using_port(int port);

// 关闭指定的进程
void kill_process(const std::string &pid);

#endif //APP_COMMUNICATION_KILL_PORT_H
