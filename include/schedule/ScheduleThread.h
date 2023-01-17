#ifndef APP_COMMUNICATION_SCHEDULETHREAD_H
#define APP_COMMUNICATION_SCHEDULETHREAD_H

#include "iostream"
#include "ctime"
#include <unistd.h>
#include "tool/CThread.h"
#include "Cron.h"
#include "schedule/date.h"
#include <nlohmann/json.hpp>
#include <pub/PubOut.h>
#include <pub/PubInner.h>

using namespace std;

class ScheduleThread : public CThread {

public:
    ScheduleThread(ros::NodeHandle handle, PubInner pubInner, PubOut pubOut);

    virtual ~ScheduleThread();

    croncpp::Cron<croncpp::LocalClock, croncpp::NullLock> cron;

    void startScheduleCheck();

    void init();

    ros::NodeHandle handle;
    PubInner pubInner;
    PubOut pubOut;
    ros::Subscriber sub_json_;
    string stimer_list;

    void subscribeCallback(const std_msgs::String &result);

    void *run() override {
        startScheduleCheck();
        return NULL;
    }
};

#endif // APP_COMMUNICATION_WEBCLIENTSUBSCRIBELOCALLY_H
