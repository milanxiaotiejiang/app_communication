//
// Created by Looper on 2023/5/11.
//

#include "exploration/cv_extend.h"
#include "segmentation/SegmentationCenter.h"

void testErode() {
    int grid_spacing_in_pixel = 10;
    // 腐蚀测试
    auto map0 = SegmentationCenter::instance().generateMat();
    cv::imshow("m0 ", map0);
    cv::waitKey();

    auto map1 = SegmentationCenter::instance().generateMat();
    cv::erode(map1, map1, cv::getStructuringElement(cv::MORPH_RECT,
                                                    cv::Size(grid_spacing_in_pixel,
                                                             grid_spacing_in_pixel),
                                                    cv::Point(-1, -1)), cv::Point(-1, -1), 1);
    cv::imshow("m1 ", map1);
    cv::waitKey();

    auto map2 = SegmentationCenter::instance().generateMat();
    cv::erode(map2, map2, cv::getStructuringElement(cv::MORPH_CROSS,
                                                    cv::Size(grid_spacing_in_pixel,
                                                             grid_spacing_in_pixel),
                                                    cv::Point(-1, -1)), cv::Point(-1, -1), 1);
    cv::imshow("m2 ", map2);
    cv::waitKey();

    auto map3 = SegmentationCenter::instance().generateMat();
    explorationErode(map3, map3, grid_spacing_in_pixel);
    cv::imshow("m3 ", map3);
    cv::waitKey();

    auto map4 = SegmentationCenter::instance().generateMat();
    cv::erode(map4, map4, cv::Mat(), cv::Point(-1, -1), grid_spacing_in_pixel);
    cv::imshow("m4 ", map4);
    cv::waitKey();
}

void explorationErode(const cv::Mat &src, cv::Mat &dst, int pixel) {
    if (pixel <= 0) {
        cv::erode(src, dst, cv::Mat(), cv::Point(-1, -1), pixel);
    } else {
        auto kernel = cv::getStructuringElement(cv::MORPH_CROSS,
                                                cv::Size(pixel, pixel),
                                                cv::Point(-1, -1));
        cv::erode(src, dst, kernel, cv::Point(-1, -1), 1);

        int half_pixel = (int) std::floor(0.5 * pixel);
        cv::erode(dst, dst, cv::Mat(), cv::Point(-1, -1), half_pixel);
        cv::dilate(dst, dst, cv::Mat(), cv::Point(-1, -1), half_pixel);
    }
}

void drawBaseStation(cv::Mat &img, const cv::Point &stationPoint, int radius, const cv::Scalar &color) {
    cv::rectangle(img, cv::Point(stationPoint.x - radius, stationPoint.y - radius),
                  cv::Point(stationPoint.x + radius, stationPoint.y + radius),
                  color, CV_FILLED);
}
