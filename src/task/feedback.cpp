//
// Created by Looper on 2023/3/3.
//

#include "task/feedback.h"
#include "segmentation/SegmentationCenter.h"
#include "segmentation/map_attribute.h"
#include "db/segmentation_data_base.h"
#include "simulation.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "cppfs/fs.h"
#include "cppfs/FileHandle.h"
#include "cppfs/FilePath.h"

#define APPROX_EPSILON_INFINITELY 0.5

TaskFeedback::TaskFeedback() {
    task_feedback_thread = std::thread(&TaskFeedback::task_feedback_thread_func, this);

    record = false;
}

void TaskFeedback::run() {

    local_path = path::robot_slam_map_dir() + "local/";
    cppfs::FileHandle dir = cppfs::fs::open(local_path);
    if (!dir.isDirectory()) {
        dir.createDirectory();
    }

    const cv::Mat &map = SegmentationCenter::instance().generateMat();

    rows = map.rows;
    cols = map.cols;

    for (int y = 0; y < map.rows; y++) {
        for (int x = 0; x < map.cols; x++) {
            if (map.at<unsigned char>(y, x) == 255) {
                MMapExtend::writeByteToByteArray(mapArray, 15);
            } else {
                MMapExtend::writeByteToByteArray(mapArray, 0);
            }
        }
    }

    const cv::Point &stationPoint = MapAttribute::instance().rosPoint2MapPoint(rows, cols, Point(0, 0));
    rrMapCharger.setCharger(stationPoint.x + 10, stationPoint.y + 20, 0);

    auto penaltyZoneList = MapAttribute::instance().getPenaltyZoneList();
    std::vector<MZone> areas;
    for (const auto &vector: penaltyZoneList) {
        const cv::Point &point0 = MapAttribute::instance().rosPoint2MapPoint(rows, cols, vector[0]);
        const cv::Point &point1 = MapAttribute::instance().rosPoint2MapPoint(rows, cols, vector[1]);
        const cv::Point &point2 = MapAttribute::instance().rosPoint2MapPoint(rows, cols, vector[2]);
        const cv::Point &point3 = MapAttribute::instance().rosPoint2MapPoint(rows, cols, vector[3]);

        MPoint p0(point0.x, point0.y);
        MPoint p1(point1.x, point1.y);
        MPoint p2(point2.x, point2.y);
        MPoint p3(point3.x, point3.y);
        MZone zone(p0, p1, p2, p3);
        areas.push_back(zone);
    }
    rrMapArea.setProhibitions(areas);

    auto virtualWallList = MapAttribute::instance().getVirtualWallList();
    std::vector<MLine> walls;
    for (const auto &vector: virtualWallList) {
        const cv::Point &pointStart = MapAttribute::instance().rosPoint2MapPoint(rows, cols, vector[0]);
        const cv::Point &pointEnd = MapAttribute::instance().rosPoint2MapPoint(rows, cols, vector[1]);
        MPoint p0(pointStart.x, pointStart.y);
        MPoint p1(pointEnd.x, pointEnd.y);
        MLine rrLine(p0, p1);
        walls.push_back(rrLine);
    }
    rrMapWall.setVirtuallys(walls);

//    rrMapTarget.setTarget(cols / 3, rows / 3);

    task_feedback_thread.detach();
}

void TaskFeedback::task_feedback_thread_func() {

    for (;;) {
        {
            std::unique_lock<std::mutex> lock(cv_mut);
            if (record)
                generateRRMap();
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

}

void TaskFeedback::generateRRMap() {

    MMapResource rrMapSize(rows, cols);
    rrMapSize.setMapArray(mapArray);

    MMapRobot rrMapRobot;
    rrMapRobot.setRobot(currentPoint.getX(), currentPoint.getY(), -87);

    std::vector<MPoint> paths;
    if (pointList.size() > 3) {
        std::vector<cv::Point2f> cvPointList;
        for (const auto &point: pointList) {
            cvPointList.emplace_back(point.getX(), point.getY());
        }
        std::vector<cv::Point2f> list;
        cv::approxPolyDP(cvPointList, list, APPROX_EPSILON_INFINITELY, false);
        for (const auto &point: list) {
            paths.emplace_back(point.x, point.y);
        }
    } else {
        for (const auto &point: pointList) {
            paths.emplace_back(point.getX(), point.getY());
        }
    }

//    rrMapPath.setPoints(paths);

//    rrMapCover.setCovers(paths);

    RRMap rrMap;
    rrMap.addArray(std::make_unique<MMapResource>(rrMapSize));
    rrMap.addArray(std::make_unique<MMapCharger>(rrMapCharger));
    rrMap.addArray(std::make_unique<MMapRobot>(rrMapRobot));
    rrMap.addArray(std::make_unique<MMapTarget>(rrMapTarget));
    rrMap.addArray(std::make_unique<MMapPath>(rrMapPath));
    rrMap.addArray(std::make_unique<MMapProhibition>(rrMapArea));
    rrMap.addArray(std::make_unique<MMapVirtually>(rrMapWall));
    rrMap.addArray(std::make_unique<MMapZone>(rrMapZone));
    rrMap.addArray(std::make_unique<MMapCover>(rrMapCover));
    rrMap.addArray(std::make_unique<MMapValid>(rrMapValid));

    const std::vector<int8_t> byteArray = rrMap.toByteArray();

    std::stringstream input;
    for (int8_t b: byteArray) {
        input << b;
    }

    //save file
    std::ofstream file(savePath, std::ios_base::out | std::ios_base::binary);
    boost::iostreams::filtering_streambuf<boost::iostreams::output> outbuf;

    outbuf.push(boost::iostreams::gzip_compressor());
    outbuf.push(file);

    boost::iostreams::copy(input, outbuf);

    boost::iostreams::close(outbuf);
    file.close();

}

void TaskFeedback::onTaskStart(const RealTask &task) {
    {
        std::unique_lock<std::mutex> lock(cv_mut);

        pointList.clear();
        savePath = local_path + task.getId() + ".rrmap";

        if (task.isRenew()) {
            std::vector<MPoint> covers;
            for (const auto &block: task.getPlanBlocks()) {
                for (const auto &point: block.plannerPoints) {
                    const cv::Point cvPoint = MapAttribute::instance().rosPoint2MapPoint(
                            rows, cols, Point(point.realPosition.x, point.realPosition.y)
                    );
                    covers.emplace_back(cvPoint.x, cvPoint.y);
                }
            }
            rrMapCover.setCovers(covers);
            rrMapPath.setPoints(covers);

            TaskMode mode = SqliteDataBase::TaskModeFromInt(task.getMode());
            if (mode == TaskMode::Zoned) {
                std::vector<MZone> zones;
                std::vector<ZoneVo> taskZones = task.getZoned();
                for (const auto &tzp: taskZones) {
                    std::vector<PointVo> points = tzp.getPoints();
                    MPoint p0(points[0].getX(), points[0].getY());
                    MPoint p1(points[1].getX(), points[1].getY());
                    MPoint p2(points[2].getX(), points[2].getY());
                    MPoint p3(points[3].getX(), points[3].getY());
                    MZone zone(p0, p1, p2, p3);
                    zones.push_back(zone);
                }
                rrMapZone.setZones(zones);
            }
        } else {
            std::vector<MZone> zones;
            rrMapZone.setZones(zones);
        }

        record = true;
    }
}

void TaskFeedback::onTaskProgress(const geometry_msgs::Pose &pose) {
    {
        std::unique_lock<std::mutex> lock(cv_mut);

        Point point(pose.position.x, pose.position.y);
        cv::Point cvPoint = MapAttribute::instance().rosPoint2MapPoint(rows, cols, point);
        currentPoint.setX(cvPoint.x);
        currentPoint.setY(cvPoint.y);
        pointList.insert(currentPoint);
    }
}

void TaskFeedback::onTaskEnd() {
    {
        std::unique_lock<std::mutex> lock(cv_mut);
        record = false;
    }
}
