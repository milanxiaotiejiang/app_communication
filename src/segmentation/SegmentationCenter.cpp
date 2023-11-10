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
#include "db/task_data_base.h"
#include "exploration/cv_extend.h"

static bool DEBUG_DISPLAYS_SHOW = false;

bool SegmentationCenter::detectionTooSmallRoom(const cv::Mat &segmented_map, Room room, const PlanPo &plan) const {
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

    explorationErode(zero_map, compute_map, cv::MORPH_CROSS, map_prohibition_expand_size_);

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

bool SegmentationCenter::pointInRoom(const cv::Mat &segmented_map, Room room, const cv::Point &point) const {
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

bool SegmentationCenter::initialize(const ros::NodeHandle &handle) {
    // 1.加载需要的地图的信息（仅地图信息）
    // MapControl::instance().initialize()
    // 2.根据地图的信息检查分区地图的数据完整性
    if (!checkPartition()) {
        resetSegmentation();
    }
    // 3.基站位置
    if (!MapAttributeSingleton::instance().loadStation()) {
        return false;
    }
    // 4.虚拟墙（与基站位置结合判断连通域）
    MapAttributeSingleton::instance().loadVirtualWall();
    // 5.禁区（生成全覆盖路径时需要）
    MapAttributeSingleton::instance().loadPenaltyZone();
    // 6.加载参数
    MapAttributeSingleton::instance().loadPlanParam();

    segmentationSubscribe = new SegmentationSubscribe(handle);

    multipleMapSubscribe = new MultipleMapSubscribe(handle);

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

    initialize_finish = true;
    return true;
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
    if (MapAttributeSingleton::instance().isCreatingMap()) {
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
                new_members.emplace_back(x, y);
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
    LOG_IF(INFO, DEBUG_SEGMENTATION) << room;//三者输出一致

    rooms.push_back(room);
    handSegmentation(segmented_map, rooms, 0, ps, pe);
}

void SegmentationCenter::handSegmentation(cv::Mat &segmented_map, std::vector<Room> &rooms, int target_index,
                                          const cv::Point &ps, const cv::Point &pe) {
    if (!initialize_finish) {
        throw app::exception(make_error_code(error::room_initialize_fail));
    }
    if (MapAttributeSingleton::instance().isCreatingMap()) {
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
    if (MapAttributeSingleton::instance().isCreatingMap()) {
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

void SegmentationCenter::reRoomName(int targetId, const std::string &name) {
    SegmentationDataBase::instance().reRoomName(targetId, name);
}

void SegmentationCenter::memory2Storage(cv::Mat &segmented_map, std::vector<Room> &rooms) {
    if (!initialize_finish) {
        throw app::exception(make_error_code(error::room_initialize_fail));
    }
    if (MapAttributeSingleton::instance().isCreatingMap()) {
        throw app::exception(make_error_code(error::in_creating_map));
    }
    SegmentationDataBase::instance().memory2Storage(segmented_map, rooms);
    ExplorationCenter::instance().repaintSubregionPath();
}

void SegmentationCenter::storage2Memory(cv::Mat &segmented_map, std::vector<Room> &rooms) {
    std::unique_lock<std::recursive_mutex> lock(cv_mut);

    if (!initialize_finish) {
        throw app::exception(make_error_code(error::room_initialize_fail));
    }
    if (MapAttributeSingleton::instance().isCreatingMap()) {
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
    if (MapAttributeSingleton::instance().isCreatingMap()) {
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

cv::Mat SegmentationCenter::choiceOneRoom(cv::Mat &segmented_map, std::vector<Room> &rooms, long targetId) {
    int targetIndex = -1;
    for (int i = 0; i < rooms.size(); ++i) {
        auto room = rooms[i];
        if (room.getDbId() == targetId) {
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

cv::Mat SegmentationCenter::generateMat() {

    std::unique_lock<std::recursive_mutex> lock(cv_mut);

    auto dbMap = SegmentationDataBase::instance().getDbMap();
    cv::Mat map = cv::imread(path::map_pgm_path().c_str(), cv::ImreadModes::IMREAD_GRAYSCALE);

    cv::normalize(map, map, 0, 255, cv::NORM_MINMAX);
//    map.convertTo(map, CV_8U);

    cv::rotate(map, map, cv::RotateFlags::ROTATE_90_COUNTERCLOCKWISE);

    //map中只包含 0 / 255
    for (int y = 0; y < map.rows; y++) {
        for (int x = 0; x < map.cols; x++) {
            if (map.at<unsigned char>(y, x) < 254) {
                map.at<unsigned char>(y, x) = 0;
            } else {
                map.at<unsigned char>(y, x) = 255;
            }
        }
    }

    auto type = map.type();
    auto cols = map.cols;//width
    auto rows = map.rows;//height

    const cv::Point &stationPoint = MapAttributeSingleton::instance().rosPoint2MapPoint(map, Point(0, 0));
    auto plan = SegmentationDataBase::instance().getDbPlan(SegmentationDataBase::instance().getDbMap().id);
    drawBaseStation(map, stationPoint, plan.range_near_base_station, cv::Scalar(255));

    return map;
}

bool SegmentationCenter::checkPartition() const {
    auto dbMap = SegmentationDataBase::instance().getDbMap();
    if (access(path::map_segmentation_path().c_str(), F_OK) == 0) {
        auto roomList = SegmentationDataBase::instance().selectRoomByMapId(dbMap.id);
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

MapRoomVo SegmentationCenter::resultMapRoomVo() const {

    cv::Mat segmented_map;
    std::vector<Room> rooms;
    SegmentationCenter::instance().storage2Memory(segmented_map, rooms);

    std::vector<RoomVo> roomVos;
    for (auto &item: rooms) {
        std::vector<PointVo> memberPoints;
        auto members = item.getMembers();
        for (const auto &member: members) {
            memberPoints.emplace_back(member.x, member.y);
        }

        std::vector<int> neighborRoomIds;
        auto neighbors = item.getNeighborIDs();
        for (const auto &neighbor: neighbors) {
            neighborRoomIds.push_back(neighbor);
        }
        auto center = item.getCenter();
        RoomVo roomVo(item.getDbId(), item.getName(),
                      PointVo(center.x, center.y), memberPoints, neighborRoomIds,
                      item.getArea(), item.getPerimeter());
        roomVos.push_back(roomVo);
    }

    return MapRoomVo(segmented_map.cols, segmented_map.rows, roomVos);
}

void
SegmentationCenter::isRestrictedZone(const cv::Mat &room_map, bool &isOffMap, bool &isRestrictedZone,
                                     bool &isMaxPassable, bool &isPlanPath, bool debug) {
    cv::Point2d map_origin = MapAttributeSingleton::instance().getMapOrigin();
    const cv::Point stationPoint = MapAttributeSingleton::instance().rosPoint2MapPoint(room_map, Point(0, 0));
    cv::Point robotPosition = MapAttributeSingleton::instance().getRobotPositionPoint(room_map);

    isOffMap = !pointInArea(room_map, stationPoint, robotPosition, false);


    cv::Mat prohibition_image = cv::Mat::zeros(room_map.rows, room_map.cols, CV_8UC1);
    int inProhibitionCount = 0;

    auto penaltyZoneList = MapAttributeSingleton::instance().getPenaltyZoneList();
    for (int i = 0; i < penaltyZoneList.size(); ++i) {
        std::vector<std::vector<cv::Point>> polygon_array;
        std::vector<cv::Point> cvPoints;
        auto vector = penaltyZoneList[i];
        for (int j = 0; j < vector.size(); ++j) {
            const cv::Point &point = MapAttributeSingleton::instance().rosPoint2MapPoint(prohibition_image, vector[j]);
            cvPoints.push_back(point);
        }
        polygon_array.push_back(cvPoints);
        cv::fillPoly(prohibition_image, polygon_array, cv::Scalar(255));

        cv::Mat prohibition_image_child = cv::Mat::zeros(room_map.rows, room_map.cols, CV_8UC1);

        if (pointInArea(prohibition_image_child, stationPoint, robotPosition, false)) {
            inProhibitionCount++;
        }
    }

    auto virtualWallList = MapAttributeSingleton::instance().getVirtualWallList();
    for (const auto &vector: virtualWallList) {
        if (vector.size() == 2) {
            const cv::Point &pointStart = MapAttributeSingleton::instance().rosPoint2MapPoint(prohibition_image,
                                                                                              vector[0]);
            const cv::Point &pointEnd = MapAttributeSingleton::instance().rosPoint2MapPoint(prohibition_image,
                                                                                            vector[1]);
            cv::line(prohibition_image, pointStart, pointEnd, cv::Scalar(255), 2);
        }
    }

//    isRestrictedZone = pointInArea(prohibition_image, stationPoint, robotPosition, false);
    isRestrictedZone = inProhibitionCount > 0;

    cv::Mat passable_map = room_map.clone();

    cv::Mat andMat;
    cv::bitwise_and(passable_map, prohibition_image, andMat);
    cv::bitwise_xor(passable_map, andMat, passable_map);
    isMaxPassable = pointInArea(passable_map, stationPoint, robotPosition, true);

    if (debug) {
        cv::imshow("room_map", room_map);
        cv::waitKey();
        cv::imshow("prohibition_image", prohibition_image);
        cv::waitKey();
        cv::imshow("passable_map", passable_map);
        cv::waitKey();
    }

    AStarPlanner path_planner;
    auto original_map = passable_map.clone();
    cv::Mat downsampled_map;
    path_planner.downsampleMap(original_map, downsampled_map, 1.0, 0.0, map_resolution_from_subscription);

    std::vector<cv::Point> current_path;
    double length = path_planner.planPath(original_map, downsampled_map, robotPosition,
                                          stationPoint, 1.0, 0.,
                                          map_resolution_from_subscription, 0, nullptr, &current_path);
    isPlanPath = length <= 1e90;

    LOG(INFO) << "  isOffMap : " << isOffMap
              << "  isRestrictedZone : " << isRestrictedZone
              << "  isMaxPassable : " << isMaxPassable
              << "  isPlanPath : " << isPlanPath;
}

struct TempPolygon {
    int contourSize;
    double stationPolygon;
    double pointPolygon;
};

bool SegmentationCenter::pointInArea(const cv::Mat &area_map, const cv::Point &stationPoint, const cv::Point &point,
                                     bool largest) const {
    bool inArea = false;
    auto map = area_map.clone();
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(map, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    std::vector<TempPolygon> records;

    for (auto &contour: contours) {
        TempPolygon tempPolygon{};
        tempPolygon.contourSize = contour.size();
        tempPolygon.stationPolygon = cv::pointPolygonTest(contour, stationPoint, true);;
        tempPolygon.pointPolygon = cv::pointPolygonTest(contour, point, true);;
        records.emplace_back(tempPolygon);
    }

    if (largest) {
        auto maxArea = 0;
        auto maxAreaDistance = 0;
        for (const auto &record: records) {
            if (record.pointPolygon > maxArea) {
                maxArea = record.pointPolygon;
                maxAreaDistance = record.pointPolygon;
            }
        }

        if (maxAreaDistance > 0) {
            inArea = true;
        }

        if (!inArea) {
            for (const auto &record: records) {
                if (record.pointPolygon >= 0 || record.stationPolygon >= 0) {
                    inArea = true;
                    break;
                }
            }
        }
    } else {
        for (const auto &record: records) {
            if (record.pointPolygon >= 0) {
                inArea = true;
                break;
            }
        }
    }
    return inArea;
}

Room SegmentationCenter::checkGateWire(cv::Mat &segmented_map, const Gate &gate) {
    if (!initialize_finish) {
        throw app::exception(make_error_code(error::room_initialize_fail));
    }
    if (MapAttributeSingleton::instance().isCreatingMap()) {
        throw app::exception(make_error_code(error::in_creating_map));
    }

    cv::Point ps(gate.start_x, gate.start_y);
    cv::Point pe(gate.end_x, gate.end_y);

    // 1.加载原始地图
    cv::Mat map = generateMat();
    map.convertTo(segmented_map, CV_32SC1, 256, 0);// rescale to 32 int, 255 --> 255*256 = 65280

    // 2.构建无分区的room
    Room base_room(rand() % 52224 + 13056);

    std::vector<cv::Point> new_members;
    for (int y = 0; y < map.rows; y++) {
        for (int x = 0; x < map.cols; x++) {
            if (map.at<unsigned char>(y, x) == 255) {
                new_members.emplace_back(x, y);
            }
        }
    }
    //寓意为第一次添加，可以添加所有，速度快
    base_room.directInsertMemberPoints(new_members, map_resolution_from_subscription);

    if (pointInRoom(segmented_map, base_room, ps) ||
        pointInRoom(segmented_map, base_room, pe)) {
        throw app::exception(make_error_code(error::room_both_ends_of_the_split_line_are_in_the_room));
    }

    if (!lineThroughRoom(segmented_map, base_room, ps, pe)) {
        throw app::exception(make_error_code(error::room_the_dividing_line_does_not_pass_through_the_room));
    }
    return base_room;
}

void SegmentationCenter::checkGatePoint(cv::Mat &segmented_map, std::vector<Room> &rooms, const Gate &gate) {

    Point gateLeftPoint(gate.left_position_x, gate.left_position_y);
    Point gateRightPoint(gate.right_position_x, gate.right_position_y);

    auto cvGateLeftPoint = MapAttributeSingleton::instance().rosPoint2MapPoint(segmented_map.rows,
                                                                               segmented_map.cols, gateLeftPoint);
    auto cvGateRightPoint = MapAttributeSingleton::instance().rosPoint2MapPoint(segmented_map.rows,
                                                                                segmented_map.cols, gateRightPoint);

    int leftValue = segmented_map.at<int>(cvGateLeftPoint);
    int rightValue = segmented_map.at<int>(cvGateRightPoint);

    if (leftValue == 0 || rightValue == 0) {
        throw app::exception(make_error_code(error::not_on_the_map));
    }
    if (leftValue == rightValue) {
        throw app::exception(make_error_code(error::the_ferry_point_is_in_the_same_area));
    }

    double distance = cv::norm(cvGateLeftPoint - cvGateRightPoint);
    if (distance > 100) {
        throw app::exception(make_error_code(error::gate_mark_points_too_far_away));
    }

    cv::Point ps(gate.start_x, gate.start_y);
    cv::Point pe(gate.end_x, gate.end_y);

//    cv::Vec4i lineGate(cvGateLeftPoint.x, cvGateLeftPoint.y, cvGateRightPoint.x, cvGateRightPoint.y);
//    cv::Vec4i lineThorough(ps.x, ps.y, pe.x, pe.y);
//
//    // 计算两条直线的方向向量
//    cv::Point2f dirGate(lineGate[2] - lineGate[0], lineGate[3] - lineGate[1]);
//    cv::Point2f dirThorough(lineThorough[2] - lineThorough[0], lineThorough[3] - lineThorough[1]);
//
//    // 计算两个方向向量的夹角（以度为单位）
//    double angleGate = atan2(dirGate.y, dirGate.x) * 180 / CV_PI;
//    double angleThorough = atan2(dirThorough.y, dirThorough.x) * 180 / CV_PI;
//
//    double angleDifference = std::abs(angleGate - angleThorough);
//
//    if (angleDifference < 80 || angleDifference > 100) {
//        throw app::exception(make_error_code(error::mark_points_as_perpendicular_as_possible_to_the_gate));
//    }

    int lineDiffY = pe.y - ps.y;
    int lineDiffX = pe.x - ps.x;
    int gateDiffY = cvGateRightPoint.y - cvGateLeftPoint.y;
    int gateDiffX = cvGateRightPoint.x - cvGateLeftPoint.x;

    double h1u = lineDiffY / sqrt(std::pow(lineDiffY, 2) + std::pow(lineDiffX, 2));
    double w1u = lineDiffX / sqrt(std::pow(lineDiffY, 2) + std::pow(lineDiffX, 2));
    double h2u = gateDiffY / sqrt(std::pow(gateDiffY, 2) + std::pow(gateDiffX, 2));
    double w2u = gateDiffX / sqrt(std::pow(gateDiffY, 2) + std::pow(gateDiffX, 2));

    auto angleThorough = (h1u * h2u + w1u * w2u) * 180 / CV_PI;

    // 检查角度差是否小于阈值，表示两条线接近垂直
    if (angleThorough > 10.0) {
        throw app::exception(make_error_code(error::mark_points_as_perpendicular_as_possible_to_the_gate));
    }

    cv::Point midPoint((cvGateLeftPoint.x + cvGateRightPoint.x) / 2, (cvGateLeftPoint.y + cvGateRightPoint.y) / 2);
    cv::Point lineVector = ps - pe;
    cv::Point pointVector = midPoint - pe;
    double minDistance = std::abs(pointVector.x * lineVector.y - pointVector.y * lineVector.x) /
                         std::sqrt(lineVector.x * lineVector.x + lineVector.y * lineVector.y);
    if (minDistance > 30) {
        throw app::exception(make_error_code(error::mark_points_in_the_gate_as_much_as_possible));
    }


    if (DEBUG_DISPLAYS_SHOW)
        whole_display(segmented_map, rooms, cvGateLeftPoint, cvGateRightPoint, "handSegmentation");
}

void SegmentationCenter::resetGateSegmentation() {
    MapPo &po = SegmentationDataBase::instance().getDbMap();
    SegmentationDataBase::instance().purgeGate(po.id);
}

void SegmentationCenter::gateSegmentation(cv::Mat &segmented_map, std::vector<Room> &rooms, const Gate &gate) {

    cv::Point ps(gate.start_x, gate.start_y);
    cv::Point pe(gate.end_x, gate.end_y);

    auto base_room = checkGateWire(segmented_map, gate);

    Room roomStart(rand() % 52224 + 13056);
    Room roomEnd(rand() % 52224 + 13056);
    std::vector<cv::Point> membersStart;
    std::vector<cv::Point> membersEnd;
    for (const auto &point: base_room.getMembers()) {
        int f = CvUtils::sideInLine(ps, pe, point);
        if (f > 0) {
            segmented_map.at<int>(point) = roomStart.getID();
            membersStart.push_back(point);
        } else {
            segmented_map.at<int>(point) = roomEnd.getID();
            membersEnd.push_back(point);
        }
    }

    // 同上
    roomStart.directInsertMemberPoints(membersStart, map_resolution_from_subscription);
    roomEnd.directInsertMemberPoints(membersEnd, map_resolution_from_subscription);

    rooms.push_back(roomStart);
    rooms.push_back(roomEnd);

    checkGatePoint(segmented_map, rooms, gate);
}

void SegmentationCenter::gateManySegmentation(cv::Mat &segmented_map, std::vector<Room> &rooms,
                                              std::map<std::pair<int, int>, std::pair<Gate, bool>> &planMap,
                                              const Gate &gate) {
    if (!initialize_finish) {
        throw app::exception(make_error_code(error::room_initialize_fail));
    }
    if (MapAttributeSingleton::instance().isCreatingMap()) {
        throw app::exception(make_error_code(error::in_creating_map));
    }

    cv::Mat map = generateMat().clone();

    cv::Point ps(gate.start_x, gate.start_y);
    cv::Point pe(gate.end_x, gate.end_y);


    if (rooms.empty()) {
        map.convertTo(segmented_map, CV_32SC1, 256, 0);// rescale to 32 int, 255 --> 255*256 = 65280

        Room room(rand() % 52224 + 13056);
        std::vector<cv::Point> new_members;
        for (int y = 0; y < map.rows; y++) {
            for (int x = 0; x < map.cols; x++) {
                if (map.at<unsigned char>(y, x) == 255) {
                    new_members.emplace_back(x, y);
                }
            }
        }
        //寓意为第一次添加，可以添加所有，速度快
        room.directInsertMemberPoints(new_members, map_resolution_from_subscription);
        rooms.push_back(room);
    }

    std::vector<std::pair<int, int>> roomPixelCounts;
    for (int i = 0; i < rooms.size(); i++) {
        auto &room = rooms[i];
        cv::Mat zero_map = cv::Mat::zeros(map.rows, map.cols, CV_8UC1);
        cv::drawContours(zero_map, std::vector<std::vector<cv::Point> >(1, room.getMembers()),
                         -1, cv::Scalar(255), CV_FILLED);

        int pixelCount = 0;
        cv::LineIterator it(zero_map, ps, pe, 8);
        for (int i = 0; i < it.count; i++, ++it) {
            if (*(*it) == 255) {
                pixelCount++;
            }
        }

        LOG_IF(INFO, DEBUG_GATE) << "直线是否穿越区域， 直线点位个数：" << it.count << " , 相交后点位个数：" << pixelCount;

        if (pixelCount > 0) {
            roomPixelCounts.emplace_back(i, pixelCount);
        }
    }

    if (roomPixelCounts.empty()) {
        throw app::exception(make_error_code(error::no_straight_line_crossing_map_area_detected));
    }

    std::sort(roomPixelCounts.begin(), roomPixelCounts.end(), [](const auto &a, const auto &b) {
        return a.second > b.second;
    });

    auto target_index = roomPixelCounts[0].first;
    auto base_room = rooms[target_index];
    int baseId = base_room.getID();

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

    Point gateLeftPoint(gate.left_position_x, gate.left_position_y);
    Point gateRightPoint(gate.right_position_x, gate.right_position_y);

    auto cvGateLeftPoint = MapAttributeSingleton::instance().rosPoint2MapPoint(segmented_map.rows,
                                                                               segmented_map.cols, gateLeftPoint);
    auto cvGateRightPoint = MapAttributeSingleton::instance().rosPoint2MapPoint(segmented_map.rows,
                                                                                segmented_map.cols, gateRightPoint);

    int leftValue = segmented_map.at<int>(cvGateLeftPoint);
    int rightValue = segmented_map.at<int>(cvGateRightPoint);

    int startId = roomStart.getID();
    int endId = roomEnd.getID();

    bool forwardDirection = true;
    if (startId == leftValue && endId == rightValue) {
        forwardDirection = true;
    } else if (startId == rightValue && endId == leftValue) {
        forwardDirection = false;
    } else {
        throw app::exception(make_error_code(error::gate_value_error));
    }

    // start -> end | start    end  
    // true  ====>    left  -> right
    // false ====>    right -> left
    std::map<std::pair<int, int>, std::pair<Gate, bool>> myPlanMap;
    for (const auto &plan: planMap) {
        std::pair<int, int> region = plan.first;
        std::pair<Gate, bool> gatePoint = plan.second;

        if (region.first == baseId) {
            region.first = obtainOriginalGatePointValue(segmented_map, gatePoint.first, true, gatePoint.second);
        } else if (region.second == baseId) {
            region.second = obtainOriginalGatePointValue(segmented_map, gatePoint.first, false, gatePoint.second);
        }
        myPlanMap[region] = gatePoint;
    }

    planMap.clear();

    for (const auto &plan: myPlanMap) {
        planMap[plan.first] = plan.second;
    }
    planMap[std::make_pair(startId, endId)] = std::make_pair(gate, forwardDirection);
    planMap[std::make_pair(endId, startId)] = std::make_pair(gate, !forwardDirection);

    if (DEBUG_DISPLAYS_SHOW)
        whole_display(segmented_map, rooms, "gateManySegmentation");
}

int SegmentationCenter::obtainOriginalGatePointValue(cv::Mat &segmented_map, const Gate originalGate,
                                                     double regionDirection, double pointDirection) {
    int recodeValue = 0;
    if (regionDirection) {
        if (pointDirection) {
            recodeValue = segmented_map.at<int>(
                    MapAttributeSingleton::instance().rosPoint2MapPoint(
                            segmented_map.rows, segmented_map.cols,
                            Point(originalGate.left_position_x, originalGate.left_position_y)
                    )
            );
        } else {
            recodeValue = segmented_map.at<int>(
                    MapAttributeSingleton::instance().rosPoint2MapPoint(
                            segmented_map.rows, segmented_map.cols,
                            Point(originalGate.right_position_x, originalGate.right_position_y)
                    )
            );
        }

    } else {
        if (pointDirection) {
            recodeValue = segmented_map.at<int>(
                    MapAttributeSingleton::instance().rosPoint2MapPoint(
                            segmented_map.rows, segmented_map.cols,
                            Point(originalGate.right_position_x, originalGate.right_position_y)
                    )
            );
        } else {
            recodeValue = segmented_map.at<int>(
                    MapAttributeSingleton::instance().rosPoint2MapPoint(
                            segmented_map.rows, segmented_map.cols,
                            Point(originalGate.left_position_x, originalGate.left_position_y)
                    )
            );
        }

    }
    return recodeValue;
}

void SegmentationCenter::gateManyOpen(cv::Mat &open_map, const Gate &gate) {
    if (!initialize_finish) {
        throw app::exception(make_error_code(error::room_initialize_fail));
    }
    if (MapAttributeSingleton::instance().isCreatingMap()) {
        throw app::exception(make_error_code(error::in_creating_map));
    }

    cv::Point ps(gate.start_x, gate.start_y);
    cv::Point pe(gate.end_x, gate.end_y);


    cv::line(open_map, ps, pe, cv::Scalar(50), 3, cv::LINE_8);

    Point gateLeftPoint(gate.left_position_x, gate.left_position_y);
    Point gateRightPoint(gate.right_position_x, gate.right_position_y);

    auto cvGateLeftPoint = MapAttributeSingleton::instance().rosPoint2MapPoint(open_map.rows,
                                                                               open_map.cols, gateLeftPoint);
    auto cvGateRightPoint = MapAttributeSingleton::instance().rosPoint2MapPoint(open_map.rows,
                                                                                open_map.cols, gateRightPoint);

    auto plan = SegmentationDataBase::instance().getDbPlan(SegmentationDataBase::instance().getDbMap().id);
    double grid_spacing_in_meter = plan.robot_radius * std::sqrt(2);//网格正方形的边长
    double grid_spacing_in_pixel = grid_spacing_in_meter / map_resolution_from_subscription;
    cv::line(open_map, cvGateLeftPoint, cvGateRightPoint, cv::Scalar(255), grid_spacing_in_pixel * 2, cv::LINE_8);

    if (DEBUG_DISPLAYS_SHOW) {
        cv::imshow("gateManyOpen", open_map);
        cv::waitKey();
    }
}
