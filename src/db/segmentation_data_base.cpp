//
// Created by Looper on 2022/9/16.
//

#include "db/segmentation_data_base.h"

#include <utility>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "segmentation/Room.h"
#include "BaseThrowable.h"
#include "db/task_data_base.h"

bool SegmentationDataBase::loadMap() {
    segmentationStorage.sync_schema();
    try {
        auto againMap = segmentationStorage.get_all<MapPo>(limit(1));
        mapPo = againMap.front();
        return true;
    } catch (const std::system_error &e) {
        LOG(ERROR) << e.what();
        return false;
    }
}

MapPo &SegmentationDataBase::getDbMap() {
    return mapPo;
}

RoomPo SegmentationDataBase::selectRoomById(long roomId) {
    return segmentationStorage.get<RoomPo>(roomId);
}

std::vector<RoomPo> SegmentationDataBase::selectRoomByMapId(const std::string &mapId) {
    return segmentationStorage.get_all<RoomPo>(where(c(&RoomPo::o_map_id) == mapId));
}

void SegmentationDataBase::removeAllRoom(const std::string &mapId) {
    segmentationStorage.remove_all<RoomPo>(where(c(&RoomPo::o_map_id) == mapId));
    TaskDataBase::instance().deleteTaskFoMode(mapId, TaskMode::Subregion, true);
}

void SegmentationDataBase::memory2Storage(cv::Mat &mat, std::vector<Room> &rooms) {
//    cv::imshow("memory2Storage", mat);
//    cv::waitKey();
    segmentationStorage.transaction([&] {

        removeAllRoom(mapPo.id);

        for (auto &item: rooms) {
            auto id = item.getID();
            auto name = item.getName();
            auto area = item.getArea();
            auto center = item.getCenter();

            auto neighborIds = item.getNeighborIDs();
            auto perimeter = item.getPerimeter();

            std::string neighbor_room_ids;
            for (int i = 0; i < neighborIds.size(); i++) {
                auto neighborId = neighborIds[i];
                if (i == neighborIds.size() - 1) {
                    neighbor_room_ids.append(std::to_string(neighborId));
                } else {
                    neighbor_room_ids.append(std::to_string(neighborId)).append(path::split_str);
                }
            }

            RoomPo dbRoom(mapPo.id, name, id, area, center.x, center.y, perimeter, neighbor_room_ids);
            segmentationStorage.insert(dbRoom);
        }

//        auto uuid = boost::uuids::to_string(boost::uuids::random_generator()());
//        auto randomPngPath = mapPo.path + SEGMENTATION_MB + uuid + ".png";
//        auto depth = mat.clone();
//        CvUtils::savePng(randomPngPath, depth);

        return CvUtils::write(path::map_segmentation_path(), mat);
    });
}

void
SegmentationDataBase::storage2Memory(cv::Mat &mat, std::vector<Room> &rooms, double map_resolution_from_subscription) {
    try {
        mat = CvUtils::read(path::map_segmentation_path());

        auto result = segmentationStorage.get_all<RoomPo>(where(c(&RoomPo::o_map_id) == mapPo.id));

        for (const auto &item: result) {

            std::vector<cv::Point> members;
            for (int y = 0; y < mat.rows; y++) {
                for (int x = 0; x < mat.cols; x++) {
                    auto value = mat.at<int>(y, x);
                    auto point = cv::Point(x, y);
                    if (item.value == value) {
                        members.push_back(point);
                    }
                }
            }

            std::vector<int> neighbor_room_ids;
            std::string str = item.neighbor_room_ids + path::split_str;
            size_t pos = str.find(path::split_str);
            int step = path::split_str.size();
            while (pos != str.npos) {
                std::string temp = str.substr(0, pos);
                neighbor_room_ids.push_back(atoi(temp.c_str()));
                //去掉已分割的字符串,在剩下的字符串中进行分割
                str = str.substr(pos + step, str.size());
                pos = str.find(path::split_str);
            }

            Room room(item.value);
            room.setName(item.name);
            room.setArea(item.area);
            room.setPerimeter(item.perimeter);
            room.directInsertMemberPoints(members, map_resolution_from_subscription);
            for (const auto &neighborRoomId: neighbor_room_ids) {
                room.addNeighborID(neighborRoomId);
            }
            room.setDbId(item.id);
            rooms.push_back(room);
        }
//        cv::imshow("storage2Memory", mat);
//        cv::waitKey();
    } catch (app::exception const &e) {
        removeAllRoom(mapPo.id);
        throw app::exception(make_error_code(error::room_mb_file_open_fail));
    }
}

void SegmentationDataBase::reRoomName(int targetId, const std::string &name) {
    RoomPo roomPo = SegmentationDataBase::instance().selectRoomById(targetId);
    roomPo.name = name;
    segmentationStorage.update(roomPo);
//    auto results = segmentationStorage.get_all<RoomPo>(
//            where(c(&RoomPo::o_map_id) == mapPo.id and c(&RoomPo::value) == targetId)
//    );
//    segmentationStorage.transaction([results, name, this] {
//        for (auto item: results) {
//            item.name = name;
//            segmentationStorage.update(item);
//        }
//        return true;
//    });
}

void SegmentationDataBase::setPlanParam(const std::string &mapId, double robotRadius,
                                        int mapCorrectionClosingNeighborhoodSize,
                                        double gridObstacleOffset, double pathEps, double minCellArea,
                                        double maxDeviationFromTrack,
                                        int rangeNearBaseStation, double roomAreaFactorLowerLimit,
                                        double roomAreaFactorUpperLimit,
                                        int neighborhoodIndex, int maxIterations, double minCriticalPointDistanceFactor,
                                        double maxAreaForMerging, int distanceFromObstacles, int numberExtension,
                                        int multipleContourSpacing, int random_number_generation_ratio,
                                        int boundary_min_area) {
    PlanPo planPo(mapId, robotRadius, mapCorrectionClosingNeighborhoodSize,
                  gridObstacleOffset, pathEps, minCellArea, maxDeviationFromTrack,
                  rangeNearBaseStation, roomAreaFactorLowerLimit, roomAreaFactorUpperLimit,
                  neighborhoodIndex, maxIterations, minCriticalPointDistanceFactor, maxAreaForMerging,
                  distanceFromObstacles, numberExtension, multipleContourSpacing,
                  random_number_generation_ratio, boundary_min_area);
    segmentationStorage.replace(planPo);
}

PlanPo SegmentationDataBase::getDbPlan(std::string map_id) {
    auto vectorPlan = segmentationStorage.get_all<PlanPo>(
            where(c(&PlanPo::map_id) == std::move(map_id))
    );
    if (!vectorPlan.empty())
        return vectorPlan.front();
    else
        return {};
}



