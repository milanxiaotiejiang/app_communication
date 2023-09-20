//
// Created by io on 23-9-20.
//

#ifndef APP_COMMUNICATION_GATECOMPREHENSIVE_H
#define APP_COMMUNICATION_GATECOMPREHENSIVE_H

#include <opencv2/opencv.hpp>
#include "db/segmentation_model.h"
#include "task/RealBlock.h"
#include "segmentation/Room.h"
#include "segmentation/SegmentationCenter.h"
#include "segmentation/map_attribute.h"
#include "segmentation/handle_segmentation_display.h"

class GateSettingCenter {
private:
    GateSettingCenter() = default;

    GateSettingCenter(GateSettingCenter &) = delete;

    GateSettingCenter &operator=(const GateSettingCenter &) = delete;

    std::atomic<bool> gateSettingMode;

public:
    ~GateSettingCenter() = default;

    static auto &instance() {
        static GateSettingCenter obj;
        return obj;
    }

    void initialize(const ros::NodeHandle &handle);

    bool isGateSettingMode();

    void startInspect();

    void stopInspect();
};

class GateComprehensive {
private:
    std::vector<Gate> gateList;
    cv::Mat segmented_map;// 255*256 = 65280
    std::vector<Room> rooms;

public:
    GateComprehensive(const std::vector<Gate> &gateList) : gateList(gateList) {
        auto generateMat = SegmentationCenter::instance().generateMat();
        segmented_map = generateMat.clone();

        int start_time = ros::Time::now().sec;

        for (const auto &gate: gateList) {
            SegmentationCenter::instance().gateManySegmentation(segmented_map, rooms, gate);
        }

        int end_time = ros::Time::now().sec;
        std::cout << "cost handSegmentation : " << end_time - start_time << " s " << std::endl;

    }

//    cv::Point leftPoint() const {
//        return MapAttributeSingleton::instance().rosPoint2MapPoint(
//                segmented_map.rows, segmented_map.cols,
//                Point(gate.left_position_x, gate.left_position_y)
//        );
//    }
//
//    cv::Point rightPoint() const {
//        return MapAttributeSingleton::instance().rosPoint2MapPoint(
//                segmented_map.rows, segmented_map.cols,
//                Point(gate.right_position_x, gate.right_position_y)
//        );
//    }
//
//    int atValue(cv::Point point) const {
//        return segmented_map.at<int>(point);
//    }
//
//    int leftValue() const {
//        return atValue(leftPoint());
//    }
//
//    int rightValue() const {
//        return atValue(rightPoint());
//    }
//
//    void leftBlock(RealBlock &realBlock) {
//        RealPoint realPoint;
//        RealPosition realPosition(gate.left_position_x, gate.left_position_y, gate.left_position_z);
//        RealOrientation realOrientation(gate.left_orientation_x, gate.left_orientation_y,
//                                        gate.left_orientation_z, gate.left_orientation_w);
//        realPoint.realPosition = std::move(realPosition);
//        realPoint.realOrientation = std::move(realOrientation);
//        realBlock.plannerPoints.push_back(realPoint);
//    }
//
//    void rightBlock(RealBlock &realBlock) {
//        RealPoint realPoint;
//        RealPosition realPosition(gate.right_position_x, gate.right_position_y, gate.right_position_z);
//        RealOrientation realOrientation(gate.right_orientation_x, gate.right_orientation_y,
//                                        gate.right_orientation_z, gate.right_orientation_w);
//        realPoint.realPosition = std::move(realPosition);
//        realPoint.realOrientation = std::move(realOrientation);
//        realBlock.plannerPoints.push_back(realPoint);
//    }
//
//    int pointValue(const Point &point) {//ros
//        auto mapPoint = MapAttributeSingleton::instance().rosPoint2MapPoint(
//                segmented_map.rows, segmented_map.cols, point);
//        return atValue(mapPoint);
//    }
//
//    int lastValue(const RealBlock &block) {
//        return pointValue(Point(block.lastPoint.realPosition.x, block.lastPoint.realPosition.y));
//    }
//
//    int firstValue(const RealBlock &block) {
//        return pointValue(Point(block.firstPoint.realPosition.x, block.firstPoint.realPosition.y));
//    }

};

#endif //APP_COMMUNICATION_GATECOMPREHENSIVE_H
