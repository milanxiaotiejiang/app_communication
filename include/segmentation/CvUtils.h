//
// Created by Looper on 2022/9/14.
//

#ifndef APP_COMMUNICATION_CVUTILS_H
#define APP_COMMUNICATION_CVUTILS_H

#include <opencv2/opencv.hpp>

#include "glog/logging.h"

namespace CvUtils {
    double sideInLine(const cv::Point &ps, const cv::Point &pe, const cv::Point &p);

    bool write(const std::string &fileName, const cv::Mat &src);

    cv::Mat read(const std::string &fileName);

    bool savePng(const std::string &fileName, const cv::Mat &src);

    bool savePgm(const std::string &fileName, const cv::Mat &src);
}


#endif //APP_COMMUNICATION_CVUTILS_H
