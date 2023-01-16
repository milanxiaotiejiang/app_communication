//
// Created by Looper on 2022/11/15.
//

#ifndef APP_COMMUNICATION_LINE_H
#define APP_COMMUNICATION_LINE_H

#include <geometry_msgs/Pose2D.h>
#include <geometry_msgs/Pose.h>
#include "model/Point.h"

namespace conversion {

    bool one_line(geometry_msgs::Pose2D pre_point,
                  geometry_msgs::Pose2D this_point,
                  geometry_msgs::Pose2D next_point);

    float cal_distance(const geometry_msgs::Pose::_position_type &ps, const geometry_msgs::Pose::_position_type &pe);

    float cal_distance(const Point &ps, const Point &pe);

}

#endif //APP_COMMUNICATION_LINE_H
