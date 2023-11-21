//
// Created by Looper on 2023/4/11.
//

#include <future>
#include "future/node/mode_validate.h"
#include "simulation.h"
#include "future/node/node_control.h"
#include "future/thread_pool.h"
#include "task/point_planner.h"
#include "leave/cartographer_node.h"
#include "future/node/motor_server.h"
#include "future/node/hardware_subscriber.h"

int ModeValidate::getMoveBaseMode() {
    int move_base_mode = -1;
    ros::param::get("/move_base_mode", move_base_mode);
    return move_base_mode;
}

bool ModeValidate::validateCartographer(node::State state) {
    switch (state) {
        case node::State::sleep:
            LOG_IF(INFO, DEBUG_NODE) << "ModeValidate  Cartographer 睡眠模式校验 ------------------------------ ";
            break;
        case node::State::work:
            LOG_IF(INFO, DEBUG_NODE) << "ModeValidate  Cartographer 定位模式校验 ------------------------------ ";
            break;
        case node::State::map:
            LOG_IF(INFO, DEBUG_NODE) << "ModeValidate  Cartographer 建图模式校验 ------------------------------ ";
            break;
    }

    if (!Environment::instance().isRealEnvironment) {
        return true;
    }

    NodeControl::instance().heart_beat = 0;

    std::mutex wait_mutex;
    std::condition_variable cond;

    std::unique_lock<std::mutex> lck(wait_mutex);
    cond.wait_for(lck, std::chrono::seconds(5));

    LOG_IF(INFO, DEBUG_NODE) << "ModeValidate  Cartographer 最终启动结果 "
                             << "  state： " << static_cast<int>(state)
                             << "  carto_mode： " << NodeControl::instance().cartoMode()
                             << "  heart_beat： " << NodeControl::instance().heart_beat;

    // 0定位，1建图，2睡眠
    switch (state) {
        case node::State::sleep:
            return NodeControl::instance().cartoMode() == 2 && NodeControl::instance().heart_beat < 20;
        case node::State::work:
            return NodeControl::instance().cartoMode() == 0 && NodeControl::instance().heart_beat > 30;
        case node::State::map:
            return NodeControl::instance().cartoMode() == 1 && NodeControl::instance().heart_beat > 30;
    }
}

bool ModeValidate::validateMoveBase(int open) {
    if (open) {
        LOG_IF(INFO, DEBUG_NODE) << "ModeValidate  MoveBase 服务启动校验 ------------------------------ ";
    } else {
        LOG_IF(INFO, DEBUG_NODE) << "ModeValidate  MoveBase 服务关闭校验 ------------------------------ ";
    }

    if (!Environment::instance().isRealEnvironment) {
        return true;
    }

    if (getMoveBaseMode() == open) {
        return true;
    }

    std::condition_variable wait_cv;
    std::mutex wait_mutex;

    NodeControl::instance().asyncOn([open, &wait_cv]() {
        int count = 0;
        bool end_loop = false;
        while (!end_loop) {
            sleep(1);

            int moveBaseMode = getMoveBaseMode();
            LOG_IF(INFO, DEBUG_NODE)
                            << "ModeValidate  MoveBase 第 " << count << " 次 获取 move_base_mode ： " << moveBaseMode;
            if (open == moveBaseMode) {
                wait_cv.notify_one();
                end_loop = true;
            }

            count++;
            if (count > 5) {
                end_loop = true;
            }
        }
        LOG_IF(INFO, DEBUG_NODE) << "ModeValidate  MoveBase 时时获取的线程结束 ...";
    });

    std::unique_lock<std::mutex> lck(wait_mutex);
    if (wait_cv.wait_for(lck, std::chrono::seconds(5)) == std::cv_status::timeout) {
        int moveBaseMode = getMoveBaseMode();
        LOG_IF(INFO, DEBUG_NODE) << "ModeValidate  MoveBase 获取结果超时再次获取 move_base_mode ：" << moveBaseMode;
        return open == moveBaseMode;
    }
    LOG_IF(INFO, DEBUG_NODE) << "ModeValidate  MoveBase Server 启动成功 ... ";
    return true;
}

bool ModeValidate::validateMoveBaseAvailable() {
    LOG_IF(INFO, DEBUG_NODE) << "ModeValidate  MoveBase 服务可用校验 ------------------------------ ";
    bool replanServer = PointPlanner::instance().waitForReplanServer();
    LOG_IF(INFO, DEBUG_NODE) << "ModeValidate  MoveBase 服务可用校验结果 " << replanServer;
    return replanServer;

    // 测试线程终端的代码
//    boost::thread moveBaseAvailableThread([]() {
//        assert(boost::this_thread::interruption_enabled());
//        try {
//            for (;;) {
//                boost::this_thread::interruption_point();
//                sleep(1);
//            }
//        } catch (boost::thread_interrupted &) {
//            std::cout << "interrupted" << std::endl; // 线程会在这里退出
//        }
//    });
//
//    boost::thread move([&moveBaseAvailableThread]() {
//        sleep(5);
//        moveBaseAvailableThread.interrupt();
//    });
//    move.detach();
//
//    moveBaseAvailableThread.join();
}

bool ModeValidate::validateCoreMoveAvailable() {
    LOG_IF(INFO, DEBUG_NODE) << "ModeValidate  CoreMove 服务可用校验 ------------------------------ ";
    bool coreMoveServer = PointPlanner::instance().waitForCoreMoveServer();
    LOG_IF(INFO, DEBUG_NODE) << "ModeValidate  CoreMove 服务可用校验结果 " << coreMoveServer;
    return coreMoveServer;
}

const int numberOfTasks = 3;

bool ModeValidate::validateHardwareServer() {

    auto recordStart = std::chrono::steady_clock::now();

    InuSubscriberSingleton::instance().recount();

    int ret = std::system("echo '123456' | sudo -S systemctl start inuservice.service");
    if (ret != 0) {
        LOG_IF(INFO, DEBUG_NODE) << "sudo -S systemctl start inuservice.service fail : " << ret;
        return false;
    }

    std::vector<std::thread> threads;
    std::vector<std::promise<bool>> promises(numberOfTasks);
    std::vector<std::future<bool>> futures;
    std::vector<std::atomic<bool>> stopFlags(numberOfTasks);

    for (int i = 0; i < numberOfTasks; ++i) {
        futures.push_back(promises[i].get_future());

        switch (i) {
            case 0:
                threads.emplace_back(validateMotorServer, i, std::ref(stopFlags[i]), std::ref(promises[i]));
                break;
            case 1:
                threads.emplace_back(validateInuServer1, i, std::ref(stopFlags[i]), std::ref(promises[i]));
                break;
            case 2:
                threads.emplace_back(validateInuServer2, i, std::ref(stopFlags[i]), std::ref(promises[i]));
                break;
        }

    }

    // 设置总超时时间
    std::chrono::seconds timeout(30);
    auto endTime = std::chrono::steady_clock::now() + timeout;

    bool allTasksCompleted = true;

    for (auto &future: futures) {
        if (future.wait_until(endTime) != std::future_status::ready) {
            allTasksCompleted = false;
            break;
        }
    }

    auto recordEnd = std::chrono::steady_clock::now();
    auto elapsed = recordEnd - recordStart;

    if (allTasksCompleted) {
        LOG_IF(INFO, DEBUG_NODE) << "ModeValidate  validateHardwareServer 所有步骤已完成，主线程继续执行！";
    } else {
        LOG_IF(INFO, DEBUG_NODE)
                        << "ModeValidate  validateHardwareServer 超时！未能完成所有步骤，后续继续再次确认(原确认逻辑)";
    }

    LOG_IF(INFO, DEBUG_NODE) << "ModeValidate  validateHardwareServer elapsed : " << elapsed.count();

    for (auto &stopFlag: stopFlags) {
        stopFlag = true;
    }

    LOG_IF(INFO, DEBUG_NODE) << "ModeValidate  validateHardwareServer 服务可用校验 ------------------------------ ";

    bool callReadyCheckFirst = CartographerServiceClient::instance().callReadyCheck();

    LOG_IF(INFO, DEBUG_NODE)
                    << "ModeValidate  validateHardwareServer 首次校验结果 " << callReadyCheckFirst
                    << " ------------------------------ ";

    if (callReadyCheckFirst) {
        return true;
    }

    sleep(2);

    bool callReadyCheckAgain = CartographerServiceClient::instance().callReadyCheck();

    LOG_IF(INFO, DEBUG_NODE)
                    << "ModeValidate  validateHardwareServer 再次校验结果 " << callReadyCheckAgain
                    << " ------------------------------ ";

    if (callReadyCheckAgain) {
        return true;
    }

    return allTasksCompleted;
}

void ModeValidate::validateMotorServer(int stepId, std::atomic<bool> &stopFlag, std::promise<bool> &promise) {

    bool firingResult = MotorServerSingleton::instance().start();
    if (!firingResult) {
        LOG_IF(INFO, DEBUG_NODE) << "ModeValidate  MotorServer 雷达启动失败 ------------------------------ ";
        promise.set_value(false);
        return;
    }

    std::this_thread::sleep_for(std::chrono::seconds(10));

    while (!stopFlag) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        int beatScan = InuSubscriberSingleton::instance().heartBeatScan();
        LOG_IF(INFO, DEBUG_NODE) << "ModeValidate  MotorServer count : " << beatScan;

        if (beatScan > 10) {
            promise.set_value(true);
            stopFlag = true;
        }
    }

    LOG_IF(INFO, DEBUG_NODE) << "ModeValidate  MotorServer stop " << stepId;
}

void ModeValidate::validateInuServer1(int stepId, std::atomic<bool> &stopFlag, std::promise<bool> &promise) {
    std::this_thread::sleep_for(std::chrono::seconds(10));

    while (!stopFlag) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        int beatInu1 = InuSubscriberSingleton::instance().heartBeatInu1();
        LOG_IF(INFO, DEBUG_NODE) << "ModeValidate  validateInuServer1 count : " << beatInu1;

        if (beatInu1 > 10) {
            promise.set_value(true);
            stopFlag = true;
        }
    }

    LOG_IF(INFO, DEBUG_NODE) << "ModeValidate  validateInuServer1 stop " << stepId;
}

void ModeValidate::validateInuServer2(int stepId, std::atomic<bool> &stopFlag, std::promise<bool> &promise) {
    std::this_thread::sleep_for(std::chrono::seconds(10));

    while (!stopFlag) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        int beatInu2 = InuSubscriberSingleton::instance().heartBeatInu2();
        LOG_IF(INFO, DEBUG_NODE) << "ModeValidate  validateInuServer2 count : " << beatInu2;

        if (beatInu2 > 10) {
            promise.set_value(true);
            stopFlag = true;
        }
    }

    LOG_IF(INFO, DEBUG_NODE) << "ModeValidate  validateInuServer2 stop " << stepId;
}
