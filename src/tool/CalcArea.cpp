//
// Created by lijiang on 2022/1/12.
//

#include "tool/CalcArea.h"

#include "model/Task.h"
#include "prohibition.h"
#include "ros/package.h"
#include "tool/write_file.hpp"
#include "yaml-cpp/yaml.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

CalcAreaClass::CalcAreaClass(/* args */) {
    map_path.append(ros::package::getPath("robot_slam"));
    map_path.append("/maps/mymap.pgm");
    // map_path.append("/home/admin1/test_ws/src/robot_slam/maps/mymap.pgm");
    yaml_path.append(ros::package::getPath("robot_slam"));
    yaml_path.append("/maps/mymap.pgm");
    // yaml_path.append("/home/admin1/test_ws/src/robot_slam/maps/mymap.yaml");

    sum_area = 0;
    can_clean_area = 0;
    actual_clean_area = 0;

    CalcAllArea();
}

bool CalcAreaClass::CalcAllArea() {
    load_map();
    load_map_yaml(yaml_path);
    calcTotalArea();
    calProhibitionArea();
    calCleanAvailableArea();
    CalcAvailableRate();
}

CalcAreaClass::~CalcAreaClass() {}

bool CalcAreaClass::load_map() {
    map_img = imread(map_path, 0);
    if (map_img.empty()) {
        ROS_ERROR("Can not load map!");
        return false;
    }
    //    cv::imshow("Map", map_img);
    return true;
}

bool CalcAreaClass::load_map_yaml(
        std::string path_to_yaml)//载入map.yaml中的resolution和origin point
{
    YAML::Node config = YAML::LoadFile(path_to_yaml);
    reslution = atof(config["resolution"].as<std::string>().c_str());
    for (int i = 0; i < config["origin"].size(); i++) {
        map_origin[i] = atof(config["origin"][i].as<std::string>().c_str());
    }
}

float CalcAreaClass::calcTotalArea() {// ok
    cv::normalize(map_img, map_img, 0, 255,
                  cv::NORM_MINMAX);// 0,1,255变为0，1/255，1
    map_img.convertTo(map_img, CV_8U);
    srcImg = map_img.clone();

    Mat outMat = srcImg;

    int total_count = 0;
    float total_area = 0;
    int width = srcImg.cols;
    int height = srcImg.rows;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (srcImg.at<uchar>(cv::Point(i, j)) == 0 ||
                srcImg.at<uchar>(cv::Point(i, j)) == 255) {
                total_count += 1;
            }
        }
    }
    total_area = total_count * 0.05 * 0.05;
    //    std::cout << "total_count: " << total_count << std::endl;
    std::cout << "total_area: " << total_area << " M2S" << std::endl;
    sum_area = total_area;
    std::cout << "sum_area: " << sum_area << " M2S" << std::endl;
    return total_area;
}

float CalcAreaClass::calProhibitionArea()//计算禁区面积，自己写
{
    Mat outMat = srcImg;

    vector<vector<float>> prohibition_info;
    std::vector<std::vector<geometry_msgs::Point>> prohibition_rec;
    int width = outMat.cols;
    int height = outMat.rows;

    if (!get_prohibition(prohibition_info)) {
        ROS_ERROR("Fail to open prohibition.yaml!");
    }

    int sum_define_wash = 0;

    for (int i = 0; i < prohibition_info.size(); i++) {
        if (prohibition_info[i].size() == 9) {//区域
            int min_x = outMat.cols + 1;
            int min_y = outMat.rows + 1;
            int max_x = 0;
            int max_y = 0;
            for (int j = 0; j < 4; j++) {
                int point_x_img =
                        int((prohibition_info[i][j * 2 + 1] - map_origin[0]) / 0.05);
                int point_y_img =
                        width -
                        int((prohibition_info[i][j * 2 + 2] - map_origin[1]) / 0.05);
                if (point_x_img < min_x)
                    min_x = point_x_img;
                if (point_x_img > max_x)
                    max_x = point_x_img;
                if (point_y_img < min_y)
                    min_y = point_y_img;
                if (point_y_img > max_y)
                    max_y = point_y_img;
            }
            for (int i = min_x; i <= max_x; i++) {
                for (int j = min_y; j <= max_y; j++) {
                    if (outMat.at<uchar>(cv::Point(i, j)) == 255) {
                        outMat.at<uchar>(cv::Point(i, j)) = 0;
                    }
                }
            }
        }

        // cv::imshow("result", outMat);
        //        cv::waitKey(0);
    }
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (outMat.at<uchar>(cv::Point(i, j)) == 0) {
                sum_define_wash += 1;
            }
        }
    }
    //    std::cout << "sum_define_wash: " << sum_define_wash << std::endl;
    can_clean_area = sum_area - sum_define_wash * 0.05 * 0.05;
    std::cout << "can_clean_area: " << can_clean_area << " M2S" << std::endl;
    return sum_define_wash;
}

float CalcAreaClass::calCleanAvailableArea()//计算可清洁面积ok
{
    float CleanAvailable = 0;
    float recArea = CalcRecFlieArea();
    CleanAvailable += recArea;
    std::cout << "RecFileArea: " << recArea << " M2S" << std::endl;
    float polArea = CalcPolygonFlieArea();
    CleanAvailable += polArea;
    std::cout << "PolFileArea: " << polArea << " M2S" << std::endl;
    float teachArea = ClacTeachFileArea();
    CleanAvailable += teachArea;
    std::cout << "TeachFileArea: " << teachArea << " M2S" << std::endl;
    actual_clean_area = CleanAvailable;
    std::cout << "CleanAvailableArea: " << CleanAvailable << " M2S" << std::endl;
    return CleanAvailable;
}

float CalcAreaClass::CalcAvailableRate() {
    float AvailableRate;
    if (sum_area != 0)//如果总面积存在
    {
        AvailableRate = actual_clean_area * 100 / can_clean_area;
    }
    // std::cout<<"actual_clean_area"<<int(sum_area)<<std::endl;
    // std::cout<<"sum_area"<<int(actual_clean_area)<<std::endl;
    std::cout << "实际清洁面积" << actual_clean_area << std::endl;
    std::cout << "总可清洁面积" << can_clean_area << std::endl;
    std::cout << "覆盖率" << AvailableRate << "%" << std::endl;
    return AvailableRate;
}

const float &CalcAreaClass::getActualCleanArea() { return actual_clean_area; }

float CalcAreaClass::CalcSingleRecArea(vector<float> point_list) {//计算单个矩形面积
    if (point_list.size() != 8) {
        std::cout << "在计算单个矩形面积中输入的point_list不等于8" << endl;
        return 0;
    }
    float len = hypot(point_list[0] - point_list[2], point_list[1] - point_list[3]);
    float wid = hypot(point_list[0] - point_list[6], point_list[1] - point_list[7]);
    return len * wid;
}

float CalcAreaClass::CalcAllRecArea(vector<Continuity_info> Continuity_info_list) {//计算全部矩形面积
    float AllRecArea = 0;
    for (auto &item: Continuity_info_list) {
        if (item.getPoints().size() != 8) {
            std::cout << "在计算单个矩形面积中输入的point_list不等于8" << endl;
            return 0;
        }
    }
    for (auto &item: Continuity_info_list) {
        AllRecArea += CalcSingleRecArea(item.getPoints());
    }
    return AllRecArea;
}

float CalcAreaClass::CalcRecFlieArea() {//计算矩形文件的面积
    float RecArea = 0;
    string fileName;
    map_path.append(ros::package::getPath("data_base"));
    map_path.append("/config/clean_zone.txt");
    // fileName.append("/home/admin1/test_ws/src/data_base/config/clean_zone.txt");
    //sh::File *pFile = new sh::File(fileName);
    std::shared_ptr<sh::File> pFile = make_shared<sh::File>(fileName);
    if (pFile->open(std::ios::in)) {
        string fileString = pFile->readAll();
        if (fileString.length() > 0) {
            json basicJson = json::parse(fileString);
            Task realTaskList =
                    basicJson.get<Task>();//数据内容，结构体格式,Task的vector类
            vector<Continuity_info> Continuity_info_list = realTaskList.getContinuity().getContinuityList();
            RecArea = CalcAllRecArea(Continuity_info_list);
        }
    } else {
        std::cout << "矩形文件不存在" << endl;
    }
    return RecArea;
}

float CalcAreaClass::CalcPolygonFlieArea() {
    float PolygonArea = 0;
    Mat outMat = srcImg.clone();
    int width = outMat.cols;
    int height = outMat.rows;
    string fileName;
    map_path.append(ros::package::getPath("data_base"));
    map_path.append("/config/clean_zone.txt");
    // fileName.append("/home/admin1/test_ws/src/data_base/config/polygon_clean_zone.txt");
    //sh::File *pFile2 = new sh::File(fileName);
    std::shared_ptr<sh::File> pFile2 = make_shared<sh::File>(fileName);
    if (pFile2->open(std::ios::in)) {
        string fileString = pFile2->readAll();
        if (fileString.length() > 0) {
            json basicJson = json::parse(fileString);
            Task realTaskList = basicJson.get<Task>();//数据内容，结构体格式,Task的vector类

            vector<Polygon_info> po_list =
                    realTaskList.getPolygon().getPolygonList();//多边形队列
            for (auto &item: po_list) {
                int col = item.getPoints().size() / 2;
                cv::Point **root_points = new cv::Point *[1];
                for (int i = 0; i < 1; i++) {
                    root_points[i] = new cv::Point[col];
                }
                for (int k = 0; k < col; k++) {
                    geometry_msgs::Point temp_point;
                    temp_point.x = item.getPoints()[k * 2];
                    temp_point.y = item.getPoints()[k * 2 + 1];
                    root_points[0][k] =
                            cv::Point(int((temp_point.x - map_origin[0]) / 0.05),
                                      height - int((temp_point.y - map_origin[1]) / 0.05));
                    //                root_points[0][k] = cv::Point(int(temp_point.x),
                    //                height-int(temp_point.y));
                }

                const cv::Point *ppt[1] = {root_points[0]};
                int npt[] = {col};
                polylines(outMat, ppt, npt, 1, 1, cv::Scalar(0, 255, 255), 1, 8, 0);

                Mat mask_ann = Mat(outMat.rows, outMat.cols, CV_8UC1);
                fillPoly(mask_ann, ppt, npt, 1, cv::Scalar(255, 255, 255));
                double sum = 0;
                for (int i = 0; i < mask_ann.cols; i++)
                    for (int j = 0; j < mask_ann.rows; j++) {
                        if (mask_ann.at<uchar>(i, j) == 255) {
                            sum = sum + 1;
                        }
                    }
                float sum_wash = sum * 0.05 * 0.05;
                PolygonArea += sum_wash;
                //            imshow("Test1", mask_ann);
            }
        } else {
            std::cout << "多边形文件为空" << endl;
        }
    } else {
        std::cout << "多边形文件打开失败" << endl;
    }
    return PolygonArea;
}

float CalcAreaClass::ClacTeachFileArea() {
    float TeachArea = 0;
    string fileName;
    map_path.append(ros::package::getPath("data_base"));
    map_path.append("/config/teach_point_json.txt");
    std::shared_ptr<sh::File> pFile2 = make_shared<sh::File>(fileName);
    if (pFile2->open(std::ios::in)) {
        string fileString = pFile2->readAll();
        if (fileString.length() > 0) {
            json basicJson = json::parse(fileString);
            std::vector<WayPointTask> WayPointTaskList =
                    basicJson.get<std::vector<WayPointTask>>();//得到了路径点的队列
            for (auto &item: WayPointTaskList) {
                std::vector<WayPoint> WayPointList = item.getWayPointList();
                TeachArea += CalcTeachArea(WayPointList);
            }
        } else {
            std::cout << "示教文件为空" << endl;
        }
    } else {
        std::cout << "示教文件打开失败" << endl;
    }
    return TeachArea;
}

float CalcAreaClass::CalcTeachArea(std::vector<WayPoint> way_point_list) {
    float TeachArea = 0;
    std::vector<cv::Point2f>
            CV_point_list;
    if (!way_point_list.size()) {
        std::cout << "路径点不存在" << endl;
        return 0;
    }
    for (auto &item: way_point_list) {//将示教文件中的点列转换为cv可用的点列
        CV_point_list.push_back(cv::Point2f(item.getPosX(), item.getPosY()));
    }
    double length = cv::arcLength(CV_point_list, false);
    std::cout << "计算完成,路径长度为" << length * 0.05 << endl;
    TeachArea = length * 0.05 * 0.4;
    return TeachArea;
}