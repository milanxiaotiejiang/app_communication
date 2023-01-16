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
                            )
        );

    }

};

using Storage = decltype(SqliteDataBase::initStorage());

#endif //APP_COMMUNICATION_SQLITEDATABASE_H
