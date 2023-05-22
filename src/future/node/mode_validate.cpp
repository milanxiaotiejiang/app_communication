//
// Created by Looper on 2023/4/11.
//

#include "future/node/mode_validate.h"
#include "simulation.h"
#include "glog/logging.h"
#include "future/node/node_control.h"
#include "future/thread_pool.h"
#include "task/point_planner.h"
#include "leave/cartographer_node.h"
#include "future/node/motor_server.h"

int ModeValidate::getMoveBaseMode() {
    int move_base_mode = -1;
    ros::param::get("/move_base_mode", move_base_mode);
    return move_base_mode;
}

bool ModeValidate::validateCartographer(node::State state) {
    switch (state) {
        case node::State::sleep:
            LOG(INFO) << "ModeValidate  Cartographer 睡眠模式校验 ------------------------------ ";
            break;
        case node::State::work:
            LOG(INFO) << "ModeValidate  Cartographer 定位模式校验 ------------------------------ ";
            break;
        case node::State::map:
            LOG(INFO) << "ModeValidate  Cartographer 建图模式校验 ------------------------------ ";
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

    LOG(INFO) << "ModeValidate  Cartographer 最终启动结果 "
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
        LOG(INFO) << "ModeValidate  MoveBase 服务启动校验 ------------------------------ ";
    } else {
        LOG(INFO) << "ModeValidate  MoveBase 服务关闭校验 ------------------------------ ";
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
            LOG(INFO) << "ModeValidate  MoveBase 第 " << count << " 次 获取 move_base_mode ： " << moveBaseMode;
            if (open == moveBaseMode) {
                wait_cv.notify_one();
                end_loop = true;
            }

            count++;
            if (count > 5) {
                end_loop = true;
            }
        }
        LOG(INFO) << "ModeValidate  MoveBase 时时获取的线程结束 ...";
    });

    std::unique_lock<std::mutex> lck(wait_mutex);
    if (wait_cv.wait_for(lck, std::chrono::seconds(5)) == std::cv_status::timeout) {
        int moveBaseMode = getMoveBaseMode();
        LOG(INFO) << "ModeValidate  MoveBase 获取结果超时再次获取 move_base_mode ：" << moveBaseMode;
        return open == moveBaseMode;
    }
    LOG(INFO) << "ModeValidate  MoveBase Server 启动成功 ... ";
    return true;
}

bool ModeValidate::validateMoveBaseAvailable() {
    LOG(INFO) << "ModeValidate  MoveBase 服务可用校验 ------------------------------ ";
    if (!Environment::instance().re_planner) {
        return PointPlanner::instance().waitForMoveBaseServer();
    } else {
        bool moveBaseServer = PointPlanner::instance().waitForMoveBaseServer();
        bool replanServer = PointPlanner::instance().waitForReplanServer();
        return moveBaseServer && replanServer;
    }

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

bool ModeValidate::validateMotorServer() {
    bool firingResult = MotorServerSingleton::instance().start();
    if (!firingResult) {
        LOG(INFO) << "ModeValidate  MotorServer 雷达启动失败 ------------------------------ ";
        return false;
    }

    sleep(2);

    LOG(INFO) << "ModeValidate  MotorServer 服务可用校验 ------------------------------ ";

    bool callReadyCheckFirst = CartographerServiceClient::instance().callReadyCheck();
    if (callReadyCheckFirst) {
        return true;
    }
    LOG(INFO) << "ModeValidate  MotorServer 首次校验结果 " << callReadyCheckFirst << " ------------------------------ ";

    sleep(2);

    bool callReadyCheckAgain = CartographerServiceClient::instance().callReadyCheck();
    if (callReadyCheckAgain) {
        return true;
    }
    LOG(INFO) << "ModeValidate  MotorServer 再次校验结果 " << callReadyCheckAgain << " ------------------------------ ";
    return false;
}
