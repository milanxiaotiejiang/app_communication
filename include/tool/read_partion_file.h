
#ifndef READ_PARTION_H
#define READ_PARTION_H

#include "../model/Partion.h"
#include "../prohibition.h"
#include "yaml-cpp/yaml.h"
#include <fstream>
#include <iostream>
#include <ros/package.h>
#include <vector>

bool get_partion_line(vector<Partion_line> &segmentationPoints);

bool set_partion_line(vector<Partion_line> &segmentationPoints);

void createBasicName(int count);

string getNameForID(int index);

bool setNameForID(const Name_info &name_info);

void clearBasicName();

bool get_map_orgin(vector<int> &origin);

// bool get_devided_area(vector<float> &partion);

// bool get_devided_area(vector<vector<float>> &devided_area);


#endif//READ_PARTION_H