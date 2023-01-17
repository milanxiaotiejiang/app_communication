//
// Created by Looper on 2022/10/8.
//

#ifndef APP_COMMUNICATION_A_STAR_PATHPLANNER_H
#define APP_COMMUNICATION_A_STAR_PATHPLANNER_H

#include <iostream>
#include <iomanip>
#include <queue>
#include <string>
#include <math.h>
#include <ctime>
#include <cstdlib>
#include <stdio.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <exploration//node.h>

class AStarPlanner {
protected:
    int n;
    int m;

    std::string route_;

    std::string
    pathFind(const int &xStart, const int &yStart, const int &xFinish, const int &yFinish, const cv::Mat &map);

public:
    AStarPlanner();

    void drawRoute(cv::Mat &map, const cv::Point start_point, const std::string &route, double step_length);

    void getRoute(const cv::Point start_point, const std::string &route, double step_length,
                  std::vector<cv::Point> &route_points);

    double planPath(const cv::Mat &map, const cv::Point &start_point, const cv::Point &end_point,
                    const double downsampling_factor, const double robot_radius, const double map_resolution,
                    const int end_point_valid_neighborhood_radius = 0, std::vector<cv::Point> *route = NULL);

    double planPath(const cv::Mat &map, const cv::Mat &downsampled_map, const cv::Point &start_point,
                    const cv::Point &end_point, const double downsampling_factor,
                    const double robot_radius, const double map_resolution,
                    const int end_point_valid_neighborhood_radius = 0, cv::Mat *draw_path_map = NULL,
                    std::vector<cv::Point> *route = NULL);

    void downsampleMap(const cv::Mat &map, cv::Mat &downsampled_map, const double downsampling_factor,
                       const double robot_radius, const double map_resolution);
};


#endif //APP_COMMUNICATION_A_STAR_PATHPLANNER_H
