//
// Created by Looper on 2023/4/18.
//

#ifndef APP_COMMUNICATION_MAP_COMPRESS_H
#define APP_COMMUNICATION_MAP_COMPRESS_H

#include <opencv2/opencv.hpp>
#include "vector"

std::vector<int> mat2Vector(cv::Mat mat);

std::vector<int> compressValueQuantity(const std::vector<int> &maps);

std::vector<int> decompressValueQuantity(const std::vector<int> &compress);

cv::Mat vector2Mat(const std::vector<int>& maps, int rows, int cols);

#endif //APP_COMMUNICATION_MAP_COMPRESS_H
