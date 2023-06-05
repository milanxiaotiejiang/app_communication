//
// Created by Looper on 2023/3/29.
//

#ifndef APP_COMMUNICATION_PATH_EXPLORATION_PREVIEW_TASK_H
#define APP_COMMUNICATION_PATH_EXPLORATION_PREVIEW_TASK_H

#include "model/task.h"
#include "task/RealTask.h"
#include <geometry_msgs/Pose2D.h>

class TaskExploration {
public:

    static void task2RealTask(const Task &task, RealTask &realTask);

    static void task2RealTask(const TaskVo &task, RealTask &realTask);

    static RoomCoverage explorationPlanningPath(const RealTask &task);

    static void planningPath2RoomCoverage(RoomCoverage &roomCoverage,
                                                  const std::vector<geometry_msgs::Pose2D> &exploration_path,
                                                  const std::vector<cv::Point> &point_path,
                                                  const std::vector<std::vector<geometry_msgs::Pose2D>> &complex_path);
};


#endif //APP_COMMUNICATION_PATH_EXPLORATION_PREVIEW_TASK_H
