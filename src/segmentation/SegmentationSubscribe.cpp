//
// Created by Looper on 2022/9/27.
//

#include "segmentation/SegmentationSubscribe.h"
#include "segmentation/SegmentationCenter.h"
#include "model/RoomVo.h"
#include "BaseThrowable.h"

SegmentationSubscribe::SegmentationSubscribe(ros::NodeHandle handle) : handle(handle) {
    subscriber_segmentation_ = handle.subscribe("/segmentation", 3, &SegmentationSubscribe::subscribeCallback, this);
}

SegmentationSubscribe::~SegmentationSubscribe() {}

void SegmentationSubscribe::subscribeCallback(const std_msgs::Int32 &result) {
    return;
    LOG(INFO) << "SegmentationSubscribe -------------------------------------------------------------------";
    try {
        if (result.data == 0) {
            SegmentationCenter::instance().resetSegmentation();
        } else if (result.data == 1) {

            cv::Mat segmented_map;
            std::vector<Room> rooms;
            SegmentationCenter::instance().storage2Memory(segmented_map, rooms);

            if (rooms.size() == 2) {
                int targetId = rooms[0].getID();
                int mergeId = rooms[1].getID();
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
                SegmentationCenter::instance().toVoRoom(segmented_map, rooms);
            } else {
                LOG(ERROR) << "当前房间数量为：" << rooms.size() << "，不支持二合一操作";
            }
        } else if (result.data == 2) {

            if (SegmentationCenter::instance().checkPartition()) {
                cv::Mat segmented_map;
                std::vector<Room> rooms;
                SegmentationCenter::instance().storage2Memory(segmented_map, rooms);
                int targetId = rooms[0].getID();
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
                cv::Point ps(0, segmented_map.rows / 2);
                cv::Point pe(segmented_map.cols, segmented_map.rows / 2);
                SegmentationCenter::instance().handSegmentation(segmented_map, rooms, targetIndex, ps, pe);

                SegmentationCenter::instance().memory2Storage(segmented_map, rooms);
                SegmentationCenter::instance().toVoRoom(segmented_map, rooms);
            } else {
                cv::Mat segmented_map;
                std::vector<Room> rooms;
                segmented_map = SegmentationCenter::instance().generateMat();
                cv::Point ps(0, segmented_map.rows / 3);
                cv::Point pe(segmented_map.cols, segmented_map.rows / 2);
                SegmentationCenter::instance().originalSegmentation(segmented_map, rooms, ps, pe);
                SegmentationCenter::instance().memory2Storage(segmented_map, rooms);
                SegmentationCenter::instance().toVoRoom(segmented_map, rooms);
            }

        } else {
            LOG(ERROR) << "暂不支持其他指令";
        }
    } catch (app::exception const &e) {
        LOG(ERROR) << "SegmentationSubscribe app::exception : " << e.what();
    } catch (const std::exception &e) {
        LOG(ERROR) << "SegmentationSubscribe std::exception : " << e.what();
    } catch (...) {
        LOG(ERROR) << "SegmentationSubscribe other exception";
    }

    LOG(INFO) << "SegmentationSubscribe finish";
}
