//
// Created by Looper on 2023/2/23.
//

#ifndef APP_COMMUNICATION_MAP_MODIFICATION_H
#define APP_COMMUNICATION_MAP_MODIFICATION_H

#include <opencv2/opencv.hpp>

class MapModification {
private:
    void forceModifyMap(const std::vector<std::vector<cv::Point>> &points, int fill);

public:

    void addObstacles(const std::vector<std::vector<cv::Point>> &points);

    void addFeasibleZone(const std::vector<std::vector<cv::Point>> &points);

    void applyIncreaseArea(const std::vector<int>& daubs);

};


#endif //APP_COMMUNICATION_MAP_MODIFICATION_H
