//
// Created by Looper on 2023/4/18.
//

#include "tool/map_compress.h"

std::vector<int> mat2Vector(cv::Mat mat) {
    std::vector<int> results;
    for (int y = 0; y < mat.rows; y++) {
        for (int x = 0; x < mat.cols; x++) {
            if (mat.at<unsigned char>(y, x) == 0) {
                results.push_back(0);
            } else if (mat.at<unsigned char>(y, x) == 255) {
                results.push_back(-1);
            } else {
                results.push_back(-1);
            }
        }
    }
    return results;
}

std::vector<int> compressValueQuantity(const std::vector<int> &maps) {
    std::vector<int> results;
    int temp;
    int count = 1;
    for (int i = 0; i < maps.size(); ++i) {
        auto data = maps[i];
        if (i == 0) {
            results.push_back(data);
        } else if (i == maps.size() - 1) {
            if (data != temp) {
                results.push_back(count);
                results.push_back(data);
                results.push_back(1);
            } else {
                count++;
                results.push_back(count);
            }
        } else {
            if (data != temp) {
                results.push_back(count);
                results.push_back(data);
                count = 1;
            } else {
                count++;
            }
        }
        temp = data;
    }
    return results;
}

std::vector<int> decompressValueQuantity(const std::vector<int> &compress) {
    std::vector<int> results;
    for (int i = 0; i < compress.size(); i = i + 2) {
        int value = compress[i];
        int quantity = compress[i + 1];
        results.insert(results.end(), quantity, value);
    }
    return results;
}

cv::Mat vector2Mat(const std::vector<int> &maps, int rows, int cols) {
    std::vector<unsigned char> vec;
    for (int map: maps) {
        if (map == 0) {
            vec.push_back(0);
        } else {
            vec.push_back(255);
        }
    }

    cv::Mat mat(rows, cols, CV_8UC1);
    unsigned char *matData = mat.ptr<unsigned char>();

    std::copy(vec.begin(), vec.end(), matData);

    return mat;
}
