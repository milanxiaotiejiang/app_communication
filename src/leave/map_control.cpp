//
// Created by Looper on 2023/3/6.
//

#include "leave/map_control.h"
#include "db/segmentation_data_base.h"
#include "db/task_data_base.h"
#include "cppfs/fs.h"
#include "cppfs/FilePath.h"
#include "cppfs/FileHandle.h"

void MapControl::changeMap(std::string map_id) {
    MapPo &po = SegmentationDataBase::instance().getDbMap();
    std::string name = po.name;

    cppfs::FileHandle dir = cppfs::fs::open(path::robot_slam_map_dir() + map_id);
    if (!dir.isDirectory())
        dir.createDirectory();

    cppfs::FileHandle nmy = cppfs::fs::open(path::map_yaml_path());
    cppfs::FileHandle nmp = cppfs::fs::open(path::map_pgm_path());
    cppfs::FileHandle nms = cppfs::fs::open(path::map_segmentation_path());
    cppfs::FileHandle npa = cppfs::fs::open(path::prohibition_areas_path());

    nmy.copy(dir);
    nmp.copy(dir);
    nms.copy(dir);
    npa.copy(dir);

}
