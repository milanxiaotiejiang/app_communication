//
// Created by lijiang on 2021/12/20.
//

#include "sub/BeforeJsonSubscribe.h"
#include "net/WsServerManager.h"

BeforeJsonSubscribe::BeforeJsonSubscribe(ros::NodeHandle handle) : handle(handle) {
    sub_json_ = handle.subscribe(APP_COMMUNICATION, 1, &BeforeJsonSubscribe::subscribeCallback, this);
}

BeforeJsonSubscribe::~BeforeJsonSubscribe() {
}

void BeforeJsonSubscribe::subscribeCallback(const std_msgs::String &str) {

    std_msgs::String command;      //用于收到消息后的回应
    geometry_msgs::Twist move_cmd; //用于发布速度指令
    geometry_msgs::Twist clean_msg;//用于发布区域清扫信息
    command.data.clear();

    std::string header(str.data, 0, 4);
    std::string len(str.data, 4, 2);
    int length = hexstring2int(len);//字节数
    std::string mission_id(str.data, 6, 2);
    int mission_ID = hexstring2int(mission_id);

    //mission_ID = hexstring2int("00027100");
    // int mission_ID = atoi(mission_id.c_str());
    std::string mission(str.data, 8, (length - 3) * 2);
    float point[8];
    int flag = -1;
    flag = (int) htoi_signed(mission.substr(0, 2));
    for (int i = 0; i < (length - 3) / 4; i++) {

        // point[i]=(float)hexstring2int(mission.substr(8*i, 8) )/10000;//存储点的信息
        point[i] = (float) htoi_signed(mission.substr(8 * i, 8)) / 10000;//存储点的信息
    }
    std::string Syn_flag(str.data, (length + 3 - 2) * 2, 2);//同步位信息
    std::string check(str.data, (length + 3 - 1) * 2, 2);   //校验位信息
    //    ROS_INFO("header: %s", header.c_str());
    //    ROS_INFO("length: %s", len.c_str());
    //    ROS_INFO("mission_id: %s", mission_id.c_str());
    //    ROS_INFO("mission: %s", mission.c_str());
    //    ROS_INFO("Syn_flag: %s", Syn_flag.c_str());
    //    ROS_INFO("check: %s", check.c_str());
    if (header == "FFAA")                              //判断校验位
    {
        float vel_value = hexstring2int(mission.substr(2, 2)) * 0.005;//针对移动指令的处理,30对应的速度是0.15
        std::string vel_direction(mission, 0, 2);
        switch (mission_ID) {
            case BASE_MOVE://底盘移动
                //clean_msg.angular.z = 2.0;//2.0表示工作，1表示返回基站，0表示停止
                //clean_msg.linear.z = 0.0;//表示非工作仅移动状态
                //pub_clean_demo.publish(clean_msg);
                if (vel_direction == "00") {
                    move_cmd.linear.x = vel_value * 2;
                } else if (vel_direction == "01") {
                    move_cmd.linear.x = -1 * vel_value * 2;
                } else if (vel_direction == "02") {
                    move_cmd.angular.z = -5 * vel_value;
                } else if (vel_direction == "03") {
                    move_cmd.angular.z = 5 * vel_value;
                } else if (vel_direction == "04") {
                    move_cmd.angular.z = 0;
                }
                PublishInnerManager::instance().publishVelocity(move_cmd);//发布速度
                break;
            default:
                break;
        }
    }
}
