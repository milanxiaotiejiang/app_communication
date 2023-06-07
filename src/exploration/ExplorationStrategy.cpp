//
// Created by Looper on 2022/10/13.
//

#include "exploration/ExplorationStrategy.h"
#include "segmentation/SegmentationCenter.h"
#include "exploration/ExplorationCenter.h"
#include "db/segmentation_data_base.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "leave/ParamManager.h"
#include "db/task_data_base.h"
#include "exploration/path_exploration_preview_task.h"
#include "task/TaskCenter.h"

#include "simulation.h"

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
    std::vector<std::vector<geometry_msgs::Pose2D>> complex_path;

    ExplorationCenter &explorationCenter = ExplorationCenter::instance();

    if (isPartition) {
        cv::Mat segmented_map;
        std::vector<Room> rooms;
        SegmentationCenter::instance().storage2Memory(segmented_map, rooms);

        if (targetId == -1) {
            if (rooms.empty()) {
                explorationCenter.generatePlanningPathFull(baseMap, explorerMode, true,
                                                           exploration_path, point_path, complex_path);
            } else {
                explorationCenter.generatePlanningSegmentationPath(baseMap, segmented_map, rooms, explorerMode, true,
                                                                   exploration_path, point_path, complex_path);
            }
        } else {
            const cv::Mat &oneMap = SegmentationCenter::instance().choiceOneRoom(segmented_map, rooms, targetId);
            explorationCenter.generatePlanningPathSub(oneMap, explorerMode, true,
                                                      exploration_path, point_path, complex_path);
        }
    } else {
        explorationCenter.generatePlanningPathFull(baseMap, explorerMode, true,
                                                   exploration_path, point_path, complex_path);
    }

    explorationCenter.pathPublish(exploration_path);

    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    string uuid_string = boost::uuids::to_string(uuid);

    RoomCoverage roomCoverage;
    TaskExploration::planningPath2RoomCoverage(roomCoverage, exploration_path, point_path, complex_path);
    roomCoverage.setCoverageId(uuid_string);
    explorationCenter.cacheRoomCoverage(roomCoverage);

    RoomCoverage result;
    result.setCoverageId(roomCoverage.getCoverageId());
    if (dataMode == DATA_MODE_GEOMETRY_POSE) {
        result.setPoseList(roomCoverage.getPoseList());
    } else if (dataMode == DATA_MODE_OPEN_CV_POINT) {
        result.setPointList(roomCoverage.getPointList());
    }
    return result;
}

PlanParam PlanParamGetStrategy::handler(string params) {
    auto planPo = SegmentationDataBase::instance().getDbPlan(SegmentationDataBase::instance().getDbMap().id);
    return PlanParam(planPo.robot_radius, planPo.map_correction_closing_neighborhood_size,
                     planPo.grid_obstacle_offset, planPo.path_eps, planPo.min_cell_area,
                     planPo.max_deviation_from_track, planPo.range_near_base_station,
                     planPo.room_area_factor_lower_limit, planPo.room_area_factor_upper_limit,
                     planPo.neighborhood_index, planPo.max_iterations,
                     planPo.min_critical_point_distance_factor, planPo.max_area_for_merging,
                     planPo.distance_from_obstacles, planPo.number_extension, planPo.multiple_contour_spacing,
                     planPo.random_number_generation_ratio, planPo.boundary_min_area);
}

bool PlanParamSetStrategy::handler(PlanParam params) {
    SegmentationDataBase::instance().setPlanParam(
            SegmentationDataBase::instance().getDbMap().id,
            params.getRobotRadius(),
            params.getMapCorrectionClosingNeighborhoodSize(),
            params.getGridObstacleOffset(),
            params.getPathEps(),
            params.getMinCellArea(),
            params.getMaxDeviationFromTrack(),
            params.getRangeNearBaseStation(),
            params.getRoomAreaFactorLowerLimit(),
            params.getRoomAreaFactorUpperLimit(),
            params.getNeighborhoodIndex(),
            params.getMaxIterations(),
            params.getMinCriticalPointDistanceFactor(),
            params.getMaxAreaForMerging(),
            params.getDistanceFromObstacles(),
            params.getNumberExtension(),
            params.getMultipleContourSpacing(),
            params.getRandomNumberGenerationRatio(),
            params.getBoundaryMinArea()
    );
    ExplorationCenter::instance().repaintCoveragePath();
}

PlanParam PlanParamResetStrategy::handler(string params) {
    MapAttribute::instance().loadDefaultPlanParam();
    auto planPo = SegmentationDataBase::instance().getDbPlan(SegmentationDataBase::instance().getDbMap().id);
    return PlanParam(planPo.robot_radius, planPo.map_correction_closing_neighborhood_size,
                     planPo.grid_obstacle_offset, planPo.path_eps, planPo.min_cell_area,
                     planPo.max_deviation_from_track, planPo.range_near_base_station,
                     planPo.room_area_factor_lower_limit, planPo.room_area_factor_upper_limit,
                     planPo.neighborhood_index, planPo.max_iterations,
                     planPo.min_critical_point_distance_factor, planPo.max_area_for_merging,
                     planPo.distance_from_obstacles, planPo.number_extension, planPo.multiple_contour_spacing,
                     planPo.random_number_generation_ratio, planPo.boundary_min_area);
}

bool SetExplorerEnergyStrategy::handler(bool params) {
    ParamManager::instance().setEnergy(params);
    return ParamManager::instance().getEnergy();
}

bool GetExplorerEnergyStrategy::handler(string params) {
    return ParamManager::instance().getEnergy();
}

RoomCoverage ExplorationTaskStrategy::handler(long params) {
    const TaskVo &task = TaskDataBase::instance().loadTaskFoId(params);
    RealTask realTask;
    TaskExploration::task2RealTask(task, realTask);
    auto coverage = TaskExploration::explorationPlanningPath(realTask);

    RoomCoverage result;
    result.setCoverageId(coverage.getCoverageId());
    result.setPoseList(coverage.getPoseList());
    result.setPointList(coverage.getPointList());
    return result;
}
