//
// Created by Looper on 2023/7/17.
//

#ifndef APP_COMMUNICATION_SENSOR_H
#define APP_COMMUNICATION_SENSOR_H

#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include <mutex>
#include <condition_variable>
#include <thread>
#include "atomic"
#include "simulation.h"

template<class RosModel, class Model>
class Sensor {
private:
    ros::NodeHandle handle;

    ros::Subscriber subscriber;

    std::thread sensor_thread;

    std::mutex mtx;
    std::condition_variable cv;

    std::atomic<bool> dataAvailable;
    std::atomic<bool> processingEnabled;
    std::atomic<bool> finished;

    std::atomic<Model> sensorData;

    void subscribeCallback(const RosModel &data) {
        dataTransmission(transformRosModel(data));
    }

    void dataTransmission(Model data) {
        std::unique_lock<std::mutex> lock(mtx);
        sensorData = data;
        dataAvailable = true;
        cv.notify_one();
    }

    void sensor_inspect_thread_func() {
        LOG_IF(INFO, DEBUG_DUMP) << "自检线程启动了...";

        while (!finished) {

            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [&] { return dataAvailable || finished; });

            if (!dataAvailable) {
                continue;
            }

            if (dataAvailable && processingEnabled) {
                int value = sensorData;
                dataAvailable = false;
                lock.unlock();

                LOG_IF(INFO, DEBUG_DUMP) << "模拟检测中 " << value << "...";
                std::this_thread::sleep_for(std::chrono::milliseconds(100)); // simulate data processing
            }
        }

        LOG_IF(INFO, DEBUG_DUMP) << "自检线程结束了...";
    }

public:

    Sensor() = default;

    Sensor(ros::NodeHandle handle, std::string topic) {
        subscriber = handle.subscribe(topic, 1, &Sensor::subscribeCallback, this);
        dataAvailable = false;
        processingEnabled = false;
        finished = false;
        sensor_thread = std::thread(&Sensor::sensor_inspect_thread_func, this);
        sensor_thread.detach();
    }

    virtual ~Sensor() {
        finished = true;
        cv.notify_all();
    }

    void startInspect() {
        LOG_IF(INFO, DEBUG_DUMP) << "启动检测...";
        processingEnabled = true;
    }

    void stopInspect() {
        LOG_IF(INFO, DEBUG_DUMP) << "结束检测...";
        processingEnabled = false;
    }

    virtual Model transformRosModel(RosModel) = 0;

    virtual void dateProgressing(Model data) = 0;
};


#endif //APP_COMMUNICATION_SENSOR_H
