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

//        whole_display(segmented_map, rooms, gateList, planMap, 2, "GateComprehensive");
    }

    bool isHasGate() {
        return hasGate;
    }

    void AStarPlannerPoint(const RealPoint &realPoint, std::vector<int> &stacks) {
        cv::Point robotPosition = MapAttributeSingleton::instance().getRobotPositionPoint(gate_open_map);
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
    }

    void generateGatePointList(const std::vector<int> &stacks, RealPoint destPoint, std::vector<RealPoint> &points) {
        if (stacks.size() <= 1) {
            throw std::runtime_error("stacks size must > 1");
        }
        bool hasError = false;
        for (int i = 0; i < stacks.size() - 1; i++) {
            int start = stacks[i];
            int end = stacks[i + 1];
            auto pair = std::make_pair(start, end);
            auto iter = planMap.find(pair);
            if (iter != planMap.end()) {
                auto gatePlan = iter->second;
                Gate &gate = gatePlan.first;
                bool direction = gatePlan.second;

                if (direction) {
                    points.push_back(leftPoint(gate, false, false));
                    points.push_back(leftPoint(gate, true, false));
                    points.push_back(rightPoint(gate, true, true));
                } else {
                    points.push_back(rightPoint(gate, false, false));
                    points.push_back(rightPoint(gate, true, false));
                    points.push_back(leftPoint(gate, true, true));
                }

                if (i == stacks.size() - 2) {
                    points.push_back(destPoint);
                }

            } else {
                hasError = true;
            }
        }

        if (hasError) {
            points.clear();
        }
    }


    int atValue(cv::Point point) const {
        return segmented_map.at<int>(point);
    }

    RealPoint leftPoint(Gate &gate, bool core_move, bool open_gate) {
        RealPoint realPoint;
        RealPosition realPosition(gate.left_position_x, gate.left_position_y, gate.left_position_z);
        RealOrientation realOrientation(gate.left_orientation_x, gate.left_orientation_y,
                                        gate.left_orientation_z, gate.left_orientation_w);
        realPoint.realPosition = std::move(realPosition);
        realPoint.realOrientation = std::move(realOrientation);
        realPoint.core_move = core_move;
        realPoint.open_gate = open_gate;
        realPoint.gate_uuid = gate.left_gate_ID;
        realPoint.gate_factory_id = gate.factory_ID;
        return realPoint;
    }

    RealPoint rightPoint(Gate &gate, bool core_move, bool open_gate) {
        RealPoint realPoint;
        RealPosition realPosition(gate.right_position_x, gate.right_position_y, gate.right_position_z);
        RealOrientation realOrientation(gate.right_orientation_x, gate.right_orientation_y,
                                        gate.right_orientation_z, gate.right_orientation_w);
        realPoint.realPosition = std::move(realPosition);
        realPoint.realOrientation = std::move(realOrientation);
        realPoint.core_move = core_move;
        realPoint.open_gate = open_gate;
        realPoint.gate_uuid = gate.right_gate_ID;
        realPoint.gate_factory_id = gate.factory_ID;
        return realPoint;
    }

};

#endif //APP_COMMUNICATION_GATECOMPREHENSIVE_H
