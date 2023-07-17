//
// Created by Looper on 2023/7/17.
//

#ifndef APP_COMMUNICATION_CARPET_H
#define APP_COMMUNICATION_CARPET_H

#include "sensor.h"

/**
 * 地毯
 * /mrrobot/carpet_detect
 */
class Carpet : public Sensor<std_msgs::Int32, int> {
public:
    explicit Carpet(const ros::NodeHandle &handle);

    ~Carpet() override;

    int transformRosModel(std_msgs::Int32 model) override;

    void dateProgressing(int data) override;
};


#endif //APP_COMMUNICATION_CARPET_H
