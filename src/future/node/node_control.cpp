//
// Created by Looper on 2023/3/7.
//

#include "future/node/node_control.h"
#include "manager/PublishInnerManager.h"
#include "simulation.h"

void NodeControl::initialize(ros::NodeHandle handle) {
    pool_.setNumOfThreads(THREAD_POOL_MAX_NUM);

    subscribe = new NodeControlSubscribe(handle);
    cartoHeartBeat = new CartoHeartBeat(handle);

    nodeSubject = new AbnormalSubject();
    nodeObserver = new AbnormalObserver(nodeSubject);
    nodeSubject->attach(nodeObserver);

    pub_clear_odom = handle.advertise<std_msgs::Int32>("/mrrobot/clear_odom", 1);

    asyncOn([]() {
        if (Environment::instance().isRealEnvironment) {
            system_start(n_load_map);
        } else {
            system_start(n_tt_load_map);
        }
    });
}

void NodeControl::release() {
    nodeSubject->detach(nodeObserver);
    nodeObserver = nullptr;
    nodeSubject = nullptr;
}

void NodeControl::onWork() {
    work_state_ = node::WorkState::way;
    asyncOn([this]() {
        NodeChain chain(&pool_);

        resetLocalization(true);

        auto *pFilterManager = new NodeManager(new OnceConfirm());
        pFilterManager->setNodeSubject(nodeSubject);
        if (Environment::instance().isRealEnvironment) {
            pFilterManager->addActivateNode(new NavigationActivateNode(2));
            pFilterManager->addActivateNode(new LoadMapActivateNode(2));
            pFilterManager->addActivateNode(new LocalizationActivateNode(2));
        } else {
            pFilterManager->addActivateNode(new NavigationActivateNode(1));
            pFilterManager->addActivateNode(new LoadMapActivateNode(1));
            pFilterManager->addActivateNode(new RvizActivateNode(1));
        }

        bool isSuccessful = pFilterManager->activateNode(chain);
        if (isSuccessful) {
            state_ = node::State::work;
            work_state_ = node::WorkState::complete;
        } else {
            trySleep();
        }
        delete pFilterManager;
    });
}

void NodeControl::onMap() {
    map_state_ = node::MapState::way;
    asyncOn([this]() {
        NodeChain chain(&pool_);

        resetLocalization(false);

        auto *pFilterManager = new NodeManager(new OnceConfirm());
        pFilterManager->setNodeSubject(nodeSubject);
        pFilterManager->addActivateNode(new KillMapServerActivateNode(1));
        pFilterManager->addActivateNode(new BuildMappingActivateNode(2));
        pFilterManager->addActivateNode(new SubmapToMapActivateNode(2));

        bool isSuccessful = pFilterManager->activateNode(chain);
        if (isSuccessful) {
            state_ = node::State::map;
            map_state_ = node::MapState::complete;
        } else {
            trySleep();
        }
        delete pFilterManager;
    });
}

void NodeControl::offSleep() {
    system_kill("map_server");
}

void NodeControl::offWork() {
    //at this time the truth was invalid
//    system_kill(n_localization);
//    system_kill(n_load_map);
//    system_kill(n_navigation);
    //this simplest way
    if (Environment::instance().isRealEnvironment) {
        system_kill("cartographer_node");
        system_kill("map_server");
        system_kill("bump_back_node");
        system_kill("move_base");
    } else {
        system_kill(N_RVIZ);
        system_kill("map_server");
        system_kill("amcl");
    }
}

void NodeControl::offMap() {
    system_kill("cartographer_occupancy_grid_node");
    system_kill("cartographer_node");
}

void NodeControl::trySleep() {
    if (!isSleep()) {
        asyncOff(5, [this]() {
            clearOdom();
            if (isWork()) {
                offWork();
            }
            if (isMap()) {
                offMap();
            }
            asyncOn([]() {
                if (Environment::instance().isRealEnvironment) {
                    system_start(n_load_map);
                } else {
                    system_start(n_tt_load_map);
                }
            });
            work_state_ = node::WorkState::normal;
            map_state_ = node::MapState::normal;
            state_ = node::State::sleep;
        });
        if (!isSleep()) {
            LOG(ERROR) << "After 5s, it has not entered sleep mode !!!";
        }
    }
}

void NodeControl::clearOdom() {
    std_msgs::Int32 message;
    message.data = 1;
    pub_clear_odom.publish(message);
}

void NodeControl::resetLocalization(bool open) {
    ros::param::set("/localization", open);
    if (open) {
        ros::param::set("/set_initial_pose_x", 0.0);
        ros::param::set("/set_initial_pose_y", 0.0);
        ros::param::set("/set_initial_pose_z", 0.0);
        ros::param::set("/set_initial_pose_ox", 0.0);
        ros::param::set("/set_initial_pose_oy", 0.0);
        ros::param::set("/set_initial_pose_oz", 0.0);
        ros::param::set("/set_initial_pose_ow", 1.0);
    }
}

void NodeControl::update() {
    if (isSleep()) {
        LOG(INFO) << "当前为睡眠模式，应该是主动切换到睡眠模式的，暂时不需要处理（也可能需要处理）";
    } else {
        if (isWork()) {
            LOG(INFO) << "当前为工作模式，应该是工作模式中一些节点莫名崩溃导致，需要重新进入工作模式";
            changeWorkMode();
        }
        if (isMap()) {
            LOG(INFO) << "当前为建图模式，应该是建图模式中一些节点莫名崩溃导致，需要重新进入建图模式";
            changeMapMode();
        }
    }
}

void NodeControl::changeWorkMode() {
    pool_.execute([this]() {
        trySleep();
        if (isSleep()) {
            onWork();
        }
    });
}

void NodeControl::changeMapMode() {
    pool_.execute([this]() {
        trySleep();
        if (isSleep()) {
            onMap();
        }
    });
}

void NodeControl::changeSleepMode() {
    pool_.execute([this]() {
        trySleep();
    });
}
