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

bool MapControl::initialize() {
    if (!SegmentationDataBase::instance().loadMainMap()) {
        return false;
    }
    MapPo &mapPo = SegmentationDataBase::instance().getDbMap();

    cppfs::FileHandle dir = cppfs::fs::open(path::robot_slam_map_dir() + mapPo.id + path::separator());
    if (!dir.exists()) {
        use2Store(mapPo.id);
    }

    store2Use(mapPo.id);
    return true;
}

bool MapControl::store2Use(const string &map_id) {
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

bool MapControl::use2Store(const string &map_id) {
    cppfs::FileHandle dir = cppfs::fs::open(path::robot_slam_map_dir() + map_id + path::separator());
    if (!dir.isDirectory())
        dir.createDirectory();

    cppfs::FileHandle nmy = cppfs::fs::open(path::map_yaml_path());
    cppfs::FileHandle nmp = cppfs::fs::open(path::map_pgm_path());
    cppfs::FileHandle nmpb = cppfs::fs::open(path::map_pbstream_path());
    cppfs::FileHandle nms = cppfs::fs::open(path::map_segmentation_path());
    cppfs::FileHandle npa = cppfs::fs::open(path::prohibition_areas_path());

    if (nmy.exists()) {
        nmy.copy(dir);
        nmy.remove();
    }
    if (nmp.exists()) {
        nmp.copy(dir);
        nmp.remove();
    }
    if (nmpb.exists()) {
        nmpb.copy(dir);
        nmpb.remove();
    }
    if (nms.exists()) {
        nms.copy(dir);
        nms.remove();
    }
    if (npa.exists()) {
        npa.copy(dir);
        npa.remove();
    }

    return true;
}

void MapControl::tt() {
    MapPo oldMap = SegmentationDataBase::instance().getDbMap();
    MapControl::instance().use2Store(oldMap.id);

    const MapPo &newMap = SegmentationDataBase::instance().installMap("tt");

    SegmentationDataBase::instance().loadMainMap();
    MapControl::instance().use2Store(newMap.id);

    ExplorationCenter::instance().repaintCoveragePath(true, true);
}
