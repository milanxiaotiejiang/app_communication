/***
 * @brief: prihibition areas
 * @author: jiang
 * @date: 20210904
***/

// #ifndef PROHIBITION_H
// #define CLEANINGPATHPPROHIBITION_HLANNING_H

#include <iostream>
#include "yaml-cpp/yaml.h"
#include<fstream>
#include<vector>
#include<geometry_msgs/Twist.h>
#include<geometry_msgs/Pose.h>
#include<std_msgs/String.h>

int set_prohibition(float *, int);

int reset_prohibition(std::string prohibition_path);

bool get_prohibition(std::vector<std::vector<float>> &prohibition_position);