//
// Created by Looper on 2022/9/16.
//

#include "segmentation/SegmentationStrategy.h"
#include "segmentation/SegmentationCenter.h"

MapRoomVo RoomMapDataStrategy::handler(string params) {

    if (SegmentationCenter::instance().checkPartition()) {
        cv::Mat segmented_map;
        std::vector<Room> rooms;
        SegmentationCenter::instance().storage2Memory(segmented_map, rooms);
        return SegmentationCenter::instance().toVoRoom(segmented_map, rooms);
    } else {
        throw app::exception(make_error_code(error::room_no_partition));
    }
}

MapRoomVo RoomMergeStrategy::handler(RoomMergeTarget params) {
    cv::Mat segmented_map;
    std::vector<Room> rooms;
    SegmentationCenter::instance().storage2Memory(segmented_map, rooms);

    int targetId = params.getTargetId();
    int mergeId = params.getMergeId();
    int targetIndex, mergeIndex = -1;
    for (int i = 0; i < rooms.size(); ++i) {
        auto room = rooms[i];
        if (room.getID() == targetId) {
            targetIndex = i;
        }
        if (room.getID() == mergeId) {
            mergeIndex = i;
        }
    }
    if (targetIndex == -1 || mergeIndex == -1) {
        throw app::exception(make_error_code(error::room_no_find_room_id));
    }

    SegmentationCenter::instance().mergeRoom(segmented_map, rooms, targetIndex, mergeIndex);

    SegmentationCenter::instance().memory2Storage(segmented_map, rooms);
    return SegmentationCenter::instance().toVoRoom(segmented_map, rooms);
}

MapRoomVo RoomSegmentationStrategy::handler(RoomSegmentationTarget params) {
    const cv::Point ps(params.getPs().getX(), params.getPs().getY());
    const cv::Point pe(params.getPe().getX(), params.getPe().getY());

    cv::Mat segmented_map;
    std::vector<Room> rooms;

    if (params.getTargetId() == ORIGINAL_SEGMENTATION) {
        segmented_map = SegmentationCenter::instance().generateMat();
        SegmentationCenter::instance().originalSegmentation(segmented_map, rooms, ps, pe);
    } else {
        SegmentationCenter::instance().storage2Memory(segmented_map, rooms);
        int targetId = params.getTargetId();
        int targetIndex = -1;
        for (int i = 0; i < rooms.size(); ++i) {
            auto room = rooms[i];
            if (room.getID() == targetId) {
                targetIndex = i;
            }
        }
        if (targetIndex == -1) {
            throw app::exception(make_error_code(error::room_no_find_room_id));
        }
        SegmentationCenter::instance().handSegmentation(segmented_map, rooms, targetIndex, ps, pe);
    }

    SegmentationCenter::instance().memory2Storage(segmented_map, rooms);
    return SegmentationCenter::instance().toVoRoom(segmented_map, rooms);
}

void RoomResetStrategy::handler() {
    SegmentationCenter::instance().resetSegmentation();
}

string RoomRenameStrategy::handler(RoomNameTarget params) {
    SegmentationCenter::instance().reRoomName(params.getTargetId(), params.getName());
    return "";
}

MapRoomVo AutoSegmentationStrategy::handler(string params) {
    SegmentationCenter::instance().resetSegmentation();

    cv::Mat segmented_map;
    std::vector<Room> rooms;
    SegmentationCenter::instance().automaticSegmentation(segmented_map, rooms);

    SegmentationCenter::instance().memory2Storage(segmented_map, rooms);

    return SegmentationCenter::instance().toVoRoom(segmented_map, rooms);
}
