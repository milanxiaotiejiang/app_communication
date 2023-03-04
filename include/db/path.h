//
// Created by Looper on 2023/3/4.
//

#ifndef APP_COMMUNICATION_PATH_H
#define APP_COMMUNICATION_PATH_H

#include "string"

namespace path {
    const std::string file_separator = "/";
    const std::string split_str = ",";

    const std::string mymap_yaml = "mymap.yaml";
    const std::string mymap_pgm = "mymap.pgm";
    const std::string mymap_pbstream = "mymap.pbstream";
    const std::string mymap_segmentation = "mymap_segmentation";
    const std::string prohibition_areas_yaml = "prohibition_areas.yaml";

    std::string data_base_path();

    std::string robot_slam_path();

    std::string zoo_bringup_path();

    std::string data_base_config_path();

    std::string robot_slam_map_path();

    std::string zoo_bringup_params_path();

    std::string map_yaml_path();

    std::string map_pgm_path();

    std::string map_segmentation_path();

    std::string prohibition_areas_path();
}

#endif //APP_COMMUNICATION_PATH_H
