//
// Created by Looper on 2022/11/5.
//

#ifndef APP_COMMUNICATION_POINT_GENERATOR_H
#define APP_COMMUNICATION_POINT_GENERATOR_H

#include "vector"
#include "RealTask.h"
#include "RealPoint.h"
#include "model/RoomVo.h"
#include "model/ViewPart.h"
#include "task/model/CombinationPoseVo.h"

/**
 * 点位生成节点，包含矩形、全覆盖、分区全覆盖等
 */
class PointGenerator {
protected:
    static void pose2RealPoint(const RealTask &realTask, std::vector<PoseVo> poseList,
                               std::vector<RealPoint> &realPointList);

    static void combinationPose2RealPoint(const RealTask &realTask, std::vector<CombinationPoseVo> poseList,
                                          std::vector<RealPoint> &realPointList);

public:
    virtual std::vector<RealPoint> taskGeneratePointList(RealTask task) = 0;
};

class CoveragePointGenerator : public PointGenerator {
protected:
    std::vector<RealPoint> taskGeneratePointList(RealTask task) override;
};

class RectanglePointGenerator : public PointGenerator {
protected:
    std::vector<RealPoint> taskGeneratePointList(RealTask task) override;
};

class CombinationPointGenerator : public PointGenerator {
private:
    bool generateRecPointListForViewPart(std::vector<Point> zoned, std::vector<CombinationPoseVo> &pointList);

protected:
    std::vector<RealPoint> taskGeneratePointList(RealTask task) override;
};

class FullPointGenerator : public PointGenerator {
protected:
    std::vector<RealPoint> taskGeneratePointList(RealTask task) override;
};

class ExplorationGenerator : public PointGenerator {
public:
    std::vector<RealPoint> taskGeneratePointList(RealTask task) override;
};

#endif //APP_COMMUNICATION_POINT_GENERATOR_H
