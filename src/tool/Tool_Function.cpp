//
// Created by lijiang on 2021/12/24.
//


#include "tool/Tool_Function.h"


bool is_in_line(const TaskPoint point1, const TaskPoint point2, const TaskPoint point3) {

    if ((abs(point1.getX() - point2.getX()) < 0.05 && abs(point1.getX() - point3.getX()) < 0.05)//x方向成直线
        || (abs(point1.getY() - point2.getY()) < 0.05 && abs(point1.getY() - point3.getY()) < 0.05))//y方向呈直线
    {
        return true;
    } else {
        return false;
    }
}

bool is_in_line(const WayPoint point1, const WayPoint point2, const WayPoint point3) {

    if (abs(point2.getOriW() - point1.getOriW()) > 0.1) {
        return false;
    }


    float x1, y1, x2, y2, x, y;
    x1 = point1.getPosX();
    y1 = point1.getPosY();
    x2 = point2.getPosX();
    y2 = point2.getPosY();
    x = point3.getPosX();
    y = point3.getPosY();
    float judge_num = (y1 - y2) * (x - x2) -(x1 - x2) * (y - y2);
//    cout<<"judge_num: "<<judge_num<<endl;
    if (abs(judge_num)<2*1e-3)
    {
        return true;
    }
    else
        return false;

}

bool is_in_line(const geometry_msgs::Pose point1, const geometry_msgs::Pose point2, const geometry_msgs::Pose point3) {

    if (abs(point2.orientation.w - point1.orientation.w) > 0.1) {
        return false;
    }


    float x1, y1, x2, y2, x, y;
    x1 = point1.position.x;
    y1 = point1.position.y;
    x2 = point2.position.x;
    y2 = point2.position.y;
    x = point3.position.x;
    y = point3.position.y;
    float judge_num = (y1 - y2) * (x - x2) -(x1 - x2) * (y - y2);
    // cout<<"judge_num: "<<judge_num<<endl;
    if (abs(judge_num)<5*1e-3)
    {
        return true;
    }
    else
        return false;

}
bool is_in_line(const Point point1, const Point point2, const Point point3) {

    float x1, y1, x2, y2, x, y;
    x1 = point1.getX();
    y1 = point1.getY();
    x2 = point2.getX();
    y2 = point2.getY();
    x = point3.getX();
    y = point3.getY();
    float judge_num = (y1 - y2) * (x - x2) -(x1 - x2) * (y - y2);
    // cout<<"judge_num: "<<judge_num<<endl;
    if (abs(judge_num)<5*1e-4)
    {
        return true;
    }
    else
        return false;

}
bool is_in_line(const vector<Point> &p){
//    if(p.size() != 5)
//        return false;
    double ave_x = 0, ave_y = 0;
    for (int i = 0; i < p.size(); ++i) {
        ave_x += p[i].getX();
        ave_y += p[i].getY();
    }
    ave_x /= p.size();
    ave_y /= p.size();
    double r1 = 0, r2 = 0, r3 = 0;
    for (int i = 0; i < p.size(); ++i) {
        r1 += (p[i].getX() - ave_x) * (p[i].getY() - ave_y);
        r2 += (p[i].getX() - ave_x) * (p[i].getX() - ave_x);
        r3 += (p[i].getY() - ave_y) * (p[i].getY() - ave_y);
    }
    double r = r1/(sqrt(r2) * sqrt(r3));
    return (abs(r) - 0.97) > 0;
}
