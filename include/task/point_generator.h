//
// Created by Looper on 2022/11/5.
//

#ifndef APP_COMMUNICATION_POINT_GENERATOR_H
#define APP_COMMUNICATION_POINT_GENERATOR_H

#include "vector"
#include "RealTask.h"
#include "task/RealBlock.h"
#include "task/RealPoint.h"
#include "model/RoomVo.h"
#include "model/ViewPart.h"
#include "task/model/CombinationPoseVo.h"

/**
 * 点位生成节点，包含矩形、全覆盖、分区全覆盖等
 */
class PointGenerator {
protected:
    static RealBlock buildBlock(int id, const RealTask &task);

    static std::vector<PoseVo> recalculateAngle(const geometry_msgs::Pose &originPose,
                                                const std::vector<PoseVo> &poseList);

    static std::vector<geometry_msgs::PoseStamped> convertToGeometry(const std::vector<PoseVo> &complex);

    static void addSinglePoint(std::vector<RealBlock> &blockList,
                               const RealTask &realTask,
                               const RealPoint &singlePoint);

    static void complexPathToRealBlock(RealTask &realTask,
                                       const std::vector<std::vector<PoseVo>> &complexList,
                                       std::vector<RealBlock> &blockList);

public:
    virtual std::vector<RealBlock> taskGeneratePointList(RealTask &task) = 0;

    static bool generateRecPointListForViewPart(std::vector<Point> zoned,
                                                std::vector<PoseVo> &pointList);

    static void
    generateChildPointFlow(const std::vector<PoseVo> &points, std::vector<PoseVo> &cpList, float resolution_);

};

//class CoveragePointGenerator : public PointGenerator {
//protected:
//    std::vector<RealPoint> taskGeneratePointList(RealTask &task) override;
//};

//class RectanglePointGenerator : public PointGenerator {
//protected:
//    std::vector<RealPoint> taskGeneratePointList(RealTask &task) override;
//};

//class CombinationPointGenerator : public PointGenerator {
//private:
//    bool generateRecPointListForViewPart(std::vector<Point> zoned, std::vector<CombinationPoseVo> &pointList);
//
//protected:
//    std::vector<RealPoint> taskGeneratePointList(RealTask &task) override;
//};

//class FullPointGenerator : public PointGenerator {
//protected:
//    std::vector<RealPoint> taskGeneratePointList(RealTask &task) override;
//};

class ExplorationGenerator : public PointGenerator {
public:
    std::vector<RealBlock> taskGeneratePointList(RealTask &task) override;
};

#endif //APP_COMMUNICATION_POINT_GENERATOR_H
