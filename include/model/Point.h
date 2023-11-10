//
// Created by lijiang on 2022/1/6.
//

#ifndef APP_COMMUNICATION_POINT_H
#define APP_COMMUNICATION_POINT_H

#include "nlohmann/json.hpp"
#include <ostream>


using json = nlohmann::json;

class Line;

class Point {
private:
    float x;
    float y;

public:
    friend void to_json(json &j, const Point &b) {
        j = json{
                {"x", b.x},
                {"y", b.y}};
    }

    friend void from_json(const json &j, Point &b) {
        j.at("x").get_to(b.x);
        j.at("y").get_to(b.y);
    }


    Point(float x, float y);

    Point();

    virtual ~Point();

    float getX() const;

    void setX(float x);

    float getY() const;

    void setY(float y);

    void setXandY(float x, float y);

    void show_point();

    float calculateDistance(Point &another_point);

    Line operator-(Point &another_point);
};

class Line {
private:
    float x;
    float y;

public:
    Line(float x, float y);

    Line();

    virtual ~Line();

    float getX() const;

    void setX(float x);

    float getY() const;

    void setY(float y);

    void setXandY(float x, float y);

    Line operator+(const Line &p) const;

    Line operator*(const float &p) const;

    Line operator/(const float &p) const;

    float cross(float x1, float y1, float x2, float y2);

    float ab_cross_ac(Point a, Point b, Point c);

    int point_on_line(Point a, Point b, Point c);

    float dot(float x1, float y1, float x2, float y2);

    int dblcmp(float a, float b);

    bool ab_cross_cd(Point a, Point b, Point c, Point d, Point &p);
};


#endif//APP_COMMUNICATION_POINT_H
