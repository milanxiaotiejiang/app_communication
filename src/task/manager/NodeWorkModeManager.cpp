//
// Created by admin1 on 22-11-25.
//

#include "task/manager/NodeWorkModeManager.h"
#include "future/node/node_control.h"

/*
    std::condition_variable wait_cv;
    std::mutex wait_mutex;

    bool isComplete = false;

    std::cout << "new " << isComplete << std::endl;

    std::thread completeThread([&isComplete, &wait_cv]() {
        int i = 0;
        while (!isComplete) {
            sleep(1);
            i++;
            if (i > 10)
                isComplete = true;
        }
        std::cout << "end " << isComplete << std::endl;
    });
    completeThread.detach();

    std::unique_lock<std::mutex> lck(wait_mutex);

    if (wait_cv.wait_for(lck, std::chrono::seconds(20), [&isComplete]() {
        return isComplete;
    })) {
        std::cout << "last " << isComplete << std::endl;
    } else {
        std::cout << "timeout " << isComplete << std::endl;
    }

    1. i > 10
    new 0
    end 1
    last 1
    正常
    2. i > 30
    new 0
    timeout 0
    end 1
    超时
 */
/*
    std::condition_variable wait_cv;
    std::mutex wait_mutex;

    bool isComplete = false;

    std::cout << "new " << isComplete << std::endl;

    std::thread completeThread([&isComplete, &wait_cv]() {
        int i = 0;
        while (!isComplete) {
            sleep(1);
            i++;
            if (i > 10) {
                isComplete = true;
                wait_cv.notify_one();
            }
        }
        std::cout << "end " << isComplete << std::endl;
    });
    completeThread.detach();

    std::unique_lock<std::mutex> lck(wait_mutex);

    if (wait_cv.wait_for(lck, std::chrono::seconds(20)) == std::cv_status::timeout) {
        std::cout << "timeout " << isComplete << std::endl;
    } else {
        std::cout << "last " << isComplete << std::endl;
    }

    1. i > 10
    new 0
    end 1
    last 1
    2. i > 30
    new 0
    timeout 0
    end 1
 */
void NodeWorkModeManager::initialize(ros::NodeHandle handle) {
//    this->nodeHandle = handle;
    pub_node_ = handle.advertise<std_msgs::Int32>("/node_control", 1);
}

WorkMode NodeWorkModeManager::nowWorkMode() {
    if (NodeControl::instance().isSleep()) {
        return WorkMode::SLEEPING;
    }
    if (NodeControl::instance().isWork()) {
        return WorkMode::WORKING;
    }
    if (NodeControl::instance().isMap()) {
        return WorkMode::MAPPING;
    }
    return WorkMode::UNKNOWN;
}

bool NodeWorkModeManager::asyncWorkMode(WorkMode mode) {
    LOG(INFO) << "NodeWorkModeManager 切换为 mode = " << mode << " 的模式 ... ";
    if (mode == WorkMode::SLEEPING) {
        std_msgs::Int32 message;
        message.data = 0;
        pub_node_.publish(message);
    } else if (mode == WorkMode::WORKING) {
        std_msgs::Int32 message;
        message.data = 1;
        pub_node_.publish(message);
    } else if (mode == WorkMode::MAPPING) {
        std_msgs::Int32 message;
        message.data = 2;
        pub_node_.publish(message);
    } else if (mode == WorkMode::UNKNOWN) {
        LOG(ERROR) << "NodeWorkModeManager asyncWorkMode unknown " << mode;
    }
//    nodeHandle.setParam("/node_controller/work_mode", mode);
}

bool NodeWorkModeManager::tryToWork() {
    std::unique_lock<std::mutex> lock(cv_mut);

    if (nowWorkMode() == WorkMode::WORKING) {
        LOG(INFO) << "NodeWorkModeManager 已经为工作模式，无需再次进入 ... ";
        return true;
    }
    if (nowWorkMode() == WorkMode::SLEEPING || nowWorkMode() == WorkMode::MAPPING) {

        asyncWorkMode(WorkMode::WORKING);

        std::condition_variable wait_cv;
        std::mutex wait_mutex;

        NodeControl::instance().asyncOn([this, &wait_cv]() {
            int counter = 0;
            while (counter < MAXIMUM_TIME_LIMIT_FOR_QUICK_EXIT) {
                sleep(1);
                counter++;
                if (nowWorkMode() == WorkMode::WORKING) {
                    LOG(INFO) << "NodeWorkModeManager 检测到已经切换为工作模式了 ... ";
                    counter = MAXIMUM_TIME_LIMIT_FOR_QUICK_EXIT;
                    wait_cv.notify_one();
                }
            }
        });

        std::unique_lock<std::mutex> lck(wait_mutex);
        if (wait_cv.wait_for(lck,
                             std::chrono::milliseconds(MAXIMUM_LIMIT_TIME_OF_TIMEOUT)
        ) == std::cv_status::timeout) {
            LOG(INFO) << "NodeWorkModeManager 切换工作模式超时，进入再次确认 ... ";
            return nowWorkMode() == WorkMode::WORKING;
        }
        return true;
    }
    return false;
}

bool NodeWorkModeManager::tryToMap() {
    std::unique_lock<std::mutex> lock(cv_mut);

    if (nowWorkMode() == WorkMode::MAPPING) {
        LOG(INFO) << "NodeWorkModeManager 已经为建图模式，无需再次进入 ... ";
        return true;
    }
    if (nowWorkMode() == WorkMode::SLEEPING || nowWorkMode() == WorkMode::WORKING) {
        asyncWorkMode(WorkMode::MAPPING);

        std::condition_variable wait_cv;
        std::mutex wait_mutex;

        NodeControl::instance().asyncOn([this, &wait_cv]() {
            int counter = 0;
            while (counter < MAXIMUM_TIME_LIMIT_FOR_QUICK_EXIT) {
                sleep(1);
                counter++;
                if (nowWorkMode() == WorkMode::MAPPING) {
                    LOG(INFO) << "NodeWorkModeManager 检测到已经切换为建图模式了 ... ";
                    counter = MAXIMUM_TIME_LIMIT_FOR_QUICK_EXIT;
                    wait_cv.notify_one();
                }
            }
        });

        std::unique_lock<std::mutex> lck(wait_mutex);
        if (wait_cv.wait_for(lck,
                             std::chrono::milliseconds(MAXIMUM_LIMIT_TIME_OF_TIMEOUT)
        ) == std::cv_status::timeout) {
            LOG(INFO) << "NodeWorkModeManager 切换建图模式超时，进入再次确认 ... ";
            return nowWorkMode() == WorkMode::MAPPING;
        }
        return true;
    }
    return false;
}

void NodeWorkModeManager::forceToWork() {
    if (!tryToWork()) {
        LOG(INFO) << "NodeWorkModeManager 尝试进入工作模式失败，开启强制进入 ... ";
        NodeControl::instance().asyncOn([this]() {
            int counter = 0;
            while (counter < MAXIMUM_NUMBER_OF_FORCED_ENTRY) {
                sleep(1);
                counter++;
                LOG(INFO) << "NodeWorkModeManager 强制进入工作模式下第 " << counter << " 次尝试 ... ";
                if (tryToWork()) {
                    LOG(INFO) << "NodeWorkModeManager 强制进入工作模式成功 ... ";
                    counter = MAXIMUM_NUMBER_OF_FORCED_ENTRY;
                }
            }
        });
    }
}

void NodeWorkModeManager::forceToMap() {
    if (!tryToMap()) {
        LOG(INFO) << "NodeWorkModeManager 尝试进入建图模式失败，开启强制进入 ... ";
        NodeControl::instance().asyncOn([this]() {
            int counter = 0;
            while (counter < MAXIMUM_NUMBER_OF_FORCED_ENTRY) {
                sleep(1);
                counter++;
                LOG(INFO) << "NodeWorkModeManager 强制进入建图模式下第 " << counter << " 次尝试 ... ";
                if (tryToMap()) {
                    LOG(INFO) << "NodeWorkModeManager 强制进入建图模式成功 ... ";
                    counter = MAXIMUM_NUMBER_OF_FORCED_ENTRY;
                }
            }
        });
    }
}

void NodeWorkModeManager::toSleep() {
    asyncWorkMode(WorkMode::SLEEPING);
}

bool NodeWorkModeManager::enterWorkMode(int enter) {
    if (enter == 0) {//建图
        return NodeWorkModeManager::instance().tryToMap();
    } else if (enter == 2) {//工作
        return NodeWorkModeManager::instance().tryToWork();
    } else {
        LOG(INFO) << "NodeWorkModeManager enterWorkMode unknown " << enter;
        return false;
    }
}

bool NodeWorkModeManager::forceEnterWorkMode(int enter) {
    if (enter == 0) {//建图
        NodeWorkModeManager::instance().forceToMap();
    } else if (enter == 2) {//工作
        NodeWorkModeManager::instance().forceToWork();
    } else {
        LOG(INFO) << "NodeWorkModeManager forceEnterWorkMode unknown " << enter;
    }
}
