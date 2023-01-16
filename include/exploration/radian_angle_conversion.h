//
// Created by Looper on 2022/11/8.
//

#ifndef APP_COMMUNICATION_RADIAN_ANGLE_CONVERSION_H
#define APP_COMMUNICATION_RADIAN_ANGLE_CONVERSION_H

namespace conversion {
    double angle_to_radian(double degree, double min, double second);

    void radian_to_angle(double rad, double ang[]);
};


#endif //APP_COMMUNICATION_RADIAN_ANGLE_CONVERSION_H
