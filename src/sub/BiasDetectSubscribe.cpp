//
// Created by Ewen Dong
//

#include "sub/BiasDetectSubscribe.h"
#include <manager/CleanHistoryManager.h>

BiasDetectSubscribe::BiasDetectSubscribe(ros::NodeHandle handle)
        : handle(handle) {
    sub_bias_detect_ = handle.subscribe("/bias_detect", 1, &BiasDetectSubscribe::subscribeCallback, this);
}

BiasDetectSubscribe::~BiasDetectSubscribe() {
}

void BiasDetectSubscribe::subscribeCallback(std_msgs::Int16) {
//    string last_task_id = TaskManager::get_instance()->getLastTaskId();
//    CleanHistory clean_history;
//    CleanHistoryManager::get_instance()->GetCleanHistory(clean_history, last_task_id);
//    clean_history.setErrorCode(20003);
//    CleanHistoryManager::get_instance()->ResetCleanHistory(clean_history, last_task_id);
}
