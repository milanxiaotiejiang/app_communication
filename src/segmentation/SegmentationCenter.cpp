//
// Created by Looper on 2022/9/15.
//

#include "segmentation/SegmentationCenter.h"
#include <ros/time.h>
#include "db/segmentation_data_base.h"
#include "segmentation/VoronoiSegmentation.h"
#include "segmentation/handle_segmentation_display.h"
#include "segmentation/contains.h"
#include "segmentation/CvUtils.h"
#include "BaseThrowable.h"
#include "exploration/ExplorationCenter.h"
#include "simulation.h"
#include "segmentation/SegmentationSubscribe.h"

static bool DEBUG_DISPLAYS_SHOW = false;

bool SegmentationCenter::detectionTooSmallRoom(const cv::Mat &segmented_map, Room room, PlanPo plan) const {
    auto room_map = segmented_map.clone();
    cv::Mat zero_map = cv::Mat::zeros(room_map.rows, room_map.cols, CV_8UC1);
    cv::drawContours(zero_map, std::vector<std::vector<cv::Point> >(1, room.getMembers()),
                     -1, cv::Scalar(255), CV_FILLED);
    cv::Mat temp, compute_map;
    cv::erode(zero_map, temp, cv::Mat(), cv::Point(-1, -1), plan.map_correction_closing_neighborhood_size);
    cv::dilate(temp, zero_map, cv::Mat(), cv::Point(-1, -1), plan.map_correction_closing_neighborhood_size);

    double grid_spacing_in_meter = plan.robot_radius * std::sqrt(2);//0.565685 网格正方形的边长
    double grid_spacing_in_pixel = grid_spacing_in_meter / map_resolution_from_subscription;
    int map_prohibition_expand_size_ = (int) std::floor(grid_spacing_in_pixel);

    cv::erode(zero_map, compute_map, cv::Mat(), cv::Point(-1, -1), map_prohibition_expand_size_);

    cv::Mat room_map_int(room_map.rows, room_map.cols, CV_32SC1);
    for (int v = 0; v < compute_map.rows; ++v) {
        for (int u = 0; u < compute_map.cols; ++u) {
            if (compute_map.at<uchar>(v, u) == 255)
                room_map_int.at<int32_t>(v, u) = -100;
            else
                room_map_int.at<int32_t>(v, u) = 0;
        }
    }

    std::map<int, int> area_to_label_map;
    int label = 1;
    for (int v = 0; v < room_map_int.rows; ++v) {
        for (int u = 0; u < room_map_int.cols; ++u) {
            if (room_map_int.at<int32_t>(v, u) == -100) {
                const int area = cv::floodFill(room_map_int, cv::Point(u, v),
                                               cv::Scalar(label), 0, 0, 0,
                                               8 | cv::FLOODFILL_FIXED_RANGE);
                area_to_label_map[area] = label;
                ++label;
            }
        }
    }

    return !area_to_label_map.empty();
}

bool SegmentationCenter::pointInRoom(const cv::Mat &segmented_map, Room room, cv::Point point) const {
    cv::Mat zero_map = cv::Mat::zeros(segmented_map.rows, segmented_map.cols, CV_8UC1);
    cv::drawContours(zero_map, std::vector<std::vector<cv::Point> >(1, room.getMembers()),
                     -1, cv::Scalar(255), CV_FILLED);
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(zero_map, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    for (auto &contour: contours) {
        double d = cv::pointPolygonTest(contour, point, false);
        if (d >= 0) {
            return true;
        }
    }
    return false;
}

bool SegmentationCenter::lineThroughRoom(const cv::Mat &segmented_map, Room room, const cv::Point &ps,
                                         const cv::Point &pe) const {
    cv::Mat zero_map = cv::Mat::zeros(segmented_map.rows, segmented_map.cols, CV_8UC1);
    cv::drawContours(zero_map, std::vector<std::vector<cv::Point> >(1, room.getMembers()),
                     -1, cv::Scalar(255), CV_FILLED);
    cv::Mat line_map = cv::Mat::zeros(segmented_map.rows, segmented_map.cols, CV_8UC1);
    cv::line(line_map, ps, pe, cv::Scalar(255), 1);
    cv::Mat and_map;
    cv::bitwise_and(zero_map, line_map, and_map);
    cv::Mat or_map;
    cv::bitwise_and(and_map, zero_map, or_map);

    int or_member_size = 0;
    for (int y = 0; y < or_map.rows; y++) {
        for (int x = 0; x < or_map.cols; x++) {
            if (or_map.at<unsigned char>(y, x) == 255) {
                or_member_size++;
            }
        }
    }
    return or_member_size > 0;
}

void SegmentationCenter::initialize(ros::NodeHandle handle) {
    ros::Time::init();
    // 1.加载需要的地图的信息（仅地图信息）
    initialize_finish = SegmentationDataBase::instance().loadMap();
    if (!initialize_finish) {
        LOG(ERROR) << "data DB Map load fail !";
        return;
    }
    // 2.根据地图的信息检查分区地图的数据完整性
    if (!checkPartition()) {
        resetSegmentation();
    }
    // 3.基站位置
    MapAttribute::instance().loadStation();
    // 4.虚拟墙（与基站位置结合判断连通域）
    MapAttribute::instance().loadVirtualWall();
    // 5.禁区（生成全覆盖路径时需要）
    MapAttribute::instance().loadPenaltyZone();
    // 6.加载参数
    MapAttribute::instance().loadPlanParam();

    segmentationSubscribe = new SegmentationSubscribe(handle);

    // test
//    resetSegmentation();

//    cv::Mat map = generateMat();
//    cv::Point ps(0, map.rows / 2);
//    cv::Point pe(map.cols, map.rows / 2);

//    cv::Mat segmented_map_1;
//    std::vector<Room> rooms_1;
//    originalSegmentation(segmented_map_1, rooms_1, ps, pe);
//    memory2Storage(segmented_map_1, rooms_1);

//    cv::Mat segmented_map_2;
//    std::vector<Room> rooms_2;
//    storage2Memory(segmented_map_2, rooms_2);
//    mergeRoom(segmented_map_2, rooms_2, 0, 1);
//    memory2Storage(segmented_map_2, rooms_2);

//    generateMat();

//    cv::Mat segmented_map;
//    std::vector<Room> rooms;
//    automaticSegmentation(segmented_map, rooms);
}

void SegmentationCenter::resetSegmentation() {
    auto dbMap = SegmentationDataBase::instance().getDbMap();
    SegmentationDataBase::instance().removeAllRoom(dbMap.id);

    if (access(path::map_segmentation_path().c_str(), F_OK) == 0) {//存在
        std::remove(path::map_segmentation_path().c_str());
    }
}

void SegmentationCenter::originalSegmentation(cv::Mat &segmented_map, std::vector<Room> &rooms,
                                              const cv::Point &ps, const cv::Point &pe) {
    if (!initialize_finish) {
        throw app::exception(make_error_code(error::room_initialize_fail));
    }
    if (MapAttribute::instance().isCreatingMap()) {
        throw app::exception(make_error_code(error::in_creating_map));
    }
    // 1.加载原始地图
    cv::Mat map = generateMat();
    map.convertTo(segmented_map, CV_32SC1, 256, 0);// rescale to 32 int, 255 --> 255*256 = 65280

    if (DEBUG_DISPLAYS_SHOW) {
        cv::Mat roomMat = map.clone();
        cv::line(roomMat, ps, pe, cv::Scalar(127), 2);
        cv::imshow("roomMat", roomMat);
        cv::waitKey();
    }

    // 2.构建无分区的room
    Room room(rand() % 52224 + 13056);

//    int start_time = ros::Time::now().sec;
    //单个点添加
//    for (int y = 0; y < map.rows; y++) {
//        for (int x = 0; x < map.cols; x++) {
//            if (map.at<unsigned char>(y, x) == 255) {
//                room.insertMemberPoint(
//                        cv::Point(x, y),
//                        map_resolution_from_subscription);
//            }
//        }
//    }
//    int end_time = ros::Time::now().sec;
//        std::cout << "cost " << end_time - start_time << " s " << std::endl;//32 s
    //添加数组
//    std::vector<cv::Point> new_members;
//    for (int y = 0; y < map.rows; y++) {
//        for (int x = 0; x < map.cols; x++) {
//            if (map.at<unsigned char>(y, x) == 255) {
//                new_members.push_back(cv::Point(x, y));
//            }
//        }
//    }
//    room.insertMemberPoints(new_members, map_resolution_from_subscription);
//    int end_time = ros::Time::now().sec;
//        std::cout << "cost " << end_time - start_time << " s " << std::endl;//32 s
    //自定义添加
    std::vector<cv::Point> new_members;
    for (int y = 0; y < map.rows; y++) {
        for (int x = 0; x < map.cols; x++) {
            if (map.at<unsigned char>(y, x) == 255) {
                new_members.push_back(cv::Point(x, y));
            }
        }
    }
    //寓意为第一次添加，可以添加所有，速度快
    room.directInsertMemberPoints(new_members, map_resolution_from_subscription);
//    int end_time = ros::Time::now().sec;
//    std::cout << "cost " << end_time - start_time << " s " << std::endl;//0 s

    //id_number_: 1 member_points_: 61651 neighbor_room_ids_: 0 neighbor_room_statistics_: 0 room_area_: 154.127 room_perimeter_: 0
    //id_number_: 1 member_points_: 61651 neighbor_room_ids_: 0 neighbor_room_statistics_: 0 room_area_: 154.128 room_perimeter_: 0
    //id_number_: 1 member_points_: 61651 neighbor_room_ids_: 0 neighbor_room_statistics_: 0 room_area_: 154.128 room_perimeter_: 0
    LOG(INFO) << room;//三者输出一致

    rooms.push_back(room);
    handSegmentation(segmented_map, rooms, 0, ps, pe);
}

void SegmentationCenter::handSegmentation(cv::Mat &segmented_map, std::vector<Room> &rooms, int target_index,
                                          const cv::Point &ps, const cv::Point &pe) {
    if (!initialize_finish) {
        throw app::exception(make_error_code(error::room_initialize_fail));
    }
    if (MapAttribute::instance().isCreatingMap()) {
        throw app::exception(make_error_code(error::in_creating_map));
    }
    if (target_index < 0 || target_index >= rooms.size()) {
        throw app::exception(make_error_code(error::room_array_out_of_bounds));
    }

    auto plan = SegmentationDataBase::instance().getDbPlan(SegmentationDataBase::instance().getDbMap().id);

    // 1.移除要分割的房间
    auto base_room = rooms[target_index];


    if (pointInRoom(segmented_map, base_room, ps) ||
        pointInRoom(segmented_map, base_room, pe)) {
        throw app::exception(make_error_code(error::room_both_ends_of_the_split_line_are_in_the_room));
    }


    if (!lineThroughRoom(segmented_map, base_room, ps, pe)) {
        throw app::exception(make_error_code(error::room_the_dividing_line_does_not_pass_through_the_room));
    }

    rooms.erase(rooms.begin() + target_index);

    // 2.初始化分割后的两个房间
    Room roomStart(rand() % 52224 + 13056);
    Room roomEnd(rand() % 52224 + 13056);
    std::vector<cv::Point> membersStart;
    std::vector<cv::Point> membersEnd;
    for (const auto &point: base_room.getMembers()) {
        int f = CvUtils::sideInLine(ps, pe, point);
        if (f > 0) {
            segmented_map.at<int>(point) = roomStart.getID();
            membersStart.push_back(point);
//            roomStart.insertMemberPoint(point, map_resolution_from_subscription);
        } else {
            segmented_map.at<int>(point) = roomEnd.getID();
            membersEnd.push_back(point);
//            roomEnd.insertMemberPoint(point, map_resolution_from_subscription);
        }
    }

    // 同上
    roomStart.directInsertMemberPoints(membersStart, map_resolution_from_subscription);
    roomEnd.directInsertMemberPoints(membersEnd, map_resolution_from_subscription);

    rooms.push_back(roomStart);
    rooms.push_back(roomEnd);

    if (!detectionTooSmallRoom(segmented_map, roomStart, plan) ||
        !detectionTooSmallRoom(segmented_map, roomEnd, plan)) {
        throw app::exception(make_error_code(error::room_has_too_small_room));
    }

    int start_time = ros::Time::now().sec;

    // 3.为每个点添加邻居ID
    for (int current_room = 0; current_room < rooms.size(); current_room++) {
        const int current_id = rooms[current_room].getID();
        const std::vector<cv::Point> &current_points = rooms[current_room].getMembers();

        std::vector<cv::Point> considered_neighbors;        // storage for already counted neighborhood points
        for (int current_point = 0; current_point < current_points.size(); current_point++) {
            auto cv_point = current_points[current_point];
            int x = cv_point.x;
            int y = cv_point.y;

            for (int row_counter = -1; row_counter <= 1; row_counter++) {
                for (int col_counter = -1; col_counter <= 1; col_counter++) {
                    if (row_counter == 0 && col_counter == 0) {
                        continue;
                    }
                    const int label = segmented_map.at<int>(y + row_counter, x + col_counter);
                    if (label == current_id) {
                        continue;
                    }
                    // 收集邻居ID
                    if (label != 0)
                        rooms[current_room].addNeighborID(label);

                    // 邻域统计
                    cv::Point neighbor_point(x + col_counter, y + row_counter);

                    if (!contains(considered_neighbors, neighbor_point)) {
                        rooms[current_room].addNeighbor(label);
                        considered_neighbors.push_back(neighbor_point);
                    }
                }
            }
        }
    }//10s

    int end_time = ros::Time::now().sec;
    std::cout << "cost handSegmentation : " << end_time - start_time << " s " << std::endl;

    if (DEBUG_DISPLAYS_SHOW)
        whole_display(segmented_map, rooms, "handSegmentation");
}

void SegmentationCenter::mergeRoom(cv::Mat &segmented_map, std::vector<Room> &rooms, int target_index,
                                   int room_to_merge_index) {
    if (!initialize_finish) {
        throw app::exception(make_error_code(error::room_initialize_fail));
    }
    if (MapAttribute::instance().isCreatingMap()) {
        throw app::exception(make_error_code(error::in_creating_map));
    }
    if (target_index < 0 || target_index >= rooms.size() ||
        room_to_merge_index < 0 || room_to_merge_index >= rooms.size()) {
        throw app::exception(make_error_code(error::room_array_out_of_bounds));
    }
    auto targetRoom = rooms[target_index];
    auto mergeRoom = rooms[room_to_merge_index];

    auto targetIt = find(targetRoom.getNeighborIDs().begin(), targetRoom.getNeighborIDs().end(), mergeRoom.getID());
    if (targetIt == targetRoom.getNeighborIDs().end()) {
        LOG(ERROR) << target_index << " Room NeighborIDs not contain " << room_to_merge_index << " Room .";
        throw app::exception(make_error_code(error::room_not_neighbor));
    }

    auto mergeIt = find(mergeRoom.getNeighborIDs().begin(), mergeRoom.getNeighborIDs().end(), targetRoom.getID());
    if (mergeIt == targetRoom.getNeighborIDs().end()) {
        LOG(ERROR) << room_to_merge_index << " Room NeighborIDs not contain " << target_index << " Room .";
        throw app::exception(make_error_code(error::room_not_neighbor));
    }

    int start_time = ros::Time::now().sec;

    // 集成房间以合并到目标并删除合并的房间
    const int target_id = rooms[target_index].getID();
    const int room_to_merge_id = rooms[room_to_merge_index].getID();
    rooms[target_index].mergeRoom(rooms[room_to_merge_index], map_resolution_from_subscription);
    rooms[room_to_merge_index].setRoomId(rooms[target_index].getID(), segmented_map);
    rooms.erase(rooms.begin() + room_to_merge_index);
    std::sort(rooms.begin(), rooms.end(), sortRoomsAscending);

    // 更新剩余房间的邻居统计信息
    for (size_t i = 0; i < rooms.size(); ++i) {
        std::vector<int> &neighbor_ids = rooms[i].getNeighborIDs();
        std::vector<int>::iterator it = std::find(neighbor_ids.begin(), neighbor_ids.end(), room_to_merge_id);
        if (it != neighbor_ids.end()) {
            std::vector<int>::iterator it2 = std::find(neighbor_ids.begin(), neighbor_ids.end(), target_id);
            if (it2 != neighbor_ids.end())
                neighbor_ids.erase(it);
            else
                *it = target_id;
        }

        std::map<int, int> &neighbor_statistics = rooms[i].getNeighborStatistics();
        std::map<int, int>::iterator it3 = neighbor_statistics.find(room_to_merge_id);
        if (it3 != neighbor_statistics.end()) {
            std::map<int, int>::iterator it4 = neighbor_statistics.find(target_id);
            if (it4 != neighbor_statistics.end())
                it4->second += it3->second;
            else
                neighbor_statistics[target_id] = it3->second;
            neighbor_statistics.erase(it3);
        }
    }

    int end_time = ros::Time::now().sec;
    std::cout << "cost mergeRoom :" << end_time - start_time << " s " << std::endl;

    if (DEBUG_DISPLAYS_SHOW)
        whole_display(segmented_map, rooms, "mergeRoom");
}

void SegmentationCenter::reRoomName(int targetId, std::string name) {
    SegmentationDataBase::instance().reRoomName(targetId, name);
}

void SegmentationCenter::memory2Storage(cv::Mat &segmented_map, std::vector<Room> &rooms) {
    if (!initialize_finish) {
        throw app::exception(make_error_code(error::room_initialize_fail));
    }
    if (MapAttribute::instance().isCreatingMap()) {
        throw app::exception(make_error_code(error::in_creating_map));
    }
    SegmentationDataBase::instance().memory2Storage(segmented_map, rooms);
}

void SegmentationCenter::storage2Memory(cv::Mat &segmented_map, std::vector<Room> &rooms) {
    if (!initialize_finish) {
        throw app::exception(make_error_code(error::room_initialize_fail));
    }
    if (MapAttribute::instance().isCreatingMap()) {
        throw app::exception(make_error_code(error::in_creating_map));
    }
    SegmentationDataBase::instance().storage2Memory(segmented_map, rooms, map_resolution_from_subscription);

    if (DEBUG_DISPLAYS_SHOW)
        whole_display(segmented_map, rooms, "storage2Memory");
}

void SegmentationCenter::automaticSegmentation(cv::Mat &segmented_map, std::vector<Room> &rooms) {
    if (!initialize_finish) {
        throw app::exception(make_error_code(error::room_initialize_fail));
    }
    if (MapAttribute::instance().isCreatingMap()) {
        throw app::exception(make_error_code(error::in_creating_map));
    }

    auto plan = SegmentationDataBase::instance().getDbPlan(SegmentationDataBase::instance().getDbMap().id);

    int start_time = ros::Time::now().sec;

    cv::Mat map = generateMat();

    VoronoiSegmentation voronoi_segmentation; //voronoi segmentation method
    voronoi_segmentation.segmentMap(map, segmented_map, rooms, map_resolution_from_subscription,
                                    plan.room_area_factor_lower_limit, plan.room_area_factor_upper_limit,
                                    plan.neighborhood_index, plan.max_iterations,
                                    plan.min_critical_point_distance_factor, plan.max_area_for_merging);

    calculation_center_point(segmented_map, rooms, plan.robot_radius, map_resolution_from_subscription);

    int end_time = ros::Time::now().sec;
    std::cout << "cost handSegmentation : " << end_time - start_time << " s " << std::endl;

    if (DEBUG_DISPLAYS_SHOW)
        whole_display(segmented_map, rooms, "automaticPartitioning");

}

cv::Mat SegmentationCenter::choiceOneRoom(cv::Mat &segmented_map, std::vector<Room> &rooms, int targetId) {
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

    Room room = rooms[targetIndex];
    std::vector<cv::Point> members = room.getMembers();
    std::vector<std::vector<cv::Point>> contours = {members};

    cv::Mat image = cv::Mat::zeros(segmented_map.rows, segmented_map.cols, CV_8UC1);
    for (const auto &point: room.getMembers()) {
        image.at<uchar>(point) = 255;
    }
    return image;
}

cv::Mat SegmentationCenter::generateMat() const {
    auto dbMap = SegmentationDataBase::instance().getDbMap();
    cv::Mat map = cv::imread(path::map_pgm_path().c_str(), cv::ImreadModes::IMREAD_GRAYSCALE);

    cv::normalize(map, map, 0, 255, cv::NORM_MINMAX);
//    map.convertTo(map, CV_8U);

    cv::rotate(map, map, cv::RotateFlags::ROTATE_90_COUNTERCLOCKWISE);

    //map中只包含 0 / 255
    for (int y = 0; y < map.rows; y++) {
        for (int x = 0; x < map.cols; x++) {
            if (map.at<unsigned char>(y, x) != 255) {
                map.at<unsigned char>(y, x) = 0;
            }
        }
    }

    auto type = map.type();
    auto cols = map.cols;//width
    auto rows = map.rows;//height

    return map;
}

bool SegmentationCenter::checkPartition() const {
    auto dbMap = SegmentationDataBase::instance().getDbMap();
    if (access(path::map_segmentation_path().c_str(), F_OK) == 0) {
        auto roomList = SegmentationDataBase::instance().selectByMapId(dbMap.id);
        if (roomList.empty()) {
            return false;
        }
        std::map<int, int> valueNumberMap;
        for (const auto &item: roomList) {
            int value = valueNumberMap[item.value];
            valueNumberMap[item.value] = value + 1;
        }
        for (std::map<int, int>::iterator iter = valueNumberMap.begin(); iter != valueNumberMap.end(); iter++) {
            if (iter->second > 1) {
                return false;
            }
        }
        return true;
    } else {
        return false;
    }
}

MapRoomVo SegmentationCenter::toVoRoom(cv::Mat &segmented_map, std::vector<Room> &rooms) const {
    std::vector<RoomVo> roomVos;
    for (auto &item: rooms) {
        std::vector<PointVo> memberPoints;
        auto members = item.getMembers();
        for (const auto &member: members) {
            memberPoints.push_back(PointVo(member.x, member.y));
        }

        std::vector<int> neighborRoomIds;
        auto neighbors = item.getNeighborIDs();
        for (const auto &neighbor: neighbors) {
            neighborRoomIds.push_back(neighbor);
        }
        auto center = item.getCenter();
        RoomVo roomVo(item.getID(), item.getName(),
                      PointVo(center.x, center.y), memberPoints, neighborRoomIds,
                      item.getArea(), item.getPerimeter());
        roomVos.push_back(roomVo);
    }
    return MapRoomVo(segmented_map.cols, segmented_map.rows, roomVos);
}
