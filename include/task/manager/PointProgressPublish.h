//
// Created by Looper on 2022/11/27.
//

#ifndef APP_COMMUNICATION_POINTPROGRESSPUBLISH_H
#define APP_COMMUNICATION_POINTPROGRESSPUBLISH_H

#include <ros/node_handle.h>
#include "task/model/PointProgressVo.h"

class PointProgressPublish {
private:
    PointProgressPublish() = default;

    PointProgressPublish(PointProgressPublish &) = delete;

    PointProgressPublish &operator=(const PointProgressPublish &) = delete;

public:
    ~PointProgressPublish() = default;

private:
    ros::Publisher pub_task_point_;

public:
    static auto &instance() {
        static PointProgressPublish obj;
        return obj;
    }

    void initialize(ros::NodeHandle handle);

    void publishProgressPoint(const PointProgressVo &pointProgressVo) const;
};


#endif //APP_COMMUNICATION_POINTPROGRESSPUBLISH_H
