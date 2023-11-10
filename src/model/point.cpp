//
// Created by lijiang on 2022/1/6.
//

#include "model/Point.h"
#include <cstring>
#include <std_msgs/String.h>


Point::Point(float x, float y) : x(x), y(y) {}

Point::~Point() {
}

float Point::getX() const {
    return x;
}

void Point::setX(float x) {
    Point::x = x;
}

float Point::getY() const {
    return y;
}

void Point::setY(float y) {
    Point::y = y;
}

void Point::setXandY(float x, float y) {
    Point::x = x;
    Point::y = y;
}

void Point::show_point() {
    std::cout << "x:" << x << "y:" << y << std::endl;
}

Point::Point() {}

float Point::calculateDistance(Point &another_point) {//求两点间距离
    return sqrt((this->getX() - another_point.getX()) * (this->getX() - another_point.getX()) +
                (this->getY() - another_point.getY()) * (this->getY() - another_point.getY()));
}

Line Point::operator-(Point &another_point) {
    return Line(this->getX() - another_point.getX(), this->getY() - another_point.getY());
}

Line::Line(float x, float y) : x(x), y(y) {}

Line::~Line() {
}

float Line::getX() const {
    return x;
}

void Line::setX(float x) {
    Line::x = x;
}

float Line::getY() const {
    return y;
}

void Line::setY(float y) {
    Line::y = y;
}

void Line::setXandY(float x, float y) {
    Line::x = x;
    Line::y = y;
}

Line::Line() {}


float Line::dot(float x1, float y1, float x2, float y2)//点积
{
    return x1 * x2 + y1 * y2;
}

int Line::dblcmp(float a, float b) {
    if (a > b)
        return 1;
    else if (a == b)
        return 0;
    else
        return -1;
}

int Line::point_on_line(Point a, Point b, Point c)//求a点是不是在线段bc上，>0不在，=0与端点重合，<0在。
{
    return dblcmp(dot(b.getX() - a.getX(), b.getY() - a.getY(), c.getX() - a.getX(), c.getY() - a.getY()), 0);
}

Line Line::operator+(const Line &p) const {
    Line result;
    result.setXandY(p.getX() + this->x, p.getY() + this->y);
    return result;
}

Line Line::operator*(const float &p) const {
    return Line(this->getX() * p, this->getY() * p);
}

Line Line::operator/(const float &p) const {
    return Line(this->getX() / p, this->getY() / p);
}

float Line::cross(float x1, float y1, float x2, float y2) {
    return x1 * y2 - x2 * y1;
}

float Line::ab_cross_ac(Point a, Point b, Point c)//ab与ac的叉积
{
    return cross(b.getX() - a.getX(), b.getY() - a.getY(), c.getX() - a.getX(), c.getY() - a.getY());
}


bool Line::ab_cross_cd(Point a, Point b, Point c, Point d, Point &p)//求ab是否与cd相交，交点为p。1规范相交，0交点是一线段的端点，-1不相交。
{
    double s1, s2, s3, s4;
    int d1, d2, d3, d4;
    d1 = dblcmp(s1 = ab_cross_ac(a, b, c), 0);
    d2 = dblcmp(s2 = ab_cross_ac(a, b, d), 0);
    d3 = dblcmp(s3 = ab_cross_ac(c, d, a), 0);
    d4 = dblcmp(s4 = ab_cross_ac(c, d, b), 0);

    //如果规范相交则求交点
    if ((d1 ^ d2) == -2 && (d3 ^ d4) == -2) {
        p.setX((c.getX() * s2 - d.getX() * s1) / (s2 - s1));
        p.setY((c.getY() * s2 - d.getY() * s1) / (s2 - s1));
        return 1;
    }

    //如果不规范相交
    if (d1 == 0 && point_on_line(c, a, b) <= 0) {
        p = c;
        return 0;
    }
    if (d2 == 0 && point_on_line(d, a, b) <= 0) {
        p = d;
        return 0;
    }
    if (d3 == 0 && point_on_line(a, c, d) <= 0) {
        p = a;
        return 0;
    }
    if (d4 == 0 && point_on_line(b, c, d) <= 0) {
        p = b;
        return 0;
    }
    //如果不相交
    return -1;
}
