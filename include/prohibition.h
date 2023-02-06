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

using namespace std;

int set_prohibition(float *, int);

int reset_prohibition(void);

bool get_prohibition(vector<vector<float>> &prohibition_position);


bool write_into_file(char const *fileName, char const *content);