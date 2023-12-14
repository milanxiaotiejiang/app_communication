//
// Created by Looper on 2022/11/5.
//

#ifndef APP_COMMUNICATION_POINT_GENERATOR_H
#define APP_COMMUNICATION_POINT_GENERATOR_H

#include <geometry_msgs/PoseStamped.h>
#include "vector"
#include "RealTask.h"
#include "task/RealBlock.h"
#include "task/RealPoint.h"
#include "model/RoomVo.h"
#include "model/Point.h"

struct PoseStamped {
    geometry_msgs::PoseStamped poseStamped;
    CmcMode cmcMode{CmcMode::Omission};
};

/**
 * 点位生成节点，包含矩形、全覆盖、分区全覆盖等
 */
class PointGenerator {
protected:
    static RealBlock buildBlock(int id, const RealTask &task);

    static std::vector<PoseMo> recalculateAngle(const cv::Point2d &point2D, const std::vector<PoseMo> &poseList);

    static std::vector<PoseStamped> convertToGeometry(const std::vector<PoseMo> &complex);

    static void addSinglePoint(std::vector<RealBlock> &blockList,
                               const RealTask &realTask,
                               const RealPoint &singlePoint);

    static void complexPathToRealBlock(RealTask &realTask,
                                       const std::vector<std::vector<PoseMo>> &complexList,
                                       std::vector<RealBlock> &blockList);

public:
    virtual std::vector<RealBlock> taskGeneratePointList(RealTask &task) = 0;

    virtual void elevatorPointList(RealTask &task) = 0;

    static bool generateRecPointListForViewPart(std::vector<Point> zoned,
                                                std::vector<PoseVo> &pointList);

    static bool generateBowPointListForViewPart(std::vector<Point> zoned, std::vector<PoseVo> &pointList);

    static void
    generateChildPointFlow(const std::vector<PoseVo> &points, std::vector<PoseVo> &cpList, float resolution_);

};

class ExplorationGenerator : public PointGenerator {
public:
    std::vector<RealBlock> taskGeneratePointList(RealTask &task) override;

    void elevatorPointList(RealTask &task) override;

    bool cleanMechanismControlMode(const ZoneVo &currentZone, const ZoneVo &nextZone, int rows, int cols);

    RealBlock createPreCirculationBlock(RealTask &task);

    RealBlock createPreElevatorBlock(RealTask &task);

    RealBlock createPreSwitchMapBlock(RealTask &task);

    RealBlock createPostCirculationBlock(RealTask &task);

    RealBlock createPostElevatorBlock(RealTask &task);

    RealBlock createPostSwitchMapBlock(RealTask &task);

};

#endif //APP_COMMUNICATION_POINT_GENERATOR_H
