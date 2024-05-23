
#ifndef APP_COMMUNICATION_PROHIBITION_H
#define APP_COMMUNICATION_PROHIBITION_H

#include <iostream>
#include "yaml-cpp/yaml.h"
#include<fstream>
#include<vector>
#include<geometry_msgs/Twist.h>
#include<geometry_msgs/Pose.h>
#include<std_msgs/String.h>

int set_prohibition(std::string, float *, int);

int reset_prohibition(std::string prohibition_path);

bool get_prohibition(std::string, std::vector<std::vector<float>> &prohibition_position);


#endif //APP_COMMUNICATION_PROHIBITION_H