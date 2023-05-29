//
// Created by Looper on 2023/3/3.
//

#ifndef APP_COMMUNICATION_FEEDBACK_H
#define APP_COMMUNICATION_FEEDBACK_H

#include <condition_variable>
#include <deque>
#include "future/async_call.h"
#include "model/Point.h"
#include "model/RoomVo.h"
#include "RealPoint.h"
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <geometry_msgs/Pose2D.h>

class TaskFeedback : public AsyncCall {
private:
    std::condition_variable cv;
    std::mutex cv_mut;

    std::deque<int> orderDeque;
    std::deque<geometry_msgs::Pose2D> dataDeque;

    std::set<PointVo> points;

    std::string run_task_id;
    std::vector<PoseVo> planPoseVos;


    cv::Point2d map_origin;
    int rows, cols;

    std::string savePath;
    int spacing_half;

    int area_px;
    int plan_px;

    void start();

    void end();

    void feedback(geometry_msgs::Pose2D data);

    cv::Point poseTransferPoint(float x, float y);

public:

    TaskFeedback();

    void execute() override;

    void triggerStart(std::string taskId, const std::vector<RealPoint> &points);

    void triggerEnd();

    void triggerFeedback(geometry_msgs::Pose2D data);

};


#endif //APP_COMMUNICATION_FEEDBACK_H
