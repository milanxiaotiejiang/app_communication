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
    bool hasGate;

    std::vector<Gate> gateList;
    cv::Mat gate_open_map;

    cv::Mat segmented_map;// 255*256 = 65280
    std::vector<Room> rooms;

    std::map<std::pair<int, int>, std::pair<Gate, bool>> planMap;

    AStarPlanner path_planner;

public:
    GateComprehensive(const std::vector<Gate> &gateList) : gateList(gateList) {
        hasGate = !gateList.empty();

        auto generateMat = SegmentationCenter::instance().generateMat();
        gate_open_map = generateMat.clone();
        segmented_map = generateMat.clone();

        int start_time = ros::Time::now().sec;

        for (const auto &gate: gateList) {
            SegmentationCenter::instance().gateManyOpen(gate_open_map, gate);
            SegmentationCenter::instance().gateManySegmentation(segmented_map, rooms, planMap, gate);
        }

        int end_time = ros::Time::now().sec;
        std::cout << "cost handSegmentation : " << end_time - start_time << " s " << std::endl;

        whole_display(segmented_map, rooms, gateList, planMap, 2, "GateComprehensive");

        const cv::Point2i &point = cv::Point(150, 320);
        const Point &rosPoint = MapAttributeSingleton::instance().mapPoint2RosPoint(gate_open_map.rows,
                                                                                    gate_open_map.cols, point);
        RealPoint realPoint;
        realPoint.realPosition.x = rosPoint.getX();
        realPoint.realPosition.y = rosPoint.getY();
        AStarPlannerPoint(realPoint);
    }

    void AStarPlannerPoint(const RealPoint &realPoint) {
        cv::Point robotPosition = MapAttributeSingleton::instance().getRobotPositionPoint(gate_open_map);
        robotPosition.x = robotPosition.x + 50;
        robotPosition.y = robotPosition.y - 100;
        auto cvDestPoint = MapAttributeSingleton::instance()
                .rosPoint2MapPoint(gate_open_map.rows, gate_open_map.cols,
                                   Point(realPoint.realPosition.x, realPoint.realPosition.y)
                );

        std::vector<cv::Point> current_path;
        double length = path_planner.planPath(gate_open_map, gate_open_map, robotPosition,
                                              cvDestPoint, 1.0, 0.,
                                              map_resolution_from_subscription, 0, nullptr, &current_path);
        if (length > 1e90) {
            return;
        }

        std::vector<int> stacks;
        for (const auto &point: current_path) {
            int value = atValue(point);
            if (value > 0) {
                if (stacks.empty()) {
                    stacks.push_back(value);
                } else {
                    int last = stacks[stacks.size() - 1];
                    if (value != last) {
                        stacks.push_back(value);
                    }
                }
            }
        }

//        auto showMap = gate_open_map.clone();
//        cv::circle(showMap, robotPosition, 4, cv::Scalar(200), CV_FILLED);
//        cv::circle(showMap, cvDestPoint, 4, cv::Scalar(200), CV_FILLED);

        for (const auto &item: stacks) {
            LOG(ERROR) << item;
        }

        LOG(ERROR) << length;
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

    int atValue(cv::Point point) const {
        return segmented_map.at<int>(point);
    }

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
