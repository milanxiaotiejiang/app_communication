//
// Created by lijiang on 2022/1/12.
//

#ifndef APP_COMMUNICATION_CALCAREA_H
#define APP_COMMUNICATION_CALCAREA_H

#include "model/Continuity_info.h"

#include "model/TeachModePoint.h"
#include "string"
#include <XmlRpcValue.h>
#include <geometry_msgs/Point.h>
#include <opencv2/opencv.hpp>
#include <ros/ros.h>

using namespace std;
using cv::imread;
using cv::Mat;

class CalcAreaClass {
private:
    Mat map_img;
    Mat srcImg;

    string map_path;
    string yaml_path;
    std::vector<std::vector<geometry_msgs::Point>> prohibition_polygons;

    float map_origin[3];
    float reslution;

    float sum_area;
    float can_clean_area;
    float actual_clean_area;

public:
    CalcAreaClass(/* args */);
    ~CalcAreaClass();
    bool load_map();
    bool load_map_yaml(std::string path_to_yaml);
    bool getPoint(XmlRpc::XmlRpcValue &val, geometry_msgs::Point &point);
    float calProhibitionArea();
    Mat deal_map_img();
    float calcTotalArea();
    float calc_can_wash_area();
    float calCleanAvailableArea();
    bool CalcAllArea();
    float CalcAvailableRate();
    const float &getActualCleanArea();

    float CalcSingleRecArea(vector<float> point_list);                 //计算单个矩形的面积
    float CalcAllRecArea(vector<Continuity_info> Continuity_info_list);//计算矩形列表中所有矩形的面积
    float CalcRecFlieArea();                                           //计算矩形文件中所有矩形的面积

    float CalcPolygonFlieArea();//计算多边形文件中所有多边形的面积
    float CalcPolygonArea();    //计算单个多边形面积

    float ClacTeachFileArea();
    float CalcTeachArea(std::vector<WayPoint> way_point_list);

private:
    static CalcAreaClass *m_instance_ptr;

public:
    static CalcAreaClass *get_instance() {
        if (m_instance_ptr == nullptr) {
            m_instance_ptr = new CalcAreaClass;
        }
        return m_instance_ptr;
    }
};

#endif// APP_COMMUNICATION_CALCAREA_H
