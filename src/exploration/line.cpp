//
// Created by Looper on 2022/11/15.
//

#include "exploration/line.h"

namespace conversion {

    bool one_line(geometry_msgs::Pose2D pre_point, geometry_msgs::Pose2D this_point, geometry_msgs::Pose2D next_point) {
        float judge_num = (pre_point.y - this_point.y) * (next_point.x - this_point.x) -
                          (pre_point.x - this_point.x) * (next_point.y - this_point.y);
        return abs(judge_num) < 5 * 1e-4;
    }

    float cal_distance(const geometry_msgs::Pose::_position_type &ps, const geometry_msgs::Pose::_position_type &pe) {
        return sqrt(pow(ps.x - pe.x, 2) + pow(ps.y - pe.y, 2));
    }

    float cal_distance(const Point &ps, const Point &pe) {
        return sqrt(pow(ps.getX() - pe.getX(), 2) + pow(ps.getY() - pe.getY(), 2));
    }
}