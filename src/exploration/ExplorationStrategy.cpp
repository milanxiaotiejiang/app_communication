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
#include "task/task_dispatcher.h"
#include "geometry_msgs/Polygon.h"

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
    std::string uuid_string = boost::uuids::to_string(uuid);

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

PlanParam PlanParamGetStrategy::handler(std::string params) {
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
    return true;
}

PlanParam PlanParamResetStrategy::handler(std::string params) {
    MapAttributeSingleton::instance().loadDefaultPlanParam();
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

bool GetExplorerEnergyStrategy::handler(std::string params) {
    return ParamManager::instance().getEnergy();
}

RoomCoverage ExplorationTaskStrategy::handler(long params) {
    long perform_task_id = params;
    //雨雪天模式
    if (ParamManager::instance().getRainSnow()) {
        MapPo map = SegmentationDataBase::instance().getDbMap();
        const TaskVo &rainSnowTask = TaskDataBase::instance().loadRainSnowTask(map.id);
        if (rainSnowTask.getId() == -1) {
            throw app::exception(make_error_code(error::the_rain_snow_task_is_not_set));
        }
        perform_task_id = rainSnowTask.getId();
    }

    const TaskVo &taskPo = TaskDataBase::instance().loadTaskFoId(perform_task_id);
    RealTask task;
    TaskExploration::task2RealTask(taskPo, task);

    std::vector<PoseVo> poses;

    TaskMode mode = SqliteDataBase::TaskModeFromInt(task.getMode());

    if (mode == TaskMode::Zoned) {
        auto originPoint = MapAttributeSingleton::instance().getMapOrigin();
        ExplorationCenter &explorationCenter = ExplorationCenter::instance();
        SegmentationCenter &segmentationCenter = SegmentationCenter::instance();
        const cv::Mat &room_map = segmentationCenter.generateMat();
        double rows = room_map.rows * map_resolution_from_subscription;
        double cols = room_map.cols * map_resolution_from_subscription;

        std::vector<PoseVo> poseList;
        std::vector<std::vector<PoseVo>> complexPoseList;

        std::vector<ZoneVo> zones = task.getZoned();
        for (const auto &zone: zones) {

            std::vector<PointVo> points = zone.getPoints();
            std::vector<Point> trs;
            geometry_msgs::Polygon polygon;
            for (const auto &point: points) {
                Point p;
                double x = point.getX() * map_resolution_from_subscription;
                double y = point.getY() * map_resolution_from_subscription;
                p.setY(cols - x + originPoint.x);
                p.setX(rows - y + originPoint.y);
                trs.push_back(p);

                geometry_msgs::Point32 point32;
                point32.x = p.getY();
                point32.y = p.getX();
                polygon.points.push_back(point32);
            }

            std::vector<PoseVo> zonePoseList;
            PointGenerator::generateRecPointListForViewPart(trs, zonePoseList);

            std::vector<PoseVo> subPoseList;
            PointGenerator::generateChildPointFlow(zonePoseList, subPoseList, 0.2);

            for (const auto &item: subPoseList) {
                poseList.push_back(item);
            }

            complexPoseList.push_back(subPoseList);
        }

        for (const auto &complex: complexPoseList) {
            for (const auto &item: complex) {
                poses.push_back(item);
            }
        }

    } else {
        auto coverage = TaskExploration::explorationPlanningPath(task);

        std::vector<std::vector<PoseVo>> complexList = coverage.getComplexList();

        for (const auto &complex: complexList) {
            for (const auto &item: complex) {
                poses.push_back(item);
            }
        }

    }

    RoomCoverage result;
    result.setCoverageId("");
    result.setPoseList(poses);
//    result.setPointList(coverage.getPointList());
    return result;
}
