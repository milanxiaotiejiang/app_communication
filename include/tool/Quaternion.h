//
// Created by lijiang on 2022/1/12.
//

#ifndef APP_COMMUNICATION_QUATERNION_H
#define APP_COMMUNICATION_QUATERNION_H
#include "math.h"

class Quaternion_ros
{
public:
    float w;
    float x;
    float y;
    float z;

    Quaternion_ros();

    void toQuaternion(float pitch, float roll, float yaw);
};

void Quaternion_ros::toQuaternion(float pitch, float roll, float yaw)//rpy转四元数
{

    float cy = cos(yaw * 0.5);
    float sy = sin(yaw * 0.5);
    float cr = cos(roll * 0.5);
    float sr = sin(roll * 0.5);
    float cp = cos(pitch * 0.5);
    float sp = sin(pitch * 0.5);

    w = cy * cr * cp + sy * sr * sp;
    x = cy * sr * cp - sy * cr * sp;
    y = cy * cr * sp + sy * sr * cp;
    z = sy * cr * cp - cy * sr * sp;
}

Quaternion_ros::Quaternion_ros()
{
    w = 1;
    x = 0;
    y = 0;
    z = 0;
}


#endif //APP_COMMUNICATION_QUATERNION_H
