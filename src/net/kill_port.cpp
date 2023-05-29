//
// Created by Looper on 2023/3/17.
//

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unistd.h>

std::string get_pid_using_port(int port) {
    std::stringstream cmd;
    cmd << "lsof -t -i :" << port;

    FILE *fp = popen(cmd.str().c_str(), "r");
    if (!fp) {
        throw std::runtime_error("popen failed");
    }

    char buf[16] = {0};
    if (fgets(buf, sizeof(buf), fp) == NULL) {
        pclose(fp);
        return "";
    }

    pclose(fp);
    return std::string(buf);
}

void kill_process(const std::string &pid) {
    std::string cmd = "kill -9 " + pid;
    system(cmd.c_str());
}
