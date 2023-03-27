//
// Created by Looper on 2022/9/16.
//

#ifndef APP_COMMUNICATION_SQLITEDATABASE_H
#define APP_COMMUNICATION_SQLITEDATABASE_H

#include "string"
#include <sqlite_orm/sqlite_orm.h>
#include "db/segmentation_model.h"
#include "segmentation/Room.h"
#include "task_model.h"
#include "path.h"
#include <ros/package.h>

const std::string SEGMENTATION_SQLITE_PATH = path::robot_slam_map_dir() + "Map.sqlite";

const std::string TASK_SQLITE_PATH = path::data_base_config_dir() + "Task.sqlite";

using namespace sqlite_orm;

class SqliteDataBase {
public:
    static int ModeToInt(TaskMode mode) {
        return static_cast<int>(mode);
    }

    static std::unique_ptr<TaskMode> ModeFromInt(const int &i) {
        if (i == 0) {
            return std::make_unique<TaskMode>(TaskMode::Zoned);
        } else if (i == 1) {
            return std::make_unique<TaskMode>(TaskMode::Cover);
        } else if (i == 2) {
            return std::make_unique<TaskMode>(TaskMode::Subregion);
        } else if (i == 3) {
            return std::make_unique<TaskMode>(TaskMode::Line);
        }
        return nullptr;
    }

    static TaskMode TaskModeFromInt(const int &i) {
        if (i == 0) {
            return TaskMode::Zoned;
        } else if (i == 1) {
            return TaskMode::Cover;
        } else if (i == 2) {
            return TaskMode::Subregion;
        } else if (i == 3) {
            return TaskMode::Line;
        }
        return TaskMode::Cover;
    }

    static std::string SourceToString(TaskSource source) {
        switch (source) {
            case TaskSource::App:
                return "App";
            case TaskSource::Pad:
                return "Pad";
            case TaskSource::Cloud:
                return "Cloud";
            default:
                return "Self";
        }
    }

    static std::unique_ptr<TaskSource> SourceFromString(const std::string &s) {
        if (s == "App") {
            return std::make_unique<TaskSource>(TaskSource::App);
        } else if (s == "Pad") {
            return std::make_unique<TaskSource>(TaskSource::Pad);
        } else if (s == "Cloud") {
            return std::make_unique<TaskSource>(TaskSource::Cloud);
        }
        return std::make_unique<TaskSource>(TaskSource::Self);
    }

    static TaskSource TaskSourceFromString(const std::string &s) {
        if (s == "App") {
            return TaskSource::App;
        } else if (s == "Pad") {
            return TaskSource::Pad;
        } else if (s == "Cloud") {
            return TaskSource::Cloud;
        }
        return TaskSource::Self;
    }

public:

    static auto initMapStorage() {
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
                                       make_column("multiple_contour", &PlanPo::multiple_contour_spacing),
                                       make_column("random_ratio", &PlanPo::random_number_generation_ratio),
                                       make_column("boundary_min_area", &PlanPo::boundary_min_area)
                            )
        );

    }

    static auto initTaskStorage() {
        return make_storage(TASK_SQLITE_PATH,
                            make_table("task",
                                       make_column("id", &TaskPo::id, primary_key(), autoincrement()),
                                       make_column("o_map_id", &TaskPo::o_map_id),
                                       make_column("name", &TaskPo::name, default_value("")),
                                       make_column("rate", &TaskPo::rate, default_value(1)),
                                       make_column("mode", &TaskPo::mode, default_value(ModeToInt(TaskMode::Cover))),
                                       make_column("sweep", &TaskPo::sweep, default_value(-1)),
                                       make_column("mop", &TaskPo::mop, default_value(-1)),
                                       make_column("vacuum", &TaskPo::vacuum, default_value(-1)),
                                       make_column("push", &TaskPo::push, default_value(-1)),
                                       make_column("aromatherapy", &TaskPo::aromatherapy, default_value(-1)),
                                       make_column("disinfect", &TaskPo::disinfect, default_value(-1)),
                                       make_column("principal", &TaskPo::principal, default_value(false)),
                                       make_column("partition", &TaskPo::partition, default_value(false)),
                                       make_column("subregion_range", &TaskPo::subregion_range, default_value("")),
                                       make_column("knife", &TaskPo::knife, default_value(false)),
                                       make_column("source", &TaskPo::source,
                                                   default_value(SourceToString(TaskSource::App))),
                                       make_column("launch_people", &TaskPo::launch_people, default_value("")),
                                       make_column("launch_time", &TaskPo::launch_time, default_value(time_t(NULL))),
                                       make_column("update_time", &TaskPo::update_time,
                                                   default_value(std::time(nullptr))),
                                       make_column("create_time", &TaskPo::create_time,
                                                   default_value(std::time(nullptr)))
                            ),
                            make_table("zone",
                                       make_column("id", &ZonePo::id, primary_key(), autoincrement()),
                                       make_column("o_task_id", &ZonePo::o_task_id),
                                       make_column("point_range", &ZonePo::point_range),
                                       foreign_key(&ZonePo::o_task_id).references(&TaskPo::id).on_delete.set_default()
                            ),
                            make_table("timer",
                                       make_column("id", &TimerPo::id, primary_key(), autoincrement()),
                                       make_column("o_map_id", &TimerPo::o_map_id),
                                       make_column("o_task_id", &TimerPo::o_task_id),
                                       make_column("rule", &TimerPo::rule),
                                       make_column("name", &TimerPo::name),
                                       make_column("is_execute", &TimerPo::is_execute, default_value(true)),
                                       make_column("rate", &TimerPo::rate, default_value(1)),
                                       make_column("is_never", &TimerPo::is_never, default_value(true)),
                                       make_column("is_skip", &TimerPo::is_skip, default_value(false)),
                                       make_column("end_year", &TimerPo::end_year, default_value(0)),
                                       make_column("end_month", &TimerPo::end_month, default_value(0)),
                                       make_column("end_day", &TimerPo::end_day, default_value(0)),
                                       foreign_key(&TimerPo::o_task_id).references(&TaskPo::id).on_delete.set_default()
                            )
        );
    }

};

using MapStorage = decltype(SqliteDataBase::initMapStorage());
using TaskStorage = decltype(SqliteDataBase::initTaskStorage());


namespace sqlite_orm {

    // TaskSource
    template<>
    struct type_printer<TaskSource> : public text_printer {
    };

    template<>
    struct statement_binder<TaskSource> {
        int bind(sqlite3_stmt *stmt, int index, const TaskSource &value) {
            return statement_binder<std::string>().bind(stmt, index, SqliteDataBase::SourceToString(value));
        }
    };

    template<>
    struct field_printer<TaskSource> {
        std::string operator()(const TaskSource &t) const {
            return SqliteDataBase::SourceToString(t);
        }
    };

    template<>
    struct row_extractor<TaskSource> {
        TaskSource extract(const char *row_value) {
            if (auto gender = SqliteDataBase::SourceFromString(row_value)) {
                return *gender;
            } else {
                throw std::runtime_error("incorrect gender string (" + std::string(row_value) + ")");
            }
        }

        TaskSource extract(sqlite3_stmt *stmt, int columnIndex) {
            auto str = sqlite3_column_text(stmt, columnIndex);
            return this->extract((const char *) str);
        }
    };

    // TaskMode
    template<>
    struct type_printer<TaskMode> : public integer_printer {
    };

    template<>
    struct statement_binder<TaskMode> {
        int bind(sqlite3_stmt *stmt, int index, const TaskMode &value) {
            return statement_binder<int>().bind(stmt, index, SqliteDataBase::ModeToInt(value));
        }
    };

    template<>
    struct field_printer<TaskMode> {
        int operator()(const TaskMode &t) const {
            return SqliteDataBase::ModeToInt(t);
        }
    };

    template<>
    struct row_extractor<TaskMode> {
        TaskMode extract(const int row_value) {
            if (auto gender = SqliteDataBase::ModeFromInt(row_value)) {
                return *gender;
            } else {
                throw std::runtime_error("incorrect gender string (" + std::to_string(row_value) + ")");
            }
        }

        TaskMode extract(sqlite3_stmt *stmt, int columnIndex) {
            auto str = sqlite3_column_int(stmt, columnIndex);
            return this->extract(str);
        }
    };
}

#endif //APP_COMMUNICATION_SQLITEDATABASE_H
