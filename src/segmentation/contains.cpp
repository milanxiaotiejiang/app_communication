//
// Created by Looper on 2022/9/13.
//

#include "segmentation/contains.h"

bool contains(std::vector<cv::Scalar> vector, const cv::Scalar &element) {
    if (!vector.empty()) {
        return vector.end() != std::find(vector.begin(), vector.end(), element);
    } else {
        return false;
    }
}

bool contains(std::vector<cv::Point> vector, const cv::Point &element) {
    if (!vector.empty()) {
        return vector.end() != std::find(vector.begin(), vector.end(), element);
    } else {
        return false;
    }
}

bool contains(std::vector<int> vector, int element) {
    if (!vector.empty()) {
        return vector.end() != std::find(vector.begin(), vector.end(), element);
    } else {
        return false;
    }
}

bool contains(std::vector<std::vector<uint> > vector, const std::vector<uint> &element) {
    if (!vector.empty()) {
        return vector.end() != std::find(vector.begin(), vector.end(), element);
    } else {
        return false;
    }
}
