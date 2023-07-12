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
#include "task/RealBlock.h"
#include "task/callback/ICallback.h"
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <geometry_msgs/Pose2D.h>
#include "alignment/rrmap2.h"

class TaskFeedback : public ITaskCallback {
private:
    std::condition_variable cv;
    std::mutex cv_mut;

    std::thread task_feedback_thread;

    void task_feedback_thread_func();

    void generateRRMap();

    void onTaskStart(const RealTask &task) override;

    void onTaskProgress(const geometry_msgs::Pose &pose) override;

    void onTaskEnd() override;

    std::atomic<bool> record;

    std::string local_path;
    std::string savePath;

    int rows, cols;
    std::vector<int8_t> mapArray;

    MMapCharger rrMapCharger;
    MMapProhibition rrMapArea;
    MMapVirtually rrMapWall;
    MMapTarget rrMapTarget;
    MMapZone rrMapZone;
    MMapPath rrMapPath;
    MMapCover rrMapCover;

    MMapValid rrMapValid;

    PointVo currentPoint;
    std::set<PointVo> pointList;

public:

    TaskFeedback();

    void run();
};


#endif //APP_COMMUNICATION_FEEDBACK_H
