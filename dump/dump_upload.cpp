//
// Created by Looper on 2023/2/20.
//

#include <iostream>
#include <dirent.h>
#include <armadillo>
#include <ros/ros.h>
#include <std_msgs/String.h>
#include "cppfs/fs.h"
#include "cppfs/FileHandle.h"
#include "cppfs/FilePath.h"

bool endsWith(const std::string &str, const std::string &suffix) {
    if (suffix.length() > str.length()) {
        return false;
    }

    return (str.rfind(suffix) == (str.length() - suffix.length()));
}

void split(const std::string &s, std::vector<std::string> &tokens, char delim) {
    tokens.clear();
    size_t lastPos = s.find_first_not_of(delim, 0);
    size_t pos = s.find(delim, lastPos);
    while (lastPos != std::string::npos) {
        tokens.emplace_back(s.substr(lastPos, pos - lastPos));
        lastPos = s.find_first_not_of(delim, pos);
        pos = s.find(delim, lastPos);
    }
}

int main(int argc, char *argv[]) {

    printf("Hello Dump Upload\n");

//    int i;
//    for (i = 0; i < argc; ++i) {
//        printf("argv[%d]: %s\n", i, argv[i]);
//    }

//    char **ptr;
//    extern char **environ;
//    for (ptr = environ; *ptr != 0; ptr++) {
//        printf("%s\n", *ptr);
//    }

    if (argc == 4) {
        std::string program_string = argv[1];
        std::string crash_file = argv[2];
        std::string log_file = argv[3];

        std::vector<std::string> uploads;

//        std::string program_string = "/home/lijiang/app_ws/devel/lib/app_communication/rec_app_node";
//        std::string crash_file = "6016650c-10bb-4403-f83b71bf-5e42555c.dmp";
//        std::string log_file = "info_20230303-111132.22002";

//        std::cout << " program_string : " << program_string << std::endl;
//        std::cout << " crash_file : " << crash_file << std::endl;
//        std::cout << " log_file : " << log_file << std::endl;

        auto program = cppfs::FilePath(program_string);

        std::vector<std::string> tokens;
        split(program_string, tokens, '/');

        if (!tokens.empty() && tokens.size() > 2) {
            std::string home_path = "/" + tokens[0] + "/" + tokens[1] + "/";

            if (program.baseName() == "rec_app_node") {

                if (!crash_file.empty() && endsWith(crash_file, ".dmp")) {

                    auto crash_dmp = cppfs::FilePath(home_path + "app_dump/" + crash_file);
                    if (cppfs::fs::open(crash_dmp.fullPath()).exists())
                        uploads.push_back(crash_dmp.fullPath());

                    auto crash_uuid = crash_dmp.baseName();
                    auto crash_txt = cppfs::FilePath(home_path + "app_dump/" + crash_uuid + ".txt");
                    if (cppfs::fs::open(crash_txt.fullPath()).exists())
                        uploads.push_back(crash_txt.fullPath());

                    auto log = cppfs::FilePath(home_path + "app_log/" + log_file);
                    if (cppfs::fs::open(log.fullPath()).exists())
                        uploads.push_back(log.fullPath());

                    ros::init(argc, argv, "dump_upload");

                    ros::NodeHandle handle;
                    ros::Publisher pubDump = handle.advertise<std_msgs::String>("/robot_internal_event", 1);

                    sleep(3);

                    std::string json = "{event_id: \"robot_internal_error\", event_params: [\"";
                    for (int i = 0; i < uploads.size(); i++) {
                        if (i == uploads.size() - 1) {
                            json.append(uploads[i]).append("\"]");
                        } else {
                            json.append(uploads[i]).append("\", \"");
                        }
                    }
                    json.append("}");

                    std::cout << json << std::endl;
                    std_msgs::String crash;
                    crash.data = json;
                    pubDump.publish(crash);
                }

            }
        }
    }

    exit(0);
}