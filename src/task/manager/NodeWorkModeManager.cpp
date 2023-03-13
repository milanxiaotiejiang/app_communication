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
    } else {
        LOG(ERROR) << "NodeWorkModeManager asyncWorkMode unknown " << mode;
    }
//    nodeHandle.setParam("/node_controller/work_mode", mode);
}

bool NodeWorkModeManager::tryToWork() {
    std::unique_lock<std::mutex> lock(cv_mut);

    if (nowWorkMode() == WorkMode::WORKING) {
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
                    counter = MAXIMUM_TIME_LIMIT_FOR_QUICK_EXIT;
                    wait_cv.notify_one();
                }
            }
        });

        std::unique_lock<std::mutex> lck(wait_mutex);
        if (wait_cv.wait_for(lck,
                             std::chrono::milliseconds(MAXIMUM_LIMIT_TIME_OF_TIMEOUT)
        ) == std::cv_status::timeout) {
            return nowWorkMode() == WorkMode::WORKING;
        }
        return true;
    }
    return false;
}

bool NodeWorkModeManager::tryToMap() {
    std::unique_lock<std::mutex> lock(cv_mut);

    if (nowWorkMode() == WorkMode::MAPPING) {
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
                    counter = MAXIMUM_TIME_LIMIT_FOR_QUICK_EXIT;
                    wait_cv.notify_one();
                }
            }
        });

        std::unique_lock<std::mutex> lck(wait_mutex);
        if (wait_cv.wait_for(lck,
                             std::chrono::milliseconds(MAXIMUM_LIMIT_TIME_OF_TIMEOUT)
        ) == std::cv_status::timeout) {
            return nowWorkMode() == WorkMode::MAPPING;
        }
        return true;
    }
    return false;
}

void NodeWorkModeManager::forceToWork() {
    if (!tryToWork()) {
        NodeControl::instance().asyncOn([this]() {
            int counter = 0;
            while (counter < MAXIMUM_NUMBER_OF_FORCED_ENTRY) {
                sleep(1);
                counter++;
                if (tryToWork()) {
                    counter = MAXIMUM_NUMBER_OF_FORCED_ENTRY;
                }
            }
        });
    }
}

void NodeWorkModeManager::forceToMap() {
    if (!tryToMap()) {
        NodeControl::instance().asyncOn([this]() {
            int counter = 0;
            while (counter < MAXIMUM_NUMBER_OF_FORCED_ENTRY) {
                sleep(1);
                counter++;
                if (tryToMap()) {
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
