//
// Created by Looper on 2023/3/4.
//

#include <ros/package.h>
#include "db/path.h"

namespace path {
    std::string separator() {
        if (file_separator.empty()) {
            return "/";
        }
        return file_separator;
    }

    std::string data_base_dir() {
        return ros::package::getPath("data_base") + separator();
    }

    std::string robot_slam_dir() {
        return ros::package::getPath("robot_slam") + separator();
    }

    std::string zoo_bringup_dir() {
        return ros::package::getPath("zoo_bringup") + separator();
    }

    std::string data_base_config_dir() {
        return data_base_dir() + "config" + separator();
    }

    std::string robot_slam_map_dir() {
        return robot_slam_dir() + "maps" + separator();
    }

    std::string zoo_bringup_params_dir() {
        return zoo_bringup_dir() + "params" + separator();
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