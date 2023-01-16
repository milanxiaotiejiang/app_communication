//
// Created by Looper on 2022/10/13.
//

#include "exploration/ExplorationStrategy.h"
#include "segmentation/SegmentationCenter.h"
#include "exploration/ExplorationCenter.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

const int DATA_MODE_GEOMETRY_POSE = 1;
const int DATA_MODE_OPEN_CV_POINT = 2;

RoomCoverage ExplorationRoomStrategy::handler(RoomExplorationTarget params) {
    bool isPartition = params.isPartition();
    int targetId = params.getTargetId();
    int dataMode = params.getDataMode();
    int explorerMode = params.getExplorerMode();

    const cv::Mat &baseMap = SegmentationCenter::instance().generateMat();

    std::vector<geometry_msgs::Pose2D> exploration_path;
    std::vector<cv::Point> point_path;

    ExplorationCenter &explorationCenter = ExplorationCenter::instance();

    if (isPartition) {
        cv::Mat segmented_map;
        std::vector<Room> rooms;
        SegmentationCenter::instance().storage2Memory(segmented_map, rooms);

        if (targetId == -1) {
            if (rooms.empty()) {
                explorationCenter.generatePlanningPath(baseMap, ExplorationModel::FULL, explorerMode, false,
                                                       cv::Point(0, 0),
                                                       exploration_path, point_path);
            } else {
                explorationCenter.generatePlanningSegmentationPath(baseMap, segmented_map, rooms, explorerMode,
                                                                   exploration_path, point_path);
            }
        } else {
            const cv::Mat &oneMap = SegmentationCenter::instance().choiceOneRoom(segmented_map, rooms, targetId);
            explorationCenter.generatePlanningPath(oneMap, ExplorationModel::SUB, explorerMode, false, cv::Point(0, 0),
                                                   exploration_path, point_path);
        }
    } else {
        explorationCenter.generatePlanningPath(baseMap, ExplorationModel::FULL, explorerMode, false, cv::Point(0, 0),
                                               exploration_path, point_path);
    }

    explorationCenter.pathPublish(exploration_path);

    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    string uuid_string = boost::uuids::to_string(uuid);

    std::vector<PoseVo> poseList;
    std::vector<PointVo> pointList;
    for (const auto &item: exploration_path) {
        poseList.emplace_back(item.y, item.x, item.theta);
    }
    for (const auto &item: point_path) {
        pointList.emplace_back(item.x, item.y);
    }

    auto coverage = RoomCoverage(uuid_string, pointList, poseList);
    explorationCenter.cacheRoomCoverage(coverage);

    RoomCoverage result;
    result.setCoverageId(coverage.getCoverageId());
    if (dataMode == DATA_MODE_GEOMETRY_POSE) {
        result.setPoseList(coverage.getPoseList());
    } else if (dataMode == DATA_MODE_OPEN_CV_POINT) {
        result.setPointList(coverage.getPointList());
    }
    return result;
}
