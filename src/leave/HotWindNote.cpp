//
// Created by Looper on 2023/5/12.
//

#include <std_msgs/Int32.h>
#include "leave/HotWindNote.h"
#include "simulation.h"
#include "manager/PublishInnerManager.h"

#define HOT_WIND_TIME_DURATION 20 * 60 * 1000L

HotWindNote::HotWindNote() {
//    time_hot_wind_milliseconds = std::numeric_limits<long>::max();
    time_hot_wind_milliseconds = 0;
    open_node = false;
    close_node = true;
    handle_thread = std::thread(&HotWindNote::hot_wind_handle_thread_func, this);
}

void HotWindNote::hot_wind_handle_thread_func() {
    for (;;) {
        sleep(1);

        long duration = currentMilliseconds() - time_hot_wind_milliseconds;

        if (duration < HOT_WIND_TIME_DURATION) {
            close_node = false;
            if (!open_node) {
                open_node = true;
                LOG_IF(INFO, DEBUG_MAINTENANCE) << "开启热风烘干 ... ";
                std_msgs::Int32 msg;
                msg.data = 1;
//                PublishInnerManager::instance().publishSelfClean(msg);
            }


        } else {
            open_node = false;
            if (!close_node) {
                close_node = true;
                LOG_IF(INFO, DEBUG_MAINTENANCE) << "关闭热风烘干 ... ";
                std_msgs::Int32 msg;
                msg.data = 0;
//                PublishInnerManager::instance().publishSelfClean(msg);
            }
        }

    }
}

long HotWindNote::currentMilliseconds() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

void HotWindNote::run() {
    handle_thread.detach();
}

void HotWindNote::openHotWind() {
    //当前时间戳（毫秒级别），记录开启热风烘干的时间
    time_hot_wind_milliseconds = currentMilliseconds();
}

void HotWindNote::closeHotWind() {
    time_hot_wind_milliseconds = currentMilliseconds() - HOT_WIND_TIME_DURATION;
}

/**
 * 是否在热风烘干中
 * 由于 time_hot_wind_milliseconds 表示烘干开启时间，因此可由当前时间与烘干开始时间的差值进行判断
 */
bool HotWindNote::isHotWind() {
    return currentMilliseconds() - time_hot_wind_milliseconds < HOT_WIND_TIME_DURATION;
}

void HotWindNoteSingleton::start(ros::NodeHandle handle) {
    getHotWindNote().run();
}

void HotWindNoteSingleton::openHotWind() {
    getHotWindNote().openHotWind();
}

/**
 * 手推建图
 * 任务执行
 * 手动模式
 * 维护模式
 */
void HotWindNoteSingleton::closeHotWind() {
    getHotWindNote().closeHotWind();
}

bool HotWindNoteSingleton::isHotWind() {
    return getHotWindNote().isHotWind();
}
