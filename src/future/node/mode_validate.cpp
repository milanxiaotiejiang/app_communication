//
// Created by Looper on 2023/4/11.
//

#include "future/node/mode_validate.h"
#include "simulation.h"
#include "glog/logging.h"
#include "future/node/node_control.h"
#include "future/thread_pool.h"
#include "task/point_planner.h"

int ModeValidate::getMoveBaseMode() {
    int move_base_mode = -1;
    ros::param::get("/move_base_mode", move_base_mode);
    return move_base_mode;
}

bool ModeValidate::validateCartographer(node::State state) {
    LOG(INFO) << "Cartographer 校验启动 ------------------------------ ";
    if (!Environment::instance().isRealEnvironment) {
        return true;
    }

    NodeControl::instance().heart_beat = 0;

    std::mutex wait_mutex;
    std::condition_variable cond;

    std::unique_lock<std::mutex> lck(wait_mutex);
    cond.wait_for(lck, std::chrono::seconds(2));

    LOG(INFO) << "Cartographer 最终启动结果 "
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
    LOG(INFO) << "MoveBase 校验启动 ------------------------------ " << open;
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
            LOG(INFO) << "MoveBase 第 " << count << " 此 获取 move_base_mode ： " << moveBaseMode;
            if (open == moveBaseMode) {
                wait_cv.notify_one();
                end_loop = true;
            }

            count++;
            if (count > 10) {
                end_loop = true;
            }
        }
        LOG(INFO) << "MoveBase 时时获取的线程结束 ...";
    });

    std::unique_lock<std::mutex> lck(wait_mutex);
    if (wait_cv.wait_for(lck, std::chrono::seconds(5)) == std::cv_status::timeout) {
        int moveBaseMode = getMoveBaseMode();
        LOG(INFO) << "MoveBase 获取结果超时再次获取 move_base_mode ：" << moveBaseMode;
        return open == moveBaseMode;
    }
    LOG(INFO) << "MoveBase 最终启动成功 ... ";
    return true;
}

bool ModeValidate::validateMoveBaseAvailable() {
    LOG(INFO) << "MoveBase 可用校验 ------------------------------ ";
    return PointPlanner::instance().waitForMoveBaseServer();

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
