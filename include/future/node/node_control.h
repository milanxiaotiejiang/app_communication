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
#include "geometry_msgs/Pose.h"

#define  THREAD_POOL_MAX_NUM 16

class NodeControl {
private:
    ros::NodeHandle nodeHandle;
    async::ThreadPool pool_;
    NodeControlSubscribe *subscribe;

    std::atomic<node::State> state_{node::State::sleep};
    std::atomic<node::WorkState> work_state_{node::WorkState::normal};
    std::atomic<node::MapState> map_state_{node::MapState::normal};

    NodeSubject *nodeSubject{};
    NodeObserver *nodeObserver{};

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

    void onMap();

    void offWork();

    void offMap();

    void onSleep();

    void trySleep();

    void setWorkMode(node::State state);

    void resetLocalization(bool open);

public:
    std::atomic<int> heart_beat;
    std::atomic<int> carto_mode;

    static auto &instance() {
        static NodeControl obj;
        return obj;
    }

    static void system_start(const std::string &ns) {
        LOG(ERROR) << "system start order is " << ns;
        std::system(ns.data());
    }

    static void system_kill(const std::string &ns) {
        std::string kill_str = "ps -ef | grep '" + ns + "' | grep -v grep | awk '{print $2}' | xargs kill -s 9";
//        std::string kill_str = "pgrep " + ns + " | xargs kill -s 9";
        LOG(ERROR) << "system kill order is " << kill_str;
        std::system(kill_str.data());
    }

    template<typename F, typename... Args>
    void asyncOn(F &&f, Args &&... args) {
        auto func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        pool_.execute([this, t = std::move(func)]() {
            t();
        });
    }

    void initialize(ros::NodeHandle handle);

    void release();

    void offSleep();

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

    void emulate();

    static void paramPose(const std::string& key, const geometry_msgs::Pose pose);
};

#endif //APP_COMMUNICATION_NODE_CONTROL_H

/*
 *
#!/usr/bin/env python
import subprocess
import rospy
import rosnode
from std_msgs.msg import Int32
from std_msgs.msg import String
from geometry_msgs.msg import PoseStamped

import time

class launch_demo:

    launch = False

    def __init__(self, cmd=None):
        self.cmd = cmd

    def launch_node(self):
        self.child = subprocess.Popen(self.cmd)
        self.launch = True
        return True

    def shutdown_node(self):
        self.launch = False
        self.child.terminate()
        self.child.wait()
        return True

#work_mode:
# 0----mapping_mode
# 1----waitting_mode
# 2----working_mode
#default is 2
work_mode = 2

#launch build_map
launch_build_map = launch_demo(["roslaunch", "launch_center", "build_mapping.launch"])
launch_submap_to_map = launch_demo(["roslaunch", "launch_center", "submap_to_map.launch"])

#launch localization
launch_localization = launch_demo(["roslaunch", "launch_center", "localization.launch"])
launch_load_map = launch_demo(["roslaunch", "launch_center", "load_map.launch"])

#launch navigation
launch_navigation = launch_demo(["roslaunch", "launch_center", "navigation.launch"])

#launch camera
launch_camera1 = launch_demo(["roslaunch", "launch_center", "inu_dev1.launch"])
launch_camera2 = launch_demo(["roslaunch", "launch_center", "inu_dev2.launch"])


def switch_callback(data):
    global map_loaded
    global work_mode
    target_mode = data.data

    #entering mapping mode
    if(target_mode == 0)://建图
        work_mode = 0
        rospy.set_param("/localization", False)
        if(launch_navigation.launch):
            launch_navigation.shutdown_node()
        if(launch_localization.launch):
            launch_localization.shutdown_node()
        if(launch_load_map.launch):
            launch_load_map.shutdown_node()
        time.sleep(1.0)
        if(not launch_build_map.launch):
            launch_build_map.launch_node()
        if(not launch_submap_to_map.launch):
            launch_submap_to_map.launch_node()
        rospy.set_param("~work_mode", 0)


    #entering  waiting mode
    if(target_mode == 1)://睡眠
        work_mode = 1
        if(launch_build_map.launch):
            launch_build_map.shutdown_node()
        if(launch_submap_to_map.launch):
            launch_submap_to_map.shutdown_node()
        if(launch_navigation.launch):
            launch_navigation.shutdown_node()
        if(launch_localization.launch):
            launch_localization.shutdown_node()
        time.sleep(0.5)
        rospy.set_param("~work_mode", 1)


    #entering woring mode
    if(target_mode == 2)://工作
        if(launch_submap_to_map.launch):
            launch_submap_to_map.shutdown_node()
        if(launch_build_map.launch):
            launch_build_map.shutdown_node()
        if(not launch_navigation.launch):
            launch_navigation.launch_node()
        if(not launch_load_map.launch):
            launch_load_map.launch_node()
        if(not launch_localization.launch):
            launch_localization.launch_node()
        time.sleep(1.0)
        work_mode = 2
        rospy.set_param("~work_mode", 2)

def carto_heart_bit_callback(data):
    global last_pose_stamp
    last_pose_stamp = rospy.get_rostime().to_sec()

if __name__ == "__main__":
    rospy.init_node('node_control', anonymous=True)
    time.sleep(20.0)
    launch_camera2.launch_node()
    time.sleep(2.0)
    launch_camera1.launch_node()

    rate = rospy.Rate(0.2)
    while not rospy.is_shutdown():
        rate.sleep()





 */