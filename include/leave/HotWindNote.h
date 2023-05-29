//
// Created by Looper on 2023/5/12.
//

#ifndef APP_COMMUNICATION_HOTWINDNOTE_H
#define APP_COMMUNICATION_HOTWINDNOTE_H

#include <ros/node_handle.h>
#include "thread"

class HotWindNote {
private:

    std::thread handle_thread;

    long time_hot_wind_milliseconds;
    bool open_node;
    bool close_node;

    void hot_wind_handle_thread_func();

    long currentMilliseconds();

public:
    HotWindNote();

    void run();

    void openHotWind();

    void closeHotWind();

    bool isHotWind();
};

class HotWindNoteSingleton {
public:
    static HotWindNoteSingleton &instance() {
        static HotWindNoteSingleton instance;
        return instance;
    }

    HotWindNote &getHotWindNote() {
        return hotWindNote;
    }

    void start(ros::NodeHandle handle);

    void openHotWind();

    void closeHotWind();

    bool isHotWind();

    HotWindNoteSingleton(const HotWindNoteSingleton &) = delete;

    HotWindNoteSingleton &operator=(const HotWindNoteSingleton &) = delete;

    HotWindNoteSingleton(HotWindNoteSingleton &&) = delete;

    HotWindNoteSingleton &operator=(HotWindNoteSingleton &&) = delete;

private:
    HotWindNote hotWindNote;

    HotWindNoteSingleton() = default;

    ~HotWindNoteSingleton() = default;
};


#endif //APP_COMMUNICATION_HOTWINDNOTE_H
