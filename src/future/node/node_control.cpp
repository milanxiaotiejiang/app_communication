//
// Created by Looper on 2023/3/7.
//

#include "future/node/node_control.h"
#include "manager/PublishInnerManager.h"
#include "simulation.h"
#include "leave/reconfigure.h"
#include "ros/xmlrpc_manager.h"
#include "future/node/mode_validate.h"
#include "leave/cartographer_node.h"
#include "future/node/motor_server.h"
#include "future/node/hardware_subscriber.h"
#include "catch2/catch.hpp"

void NodeControl::initialize(ros::NodeHandle handle) {
    nodeHandle = handle;

    MotorServerSingleton::instance().init(handle);
    InuSubscriberSingleton::instance().init(handle);

    pool_.setNumOfThreads(THREAD_POOL_MAX_NUM);

    subscribe = new NodeControlSubscribe(handle);

    nodeSubject = new AbnormalSubject();
    nodeObserver = new AbnormalObserver(nodeSubject);
    nodeSubject->attach(nodeObserver);

//    onSleep();
    asyncOn([&handle]() {
        int p_OR_percent_1 = 6;
        if (ros::param::has("/OR_percent_1")) {
            ros::param::get("/OR_percent_1", p_OR_percent_1);
        }

        int p_OR_percent_2 = 3;
        if (ros::param::has("/OR_percent_2")) {
            ros::param::get("/OR_percent_2", p_OR_percent_2);
        }

        bool end_loop = false;
        while (!end_loop) {
            bool start_finish = false;
            ros::param::get("/node_controller/start_finish", start_finish);
            if (start_finish)
                end_loop = true;
            sleep(1);
        }
        sleep(3);
        DR OR_percent_1 = DR("/1/inudev_ros_nodelet1", "OR_percent");
        OR_percent_1.i(p_OR_percent_1);
        DR OR_percent_2 = DR("/2/inudev_ros_nodelet2", "OR_percent");
        OR_percent_2.i(p_OR_percent_2);
        LOG_IF(INFO, DEBUG_FIRING) << "inu cameraFiringAvailable OR_percent 设置成功 ... ";
    });

    int work_mode = -1;
    ros::param::get(NODE_CONTROLLER_WORK_MODE, work_mode);
    if (work_mode == 1) {
        setWorkMode(node::State::sleep);
    }

    int availableInu;
    handle.param<int>("/node_controller/available/inu", availableInu, 0);
    NodeControl::instance().cameraFiringAvailable = availableInu;

    if (!Environment::instance().isRealEnvironment) {
        setCameraFiringAvailable(Firing::INUStatus::SUCCESS);
        return;
    }

    if (NodeControl::instance().cameraFiringAvailable == Firing::INUStatus::SUCCESS) {
        LOG_IF(INFO, DEBUG_FIRING) << "inu cameraFiringAvailable 已经启动,无需再次启动 ... ";
        setCameraFiringAvailable(Firing::INUStatus::SUCCESS);
    } else if (NodeControl::instance().cameraFiringAvailable == Firing::INUStatus::UNKNOWN ||
               NodeControl::instance().cameraFiringAvailable == Firing::INUStatus::FAIL) {

        if (!Environment::instance().isRealEnvironment) {
            setCameraFiringAvailable(Firing::INUStatus::SUCCESS);
            return;
        }
        if (NodeControl::instance().cameraFiringAvailable == Firing::INUStatus::UNKNOWN)
            LOG_IF(INFO, DEBUG_FIRING) << "inu cameraFiringAvailable 未启动(状态未知),需要重新启动 ... ";
        else if (NodeControl::instance().cameraFiringAvailable == Firing::INUStatus::FAIL)
            LOG_IF(INFO, DEBUG_FIRING) << "inu cameraFiringAvailable 启动失败,需要重新启动 ... ";

        asyncOn([&handle, this]() {
            std::this_thread::sleep_for(std::chrono::seconds(
                    Environment::instance().inu_firing_launch_interval));

            LOG_IF(INFO, DEBUG_FIRING) << "inu cameraFiringAvailable 启动开始 ... ";
            auto inu_dev1_thread = std::thread([this] {
                std::string inu1Log = Environment::instance().glog_info_time_pid + "_inu_dev1.log";
                std::string systemStr = "roslaunch launch_center inu_dev1.launch > " + inu1Log + " 2>&1";
                LOG_IF(INFO, DEBUG_FIRING) << systemStr;
                int error = std::system(systemStr.data());
                if (error != 0) {
                    LOG_IF(INFO, DEBUG_FIRING) << "roslaunch launch_center inu_dev1.launch fail " << error << " ... ";
                    setCameraFiringAvailable(Firing::INUStatus::FAIL);
                }
                LOG_IF(INFO, DEBUG_FIRING) << "inu cameraFiringAvailable inu_dev1_thread finish ... ";
            });
            inu_dev1_thread.detach();

            std::this_thread::sleep_for(std::chrono::seconds(Environment::instance().inu_launch_middle_interval));
            auto inu_dev2_thread = std::thread([this] {
                std::string inu2Log = Environment::instance().glog_info_time_pid + "_inu_dev2.log";
                std::string systemStr = "roslaunch launch_center inu_dev2.launch > " + inu2Log + " 2>&1";
                LOG_IF(INFO, DEBUG_FIRING) << systemStr;
                int error = std::system(systemStr.data());
                if (error != 0) {
                    LOG_IF(INFO, DEBUG_FIRING) << "roslaunch launch_center inu_dev2.launch fail " << error << " ... ";
                    setCameraFiringAvailable(Firing::INUStatus::FAIL);
                    return;
                }
                LOG_IF(INFO, DEBUG_FIRING) << "inu cameraFiringAvailable inu_dev2_thread finish ... ";
            });
            inu_dev2_thread.detach();

            setCameraFiringAvailable(Firing::INUStatus::LAUNCH);

            finalConfirmation();
        });
    } else if (NodeControl::instance().cameraFiringAvailable == Firing::INUStatus::LAUNCH) {
        LOG_IF(INFO, DEBUG_FIRING) << "inu cameraFiringAvailable 启动中一些原因导致rec进程中断,此处为启动了launch ... ";

        asyncOn([&handle, this]() {
            finalConfirmation();
        });
    }
}

void NodeControl::finalConfirmation() {

    InuSubscriberSingleton::instance().recount();

    std::this_thread::sleep_for(std::chrono::seconds(Environment::instance().inu_final_confirmation_interval));

    LOG_IF(INFO, DEBUG_FIRING) << "inu cameraFiringAvailable 验收数据 ... ";

    int end_loop_count = 0;
    while (end_loop_count < 20) {
        int beatInu1 = InuSubscriberSingleton::instance().heartBeatInu1();
        int beatInu2 = InuSubscriberSingleton::instance().heartBeatInu2();

        LOG_IF(INFO, DEBUG_FIRING) << "inu cameraFiringAvailable count : " << end_loop_count
                                   << ", beatInu1 : " << beatInu1
                                   << ", beatInu2 : " << beatInu2 << " ... ";

        if (beatInu1 > SSDF && beatInu2 > SSDF)
            end_loop_count = 20;

        std::this_thread::sleep_for(std::chrono::seconds(1));
        end_loop_count++;
    }

    int beatInu1 = InuSubscriberSingleton::instance().heartBeatInu1();
    int beatInu2 = InuSubscriberSingleton::instance().heartBeatInu2();

    if (beatInu1 > SSDF && beatInu2 > SSDF) {
        LOG_IF(INFO, DEBUG_FIRING) << "inu cameraFiringAvailable 启动成功 ... ";
        std::this_thread::sleep_for(std::chrono::seconds(Environment::instance().inu_start_last_stop_server_interval));
        setCameraFiringAvailable(Firing::INUStatus::SUCCESS);
        MotorServerSingleton::instance().stopInu();
    } else {
        LOG_IF(INFO, DEBUG_FIRING) << "inu cameraFiringAvailable 启动失败 ... ";
        setCameraFiringAvailable(Firing::INUStatus::FAIL);
    }
}

void NodeControl::setCameraFiringAvailable(Firing::INUStatus status) {
    cameraFiringAvailable = status;
    ros::param::set("/node_controller/available/inu", status);

    //兼容之前业务
    if (status == Firing::INUStatus::FAIL) {
        ros::param::set("/node_controller/start_finish", false);
    } else if (status == Firing::INUStatus::SUCCESS) {
        ros::param::set("/node_controller/start_finish", true);
    }
}

void NodeControl::release() {
    nodeSubject->detach(nodeObserver);
    nodeObserver = nullptr;
    nodeSubject = nullptr;
}

void NodeControl::onWork() {
    work_state_ = node::WorkState::way;
//    asyncOn([this]() {
//        NodeChain chain(&pool_);
//
//        resetLocalization(true);
//        clearOdom();
//
//        auto *pFilterManager = new NodeManager(new OnceConfirm());
//        pFilterManager->setNodeSubject(nodeSubject);
//        if (Environment::instance().isRealEnvironment) {
//            pFilterManager->addActivateNode(new NavigationActivateNode(2));
//            pFilterManager->addActivateNode(new LoadMapActivateNode(2));
//            pFilterManager->addActivateNode(new LocalizationActivateNode(2));
//        } else {
//            pFilterManager->addActivateNode(new NavigationActivateNode(1));
//            pFilterManager->addActivateNode(new LoadMapActivateNode(1));
//            pFilterManager->addActivateNode(new RvizActivateNode(1));
//        }
//
//        bool isSuccessful = pFilterManager->activateNode(chain);
//        if (isSuccessful) {
//            setWorkMode(node::State::work);
//            state_ = node::State::work;
//            work_state_ = node::WorkState::complete;
//        } else {
//            defeatModeStart(node::State::work);
//        }
//        delete pFilterManager;
//    });
    asyncOn([this]() {
        bool motorServer = ModeValidate::validateHardwareServer();
        if (motorServer) {

//            CartographerPublisher::instance().publishStartCartoLocalization();
//            bool validateCartographer = ModeValidate::validateCartographer(node::State::work);
            bool validateCartographer = CartographerServiceClient::instance().callStartLocalization();
            if (validateCartographer) {

                if (Environment::instance().direct_start_move_base) {

                    {
                        bool baseAvailable = ModeValidate::validateMoveBaseAvailable();
                        if (baseAvailable) {
                            bool coreMoveAvailable = ModeValidate::validateCoreMoveAvailable();
                            if (coreMoveAvailable) {
                                setWorkMode(node::State::work);
                                state_ = node::State::work;
                                work_state_ = node::WorkState::complete;
                            } else {
                                defeatModeStart(node::State::work);
                            };
                        } else {
                            defeatModeStart(node::State::work);
                        }
                    }

                } else {

                    CartographerPublisher::instance().publishControlMoveBase(true);
                    bool validateMoveBase = ModeValidate::validateMoveBase(1);
                    if (validateMoveBase) {

                        bool baseAvailable = ModeValidate::validateMoveBaseAvailable();
                        if (baseAvailable) {
                            bool coreMoveAvailable = ModeValidate::validateCoreMoveAvailable();
                            if (coreMoveAvailable) {
                                setWorkMode(node::State::work);
                                state_ = node::State::work;
                                work_state_ = node::WorkState::complete;
                            } else {
                                defeatModeStart(node::State::work);
                            }
                        } else {
                            defeatModeStart(node::State::work);
                        }
                    } else {
                        defeatModeStart(node::State::work);
                    }
                }
            } else {
                defeatModeStart(node::State::work);
            }
        } else {
            defeatModeStart(node::State::work);
        }

    });
}

void NodeControl::onMap() {
    map_state_ = node::MapState::way;
//    asyncOn([this]() {
//        NodeChain chain(&pool_);
//
//        resetLocalization(false);
//        clearOdom();
//
//        auto *pFilterManager = new NodeManager(new OnceConfirm());
//        pFilterManager->setNodeSubject(nodeSubject);
//        pFilterManager->addActivateNode(new KillMapServerActivateNode(1));
//        pFilterManager->addActivateNode(new BuildMappingActivateNode(2));
//        pFilterManager->addActivateNode(new SubmapToMapActivateNode(2));
//
//        bool isSuccessful = pFilterManager->activateNode(chain);
//        if (isSuccessful) {
//            setWorkMode(node::State::map);
//            state_ = node::State::map;
//            map_state_ = node::MapState::complete;
//        } else {
//            defeatModeStart(node::State::map);
//        }
//        delete pFilterManager;
//    });
    asyncOn([this]() {

        bool motorServer = ModeValidate::validateHardwareServer();
        if (motorServer) {
            CartographerPublisher::instance().publishStartCartoMapping();
            bool validateCartographer = ModeValidate::validateCartographer(node::State::map);
            if (validateCartographer) {
                setWorkMode(node::State::map);
                state_ = node::State::map;
                map_state_ = node::MapState::complete;
            } else {
                defeatModeStart(node::State::map);
            }
        } else {
            defeatModeStart(node::State::map);
        }
    });
}

void NodeControl::offSleep() {
//    system_kill("map_server");
}

void NodeControl::offWork() {
//    //at this time the truth was invalid
////    system_kill(n_localization);
////    system_kill(n_load_map);
////    system_kill(n_navigation);
//    //this simplest way
//    if (Environment::instance().isRealEnvironment) {
//        system_kill("cartographer_node");
//        system_kill("map_server");
//        system_kill("bump_back_node");
//        system_kill("move_base");
//    } else {
//        system_kill("rviz");
//        system_kill("map_server");
//        system_kill("amcl");
//        system_kill("move_base");
//    }
}

void NodeControl::offMap() {
//    system_kill("cartographer_occupancy_grid_node");
//    system_kill("cartographer_node");
}

void NodeControl::onSleep() {
//    asyncOn([]() {
//        if (Environment::instance().isRealEnvironment) {
//            system_start(n_load_map);
//        } else {
////            system_start(n_tt_load_map);
//        }
//    });
//    setWorkMode(node::State::sleep);
}

void NodeControl::trySleep() {
//    if (!isSleep()) {
//        asyncOff(5, [this]() {
//            clearOdom();
//            if (isWork()) {
//                offWork();
//            }
//            if (isMap()) {
//                offMap();
//            }
//            onSleep();
//            work_state_ = node::WorkState::normal;
//            map_state_ = node::MapState::normal;
//            state_ = node::State::sleep;
//        });
//        if (!isSleep()) {
//            LOG(ERROR) << "After 5s, it has not entered sleep mode !!!";
//        }
//    }
    if (!Environment::instance().isRealEnvironment) {
        return;
    }
    asyncOn([this]() {
        node::WorkState back_work_state_ = work_state_;
        node::MapState back_map_state_ = map_state_;
        node::State back_state_ = state_;

        work_state_ = node::WorkState::normal;
        map_state_ = node::MapState::normal;
        state_ = node::State::sleep;

        if (Environment::instance().direct_start_move_base) {

            {
//                CartographerPublisher::instance().publishShutdownCarto();
//                CartographerPublisher::instance().publishClearCurrentPose();
//                bool validateCartographer = ModeValidate::validateCartographer(node::State::sleep);
                bool validateCartographer = CartographerServiceClient::instance().callStopLocalization();
                if (validateCartographer) {
                    setWorkMode(node::State::sleep);
                    work_state_ = node::WorkState::normal;
                    map_state_ = node::MapState::normal;
                    state_ = node::State::sleep;
                } else {
                    LOG(ERROR) << "After 5s, it has not entered sleep mode !!!";
                    work_state_ = back_work_state_;
                    map_state_ = back_map_state_;
                    state_ = back_state_;
                }
            }

        } else {

            CartographerPublisher::instance().publishControlMoveBase(false);
            bool validateMoveBase = ModeValidate::validateMoveBase(0);
            if (validateMoveBase) {

//                CartographerPublisher::instance().publishShutdownCarto();
//                CartographerPublisher::instance().publishClearCurrentPose();
//                bool validateCartographer = ModeValidate::validateCartographer(node::State::sleep);
                bool validateCartographer = CartographerServiceClient::instance().callStopLocalization();
                if (validateCartographer) {
                    setWorkMode(node::State::sleep);
                    work_state_ = node::WorkState::normal;
                    map_state_ = node::MapState::normal;
                    state_ = node::State::sleep;
                } else {
                    LOG(ERROR) << "After 5s, it has not entered sleep mode !!!";
                    work_state_ = back_work_state_;
                    map_state_ = back_map_state_;
                    state_ = back_state_;
                }

            } else {
                LOG(ERROR) << "After 5s, it has not entered sleep mode !!!";
                work_state_ = back_work_state_;
                map_state_ = back_map_state_;
                state_ = back_state_;
            }

        }

    });
}

void NodeControl::setWorkMode(node::State state) {
    if (state == node::State::work) {
        nodeHandle.setParam(NODE_CONTROLLER_WORK_MODE, 2);
    } else if (state == node::State::map) {
        nodeHandle.setParam(NODE_CONTROLLER_WORK_MODE, 0);
    } else {
        nodeHandle.setParam(NODE_CONTROLLER_WORK_MODE, 1);
    }
}

void NodeControl::resetLocalization(bool open) {
    ros::param::set("/localization", open);
    if (open) {
        //生成一个Pose消息，摆渡点出发，建图起点为基站，所以是-1.0
        geometry_msgs::Pose pose;
        pose.position.x = -1.0;
        pose.position.y = 0.0;
        pose.position.z = 0.0;
        pose.orientation.x = 0.0;
        pose.orientation.y = 0.0;
        pose.orientation.z = 0.0;
        pose.orientation.w = 1.0;
        // 将 Pose 消息转换为字典
        paramPose("/set_initial_pose", pose);
    }
}

void NodeControl::defeatModeStart(node::State state) {
    if (state == node::State::work) {
        LOG_IF(INFO, DEBUG_NODE) << "工作模式启动失败，休息几秒尝试进入睡眠模式 ... ";
    } else if (state == node::State::map) {
        LOG_IF(INFO, DEBUG_NODE) << "建图模式启动失败，休息几秒尝试进入睡眠模式 ... ";
    }
    sleep(5);
    trySleep();
}


void NodeControl::update() {
    if (isSleep()) {
        LOG_IF(INFO, DEBUG_NODE) << "当前为睡眠模式，应该是主动切换到睡眠模式的，暂时不需要处理（也可能需要处理）";
    } else {
        if (isWork()) {
            LOG_IF(INFO, DEBUG_NODE) << "当前为工作模式，应该是工作模式中一些节点莫名崩溃导致，需要重新进入工作模式";
            changeWorkMode();
        }
        if (isMap()) {
//            LOG_IF(INFO, DEBUG_NODE) << "当前为建图模式，应该是建图模式中一些节点莫名崩溃导致，需要重新进入建图模式";
//            changeMapMode();
            LOG_IF(INFO, DEBUG_NODE) << "当前为建图模式，应该是建图模式中一些节点莫名崩溃导致，退出建图模式进入睡眠模式";
            changeSleepMode();
        }
    }
}

void NodeControl::changeWorkMode() {
//    pool_.execute([this]() {
//        trySleep();
//        if (isSleep()) {
//            onWork();
//        }
//    });
    pool_.execute([this]() {
        onWork();
    });
}

void NodeControl::changeMapMode() {
//    pool_.execute([this]() {
//        trySleep();
//        if (isSleep()) {
//            onMap();
//        }
//    });
    pool_.execute([this]() {
        onMap();
    });
}

void NodeControl::changeSleepMode() {
    pool_.execute([this]() {
        trySleep();
    });
}

int NodeControl::cartoMode() {
    int cartoMode = 0;
    ros::param::get(CARTOGRAPHER_WORK_MODE, cartoMode);
    return cartoMode;
}

void NodeControl::emulate() {
//    work_state_ = node::WorkState::complete;
//    map_state_ = node::MapState::normal;
//    state_ = node::State::work;
}

void NodeControl::paramPose(const std::string &key, const geometry_msgs::Pose pose) {
    // 将 Pose 消息转换为字典
    XmlRpc::XmlRpcValue pose_dict;
    pose_dict["position"]["x"] = pose.position.x;
    pose_dict["position"]["y"] = pose.position.y;
    pose_dict["position"]["z"] = pose.position.z;
    pose_dict["orientation"]["x"] = pose.orientation.x;
    pose_dict["orientation"]["y"] = pose.orientation.y;
    pose_dict["orientation"]["z"] = pose.orientation.z;
    pose_dict["orientation"]["w"] = pose.orientation.w;
    // 将字典存储为 ROS 参数
    ros::param::set(key, pose_dict);
}

int NodeControl::restoreWork() {

    int step = 0;
    if (Environment::instance().direct_start_move_base) {
        if (CartographerServiceClient::instance().callStopLocalization())
            step++;
    } else {
        CartographerPublisher::instance().publishControlMoveBase(false);
        bool validateMoveBase = ModeValidate::validateMoveBase(0);
        if (validateMoveBase)
            if (CartographerServiceClient::instance().callStopLocalization())
                step++;
    }

    if (step == 1) {
        if (CartographerServiceClient::instance().callStartLocalization())
            if (Environment::instance().direct_start_move_base) {
                if (ModeValidate::validateMoveBaseAvailable())
                    if (ModeValidate::validateCoreMoveAvailable())
                        step++;
            } else {
                CartographerPublisher::instance().publishControlMoveBase(true);
                if (ModeValidate::validateMoveBase(1))
                    if (ModeValidate::validateMoveBaseAvailable())
                        if (ModeValidate::validateCoreMoveAvailable())
                            step++;
            }
    }

    return step;
}
