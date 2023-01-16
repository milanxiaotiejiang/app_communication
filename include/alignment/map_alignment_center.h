//
// Created by Looper on 2022/10/24.
//

#ifndef APP_COMMUNICATION_MAP_ALIGNMENT_CENTER_H
#define APP_COMMUNICATION_MAP_ALIGNMENT_CENTER_H

#include <ros/ros.h>
#include <opencv2/opencv.hpp>

class AlignmentCenter {
private:
    bool initialize_finish = false;
public:
    static auto &instance() {
        static AlignmentCenter obj;
        return obj;
    }

    void initialize(ros::NodeHandle handle);

    void merge_lines(const cv::Mat& mat);
};


#endif //APP_COMMUNICATION_MAP_ALIGNMENT_CENTER_H
