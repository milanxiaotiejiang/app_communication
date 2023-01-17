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


int set_partition(vector<float> &point, int);

int reset_partition(void);

bool get_partition(vector<vector<float>> &partition);

// int set_clean_zone(float * );
// int reset_clean_zone(void);
// bool get_clean_zone(vector<vector<float>>& clean_zone );

string read_from_file(char const *fileName);

bool write_into_file(char const *fileName, char const *content);