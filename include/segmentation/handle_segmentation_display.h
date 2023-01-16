//
// Created by Looper on 2022/9/15.
//

#ifndef APP_COMMUNICATION_HANDLE_SEGMENTATION_DISPLAY_H
#define APP_COMMUNICATION_HANDLE_SEGMENTATION_DISPLAY_H

#include "Room.h"
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "map"
#include "segmentation/meanshift2d.h"
#include "segmentation/Room.h"

void
calculation_center_point(cv::Mat &segmented_map, std::vector<Room> &rooms, double robot_radius, double map_resolution);

void single_display(const cv::Mat &roomMat, Room &room, const std::string &winname);

void ordinary_display(const cv::Mat &segmented_map, const std::vector<Room> &rooms, const std::string &winname);

void whole_display(const cv::Mat &segmented_map, const std::vector<Room> &rooms, const std::string &winname);

#endif //APP_COMMUNICATION_HANDLE_SEGMENTATION_DISPLAY_H
