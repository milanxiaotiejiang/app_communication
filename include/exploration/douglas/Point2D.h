//
// Created by Looper on 2023/5/24.
//

#ifndef APP_COMMUNICATION_POINT2D_H
#define APP_COMMUNICATION_POINT2D_H

struct Point2D {
    float x, y;

    Point2D(float x, float y) : x(x), y(y) {}

    Point2D operator-(const Point2D &other) const {
        return {x - other.x, y - other.y};
    }
};

struct Point2DAccessor {
    static double getX(const Point2D &p) {
        return p.x;
    }

    static double getY(const Point2D &p) {
        return p.y;
    }
};

#endif //APP_COMMUNICATION_POINT2D_H
