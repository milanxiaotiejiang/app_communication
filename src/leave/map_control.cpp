//
// Created by Looper on 2023/3/6.
//

#include "leave/map_control.h"
#include "db/segmentation_data_base.h"
#include "db/task_data_base.h"
#include "cppfs/fs.h"
#include "cppfs/FilePath.h"
#include "cppfs/FileHandle.h"
#include "exploration/ExplorationCenter.h"
#include "nav_msgs/LoadMap.h"

bool MapControl::initialize(ros::NodeHandle handle) {
    if (!SegmentationDataBase::instance().loadMainMap()) {
        return false;
    }

    change_map_service_client = handle.serviceClient<nav_msgs::LoadMap>("change_map");

    MapPo &mapPo = SegmentationDataBase::instance().getDbMap();

    cppfs::FileHandle dir = cppfs::fs::open(path::robot_slam_map_dir() + mapPo.id + path::separator());
    if (!dir.exists()) {
        backupAndRetrieve(mapPo.id);
    }

    loadInformation(mapPo.id);
    return true;
}

bool MapControl::loadInformation(const string &map_id) {
    std::string dir = path::robot_slam_map_dir() + map_id + path::separator();

    cppfs::FileHandle omy = cppfs::fs::open(dir + path::mymap_yaml);
    cppfs::FileHandle omp = cppfs::fs::open(dir + path::mymap_pgm);
    cppfs::FileHandle ompb = cppfs::fs::open(dir + path::mymap_pbstream);
    cppfs::FileHandle oms = cppfs::fs::open(dir + path::mymap_segmentation);
    cppfs::FileHandle opa = cppfs::fs::open(dir + path::prohibition_areas_yaml);

    if (omy.exists()) {
        cppfs::FileHandle nmy = cppfs::fs::open(path::robot_slam_map_dir());
        omy.copy(nmy);
    }
    if (omp.exists()) {
        cppfs::FileHandle nmp = cppfs::fs::open(path::robot_slam_map_dir());
        omp.copy(nmp);
    }
    if (ompb.exists()) {
        cppfs::FileHandle nmpb = cppfs::fs::open(path::robot_slam_map_dir());
        ompb.copy(nmpb);
    }
    if (oms.exists()) {
        cppfs::FileHandle nms = cppfs::fs::open(path::robot_slam_map_dir());
        oms.copy(nms);
    }
    if (opa.exists()) {
        cppfs::FileHandle npa = cppfs::fs::open(path::data_base_config_dir());
        opa.copy(npa);
    }

    return true;
}

bool MapControl::backupAndRetrieve(const string &map_id) {
    backupMap(map_id, true);
    backupProhibition(map_id, true);
    return true;
}

bool MapControl::backupProhibition(const string &map_id, bool retrieve) {
    cppfs::FileHandle dir = cppfs::fs::open(path::robot_slam_map_dir() + map_id + path::separator());
    if (!dir.isDirectory())
        dir.createDirectory();

    cppfs::FileHandle npa = cppfs::fs::open(path::prohibition_areas_path());
    if (npa.exists()) {
        npa.copy(dir);
        if (retrieve)
            npa.remove();
    }

    return true;
}

bool MapControl::backupMap(const string &map_id, bool retrieve) {
    cppfs::FileHandle dir = cppfs::fs::open(path::robot_slam_map_dir() + map_id + path::separator());
    if (!dir.isDirectory())
        dir.createDirectory();

    cppfs::FileHandle nmy = cppfs::fs::open(path::map_yaml_path());
    cppfs::FileHandle nmp = cppfs::fs::open(path::map_pgm_path());
    cppfs::FileHandle nmpb = cppfs::fs::open(path::map_pbstream_path());
    cppfs::FileHandle nms = cppfs::fs::open(path::map_segmentation_path());

    if (nmy.exists()) {
        nmy.copy(dir);
        if (retrieve)
            nmy.remove();
    }
    if (nmp.exists()) {
        nmp.copy(dir);
        if (retrieve)
            nmp.remove();
    }
    if (nmpb.exists()) {
        nmpb.copy(dir);
        if (retrieve)
            nmpb.remove();
    }
    if (nms.exists()) {
        nms.copy(dir);
        if (retrieve)
            nms.remove();
    }

    return true;
}

bool MapControl::changeMapServer() {
    nav_msgs::LoadMap srv;
    srv.request.map_url = path::map_yaml_path();
    return change_map_service_client.call(srv);
}
