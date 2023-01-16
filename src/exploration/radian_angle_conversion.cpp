//
// Created by Looper on 2022/11/8.
//

#include "exploration/radian_angle_conversion.h"

#define PI 3.14159265359

namespace conversion {
    double angle_to_radian(double degree, double min, double second) {
        double flag = (degree < 0) ? -1.0 : 1.0;            //判断正负
        if (degree < 0) {
            degree = degree * (-1.0);
        }
        double angle = degree + min / 60 + second / 3600;
        double result = flag * (angle * PI) / 180;
        return result;
    }

    void radian_to_angle(double rad, double ang[]) {
        double flag = (rad < 0) ? -1.0 : 1.0;
        if (rad < 0) {
            rad = rad * (-1.0);
        }
        double result = (rad * 180) / PI;
        double degree = int(result);
        double min = (result - degree) * 60;
        double second = (min - int(min)) * 60;
        ang[0] = flag * degree;
        ang[1] = int(min);
        ang[2] = second;
    }
}