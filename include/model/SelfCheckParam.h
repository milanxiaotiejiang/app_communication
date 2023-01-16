//
// Created by mi on 2022/9/5.
//

#ifndef APP_COMMUNICATION_SELFCHECKPARAM_H
#define APP_COMMUNICATION_SELFCHECKPARAM_H

// 激光雷达scan参数
const float R_ROBOT = 0.09;                                   //机器人圆角半径
const float HW = 0.247;                                       // 极光壹号顶部横轴（与前进方向垂直）一半长度
const float HL = 0.287;                                       // 极光壹号顶部纵轴（与前进方向平行）一半长度
const float LASER_RADIUS = 0.025;                             // 激光雷达半径
const float CENT2LASER_Y = 0.17;                              // 激光雷达与顶部中心距离
const float DEG0 = -88.058;                                   // 激光雷达0度偏向角
const double PI = 3.14159265359;                              // 圆周率
const float LASER_ALARM_WIDTH = 0.35;                         //警戒宽度的一半
const float INFALTION_ALARM_WIDTH = 1.5;                      //膨胀层警戒宽度的一半
const float ALARM_LENGTH_MIN = HL + CENT2LASER_Y + 0.1;       //机器人最前端
const float ALARM_LENGTH_MAX = ALARM_LENGTH_MIN + 0.4;        //警戒距离50cm
const float INFLATION_ALARM_LENGTH_MAX = ALARM_LENGTH_MIN + 2;//膨胀层警戒距离4m

// TOF参数
//防跌
//左上
const float TOF_MIN1 = 0.0199;// 检测最小值（下同）
const float TOF_MAX1 = 0.201; // 最大值（下同）
const int TOF_FFL = 1;        // 传感器编号（如传感器对应话题为/mrrobot/ls_senser1,则其编号为1）（下同）

//右上
const float TOF_MIN2 = 0.0199;
const float TOF_MAX2 = 0.201;
const int TOF_FFR = 2;

//左
const float TOF_MIN3 = 0.0199;
const float TOF_MAX3 = 0.201;
const int TOF_FL = 3;
//右
const float TOF_MIN4 = 0.0199;
const float TOF_MAX4 = 0.201;
const int TOF_FR = 4;
//贴边
//前
const float TOF_MIN5 = 0.0199;
const float TOF_MAX5 = 65.536;
const int TOF_WF = 5;
//后
const float TOF_MIN6 = 0.0299;
const float TOF_MAX6 = 65.536;
const int TOF_WB = 6;

// 超声波参数
//左上传感器信息
const float SPS_MIN1 = 0.0199;//最小值（下同）
const float SPS_MAX1 = 65.536;//最大值（下同）
const int SPS_FL = 1;         // 传感器编号（如传感器对应话题为/mrrobot/ul_senser1,则其编号为1）（下同）

//右上传感器信息
const float SPS_MIN2 = 0.0199;
const float SPS_MAX2 = 65.536;
const int SPS_FR = 2;

//角左传感器信息
const float SPS_MIN3 = 0.0199;
const float SPS_MAX3 = 65.536;
const int SPS_CL = 3;

//角右传感器信息
const float SPS_MIN4 = 0.0199;
const float SPS_MAX4 = 65.536;
const int SPS_CR = 4;

//左传感器信息
const float SPS_MIN5 = 0.0199;
const float SPS_MAX5 = 65.536;
const int SPS_L = 5;

//右传感器信息
const float SPS_MIN6 = 0.0199;
const float SPS_MAX6 = 65.536;
const int SPS_R = 6;
//前面两个
const float SPS_OBS_MIN1 = 0.25;
const float SPS_OBS_MAX1 = 0.8;
const float SPS_OBS_MIN2 = 0.25;
const float SPS_OBS_MAX2 = 0.8;
//角上两个
const float SPS_OBS_MIN3 = 0.2;
const float SPS_OBS_MIN4 = 0.2;
const float SPS_OBS_MAX3 = 0.45;
const float SPS_OBS_MAX4 = 0.45;

// 奥比中光深度相机的参数
const float RANGE = 0.99;        // 像素中0或者255最大允许比例范围:0-1
const float IS_OBSTACLE_NUM = 30;//判断为障碍物的像素数量

#endif //APP_COMMUNICATION_SELFCHECKPARAM_H
