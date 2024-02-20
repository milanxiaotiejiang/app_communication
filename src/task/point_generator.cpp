//
// Created by Looper on 2022/11/5.
//

#include "task/point_generator.h"

#include "exploration/ExplorationCenter.h"
#include "BaseThrowable.h"
#include "segmentation/map_attribute.h"
#include <Eigen/Dense>
#include <eigen_conversions/eigen_msg.h>
#include "exploration/radian_angle_conversion.h"
#include "exploration/line.h"
#include "tool/Variable.h"
#include "segmentation/SegmentationCenter.h"
#include "exploration/path_exploration_preview_task.h"
#include "db/segmentation_data_base.h"
#include "geometry_msgs/Polygon.h"
#include "task/point_planner.h"

#include "simulation.h"

RealBlock PointGenerator::buildBlock(int id, const RealTask &task) {
    RealBlock block;
    block.id = id;
    block.taskId = task.getId();

    block.newTaskId = task.getTaskId();

    block.name = task.getName();
    block.rate = task.getRate();
    block.totalFrequency = task.getRate();
    block.mode = task.getMode();

    block.knife = task.isKnife();
    block.work_status = task.getWorkStatus();
    return block;
}

void PointGenerator::complexPathToRealBlock(RealTask &realTask,
                                            const std::vector<std::vector<PoseMo>> &complexList,
                                            std::vector<RealBlock> &blockList) {
    float totalDistance = 0;
    // 获取当前地图的原点位置，以便后续转换 cv 点和 ros 点
    auto originPoint = MapAttributeSingleton::instance().getMapOrigin();

    // 对于单个点列大于 complex_path_num_splits 值的，进行近似平均的拆分
    std::vector<std::vector<PoseMo>> splitVectors;
    for (const auto &vec: complexList) {
        //complex_path_num_splits 默认 1000
        int numSubVec = (vec.size() + Environment::instance().complex_path_num_splits - 1) /
                        Environment::instance().complex_path_num_splits;

        int remainder = vec.size() % numSubVec;
        int sizePerSubVec = vec.size() / numSubVec;
        int startIdx = 0;

        for (int i = 0; i < numSubVec; ++i) {
            int endIdx = startIdx + sizePerSubVec + (i < remainder ? 1 : 0);
            std::vector<PoseMo> subVec(vec.begin() + startIdx, vec.begin() + endIdx);
            splitVectors.push_back(subVec);
            startIdx = endIdx;
        }
    }

    // 根据前后两个点位，重新计算每个点位的角度值
    std::vector<std::vector<PoseMo>> complexAngleList;
    for (const auto &complex: splitVectors) {
        std::vector<PoseMo> poseList = recalculateAngle(originPoint, complex);
        complexAngleList.push_back(poseList);
    }

    // 将 PoseVo 转为 geometry_msgs::PoseStamped，主要是将 PoseVo 中 theta 转为 pose.orientation
    std::vector<std::vector<PoseStamped>> complexGeometryList;
    for (const auto &complex: complexAngleList) {
        std::vector<PoseStamped> geometryList = convertToGeometry(complex);
        complexGeometryList.push_back(geometryList);
    }

    // 将 geometry_msgs::PoseStamped 集合转为 Block 点列，一个 Block 中包含 1 或很多点，其中 geometry_msgs::PoseStamped 转为 RealPosition + RealOrientation
    std::vector<RealBlock> initBlockList;
    for (const auto &complex: complexGeometryList) {
        auto realBlock = buildBlock(0, realTask);

        for (const auto &pose: complex) {
            auto geometry_msgs_pose = pose.poseStamped.pose;
            RealPoint realPoint;
            RealPosition realPosition(geometry_msgs_pose.position.x, geometry_msgs_pose.position.y,
                                      geometry_msgs_pose.position.z);
            RealOrientation realOrientation(geometry_msgs_pose.orientation.x, geometry_msgs_pose.orientation.y,
                                            geometry_msgs_pose.orientation.z, geometry_msgs_pose.orientation.w);
            realPoint.realPosition = std::move(realPosition);
            realPoint.realOrientation = std::move(realOrientation);
            realPoint.cmcMode = pose.cmcMode;
            realBlock.plannerPoints.push_back(realPoint);
        }

        initBlockList.push_back(realBlock);
    }

    // 根据任务次数追加 block ，并设置总进度
    std::vector<RealBlock> rateBlockList;
    for (int i = 0; i < realTask.getRate(); ++i) {
        for (auto &block: initBlockList) {
            if (!block.plannerPoints.empty()) {
                block.currentFrequency = i + 1;
                rateBlockList.push_back(block);
            }
        }
    }

    // 重点关注第一个点，第一个点有重试逻辑，且后续都应当将第一个点剥离出来，每次前往单个点与整个路径的行使规划参数不同
    std::vector<RealBlock> wholeBlockList;
    if (!rateBlockList.empty()) {
        const std::vector<RealPoint> &plannerPoints = rateBlockList[0].plannerPoints;
        if (!plannerPoints.empty()) {
            RealPoint point = plannerPoints[0];
            point.cmcMode = CmcMode::Omission;
            addSinglePoint(wholeBlockList, realTask, point);
        }
    }

    // 剥离下一个 block 中的第一个点，形成 block，以便动态调整规划器参数。注意处理最后一个 block 问题
    for (int i = 0; i < rateBlockList.size(); i++) {
        if (i == rateBlockList.size() - 1) {
            auto currentBlock = rateBlockList[i];
            wholeBlockList.push_back(currentBlock);
        } else {
            auto currentBlock = rateBlockList[i];
            auto nextBlock = rateBlockList[i + 1];

            wholeBlockList.push_back(currentBlock);
            RealPoint point = nextBlock.plannerPoints[0];
            point.cmcMode = CmcMode::Omission;
            addSinglePoint(wholeBlockList, realTask, point);
        }
    }

//    addSinglePoint(wholeBlockList, realTask, PointPlanner::createBackBasePoint());

    // 统计 block_accumulation , point_accumulation 以便计算进度
    int block_accumulation = 0;
    int point_accumulation = 0;
    for (auto &block: wholeBlockList) {
        block.id = block_accumulation;

        int step_accumulation = 0;
        for (auto &point: block.plannerPoints) {
            step_accumulation++;
            point.id = point_accumulation;
            point.blockId = block_accumulation;
            point.currentStep = step_accumulation;
            point_accumulation++;
        }
        block_accumulation++;
    }

    // 计算两点间距，得出超时时间，并计算总共时间、总步数，此处取出当前的机器人位置，为计算第一个超时时间处理
    geometry_msgs::Pose2D pose2D = MapAttributeSingleton::instance().getRobotPositionPose();
    geometry_msgs::Pose::_position_type lastPose;
    lastPose.x = pose2D.x;
    lastPose.y = pose2D.y;
    for (auto &block: wholeBlockList) {
        auto plannerPoints = block.plannerPoints;

        long timeout_accumulation = 0;
        for (auto &pose: plannerPoints) {
            geometry_msgs::Pose::_position_type currentPose;
            currentPose.x = pose.realPosition.x;
            currentPose.y = pose.realPosition.y;
            currentPose.z = pose.realPosition.z;
            auto distance = conversion::cal_distance(lastPose, currentPose);
            long timeout = distance * 20 + 5;//掉头5s
            pose.timeout = timeout;
            timeout_accumulation += timeout;
            totalDistance += distance;

            lastPose.x = pose.realPosition.x;
            lastPose.y = pose.realPosition.y;
            lastPose.z = pose.realPosition.z;
        }

        block.timeout = timeout_accumulation * 2;
        block.totalStep = point_accumulation;
        block.plannerPoints = plannerPoints;
    }

    for (auto &block: wholeBlockList) {
        block.totalDistance = totalDistance;
    }

    realTask.setTotalStep(point_accumulation);
    realTask.setTotalFrequency(realTask.getRate());

    for (const auto &block: wholeBlockList) {
        blockList.emplace_back(block);
    }

//    auto generateMat = SegmentationCenter::instance().generateMat();
//    for (const auto &block: blockList) {
//
//        for (const auto &point: block.plannerPoints) {
//
//            auto cvPoint = MapAttributeSingleton::instance().rosPoint2MapPoint(generateMat.rows, generateMat.cols,
//                                                                               Point(point.realPosition.x,
//                                                                                     point.realPosition.y));
//            cv::circle(generateMat, cvPoint, 3, cv::Scalar(200), CV_FILLED);
//        }
//
//        LOG(ERROR) << "timeout : " << block.timeout;
//        cv::imshow("complexPathToRealBlock", generateMat);
//        cv::waitKey();
//    }

}

std::vector<PoseMo> PointGenerator::recalculateAngle(const cv::Point2d &point2D,
                                                     const std::vector<PoseMo> &poseList) {
    std::vector<PoseMo> results(poseList.size());
    for (size_t point_index = 0; point_index < poseList.size(); ++point_index) {
        double theta = 0.;

        auto current_point = cv::Point2f(poseList[point_index].getX(), poseList[point_index].getY());

        cv::Point2f vector(0, 0);
        if (point_index < poseList.size() - 1) {
            auto next_point = cv::Point2f(poseList[point_index + 1].getX(), poseList[point_index + 1].getY());
            vector = next_point - current_point;
            if (vector.x != 0 || vector.y != 0) {
                theta = atan2(vector.y, vector.x);
            }
        } else {
            auto next_point = cv::Point2f(point2D.x, point2D.y);
            vector = next_point - current_point;
            if (vector.x != 0 || vector.y != 0) {
                theta = atan2(vector.y, vector.x);
            }
        }

        results[point_index] = PoseMo(poseList[point_index].getX(), poseList[point_index].getY(), theta,
                                      poseList[point_index].getCmcMode());
    }
    return results;
}

std::vector<PoseStamped> PointGenerator::convertToGeometry(const std::vector<PoseMo> &complex) {
    std::vector<PoseStamped> results(complex.size());
    std_msgs::Header header;
    header.stamp = ros::Time::now();
    header.frame_id = "/map";
    for (size_t i = 0; i < complex.size(); ++i) {
        PoseMo vo = complex[i];

        results[i].poseStamped.header = header;
        results[i].poseStamped.header.seq = i;
        results[i].poseStamped.pose.position.x = vo.getX();
        results[i].poseStamped.pose.position.y = vo.getY();
        results[i].poseStamped.pose.position.z = 0.;
        Eigen::Quaterniond quaternion;
        quaternion = Eigen::AngleAxisd(vo.getTheta(), Eigen::Vector3d::UnitZ());
        tf::quaternionEigenToMsg(quaternion, results[i].poseStamped.pose.orientation);

        results[i].cmcMode = vo.getCmcMode();
    }
    return results;
}

void PointGenerator::addSinglePoint(std::vector<RealBlock> &blockList, const RealTask &realTask,
                                    const RealPoint &singlePoint) {
    RealBlock firstBlock = buildBlock(0, realTask);
    firstBlock.inClean = false;
    firstBlock.plannerPoints.push_back(singlePoint);
    blockList.push_back(firstBlock);
}

//void PointGenerator::combinationPose2RealPoint(RealTask realTask, std::vector<CombinationPoseVo> poseList,
//                                               vector<RealPoint> &realPointList) {
//    auto originPose = MapAttribute::instance().getMapOriginPose();
//
//    for (size_t point_index = 0; point_index < poseList.size(); ++point_index) {
//        double theta = 0.;
//
//        auto current_point = cv::Point2f(poseList[point_index].getX(), poseList[point_index].getY());
//
//        cv::Point2f vector(0, 0);
//        if (point_index < poseList.size() - 1) {
//            auto next_point = cv::Point2f(poseList[point_index + 1].getX(), poseList[point_index + 1].getY());
//            vector = next_point - current_point;
//            if (vector.x != 0 || vector.y != 0) {
//                theta = std::atan2(vector.y, vector.x);
//            }
//        } else {
//            auto next_point = cv::Point2f(originPose.position.x, originPose.position.y);
//            vector = next_point - current_point;
//            if (vector.x != 0 || vector.y != 0) {
//                theta = std::atan2(vector.y, vector.x);
//            }
//        }
//        poseList[point_index].setTheta(theta);
//    }
//
//
//    std_msgs::Header header;
//    header.stamp = ros::Time::now();
//    header.frame_id = "/map";
//
//    int rate = realTask.getRate();
//    int totalStep = poseList.size() * rate;
//    int accumulation = 0;
//
//    geometry_msgs::Pose::_position_type lastPose;
//    for (int i = 0; i < rate; ++i) {
//        for (int j = 0; j < poseList.size(); ++j) {
//
//            CombinationPoseVo &vo = poseList[j];
//
//            geometry_msgs::PoseStamped pose;
//            pose.header = header;
//            pose.header.seq = i;
//            pose.pose.position.x = vo.getX();
//            pose.pose.position.y = vo.getY();
//            pose.pose.position.z = 0.;
//            Eigen::Quaterniond quaternion;
//            quaternion = Eigen::AngleAxisd(vo.getTheta(), Eigen::Vector3d::UnitZ());
//            tf::quaternionEigenToMsg(quaternion, pose.pose.orientation);
//
//
//            RealPosition realPosition(pose.pose.position.x, pose.pose.position.y, pose.pose.position.z);
//            RealOrientation realOrientation(pose.pose.orientation.x, pose.pose.orientation.y,
//                                            pose.pose.orientation.z, pose.pose.orientation.w);
//            RealProgress realProgress(j + 1, totalStep, i + 1, rate);
//
//            auto realPoint = buildPoint(accumulation, realTask);
//            realPoint.realPosition = std::move(realPosition);
//            realPoint.realOrientation = std::move(realOrientation);
//            realPoint.realProgress = std::move(realProgress);
//
//            realPoint.timeout = conversion::cal_distance(lastPose, pose.pose.position) * 20 + 5;//掉头5s
//
//            realPoint.inClean = j != 0;
//
//            realPointList.push_back(realPoint);
//
//            accumulation++;
//
//            lastPose.x = pose.pose.position.x;
//            lastPose.y = pose.pose.position.y;
//            lastPose.z = pose.pose.position.z;
//        }
//    }
//
//    realTask.setTotalStep(totalStep);
//    realTask.setTotalFrequency(rate);
//}


// add gang,该函数输入为zoned，即矩形的四个端点，输出为PoseVo的队列
bool PointGenerator::generateRecPointListForViewPart(std::vector<Point> zoned,
                                                     std::vector<PoseVo> &pointList) {
    // 这里根据矩形（View Part）生成回字形路径
    // zoned:矩形
    // pointList:点列
    if (zoned.empty() || zoned.size() != 4) {
        return false;
    }

    auto pointDistance = [](Point A, Point B) {
        return sqrt(pow(B.getX() - A.getX(), 2) + pow(B.getY() - A.getY(), 2));
    };

    auto addPoint = [](Point point, Line inc) {
        Point new_point;
        new_point.setX(point.getX() + inc.getX());
        new_point.setY(point.getY() + inc.getY());
        return new_point;
    };
//    for (int i = 0; i < 4; i++) {
//        LOG(INFO) << "point" << i << ": " << zoned[i].getX() << "  " << zoned[i].getY();
//    }

    float step = 0.25;
    float x_length = pointDistance(zoned[0], zoned[1]);
    float y_length = pointDistance(zoned[0], zoned[3]);
    int x_size = ceil(x_length / step);
    int y_size = ceil(y_length / step);
    Line x_vector = zoned[1] - zoned[0];
    Line x_step_vector = x_vector / x_size;

    Line y_vector = zoned[3] - zoned[0];
    Line y_step_vector = y_vector / y_size;

    float origin_x = zoned[0].getX();
    float origin_y = zoned[0].getY();
    std::vector<std::vector<int>> dir{{1,  0},
                                      {0,  1},
                                      {-1, 0},
                                      {0,  -1}};
    int map_size_x = x_size + 1;
    int map_size_y = y_size + 1;
    std::vector<std::vector<int>> map(map_size_x,
                                      std::vector<int>(map_size_y, 0));
    int cnt = (x_size + 1) * (y_size + 1);
    int dir_index = 0;
    std::vector<int> current_index = {0, 0};
    PoseVo current_point;
    while (cnt > 0) {
        current_point.setX(
                origin_x + current_index[0] * x_step_vector.getX() + current_index[1] * y_step_vector.getX());
        current_point.setY(
                origin_y + current_index[0] * x_step_vector.getY() + current_index[1] * y_step_vector.getY());
        pointList.push_back(current_point);
        map[current_index[0]][current_index[1]] = 1;
        int next_x = current_index[0] + dir[dir_index][0];
        int next_y = current_index[1] + dir[dir_index][1];
        if (next_x >= map_size_x || next_y >= map_size_y || next_x < 0 ||
            next_y < 0 || map[next_x][next_y] == 1) {
            dir_index = (dir_index + 1) % 4;
        }
        current_index[0] = current_index[0] + dir[dir_index][0];
        current_index[1] = current_index[1] + dir[dir_index][1];
        cnt--;
    }
    return true;
}

bool PointGenerator::generateBowPointListForViewPart(std::vector<Point> zoned, std::vector<PoseVo> &pointList) {
    if (zoned.empty() || zoned.size() != 4) {
        return false;
    }

    auto pointDistance = [](Point A, Point B) {
        return sqrt(pow(B.getX() - A.getX(), 2) + pow(B.getY() - A.getY(), 2));
    };

    auto addPoint = [](Point point, Line inc) {
        Point new_point;
        new_point.setX(point.getX() + inc.getX());
        new_point.setY(point.getY() + inc.getY());
        return new_point;
    };

    float path_width = 0.3;
    float step = 0.2;

    float length = pointDistance(zoned[0], zoned[1]);
    float width = pointDistance(zoned[0], zoned[3]);

    Line length_vector = zoned[1] - zoned[0];
    Line width_vector = zoned[3] - zoned[0];

    //保证x方向为长边方向
    if (width > length) {
        std::swap(length, width);
        std::swap(length_vector, width_vector);
    }

    int length_size = ceil(length / step);
    int width_size = ceil(width / path_width);


    Line length_step_vector = length_vector / length_size;
    Line width_step_vector = width_vector / width_size;

    float origin_x = zoned[0].getX();
    float origin_y = zoned[0].getY();


    int width_cnt = 0;
    int length_dir = 1;

    PoseVo current_point;
    current_point.setX(origin_x);
    current_point.setY(origin_y);
    pointList.push_back(current_point);
    while (width_cnt < width_size) {
        int length_cnt = 0;
        while (length_cnt < length_size) {
            current_point.setX(current_point.getX() + length_dir * length_step_vector.getX());
            current_point.setY(current_point.getY() + length_dir * length_step_vector.getY());
            pointList.push_back(current_point);
            length_cnt++;
        }
        length_dir = -length_dir;
        current_point.setX(current_point.getX() + width_step_vector.getX());
        current_point.setY(current_point.getY() + width_step_vector.getY());
        pointList.push_back(current_point);
        width_cnt++;
    }
    int length_cnt = 0;
    while (length_cnt < length_size) {
        current_point.setX(current_point.getX() + length_dir * length_step_vector.getX());
        current_point.setY(current_point.getY() + length_dir * length_step_vector.getY());
        pointList.push_back(current_point);
        length_cnt++;
    }
    return true;
}

void PointGenerator::generateChildPointFlow(const std::vector<PoseVo> &points, std::vector<PoseVo> &cpList,
                                            float resolution_) {
    if (points.empty()) {
        return;
    }

    double min_sq_resolution = resolution_ * resolution_;

    int accumulate = 0;

    auto firstPoint = points[0];
    cpList.push_back(firstPoint);
    accumulate++;

    double last_x = firstPoint.getX();
    double last_y = firstPoint.getY();

    for (unsigned int i = 1; i < points.size(); ++i) {
        auto currentPoint = points[i];
        double loop_x = currentPoint.getX();
        double loop_y = currentPoint.getY();
//        auto loopPosition = currentPoint.realPosition;
//        auto loopOrientation = currentPoint.realOrientation;

        double dist = (loop_x - last_x) * (loop_x - last_x) + (loop_y - last_y) * (loop_y - last_y);
        if (dist > min_sq_resolution) {
            int steps = ceil((std::sqrt(dist)) / resolution_);
            // add a points in-between
            double deltaX = (loop_x - last_x) / steps;
            double deltaY = (loop_y - last_y) / steps;
            for (int j = 1; j < steps; ++j) {
                PoseVo position(last_x + j * deltaX, last_y + j * deltaY, currentPoint.getTheta());
                cpList.push_back(position);
                accumulate++;
            }
        }

        cpList.emplace_back(loop_x, loop_y, currentPoint.getTheta());
        accumulate++;
        last_x = loop_x;
        last_y = loop_y;
    }
}

//std::vector<RealPoint> CoveragePointGenerator::taskGeneratePointList(RealTask &task) {
//    std::vector<RealPoint> taskPointList;
//    auto taskId = task.getId();
//    auto roomCoverage = ExplorationCenter::instance().findRoomCoverage(taskId, false);
//    auto poseList = roomCoverage.getPoseList();
//    std::vector<RealPoint> realPoints;
//    pose2RealPoint(task, poseList, realPoints);
//    return realPoints;
//}

//std::vector<RealPoint> RectanglePointGenerator::taskGeneratePointList(RealTask &task) {
//    std::vector<float> zoned = task.getZoned0();
//    if (zoned.size() != 8)
//        throw app::exception(make_error_code(error::room_mb_file_open_fail));
//
//    std::vector<std::vector<cv::Point>> polygon_array;
//    auto map = SegmentationCenter::instance().generateMat();
//
//    std::vector<cv::Point> cvPoints;
//    for (int i = 0; i < zoned.size(); i = i + 2) {
//        Point point(zoned[i], zoned[i + 1]);
//        auto cvPoint = MapAttribute::instance().rosPoint2MapPoint(map, point);
//        cvPoints.push_back(cvPoint);
//    }
//
//    polygon_array.push_back(cvPoints);
//    cv::Mat zoned_image = cv::Mat::zeros(map.rows, map.cols, CV_8UC1);
//    cv::fillPoly(zoned_image, polygon_array, cv::Scalar(255));
//
//    std::vector<geometry_msgs::Pose2D> exploration_path;
//    std::vector<cv::Point> point_path;
//    std::vector<std::vector<geometry_msgs::Pose2D>> complex_path;
//    ExplorationCenter::instance().generatePlanningPathRect(zoned_image, BOUSTROPHEDON_EXPLORER_MODE,
//                                                           exploration_path, point_path, complex_path);
//
//    ExplorationCenter::instance().pathPublish(exploration_path);
//
//    std::vector<PoseVo> poseList;
//    for (const auto &item: exploration_path) {
//        poseList.emplace_back(item.y, item.x, item.theta);
//    }
//    std::vector<RealPoint> realPoints;
//    pose2RealPoint(task, poseList, realPoints);
//
//    return realPoints;
//}

//bool
//CombinationPointGenerator::generateRecPointListForViewPart(std::vector<Point> zoned,
//                                                           std::vector<CombinationPoseVo> &pointList) {
//    if (zoned.empty() || zoned.size() != 4) {
//        return false;
//    }
//
//    int index = 0;
//    float min_sum = zoned[index].getX() + zoned[index].getY();
//
//    Point start_point;
//    for (int i = 1; i < zoned.size(); i++) {
//        if (min_sum > (zoned[i].getX() + zoned[i].getY())) {
//            min_sum = zoned[i].getX() + zoned[i].getY();
//            index = i;
//        }
//    }
//
//    int index_last, index_next;
//    index_last = index - 1;
//    index_next = index + 1;
//
//    if (index == 0) {
//        index_last = 3;
//    }
//    if (index == 3) {
//        index_next = 0;
//    }
//
//    Point point = zoned[index];
//    Point point_last = zoned[index_last];
//    Point point_next = zoned[index_next];
//
//    float x_len, y_len;
//    x_len = conversion::cal_distance(point, point_last);
//    y_len = conversion::cal_distance(point, point_next);
//
//    start_point.setXandY(point.getX(), point.getY());//存放目标起点坐标
//
//
//    float x_dir[2], y_dir[2];
//    x_dir[0] = point_last.getX() - point.getX();
//    x_dir[1] = point_last.getY() - point.getY();
//    y_dir[0] = point_next.getX() - point.getX();
//    y_dir[1] = point_next.getY() - point.getY();
//
//
//    int num = 12;
//    if (x_len <= y_len) {
//
//        CombinationPoseVo first(point.getX(), point.getY(), 0, 0);
//        pointList.push_back(first);
//
//        if (x_len > 1.5) {
//            num = ((int) ((x_len + 0.5) / 0.3) + 1 + 1) * 2;
//        }
//
//        for (int i = 0; i < num - 1; i++) {
//            float x_param, y_param;
//            if (i % 4 < 2)
//                y_param = 1;
//            else
//                y_param = 0;
//            x_param = (float) (((i) / 2 + (i) % 2)) / (num / 2 - 1);
//
//            CombinationPoseVo temp_task(point.getX() + x_param * x_dir[0] + y_param * y_dir[0],
//                                        point.getY() + x_param * x_dir[1] + y_param * y_dir[1],
//                                        0, i + 1
//            );
//            pointList.push_back(temp_task);
//        }
//    } else {
//        CombinationPoseVo first(point.getX(), point.getY(), 0, 0);
//        pointList.push_back(first);
//
//
//        if (y_len > 1.5) {
//            num = ((int) ((y_len + 0.5) / 0.3) + 1 + 1) * 2;
//        }
//
//        for (int i = 0; i < num - 1; i++) {
//            float x_param, y_param;
//            if (i % 4 < 2)
//                x_param = 1;
//            else
//                x_param = 0;
//            y_param = (float) (((i) / 2 + (i) % 2)) / (num / 2 - 1);
//
//
//            CombinationPoseVo temp_task(point.getX() + x_param * x_dir[0] + y_param * y_dir[0],
//                                        point.getY() + x_param * x_dir[1] + y_param * y_dir[1],
//                                        0, i + 1
//            );
//            pointList.push_back(temp_task);
//        }
//    }
//
//    return true;
//}

//std::vector<RealPoint> CombinationPointGenerator::taskGeneratePointList(RealTask &task) {
//    auto combination = task.getCombination();
//
//    ViewPartList viewPartListTemp;
//    if (!ViewPartManager::get_instance()->GetViewPartList(viewPartListTemp)) {
//        throw app::exception(make_error_code(error::combination_point_get_view_part_fail));
//    }
//
//    std::string combination_id_temp = combination.getCombinationID();
//    CombinationBrief combination_brief_temp;
//    if (CombinationManager::get_instance()->GetCombination(combination_brief_temp, combination_id_temp) != SUCCESS_) {
//        throw app::exception(make_error_code(error::combination_brief_get_fail));
//    }
//    CombinationDetail combination_detail_temp(combination_brief_temp);
//    vector<string> partNotMached = combination_brief_temp.toDetail(combination_detail_temp, viewPartListTemp);
//    if (partNotMached.size() > 0) {
//        for (auto &item: partNotMached) {
//            CombinationManager::get_instance()->DelatePartID(item);
//        }
//    }
//
//    std::vector<ViewPart> viewPartList = combination_detail_temp.getViewPartList();
//    if (viewPartList.size() <= 0) {
//        throw app::exception(make_error_code(error::combination_pointlist_empty));
//    }
//
//    std::vector<CombinationPoseVo> pointList;
//    for (auto &item: viewPartList) {
//        switch (item.getMode()) {
//            case 1: {
//                auto zoned = item.getZoned();
//                if (!generateRecPointListForViewPart(zoned, pointList)) {
//                    LOG(ERROR) << "加载矩形" << item.getPartID() << "失败";
//                }
//                break;
//            }
//        }
//    }
//
//    if (pointList.empty()) {
//        throw app::exception(make_error_code(error::combination_pointlist_empty));
//    }
//
//    std::vector<PoseVo> poseList;
//    for (const auto &item: pointList) {
//        poseList.push_back(PoseVo(item.getY(), item.getX(), 0));
//    }
//
//    std::vector<geometry_msgs::Pose2D> exploration_path;
//    for (const auto &item: poseList) {
//        geometry_msgs::Pose2D pose;
//        pose.x = item.getX();
//        pose.y = item.getY();
//        exploration_path.push_back(pose);
//    }
//    ExplorationCenter::instance().pathPublish(exploration_path);
//
//    std::vector<RealPoint> realPoints;
//    combinationPose2RealPoint(task, pointList, realPoints);
//    return realPoints;
//}

//std::vector<RealPoint> FullPointGenerator::taskGeneratePointList(RealTask &task) {
//    std::vector<RealPoint> taskPointList;
//    auto taskId = task.getId();
//    auto roomCoverage = ExplorationCenter::instance().findRoomCoverage(taskId, true);
//    auto poseList = roomCoverage.getPoseList();
//    std::vector<RealPoint> realPoints;
//    pose2RealPoint(task, poseList, realPoints);
//    return realPoints;
//}

std::vector<RealBlock> ExplorationGenerator::taskGeneratePointList(RealTask &task) {

    TaskMode mode = SqliteDataBase::TaskModeFromInt(task.getMode());

    if (mode == TaskMode::Zoned) {
        auto originPoint = MapAttributeSingleton::instance().getMapOrigin();
        ExplorationCenter &explorationCenter = ExplorationCenter::instance();
        SegmentationCenter &segmentationCenter = SegmentationCenter::instance();
        const cv::Mat &room_map = segmentationCenter.generateMat();
        double rows = room_map.rows * map_resolution_from_subscription;
        double cols = room_map.cols * map_resolution_from_subscription;

        std::vector<PoseVo> recordPoseList;
        std::vector<std::vector<PoseMo>> complexPoseList;

        std::vector<ZoneVo> zones = task.getZoned();
        std::vector<bool> intersects;

        bool lastIntersect = false;
        for (int i = 0; i < zones.size(); i++) {
            auto &zone = zones[i];

            std::vector<PointVo> points = zone.getPoints();
            std::vector<Point> trs;
            geometry_msgs::Polygon polygon;
            for (const auto &point: points) {
                Point p;
                double x = point.getX() * map_resolution_from_subscription;
                double y = point.getY() * map_resolution_from_subscription;
                p.setY(cols - x + originPoint.x);
                p.setX(rows - y + originPoint.y);
                trs.push_back(p);

                geometry_msgs::Point32 point32;
                point32.x = p.getY();
                point32.y = p.getX();
                polygon.points.push_back(point32);
            }

            std::vector<PoseVo> zonePoseList;
            if (Environment::instance().rectangular_ambulatory_plane) {
                generateRecPointListForViewPart(trs, zonePoseList);
            } else {
                generateBowPointListForViewPart(trs, zonePoseList);
            }

            std::vector<PoseVo> subPoseList;
            generateChildPointFlow(zonePoseList, subPoseList, 0.2);

            bool intersect = false;
            if (i < zones.size() - 1) {
                auto currentZone = zones[i];
                auto nextZone = zones[i + 1];
                intersect = cleanMechanismControlMode(currentZone, nextZone, room_map.rows, room_map.cols);
            }

            std::vector<PoseMo> poseList;
            for (int j = 0; j < subPoseList.size(); j++) {
                const auto &item = subPoseList[j];
                recordPoseList.push_back(item);

                CmcMode cmcMode = CmcMode::Omission;
                if (j == 0) {
                    if (!lastIntersect) {
                        cmcMode = CmcMode::Open;
                    }
                } else if (j == subPoseList.size() - 1) {
                    if (!intersect) {
                        cmcMode = CmcMode::Close;
                    }
                    lastIntersect = intersect;
                }
                poseList.emplace_back(item.getX(), item.getY(), item.getTheta(), cmcMode);
            }

            complexPoseList.push_back(poseList);
        }

        std::vector<geometry_msgs::Pose2D> exploration_path;
        for (const auto &item: recordPoseList) {
            geometry_msgs::Pose2D pose;
            pose.x = item.getY();
            pose.y = item.getX();
            pose.theta = item.getTheta();
            exploration_path.push_back(pose);
        }
//        std::vector<std::vector<geometry_msgs::Pose2D>> complex_path;
//        std::vector<geometry_msgs::Pose2D> complex_exploration_path;
//        for (const auto &complex: complexPoseList) {
//            for (const auto &item: complex) {
//                geometry_msgs::Pose2D pose2D;
//                pose2D.x = item.getY();
//                pose2D.y = item.getX();
//                pose2D.theta = item.getTheta();
//                complex_exploration_path.push_back(pose2D);
//            }
//            complex_path.push_back(complex_exploration_path);
//        }
        explorationCenter.pathPublish(exploration_path);

        if (DEBUG_CLEAN_MECHANISM) {
            std::cout << "--------------------- clean mechanism control mode print start -------------------------"
                      << std::endl;
            for (const auto &list: complexPoseList) {
                for (const auto &item: list) {
                    if (item.getCmcMode() == CmcMode::Open) {
                        std::cout << "1";
                    } else if (item.getCmcMode() == CmcMode::Close) {
                        std::cout << "0";
                    } else {
                        std::cout << "-";
                    }
                }
                std::cout << "\n ";
            }
            std::cout << "--------------------- clean mechanism control mode print end -------------------------"
                      << std::endl;
        }

        std::vector<RealBlock> blocks;
        complexPathToRealBlock(task, complexPoseList, blocks);
        return blocks;
    } else {
        auto coverage = TaskExploration::explorationPlanningPath(task);

        const std::vector<std::vector<PoseVo>> &vComplexList = coverage.getComplexList();
        std::vector<std::vector<PoseMo>> mComplexList;
        for (const auto &vList: vComplexList) {
            std::vector<PoseMo> pms;
            for (const auto &item: vList) {
                pms.emplace_back(item.getX(), item.getY(), item.getTheta(), CmcMode::Omission);
            }
            mComplexList.push_back(pms);
        }

        std::vector<RealBlock> blocks;
        complexPathToRealBlock(task, mComplexList, blocks);

        return blocks;
    }

}

bool
ExplorationGenerator::cleanMechanismControlMode(const ZoneVo &currentZone, const ZoneVo &nextZone, int rows, int cols) {

    std::vector<std::vector<cv::Point>> current_polygon_array;
    std::vector<cv::Point> currentCvPoints;
    for (const auto &point: currentZone.getPoints()) {
        currentCvPoints.emplace_back(point.getX(), point.getY());
    }
    current_polygon_array.push_back(currentCvPoints);
    cv::Mat current_zoned_image = cv::Mat::zeros(rows, cols, CV_8UC1);
    cv::fillPoly(current_zoned_image, current_polygon_array, cv::Scalar(255));

    std::vector<std::vector<cv::Point>> next_polygon_array;
    std::vector<cv::Point> nextCvPoints;
    for (const auto &point: nextZone.getPoints()) {
        nextCvPoints.emplace_back(point.getX(), point.getY());
    }
    next_polygon_array.push_back(nextCvPoints);
    cv::Mat next_zoned_image = cv::Mat::zeros(rows, cols, CV_8UC1);
    cv::fillPoly(next_zoned_image, next_polygon_array, cv::Scalar(255));

//    cv::imshow("current_zoned_image", current_zoned_image);
//    cv::waitKey();
//    cv::imshow("next_zoned_image", next_zoned_image);
//    cv::waitKey();

    cv::Mat andMat;
    cv::bitwise_and(current_zoned_image, next_zoned_image, andMat);

    int intersect = 0;
    for (int y = 0; y < andMat.rows; y++) {
        for (int x = 0; x < andMat.cols; x++) {
            if (andMat.at<unsigned char>(y, x) > 254) {
                intersect++;
            }
        }
    }

    LOG(INFO) << "rect intersect count " << intersect << " ...";
    return intersect > 0;
}

void ExplorationGenerator::elevatorPointList(RealTask &task) {
    /**
        去电梯点位
        梯控（进电梯、乘电梯、出电梯）
        切换地图
        执行任务
        回电梯点位（考虑回基站）
        梯控（进电梯、乘电梯、出电梯）
        切换地图
        返回基站
     */

    std::vector<RealBlock> proList;
    if (task.getDoMapId() != task.getPreMapId()) {
        const RealBlock &preCirculationBlock = createPreCirculationBlock(task);
        const RealBlock &preElevatorBlock = createPreElevatorBlock(task);
        const RealBlock &preSwitchMapBlock = createPreSwitchMapBlock(task);
        proList.push_back(preCirculationBlock);
        proList.push_back(preElevatorBlock);
        proList.push_back(preSwitchMapBlock);
    }
    task.setProList(proList);

    std::vector<RealBlock> postList;
    if (task.getDoMapId() != task.getPostMapId()) {
        const RealBlock &postCirculationBlock = createPostCirculationBlock(task);
        const RealBlock &postElevatorBlock = createPostElevatorBlock(task);
        const RealBlock &postSwitchMapBlock = createPostSwitchMapBlock(task);
        postList.push_back(postCirculationBlock);
        postList.push_back(postElevatorBlock);
        postList.push_back(postSwitchMapBlock);
    }
    task.setPostList(postList);
}

RealBlock ExplorationGenerator::createPreCirculationBlock(RealTask &task) {
    const RealPoint &point = task.getPreOutPoint();

    RealPoint realPoint;
    realPoint.realPosition = point.realPosition;
    realPoint.realOrientation = point.realOrientation;
    realPoint.cmcMode = CmcMode::Close;

    auto realBlock = buildBlock(0, task);
    realBlock.mustArrive = true;
    realBlock.plannerPoints.push_back(realPoint);
    return realBlock;
}

RealBlock ExplorationGenerator::createPreElevatorBlock(RealTask &task) {
    RealPoint realPoint;
    realPoint.targetFloorPair = std::make_pair(task.getPreFloor(), task.getDoFloor());

    auto realBlock = buildBlock(0, task);
    realBlock.mustArrive = true;
    realBlock.plannerPoints.push_back(realPoint);
    realBlock.plannerPoints.push_back(task.getPreOutPoint());
    realBlock.plannerPoints.push_back(task.getPreInPoint());
    realBlock.plannerPoints.push_back(task.getDoOutPoint());
    realBlock.plannerPoints.push_back(task.getDoInPoint());
    return realBlock;
}

RealBlock ExplorationGenerator::createPreSwitchMapBlock(RealTask &task) {
    const RealPoint &point = task.getDoOutPoint();

    RealPoint realPoint;
    realPoint.realPosition = point.realPosition;
    realPoint.realOrientation = point.realOrientation;
    realPoint.cmcMode = CmcMode::Close;

    realPoint.targetMapIdPair = std::make_pair(task.getPreMapId(), task.getDoMapId());

    auto realBlock = buildBlock(0, task);
    realBlock.mustArrive = true;
    realBlock.plannerPoints.push_back(realPoint);
    return realBlock;
}

RealBlock ExplorationGenerator::createPostCirculationBlock(RealTask &task) {
    const RealPoint &point = task.getDoOutPoint();

    RealPoint realPoint;
    realPoint.realPosition = point.realPosition;
    realPoint.realOrientation = point.realOrientation;
    realPoint.cmcMode = CmcMode::Close;

    auto realBlock = buildBlock(0, task);
    realBlock.mustArrive = true;
    realBlock.plannerPoints.push_back(realPoint);
    return realBlock;
}

RealBlock ExplorationGenerator::createPostElevatorBlock(RealTask &task) {

    RealPoint realPoint;
    realPoint.targetFloorPair = std::make_pair(task.getDoFloor(), task.getPostFloor());

    auto realBlock = buildBlock(0, task);
    realBlock.mustArrive = true;
    realBlock.plannerPoints.push_back(realPoint);
    realBlock.plannerPoints.push_back(task.getDoOutPoint());
    realBlock.plannerPoints.push_back(task.getDoInPoint());
    realBlock.plannerPoints.push_back(task.getPostOutPoint());
    realBlock.plannerPoints.push_back(task.getPostInPoint());
    return realBlock;
}

RealBlock ExplorationGenerator::createPostSwitchMapBlock(RealTask &task) {
    const RealPoint &point = task.getPostOutPoint();

    RealPoint realPoint;
    realPoint.realPosition = point.realPosition;
    realPoint.realOrientation = point.realOrientation;
    realPoint.cmcMode = CmcMode::Close;

    realPoint.targetMapIdPair = std::make_pair(task.getDoMapId(), task.getPostMapId());

    auto realBlock = buildBlock(0, task);
    realBlock.mustArrive = true;
    realBlock.plannerPoints.push_back(realPoint);
    return realBlock;
}
