//
// Created by Looper on 2022/11/28.
//

#ifndef APP_COMMUNICATION_SWITCHMODEPUBLISH_H
#define APP_COMMUNICATION_SWITCHMODEPUBLISH_H

#include <ros/node_handle.h>
#include <mutex>
#include <condition_variable>

//发布模式选择
class SwitchModePublish {
private:
    std::mutex cv_m;
    std::condition_variable cv;

    bool isPublish;
    bool isReset;

public:
    static auto &instance() {
        static SwitchModePublish obj;
        return obj;
    }

    void initialize(ros::NodeHandle handle);

    void publish();

    void cancel();

};


#endif //APP_COMMUNICATION_SWITCHMODEPUBLISH_H
