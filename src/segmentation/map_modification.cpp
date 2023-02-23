//
// Created by Looper on 2023/2/23.
//

#include "segmentation/map_modification.h"

#include "db/segmentation_data_base.h"

void MapModification::forceModifyMap(const std::vector<std::vector<cv::Point>> &points, int fill) {
    auto dbMap = SegmentationDataBase::instance().getDbMap();
    std::string image_filename = dbMap.path + dbMap.name;//"sim_mymap.pgm";

    cv::Mat unchanged = cv::imread(image_filename.c_str(), cv::ImreadModes::IMREAD_UNCHANGED);

    for (const auto &v: points) {
        std::vector<std::vector<cv::Point>> polygon_array;
        polygon_array.push_back(v);
        cv::fillPoly(unchanged, polygon_array, cv::Scalar(fill));
    }

    cv::imwrite(image_filename, unchanged);
}

void MapModification::addObstacles(const std::vector<std::vector<cv::Point>> &points) {
    forceModifyMap(points, 128);
}

void MapModification::addFeasibleZone(const std::vector<std::vector<cv::Point>> &points) {
    forceModifyMap(points, 255);
}