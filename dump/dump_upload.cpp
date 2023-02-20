//
// Created by Looper on 2023/2/20.
//

#include <iostream>
#include "sys/wait.h"
#include <sys/types.h>
#include <dirent.h>
#include <armadillo>
#include <ros/ros.h>
#include <std_msgs/String.h>

bool endsWith(const std::string &str, const std::string &suffix) {
    if (suffix.length() > str.length()) {
        return false;
    }

    return (str.rfind(suffix) == (str.length() - suffix.length()));
}

int main(int argc, char *argv[]) {

    ros::init(argc, argv, "dump_upload");

    int i;
    char **ptr;
    extern char **environ;
    for (i = 0; i < argc; ++i) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    for (ptr = environ; *ptr != 0; ptr++) {
        printf("%s\n", *ptr);
    }

    ros::NodeHandle handle;
    ros::Publisher pubDump = handle.advertise<std_msgs::String>("/crash", 1);
    std_msgs::String crash;
    crash.data = "1234";
    sleep(1);
    pubDump.publish(crash);

    std::string crash_file = argv[1];
    if (!crash_file.empty()) {
        if (endsWith(crash_file, ".dmp")) {

            std::cout << crash_file << " log ... " << std::endl;
            std::cout << crash_file << " compress ... " << std::endl;
            std::cout << crash_file << " upload ... " << std::endl;


            ros::NodeHandle handle;
            ros::Publisher pubDump = handle.advertise<std_msgs::String>("/crash", 1);
            std_msgs::String crash;
            crash.data = crash_file;
            sleep(1);
            pubDump.publish(crash);
        }

    }
    exit(0);
}

