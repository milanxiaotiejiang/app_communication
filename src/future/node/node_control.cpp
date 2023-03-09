//
// Created by Looper on 2023/3/7.
//

#include "future/node/node_control.h"
#include "manager/PublishInnerManager.h"

void NodeControl::initialize(ros::NodeHandle handle) {
    pool_.setNumOfThreads(THREAD_POOL_MAX_NUM);

    subscribe = new NodeControlSubscribe(handle);

    nodeSubject = new AbnormalSubject();
    nodeObserver = new AbnormalObserver(nodeSubject);
    nodeSubject->attach(nodeObserver);

    pub_clear_odom = handle.advertise<std_msgs::Int32>("/mrrobot/clear_odom", 1);
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

        auto *pFilterManager = new NodeManager(new OnceConfirm());
        pFilterManager->setNodeSubject(nodeSubject);
//        pFilterManager->addActivateNode(new RvizActivateNode(3));
        pFilterManager->addActivateNode(new NavigationActivateNode(3));
        pFilterManager->addActivateNode(new LoadMapActivateNode(3));
        pFilterManager->addActivateNode(new LocalizationActivateNode(3));

        bool isSuccessful = pFilterManager->activateNode(chain);
        if (isSuccessful) {
            state_ = node::State::work;
            work_state_ = node::WorkState::complete;
        } else {
            offWork();
        }
        delete pFilterManager;
    });
}

void NodeControl::offWork() {
    asyncOff(3, [this]() {
        work_state_ = node::WorkState::normal;
        state_ = node::State::sleep;
//        system_kill(N_RVIZ);
        system_kill(n_localization);
        system_kill(n_load_map);
        system_kill(n_navigation);
    });
}

void NodeControl::onMap() {
    map_state_ = node::MapState::way;
    asyncOn([this]() {
        NodeChain chain(&pool_);

        resetLocalization(false);

        auto *pFilterManager = new NodeManager(new OnceConfirm());
        pFilterManager->setNodeSubject(nodeSubject);
        pFilterManager->addActivateNode(new BuildMappingActivateNode(3));
        pFilterManager->addActivateNode(new SubmapToMapActivateNode(3));

        bool isSuccessful = pFilterManager->activateNode(chain);
        if (isSuccessful) {
            state_ = node::State::map;
            map_state_ = node::MapState::complete;
        } else {
            offMap();
        }
        delete pFilterManager;
    });
}

void NodeControl::offMap() {
    asyncOff(3, [this]() {
        map_state_ = node::MapState::normal;
        state_ = node::State::sleep;
        system_kill(n_submap_to_map);
        system_kill(n_build_mapping);
    });
}

void NodeControl::trySleep() {
    if (!isSleep()) {
        clearOdom();
        if (isWork()) {
            offWork();
        }
        if (isMap()) {
            offMap();
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
