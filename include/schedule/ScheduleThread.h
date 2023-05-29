#ifndef APP_COMMUNICATION_SCHEDULETHREAD_H
#define APP_COMMUNICATION_SCHEDULETHREAD_H

#include "iostream"
#include "ctime"
#include <unistd.h>
#include "future/CThread.h"
#include "Cron.h"
#include "schedule/date.h"
#include <nlohmann/json.hpp>
#include <ros/node_handle.h>
#include <std_msgs/String.h>

using namespace std;

class ScheduleThread : public CThread {

public:
    ScheduleThread(ros::NodeHandle handle);

    ~ScheduleThread() override;

    croncpp::Cron<croncpp::LocalClock, croncpp::NullLock> cron;

    void startScheduleCheck();

    ros::NodeHandle handle;
    ros::Subscriber sub_json_;
    string stimer_list;

    void subscribeCallback(const std_msgs::String &result);

    void *run() override {
        startScheduleCheck();
        return NULL;
    }
};

#endif // APP_COMMUNICATION_WEBCLIENTSUBSCRIBELOCALLY_H
