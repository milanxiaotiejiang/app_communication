//
// Created by Looper on 2022/9/13.
//

#ifndef APP_COMMUNICATION_CONTAINS_H
#define APP_COMMUNICATION_CONTAINS_H

#include <vector>
#include <opencv2/opencv.hpp>

bool contains(std::vector<cv::Scalar> vector, const cv::Scalar &element);

bool contains(std::vector<cv::Point> vector, const cv::Point &element);

bool contains(std::vector<int> vector, int element);

bool contains(std::vector<std::vector<unsigned int> > vector, const std::vector<unsigned int> &element);

#endif //APP_COMMUNICATION_CONTAINS_H
