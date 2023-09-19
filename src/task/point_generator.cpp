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
#include "model/ViewPart.h"
#include "manager/ViewPartManager.h"
#include "model/Combination.h"
#include "manager/CombinationManager.h"
#include "exploration/line.h"
#include "tool/Variable.h"
#include "segmentation/SegmentationCenter.h"
#include "task/model/CombinationPoseVo.h"
#include "exploration/path_exploration_preview_task.h"
#include "db/segmentation_data_base.h"
#include "geometry_msgs/Polygon.h"
#include "task/point_planner.h"

#include "simulation.h"
#include "segmentation/handle_segmentation_display.h"

RealBlock PointGenerator::buildBlock(int id, const RealTask &task) {
    RealBlock block;
    block.id = id;
    block.taskId = task.getId();

    block.renew = task.isRenew();
    if (task.isRenew()) {
        block.newTaskId = task.getTaskId();
    } else {
        if (task.getMode() == 7) {
            block.oldTaskId = task.getCombination().getCombinationID();
        }
    }

    block.name = task.getName();
    block.rate = task.getRate();
    block.totalFrequency = task.getRate();
    block.mode = task.getMode();

    block.knife = task.isKnife();
    block.work_status = task.getWorkStatus();
    return block;
}

void PointGenerator::complexPathToRealBlock(RealTask &realTask,
                                            const std::vector<std::vector<PoseVo>> &complexList,
                                            std::vector<RealBlock> &blockList) {
    float totalDistance = 0;
    // 获取当前地图的原点位置，以便后续转换 cv 点和 ros 点
    auto originPoint = MapAttributeSingleton::instance().getMapOrigin();

    // 对于单个点列大于 complex_path_num_splits 值的，进行近似平均的拆分
    std::vector<std::vector<PoseVo>> splitVectors;
    for (const auto &vec: complexList) {
        //complex_path_num_splits 默认 1000
        int numSubVec = (vec.size() + Environment::instance().complex_path_num_splits - 1) /
                        Environment::instance().complex_path_num_splits;

        int remainder = vec.size() % numSubVec;
        int sizePerSubVec = vec.size() / numSubVec;
        int startIdx = 0;

        for (int i = 0; i < numSubVec; ++i) {
            int endIdx = startIdx + sizePerSubVec + (i < remainder ? 1 : 0);
            std::vector<PoseVo> subVec(vec.begin() + startIdx, vec.begin() + endIdx);
            splitVectors.push_back(subVec);
            startIdx = endIdx;
        }
    }

    // 根据前后两个点位，重新计算每个点位的角度值
    std::vector<std::vector<PoseVo>> complexAngleList;
    for (const auto &complex: splitVectors) {
        std::vector<PoseVo> poseList = recalculateAngle(originPoint, complex);
        complexAngleList.push_back(poseList);
    }

    // 将 PoseVo 转为 geometry_msgs::PoseStamped，主要是将 PoseVo 中 theta 转为 pose.orientation
    std::vector<std::vector<geometry_msgs::PoseStamped>> complexGeometryList;
    for (const auto &complex: complexAngleList) {
        std::vector<geometry_msgs::PoseStamped> geometryList = convertToGeometry(complex);
        complexGeometryList.push_back(geometryList);
    }

    // 将 geometry_msgs::PoseStamped 集合转为 Block 点列，一个 Block 中包含 1 或很多点，其中 geometry_msgs::PoseStamped 转为 RealPosition + RealOrientation
    std::vector<RealBlock> initBlockList;
    for (const auto &complex: complexGeometryList) {
        auto realBlock = buildBlock(0, realTask);

        for (const auto &pose: complex) {
            RealPoint realPoint;
            RealPosition realPosition(pose.pose.position.x, pose.pose.position.y, pose.pose.position.z);
            RealOrientation realOrientation(pose.pose.orientation.x, pose.pose.orientation.y,
                                            pose.pose.orientation.z, pose.pose.orientation.w);
            realPoint.realPosition = std::move(realPosition);
            realPoint.realOrientation = std::move(realOrientation);
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
        RealBlock &block = rateBlockList[0];
        std::vector<RealPoint> &plannerPoints = block.plannerPoints;
        if (!plannerPoints.empty()) {
            RealPoint point = plannerPoints[0];
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
            addSinglePoint(wholeBlockList, realTask, nextBlock.plannerPoints[0]);
        }
    }

    // 统计 block_accumulation、point_accumulation 以便计算进度
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

    // 预制基站的摆渡点位，方便下方闸机逻辑处理中添加闸机 block
    addSinglePoint(wholeBlockList, realTask, PointPlanner::createBackBasePoint());

    // 取出当前的机器人位置，为计算闸机逻辑做准备
    auto currentPoint = MapAttributeSingleton::createCurrentPoint();

    // 1. 计算两点间距，得出超时时间，并计算总共时间、总步数
    // 2. 为每个 block 添加来向 lastPoint，并取出第一个点，方便计算
    auto lastPoint = currentPoint;
    geometry_msgs::Pose::_position_type lastPose;
    for (int i = 0; i < wholeBlockList.size(); i++) {
        auto &block = wholeBlockList[i];
        auto plannerPoints = block.plannerPoints;

        block.firstPoint = plannerPoints[0];
        if (i == 0) {
            block.lastPoint = currentPoint;
        } else {
            block.lastPoint = lastPoint;
        }

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

            lastPoint = pose;
        }

        block.timeout = timeout_accumulation * 2;
        block.totalStep = point_accumulation;
        block.plannerPoints = plannerPoints;
    }

    // 1. 取出闸机相关信息（闸机区域、闸机2个摆渡点）
    // 2. 根据闸机区域，拆分地图，得到被拆分后的两块区域
    // 3. 由闸机的2个摆渡点，形成两个摆渡 block
    bool hasGate = false;
    auto segmented_map = SegmentationCenter::instance().generateMat().clone();
    auto gateList = SegmentationDataBase::instance().loadGate(SegmentationDataBase::instance().getDbMap().id);
    RealBlock leftBlock = buildBlock(0, realTask);
    RealBlock rightBlock = buildBlock(0, realTask);
    int leftValue, rightValue = 0;
    if (!gateList.empty()) {
        Gate gate = gateList[gateList.size() - 1];

//        cv::Point lineStart(gate.start_x, gate.start_y);
//        cv::Point lineEnd(gate.end_x, gate.end_y);

        RealPoint realPointLeft;
        RealPosition realPositionLeft(gate.left_position_x, gate.left_position_y, gate.left_position_z);
        RealOrientation realOrientationLeft(gate.left_orientation_x, gate.left_orientation_y,
                                            gate.left_orientation_z, gate.left_orientation_w);
        realPointLeft.realPosition = std::move(realPositionLeft);
        realPointLeft.realOrientation = std::move(realOrientationLeft);
        leftBlock.plannerPoints.push_back(realPointLeft);

        RealPoint realPointRight;
        RealPosition realPositionRight(gate.right_position_x, gate.right_position_y, gate.right_position_z);
        RealOrientation realOrientationRight(gate.right_orientation_x, gate.right_orientation_y,
                                             gate.right_orientation_z, gate.right_orientation_w);
        realPointRight.realPosition = std::move(realPositionRight);
        realPointRight.realOrientation = std::move(realOrientationRight);
        rightBlock.plannerPoints.push_back(realPointRight);

        std::vector<Room> rooms;
        SegmentationCenter::instance().gateSegmentation(segmented_map, rooms, gate);


        Point gateLeftPoint(gate.left_position_x, gate.left_position_y);
        Point gateRightPoint(gate.right_position_x, gate.right_position_y);

        auto cvGateLeftPoint = MapAttributeSingleton::instance().rosPoint2MapPoint(segmented_map.rows,
                                                                                   segmented_map.cols, gateLeftPoint);
        auto cvGateRightPoint = MapAttributeSingleton::instance().rosPoint2MapPoint(segmented_map.rows,
                                                                                    segmented_map.cols, gateRightPoint);

        leftValue = segmented_map.at<int>(cvGateLeftPoint);
        rightValue = segmented_map.at<int>(cvGateRightPoint);

        whole_display(segmented_map, rooms, cvGateLeftPoint, cvGateRightPoint, "GateSegmentation");

        hasGate = true;
    }

    // 根据来向和取向，判断是否经过闸机，经过则添加闸机2个摆渡点
    std::vector<RealBlock> gateBlockList;
    for (auto &block: wholeBlockList) {
        block.totalDistance = totalDistance;
        auto plannerPoints = block.plannerPoints;

        if (hasGate && plannerPoints.size() == 1) {
            Point lastBlockLastPoint(block.lastPoint.realPosition.x, block.lastPoint.realPosition.y);
            Point currentBlockFirstPoint(block.firstPoint.realPosition.x, block.firstPoint.realPosition.y);

            auto cvLastBlockLastPoint = MapAttributeSingleton::instance().rosPoint2MapPoint(
                    segmented_map.rows, segmented_map.cols, lastBlockLastPoint);
            auto cvCurrentBlockFirstPoint = MapAttributeSingleton::instance().rosPoint2MapPoint(
                    segmented_map.rows, segmented_map.cols, currentBlockFirstPoint);

            int lastValue = segmented_map.at<int>(cvLastBlockLastPoint);
            int currentValue = segmented_map.at<int>(cvCurrentBlockFirstPoint);

            if (lastValue != currentValue) {

                if (lastValue == leftValue && currentValue == rightValue) {
                    leftBlock.core_move = false;
                    gateBlockList.push_back(leftBlock);
                    leftBlock.core_move = true;
                    leftBlock.open_gate = true;
                    gateBlockList.push_back(leftBlock);
                    rightBlock.core_move = true;
                    leftBlock.open_gate = true;
                    gateBlockList.push_back(rightBlock);
                    gateBlockList.push_back(block);
                } else if (lastValue == rightValue && currentValue == leftValue) {
                    rightBlock.core_move = false;
                    gateBlockList.push_back(rightBlock);
                    rightBlock.core_move = true;
                    rightBlock.open_gate = true;
                    gateBlockList.push_back(rightBlock);
                    leftBlock.core_move = true;
                    leftBlock.open_gate = true;
                    gateBlockList.push_back(leftBlock);
                    gateBlockList.push_back(block);
                } else {
                    gateBlockList.push_back(block);
                }

            } else {
                gateBlockList.push_back(block);
            }
        } else {
            gateBlockList.push_back(block);
        }

    }
    realTask.setTotalStep(point_accumulation);
    realTask.setTotalFrequency(realTask.getRate());

    // 移除预制的最后一个点位
    gateBlockList.pop_back();

    // 交由上个方法处理
    for (const auto &block: gateBlockList) {
        blockList.emplace_back(block);
    }

//    for (const auto &block: blockList) {
//        for (const auto &point: block.plannerPoints) {
//
//            auto cvPoint = MapAttributeSingleton::instance().rosPoint2MapPoint(map.rows, map.cols,
//                                                                               Point(point.realPosition.x,
//                                                                                     point.realPosition.y));
//            const cv::Mat &mat = segmented_map.clone();
//            cv::circle(mat, cvPoint, 3, cv::Scalar(200), CV_FILLED);
//
//            cv::imshow("1", mat);
//            cv::waitKey();
//        }
//    }
}

std::vector<PoseVo> PointGenerator::recalculateAngle(const cv::Point2d &point2D,
                                                     const std::vector<PoseVo> &poseList) {
    std::vector<PoseVo> results(poseList.size());
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

        results[point_index] = PoseVo(poseList[point_index].getX(), poseList[point_index].getY(), theta);
    }
    return results;
}

std::vector<geometry_msgs::PoseStamped> PointGenerator::convertToGeometry(const std::vector<PoseVo> &complex) {
    std::vector<geometry_msgs::PoseStamped> results(complex.size());
    std_msgs::Header header;
    header.stamp = ros::Time::now();
    header.frame_id = "/map";
    for (size_t i = 0; i < complex.size(); ++i) {
        PoseVo vo = complex[i];

        results[i].header = header;
        results[i].header.seq = i;
        results[i].pose.position.x = vo.getX();
        results[i].pose.position.y = vo.getY();
        results[i].pose.position.z = 0.;
        Eigen::Quaterniond quaternion;
        quaternion = Eigen::AngleAxisd(vo.getTheta(), Eigen::Vector3d::UnitZ());
        tf::quaternionEigenToMsg(quaternion, results[i].pose.orientation);
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

        std::vector<PoseVo> poseList;
        std::vector<std::vector<PoseVo>> complexPoseList;

        std::vector<ZoneVo> zones = task.getZoned();
        for (const auto &zone: zones) {

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

            for (const auto &item: subPoseList) {
                poseList.push_back(item);
            }

            complexPoseList.push_back(subPoseList);
        }

        std::vector<geometry_msgs::Pose2D> exploration_path;
        for (const auto &item: poseList) {
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

        std::vector<RealBlock> blocks;
        complexPathToRealBlock(task, complexPoseList, blocks);
        return blocks;
    } else {
        auto coverage = TaskExploration::explorationPlanningPath(task);

        const std::vector<std::vector<PoseVo>> &complexList = coverage.getComplexList();
        std::vector<RealBlock> blocks;
        complexPathToRealBlock(task, complexList, blocks);

        return blocks;
    }

}
