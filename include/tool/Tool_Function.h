//
// Created by lijiang on 2021/12/24.
//

#ifndef APP_COMMUNICATION_TOOL_FUNCTION_H
#define APP_COMMUNICATION_TOOL_FUNCTION_H
#include <model/TaskPoint.h>
#include <tf2/utils.h>
#include "model/TeachModePoint.h"

bool is_in_line(const TaskPoint point1, const TaskPoint point2, const TaskPoint point3);

bool is_in_line(const WayPoint point1, const WayPoint point2, const WayPoint point3) ;

bool is_in_line(const geometry_msgs::Pose point1, const geometry_msgs::Pose point2, const geometry_msgs::Pose point3) ;

bool is_in_line(const Point point1, const Point point2, const Point point3) ;

bool is_in_line(const vector<Point> &p);
#endif //APP_COMMUNICATION_TOOL_FUNCTION_H
