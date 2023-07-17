//
// Created by Looper on 2023/7/17.
//

#include "leave/sensor/carpet.h"

Carpet::Carpet(const ros::NodeHandle &handle) : Sensor(handle, "/sub_carpet") {}

Carpet::~Carpet() = default;

int Carpet::transformRosModel(std_msgs::Int32 model) {
    return model.data;
}

void Carpet::dateProgressing(int data) {

}
