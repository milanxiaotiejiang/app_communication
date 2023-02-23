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

/*
argv[0]: /home/lijiang/app_ws/devel/lib/app_communication/dump_upload
argv[1]: a7ece53e-07c4-4d3b-89b579b0-a0be62b6.dmp
 */

int main(int argc, char *argv[]) {

    printf("Hello Dump Upload\n");

//    int i;
//    char **ptr;
//    extern char **environ;
//    for (i = 0; i < argc; ++i) {
//        printf("argv[%d]: %s\n", i, argv[i]);
//    }

//    for (ptr = environ; *ptr != 0; ptr++) {
//        printf("%s\n", *ptr);
//    }

    if (argc == 3) {
        std::string crash_file = argv[1];
        std::string log_file = argv[2];
        if (!crash_file.empty()) {
            if (endsWith(crash_file, ".dmp")) {

                ros::init(argc, argv, "dump_upload");

                ros::NodeHandle handle;
                ros::Publisher pubDump = handle.advertise<std_msgs::String>("/dump_crash", 1);
                std_msgs::String crash;
                crash.data = crash_file;
                sleep(3);

                std::cout << crash_file << " upload ... " << std::endl;
                std::cout << log_file << " upload ... " << std::endl;

                pubDump.publish(crash);
            }

        }
    }

    exit(0);
}