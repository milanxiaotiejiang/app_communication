//
// Created by Looper on 2022/9/16.
//

#ifndef APP_COMMUNICATION_SQLITEDATABASE_H
#define APP_COMMUNICATION_SQLITEDATABASE_H

#include "string"
#include <sqlite_orm/sqlite_orm.h>
#include "db/segmentation_model.h"
#include "segmentation/Room.h"
#include <ros/package.h>

const static std::string SEGMENTATION_DIR = ros::package::getPath("robot_slam");
const std::string SEGMENTATION_PATH = SEGMENTATION_DIR + "/maps/";
const std::string SEGMENTATION_SQLITE_PATH = SEGMENTATION_PATH + "Map.sqlite";
const std::string SEGMENTATION_MB = "segmentation_";
const std::string SPLIT_STR = ",";

using namespace sqlite_orm;

class SqliteDataBase {
public:
    static auto initStorage() {
        return make_storage(SEGMENTATION_SQLITE_PATH,
                            make_table("map",
                                       make_column("id", &MapPo::id, primary_key()),
                                       make_column("name", &MapPo::name),
                                       make_column("path", &MapPo::path)
                            ),
                            make_table("segmentation",
                                       make_column("id", &RoomPo::id, autoincrement(), primary_key()),
                                       make_column("o_map_id", &RoomPo::o_map_id),
                                       make_column("value", &RoomPo::value),
                                       make_column("area", &RoomPo::area),
                                       make_column("center_x", &RoomPo::center_x),
                                       make_column("center_y", &RoomPo::center_y),
                                       make_column("perimeter", &RoomPo::perimeter),
                                       make_column("neighbor_room_ids", &RoomPo::neighbor_room_ids),
                                       make_column("name", &RoomPo::name)
                            ),
                            make_table("plan_param",
                                       make_column("map_id", &PlanPo::map_id, primary_key()),
                                       make_column("robot_radius", &PlanPo::robot_radius),
                                       make_column("iterations", &PlanPo::map_correction_closing_neighborhood_size),
                                       make_column("obstacle_offset", &PlanPo::grid_obstacle_offset),
                                       make_column("path_eps", &PlanPo::path_eps),
                                       make_column("min_cell_area", &PlanPo::min_cell_area),
                                       make_column("near_base", &PlanPo::range_near_base_station),
                                       make_column("max_deviation", &PlanPo::max_deviation_from_track),
                                       make_column("area_factor_lower", &PlanPo::room_area_factor_lower_limit),
                                       make_column("area_factor_upper", &PlanPo::room_area_factor_upper_limit),
                                       make_column("neighborhood_index", &PlanPo::neighborhood_index),
                                       make_column("max_iterations", &PlanPo::max_iterations),
                                       make_column("min_distance_factor", &PlanPo::min_critical_point_distance_factor),
                                       make_column("max_area_for_merging", &PlanPo::max_area_for_merging),
                                       make_column("distance_obstacles_", &PlanPo::distance_from_obstacles),
                                       make_column("number_extension_", &PlanPo::number_extension),
                                       make_column("multiple_contour", &PlanPo::multiple_contour_spacing)
                            )
        );

    }

};

using Storage = decltype(SqliteDataBase::initStorage());

#endif //APP_COMMUNICATION_SQLITEDATABASE_H
