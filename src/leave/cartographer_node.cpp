//
// Created by Looper on 2023/4/11.
//

#include "leave/cartographer_node.h"
#include "exploration/ExplorationCenter.h"
#include "future/node/node_control.h"
#include "future/timer_call.h"
#include "simulation.h"
#include "back_charge_msgs/sensor_status.h"
#include "back_charge_msgs/ready_check.h"
#include "back_charge_msgs/start_localization.h"
#include "back_charge_msgs/stop_localization.h"
#include "db/segmentation_data_base.h"
#include "segmentation/SegmentationCenter.h"
#include "leave/map_control.h"
#include "exploration/handle_exploration_display.h"

void CartographerPublisher::initialize(ros::NodeHandle handle) {
    save_map = handle.advertise<std_msgs::Int32>("/save_map", 1);
    update_map = handle.advertise<std_msgs::Int32>("/update_map", 1);
    start_carto_mapping = handle.advertise<std_msgs::Int32>("/start_carto_mapping", 1);
    start_carto_localization = handle.advertise<std_msgs::Int32>("/start_carto_localization", 1);
    shutdown_carto = handle.advertise<std_msgs::Int32>("/shutdown_carto", 1);
    restart_carto = handle.advertise<std_msgs::Int32>("/restart_carto", 1);
    initial_pose = handle.advertise<geometry_msgs::PoseWithCovarianceStamped>("/initial_pose", 1);
    clear_current_pose = handle.advertise<std_msgs::Int32>("/clear_current_pose", 1);

    control_move_base = handle.advertise<std_msgs::Int32>("/control_move_base", 1);
}

void CartographerPublisher::publishSaveMap() const {
    LOG(INFO) << "rec to carto save map";
    std_msgs::Int32 message;
    message.data = 1;
    save_map.publish(message);
}

void CartographerPublisher::publishUpdateMap() const {
    save_dynamic_map("dynamic_before");

    std_msgs::Int32 message;
    message.data = 1;
    update_map.publish(message);
    if (!Environment::instance().isRealEnvironment) {
        async::TimerCall::instance().baseLoop()->scheduleLater(std::chrono::seconds(10), []() {
            CartographerSubscribe::instance().coverResult();
        });
    }
}

void CartographerPublisher::publishStartCartoMapping() const {
    std_msgs::Int32 message;
    message.data = 1;
    start_carto_mapping.publish(message);
}

void CartographerPublisher::publishStartCartoLocalization() const {
    std_msgs::Int32 message;
    message.data = 1;
    start_carto_localization.publish(message);
}

void CartographerPublisher::publishShutdownCarto() const {
    std_msgs::Int32 message;
    message.data = 1;
    shutdown_carto.publish(message);
}

void CartographerPublisher::publishRestartCarto() const {
    std_msgs::Int32 message;
    message.data = 1;
    restart_carto.publish(message);
}

void CartographerPublisher::publishInitialPose(const geometry_msgs::PoseWithCovarianceStamped &pose) const {
    initial_pose.publish(pose);
}

void CartographerPublisher::publishClearCurrentPose() const {
    std_msgs::Int32 message;
    message.data = 1;
    clear_current_pose.publish(message);
}

void CartographerPublisher::publishControlMoveBase(bool open) const {
    if (open) {
        std_msgs::Int32 message;
        message.data = 1;
        control_move_base.publish(message);
    } else {
        std_msgs::Int32 message;
        message.data = 0;
        control_move_base.publish(message);
    }
}

void CartographerSubscribe::initialize(ros::NodeHandle handle) {
    update_finish = handle.subscribe("/update_finish", 1, &CartographerSubscribe::updateFinishCallback, this);
    build_map_finish = handle.subscribe("/build_map_finish", 1, &CartographerSubscribe::buildMapFinishCallback, this);
    carto_heart_beat = handle.subscribe("/carto_heart_beat", 1, &CartographerSubscribe::cartoHeartBeatCallback, this);
    current_cartographer_pose = handle.subscribe("/current_cartographer_pose", 1,
                                                 &CartographerSubscribe::currentCartographerPoseCallback, this);
}

void CartographerSubscribe::updateFinishCallback(const std_msgs::Int32 &carto_result) {
    if (carto_result.data == 1) {

        MapAttribute currentAttr = MapAttributeSingleton::instance().getCurrentMapAttribute();
        LOG_IF(INFO, DEBUG_NODE) << "currentAttr  " << currentAttr;

        MapAttribute changeAttr(path::map_yaml_path());
        MapAttributeSingleton::instance().readAnyMapInfo(changeAttr);
        LOG_IF(INFO, DEBUG_NODE) << "changeAttr  " << changeAttr;

        const cv::Point2d diffPoint = changeAttr.originPoint - currentAttr.originPoint;
        LOG_IF(INFO, DEBUG_NODE) << "diffPoint  " << diffPoint;

        MapPo mapPo = SegmentationDataBase::instance().getDbMap();
        const std::vector<TaskVo> &tasks = TaskDataBase::instance().loadTaskFoMap(mapPo.id);

        for (auto task: tasks) {
            TaskMode mode = SqliteDataBase::TaskModeFromInt(task.getMode());
            if (mode == TaskMode::Zoned) {

                std::vector<ZoneVo> replaceZones;
                std::vector<ZoneVo> zones = task.getZones();

                for (const auto &zone: zones) {

                    std::vector<PointVo> replacePoints;
                    for (const auto &point: zone.getPoints()) {
                        cv::Point2d cvPoint(point.getX(), point.getY());
                        auto changePoint = cvPoint + diffPoint;

                        replacePoints.emplace_back(changePoint.x, changePoint.y);
                    }
                    ZoneVo replaceZone(zone.getZoneId(), replacePoints);

                    replaceZones.push_back(replaceZone);
                }

                task.setZones(replaceZones);
                TaskDataBase::instance().modifyTask(task);
            }
        }

        const auto &generateMat = SegmentationCenter::instance().generateMat();
        auto gateList = SegmentationDataBase::instance().loadGate(mapPo.id);
        for (const auto &gate: gateList) {

            LOG_IF(INFO, DEBUG_NODE) << "gate  " << gate;

            cv::Point2d ps;
            cv::Point2d pe;

            // 预埋点
            std::vector<cv::Point> intersectionPoints;
            cv::Point2d originalStartPoint(gate.start_x, gate.start_y);
            auto changeStartPoint = originalStartPoint + diffPoint;
            cv::Point2d originalEndPoint(gate.end_x, gate.end_y);
            auto changeEndPoint = originalEndPoint + diffPoint;

            intersectionPoints.push_back(changeStartPoint);
            intersectionPoints.push_back(changeEndPoint);

            // 求直线
            double m = (changeEndPoint.x - changeStartPoint.x) != 0.0 ?
                       (changeEndPoint.y - changeStartPoint.y) / (changeEndPoint.x - changeStartPoint.x)
                                                                      : std::numeric_limits<double>::infinity();
            double b = changeStartPoint.y - m * changeStartPoint.x;

            cv::Point startPoint, endPoint;

            if (m != 0.0) {
                // 如果斜率不是零，计算与左边界的交点
                startPoint.x = 0;
                startPoint.y = static_cast<int>(b);

                // 计算与右边界的交点
                endPoint.x = changeAttr.mapCols - 1;
                endPoint.y = static_cast<int>(m * endPoint.x + b);
            } else {
                // 如果斜率为零，直线与上边界平行，交点在y轴上
                startPoint.x = static_cast<int>(changeStartPoint.x);
                startPoint.y = 0;

                endPoint.x = static_cast<int>(changeStartPoint.x);
                endPoint.y = changeAttr.mapRows - 1;
            }

            LOG_IF(INFO, DEBUG_NODE) << "startPoint  " << startPoint << " , endPoint  " << endPoint;

            cv::LineIterator lineIterator(generateMat, startPoint, endPoint);

            // 求轮廓
            std::vector<std::vector<cv::Point>> contours;
            cv::findContours(generateMat, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

            size_t largestContourIndex = 0;
            double largestContourArea = 0.0;

            for (size_t i = 0; i < contours.size(); i++) {
                double area = cv::contourArea(contours[i]);
                if (area > largestContourArea) {
                    largestContourArea = area;
                    largestContourIndex = i;
                }
            }

            auto maxContour = contours[largestContourIndex];

            for (int i = 0; i < lineIterator.count; i++, ++lineIterator) {
                cv::Point linePoint = lineIterator.pos();
                bool isIntersection = cv::pointPolygonTest(maxContour, linePoint, false) == 0;
                if (isIntersection)
                    intersectionPoints.push_back(linePoint);
            }

            LOG_IF(INFO, DEBUG_NODE) << "intersectionPoints.size  " << intersectionPoints.size();

            // 求最佳
            size_t minStartIndex = 0;
            double minStartDistance = INT_MAX;
            for (size_t i = 0; i < intersectionPoints.size(); i++) {
                double distance = cv::norm(startPoint - intersectionPoints[i]);
                if (distance < minStartDistance) {
                    minStartDistance = distance;
                    minStartIndex = i;
                }
            }
            auto closestStartPoint = intersectionPoints[minStartIndex];
            auto neutralStartPoint = cv::Point((startPoint.x + closestStartPoint.x) / 2,
                                               (startPoint.y + closestStartPoint.y) / 2);
            ps = generateMat.at<unsigned char>(neutralStartPoint) == 255 ? startPoint : neutralStartPoint;

            size_t minEndIndex = 0;
            double minEndDistance = INT_MAX;
            for (size_t i = 0; i < intersectionPoints.size(); i++) {
                double distance = cv::norm(endPoint - intersectionPoints[i]);
                if (distance < minEndDistance) {
                    minEndDistance = distance;
                    minEndIndex = i;
                }
            }
            auto closestEndPoint = intersectionPoints[minEndIndex];
            auto neutralEndPoint = cv::Point((endPoint.x + closestEndPoint.x) / 2,
                                             (endPoint.y + closestEndPoint.y) / 2);
            pe = generateMat.at<unsigned char>(neutralEndPoint) == 255 ? endPoint : neutralEndPoint;

            LOG_IF(INFO, DEBUG_NODE)
                            << "closestStartPoint  " << closestStartPoint << " , closestEndPoint  " << closestEndPoint;
            LOG_IF(INFO, DEBUG_NODE) << "ps  " << ps << " , pe  " << pe;

            SegmentationDataBase::instance().modifyGateLine(gate.id, ps.x, ps.y, pe.x, pe.y);
        }

        MapControl::instance().backupMap(SegmentationDataBase::instance().getDbMap().id, false);

        MapAttributeSingleton::instance().loadStation();
        SegmentationCenter::instance().resetSegmentation();
        ExplorationCenter::instance().repaintCoveragePath();
        CartographerSubscribe::instance().coverResult();

        save_dynamic_map("dynamic_after");
    }
}

void CartographerSubscribe::buildMapFinishCallback(const std_msgs::Int32 &carto_result) {
    LOG(INFO) << "carto to rec finish map " << carto_result.data;
    if (carto_result.data == 1) {
        MapAttributeSingleton::instance().notifySaveMap();
    }
}

void CartographerSubscribe::cartoHeartBeatCallback(const std_msgs::Int32 &carto_result) {
    if (carto_result.data == 1) {
        if (NodeControl::instance().heart_beat > 10000)
            NodeControl::instance().heart_beat = 0;
        NodeControl::instance().heart_beat++;
    }
}

void
CartographerSubscribe::currentCartographerPoseCallback(const geometry_msgs::PoseWithCovarianceStamped &carto_result) {

}

void CartographerSubscribe::coverResult() {
    asyncTaskCall->executeCover();
}

void CartographerServiceClient::initialize(ros::NodeHandle handle) {
    sensor_status = handle.serviceClient<back_charge_msgs::sensor_status>("sensor_status");
    ready_check = handle.serviceClient<back_charge_msgs::ready_check>("ready_check");
    start_localization = handle.serviceClient<back_charge_msgs::start_localization>("start_localization");
    stop_localization = handle.serviceClient<back_charge_msgs::stop_localization>("stop_localization");
}

bool CartographerServiceClient::callSensorStatus() {
    if (!Environment::instance().isRealEnvironment) {
        return true;
    }
    back_charge_msgs::sensor_status srv;
    bool result = sensor_status.call(srv);
    if (result) {
        bool hlsStatus = srv.response.hls_status;//电机失能
        bool imuStatus = srv.response.imu_status;//imu
        bool laserStatus = srv.response.laser_status;//激光雷达
        bool localizationStatus = srv.response.localization_status;//定位
        bool camera1Status = srv.response.camera1_status;
        bool camera2Status = srv.response.camera2_status;
        LOG_IF(INFO, DEBUG_NODE) << "callSensorStatus  hlsStatus : " << hlsStatus
                                 << " , imuStatus : " << imuStatus
                                 << " , laserStatus : " << laserStatus
                                 << " , localizationStatus : " << localizationStatus
                                 << " , camera1Status : " << camera1Status
                                 << " , camera2Status : " << camera2Status;
    } else {
        LOG(ERROR) << "Failed to call service sensor_status ...";
    }
    return result;
}

bool CartographerServiceClient::callReadyCheck() {
    if (!Environment::instance().isRealEnvironment) {
        return true;
    }
    back_charge_msgs::ready_check srv;
    bool result = ready_check.call(srv);
    if (result) {
        bool hlsStatus = srv.response.hls_status;//电机失能
        bool imuStatus = srv.response.imu_status;//imu
        bool laserStatus = srv.response.laser_status;//激光雷达
        bool bumpTriggeredStatus = srv.response.bump_triggered;//后碰撞
        bool camera1Status = srv.response.camera1_status;
        bool camera2Status = srv.response.camera2_status;
        LOG_IF(INFO, DEBUG_NODE) << "callReadyCheck  hlsStatus : " << hlsStatus
                                 << " , imuStatus : " << imuStatus
                                 << " , laserStatus : " << laserStatus
                                 << " , bumpTriggeredStatus : " << bumpTriggeredStatus
                                 << " , camera1Status : " << camera1Status
                                 << " , camera2Status : " << camera2Status;
        return hlsStatus && imuStatus && laserStatus && !bumpTriggeredStatus && camera1Status && camera2Status;
    } else {
        LOG(ERROR) << "Failed to call service ready_check ...";
        return result;
    }
}

bool CartographerServiceClient::callStartLocalization() {
//    if (!Environment::instance().isRealEnvironment) {
//        return true;
//    }
//
//    LOG_IF(INFO, DEBUG_NODE) << "callStartLocalization  ready call ... ";
//
//    back_charge_msgs::start_localization srv;
//    bool result = start_localization.call(srv);
//    if (result) {
//        bool tfValid = srv.response.tf_valid;
//        LOG_IF(INFO, DEBUG_NODE) << "callStartLocalization  tfValid : " << tfValid;
//        return tfValid;
//    } else {
//        LOG(ERROR) << "Failed to call service start_localization ...";
//        return result;
//    }
    return true;
}

bool CartographerServiceClient::callStopLocalization() {
//    if (!Environment::instance().isRealEnvironment) {
//        return true;
//    }
//    back_charge_msgs::stop_localization srv;
//    bool result = stop_localization.call(srv);
//    if (result) {
//        bool tfValid = srv.response.tf_valid;
//        LOG_IF(INFO, DEBUG_NODE) << "callStopLocalization  tfValid : " << tfValid;
//        return !tfValid;
//    } else {
//        LOG(ERROR) << "Failed to call service stop_localization ...";
//        return result;
//    }
    return true;
}
