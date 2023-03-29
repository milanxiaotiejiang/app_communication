//
// Created by Looper on 2022/11/27.
//

#include <std_msgs/String.h>
#include "task/manager/PointProgressPublish.h"
#include "net/WsServerManager.h"
#include "net/base/RequestModel.h"

void PointProgressPublish::initialize(ros::NodeHandle handle) {
    pub_task_point_ = handle.advertise<std_msgs::String>(TASK_POINT, 10);
}

void PointProgressPublish::publishProgressPoint(const PointProgressVo &pointProgressVo) const {
    RequestModel<PointProgressVo> requestModel;
    requestModel.setOp("publish");
    requestModel.setTopic(TASK_POINT);
    requestModel.setMsg(pointProgressVo);

    json jsonResult = requestModel;

    WsServerManager::instance().sendRequestData(TASK_POINT, jsonResult.dump());

    std_msgs::String result;
    result.data.append(jsonResult.dump());
    pub_task_point_.publish(result);
}
