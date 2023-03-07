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

#define  THREAD_POOL_MAX_NUM 16

const std::string N_GAZEBO = "roslaunch turtlebot3_gazebo turtlebot3_test.launch > rviz.log 2>&1";


const std::string K_RVIZ = "rosnode kill /rviz";

namespace node {

    enum class State {
        normal,
        started,
        stopped,
    };

    class NodeControl {
    public:
        static auto &instance() {
            static NodeControl obj;
            return obj;
        }

        async::ThreadPool pool_;

        std::atomic<State> state_{State::normal};

        NodeSubject *nodeSubject{};
        NodeObserver *nodeObserver{};

        void initialize();

        void release();

        void start() {
            std::thread nodeThread([this]() {

                std::mutex count_mutex;
                std::condition_variable count_cond;

                NodeChain chain(&pool_);

                auto *pFilterManager = new NodeManager(new OnceConfirm());
                pFilterManager->setNodeSubject(nodeSubject);
                pFilterManager->addActivateNode(new DumpActivateNode(2));
                pFilterManager->addActivateNode(new RvizActivateNode(3));

                bool isSuccessful = pFilterManager->activateNode(chain);
                if (isSuccessful) {
                    state_ = State::started;
                }

                delete pFilterManager;

            });
            nodeThread.detach();
        }

        void stop() {
            pool_.execute([]() {
                std::system(K_RVIZ.data());
            });
        }
    };


}


#endif //APP_COMMUNICATION_NODE_CONTROL_H
