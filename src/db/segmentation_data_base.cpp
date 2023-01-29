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

bool SegmentationDataBase::loadMap() {
    segmentationStorage.sync_schema();
    try {
        auto vectorMap = segmentationStorage.get_all<MapPo>(limit(1));
        if (!vectorMap.empty()) {
            mapPo = vectorMap.front();
        } else {
            auto mapId = boost::uuids::to_string(boost::uuids::random_generator()());
            segmentationStorage.replace(
                    MapPo(mapId, "mymap.pgm",
                          SEGMENTATION_PATH)
            );
            auto againMap = segmentationStorage.get_all<MapPo>(limit(1));
            mapPo = againMap.front();
        }
        return true;
    } catch (const std::system_error &e) {
        LOG(ERROR) << e.what();
        return false;
    }
}

MapPo &SegmentationDataBase::getDbMap() {
    return mapPo;
}

std::vector<RoomPo> SegmentationDataBase::selectByMapId(const std::string &mapId) {
    return segmentationStorage.get_all<RoomPo>(
            where(c(&RoomPo::o_map_id) == mapId));
}

void SegmentationDataBase::removeAllRoom(const std::string &mapId) {
    segmentationStorage.remove_all<RoomPo>(where(c(&RoomPo::o_map_id) == mapId));
}

void SegmentationDataBase::memory2Storage(cv::Mat &mat, std::vector<Room> &rooms) {
//    cv::imshow("memory2Storage", mat);
//    cv::waitKey();
    segmentationStorage.transaction([&] {
        segmentationStorage.remove_all<RoomPo>(where(c(&RoomPo::o_map_id) == mapPo.id));

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
                    neighbor_room_ids.append(std::to_string(neighborId)).append(SPLIT_STR);
                }
            }

            RoomPo dbRoom(mapPo.id, name, id, area, center.x, center.y, perimeter, neighbor_room_ids);
            segmentationStorage.insert(dbRoom);
        }

//        auto uuid = boost::uuids::to_string(boost::uuids::random_generator()());
//        auto randomPngPath = mapPo.path + SEGMENTATION_MB + uuid + ".png";
//        auto depth = mat.clone();
//        CvUtils::savePng(randomPngPath, depth);

        auto segmentationPgmPath = mapPo.path + SEGMENTATION_MB + mapPo.id;
        return CvUtils::write(segmentationPgmPath, mat);
    });
}

void
SegmentationDataBase::storage2Memory(cv::Mat &mat, std::vector<Room> &rooms, double map_resolution_from_subscription) {
    auto segmentationPgmPath = mapPo.path + SEGMENTATION_MB + mapPo.id;
    try {
        mat = CvUtils::read(segmentationPgmPath);

        auto result = segmentationStorage.get_all<RoomPo>(
                where(c(&RoomPo::o_map_id) == mapPo.id));

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
            std::string str = item.neighbor_room_ids + SPLIT_STR;
            size_t pos = str.find(SPLIT_STR);
            int step = SPLIT_STR.size();
            while (pos != str.npos) {
                std::string temp = str.substr(0, pos);
                neighbor_room_ids.push_back(atoi(temp.c_str()));
                //去掉已分割的字符串,在剩下的字符串中进行分割
                str = str.substr(pos + step, str.size());
                pos = str.find(SPLIT_STR);
            }

            Room room(item.value);
            room.setName(item.name);
            room.setArea(item.area);
            room.setPerimeter(item.perimeter);
            room.directInsertMemberPoints(members, map_resolution_from_subscription);
            for (const auto &neighborRoomId: neighbor_room_ids) {
                room.addNeighborID(neighborRoomId);
            }

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
    auto results = segmentationStorage.get_all<RoomPo>(
            where(c(&RoomPo::o_map_id) == mapPo.id and c(&RoomPo::value) == targetId)
    );
    segmentationStorage.transaction([results, name, this] {
        for (auto item: results) {
            item.name = name;
            segmentationStorage.update(item);
        }
        return true;
    });
}

void SegmentationDataBase::setPlanParam(const std::string &map_id, double robot_radius,
                                        int map_correction_closing_neighborhood_size,
                                        double grid_obstacle_offset, double path_eps,
                                        double min_cell_area, double max_deviation_from_track,
                                        double room_area_factor_lower_limit,
                                        double room_area_factor_upper_limit,
                                        int neighborhood_index, int max_iterations,
                                        double min_critical_point_distance_factor,
                                        double max_area_for_merging) {
    PlanPo planPo(map_id, robot_radius, map_correction_closing_neighborhood_size,
                  grid_obstacle_offset, path_eps, min_cell_area, max_deviation_from_track,
                  room_area_factor_lower_limit, room_area_factor_upper_limit,
                  neighborhood_index, max_iterations, min_critical_point_distance_factor, max_area_for_merging);
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



