//
// Created by Looper on 2022/11/28.
//

#include <std_msgs/Int32.h>
#include <thread>
#include "task/manager/SwitchModePublish.h"
#include "glog/logging.h"

//0 建图 1 睡眠 //2 工作
void SwitchModePublish::initialize(ros::NodeHandle handle) {
    pub_node_mode_ = handle.advertise<std_msgs::Int32>("/mrrobot/switch_mode", 10);

    //开个线程lambda表达式
    std::thread postponePublish(
            [this]() {
                for (;;) {
                    std::unique_lock<std::mutex> lk(cv_m);
                    auto delay = std::chrono::milliseconds(15 * 1000);
                    auto now = std::chrono::system_clock::now();
                    auto end = now + delay;
                    cv.wait_until(lk, end, [this]() {
                        return isReset;
                    });

                    if (!isReset) {
                        if (isPublish) {
                            LOG(INFO) << "发布睡眠模式 ...";
                            std_msgs::Int32 result;
                            result.data = 1;
                            pub_node_mode_.publish(result);
                            isPublish = false;
                        }
                    }
                    isReset = false;
                }
            }
    );
    postponePublish.detach();
}

void SwitchModePublish::mapping() {
    std_msgs::Int32 result;
    result.data = 0;
    pub_node_mode_.publish(result);
}

void SwitchModePublish::publish() {
    {
        std::unique_lock<std::mutex> lk(cv_m);
        isPublish = true;
        isReset = true;
    }
    cv.notify_one();
}

void SwitchModePublish::cancel() {
    {
        std::unique_lock<std::mutex> lk(cv_m);
        isPublish = false;
        isReset = false;
    }
    cv.notify_one();
}