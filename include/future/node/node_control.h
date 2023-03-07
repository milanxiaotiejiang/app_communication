//
// Created by Looper on 2023/3/7.
//

#ifndef APP_COMMUNICATION_NODE_CONTROL_H
#define APP_COMMUNICATION_NODE_CONTROL_H

#include <cassert>
#include <iostream>
#include <list>
#include <utility>
#include "string"
#include "future/thread_pool.h"
#include "glog/logging.h"
#include "node_manager.h"
#include "child_activate_node.h"
#include "node_observer_mode.h"
#include "machine.h"
#include "node_control_subscribe.h"

#define  THREAD_POOL_MAX_NUM 16

const std::string N_GAZEBO = "roslaunch turtlebot3_gazebo turtlebot3_test.launch > rviz.log 2>&1";

const std::string K_RVIZ = "rosnode kill /rviz";

class NodeControl {
private:
    template<typename F, typename... Args>
    void asyncOn(F &&f, Args &&... args) {
        auto func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        pool_.execute([this, t = std::move(func)]() {
            t();
        });
    }

    template<typename F, typename... Args>
    void asyncOff(int seconds, F &&f, Args &&... args) {
        std::mutex mutex;
        std::condition_variable cond;

        auto func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        pool_.execute([this, t = std::move(func), &cond]() {
            t();
            cond.notify_one();
        });

        std::unique_lock<std::mutex> guard(mutex);
        cond.wait_for(guard, std::chrono::seconds(seconds));
    }

    void onWork();

    void offWork();

    void onMap();

    void offMap();

    void trySleep();

public:
    static auto &instance() {
        static NodeControl obj;
        return obj;
    }

    async::ThreadPool pool_;
    NodeControlSubscribe *subscribe;

    std::atomic<node::State> state_{node::State::sleep};
    std::atomic<node::WorkState> work_state_{node::WorkState::normal};
    std::atomic<node::MapState> map_state_{node::MapState::normal};

    NodeSubject *nodeSubject{};
    NodeObserver *nodeObserver{};

    void initialize(ros::NodeHandle handle);

    void release();

    bool isWork() const {
        return state_ == node::State::work && work_state_ == node::WorkState::complete;
    }

    bool isMap() const {
        return state_ == node::State::map && map_state_ == node::MapState::complete;
    }

    bool isSleep() const {
        return state_ == node::State::sleep
               && work_state_ == node::WorkState::normal
               && map_state_ == node::MapState::normal;
    }

    void update();

    void changeWorkMode();

    void changeMapMode();

    void changeSleepMode();
};


#endif //APP_COMMUNICATION_NODE_CONTROL_H
