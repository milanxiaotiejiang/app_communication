//
// Created by Looper on 2023/3/4.
//

#include <ros/package.h>
#include "db/path.h"

namespace path {
    std::string data_base_dir() {
        return ros::package::getPath("data_base") + file_separator;
    }

    std::string robot_slam_dir() {
        return ros::package::getPath("robot_slam") + file_separator;
    }

    std::string zoo_bringup_dir() {
        return ros::package::getPath("zoo_bringup") + file_separator;
    }

    std::string data_base_config_dir() {
        return data_base_dir() + "config" + file_separator;
    }

    std::string robot_slam_map_dir() {
        return robot_slam_dir() + "maps" + file_separator;
    }

    std::string zoo_bringup_params_dir() {
        return zoo_bringup_dir() + "params" + file_separator;
    }

    std::string map_yaml_path() {
        return robot_slam_map_dir() + mymap_yaml;
    }

    std::string map_pgm_path() {
        return robot_slam_map_dir() + mymap_pgm;
    }

    std::string map_segmentation_path() {
        return robot_slam_map_dir() + mymap_segmentation;
    }

    std::string prohibition_areas_path() {
        return data_base_config_dir() + prohibition_areas_yaml;
    }
}