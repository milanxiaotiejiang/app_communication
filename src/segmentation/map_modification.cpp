//
// Created by Looper on 2023/2/23.
//

#include "segmentation/map_modification.h"

#include "db/segmentation_data_base.h"
#include "segmentation/SegmentationCenter.h"
#include "tool/map_compress.h"

void MapModification::forceModifyMap(const std::vector<std::vector<cv::Point>> &points, int fill) {
    auto dbMap = SegmentationDataBase::instance().getDbMap();

    cv::Mat unchanged = cv::imread(path::map_pgm_path().c_str(), cv::ImreadModes::IMREAD_UNCHANGED);
    cv::rotate(unchanged, unchanged, cv::RotateFlags::ROTATE_90_COUNTERCLOCKWISE);

    for (const auto &v: points) {
        std::vector<std::vector<cv::Point>> polygon_array;
        polygon_array.push_back(v);
        cv::fillPoly(unchanged, polygon_array, cv::Scalar(fill));
    }

    cv::rotate(unchanged, unchanged, cv::RotateFlags::ROTATE_90_CLOCKWISE);

    cv::imwrite(path::map_pgm_path(), unchanged);
}

void MapModification::addObstacles(const std::vector<std::vector<cv::Point>> &points) {
    forceModifyMap(points, 128);
}

void MapModification::addFeasibleZone(const std::vector<std::vector<cv::Point>> &points) {
    forceModifyMap(points, 255);
}

void MapModification::applyIncreaseArea(const std::vector<int> &daubs) {
    const cv::Mat &map = SegmentationCenter::instance().generateMat();
    //模拟测试
//    cv::Mat room_mat = cv::Mat::zeros(map.rows, map.cols, CV_8UC1);
//    cv::circle(room_mat, cv::Point(map.cols / 2, map.rows / 3), 100, 255, CV_FILLED);
//    cv::circle(room_mat, cv::Point(map.cols / 2, map.rows / 5 * 4), 50, 255, CV_FILLED);
//    std::vector<int> mapDataList = mat2Vector(room_mat);
//    std::vector<int> imitate = compressValueQuantity(mapDataList);

    std::vector<int> maps = decompressValueQuantity(daubs);
    const cv::Mat &daubMap = vector2Mat(maps, map.rows, map.cols);

    auto originalMap = map.clone();
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(originalMap, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    int max_size = 0;
    int max_position = 0;
    for (int i = 0; i < contours.size(); i++) {
        auto contour = contours[i];
        if (contour.size() > max_size) {
            max_size = contour.size();
            max_position = i;
        }
    }
    auto mostContour = contours[max_position];
    cv::drawContours(originalMap, std::vector<std::vector<cv::Point> >(1, mostContour), -1, cv::Scalar(255), CV_FILLED);

    cv::Mat andMat;
    cv::bitwise_and(daubMap, originalMap, andMat);
    cv::bitwise_or(map, andMat, map);

    cv::Mat unchanged = cv::imread(path::map_pgm_path().c_str(), cv::ImreadModes::IMREAD_UNCHANGED);

    cv::rotate(unchanged, unchanged, cv::RotateFlags::ROTATE_90_COUNTERCLOCKWISE);

    for (int y = 0; y < map.rows; y++) {
        for (int x = 0; x < map.cols; x++) {
            if (map.at<unsigned char>(y, x) == 255) {
                cv::circle(unchanged, cv::Point(x, y), 1, cv::Scalar(255), CV_FILLED);
            }
        }
    }

    cv::rotate(unchanged, unchanged, cv::RotateFlags::ROTATE_90_CLOCKWISE);

    cv::imwrite(path::map_pgm_path(), unchanged);
}
