//
// Created by Looper on 2023/7/17.
//

#ifndef APP_COMMUNICATION_SENSOR_H
#define APP_COMMUNICATION_SENSOR_H

#include "ros/ros.h"
#include <mutex>
#include <condition_variable>
#include <thread>
#include "atomic"
#include "simulation.h"

class SensorBase {
protected:
    bool outLog = false;
public:
    virtual ~SensorBase() = default;

    virtual void startInspect() = 0;

    virtual void stopInspect() = 0;
};

template<class RosModel>
class Sensor : public SensorBase {
private:
    ros::NodeHandle handle;

    ros::Subscriber subscriber;

    std::thread sensor_thread;

    std::mutex mtx;
    std::condition_variable cv;

    std::atomic<bool> dataAvailable;
    std::atomic<bool> processingEnabled;
    std::atomic<bool> finished;

    RosModel sensorData;

    void subscribeCallback(const RosModel &data) {
        dataTransmission(data);
    }

    void dataTransmission(RosModel data) {
        std::lock_guard<std::mutex> lock(mtx);
        sensorData = data;
        dataAvailable = true;
        cv.notify_one();
    }

    void sensor_inspect_thread_func() {
        while (!finished) {

            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [&] { return dataAvailable || finished; });

            if (!dataAvailable) {
                continue;
            }

            if (dataAvailable && processingEnabled) {
                RosModel value = sensorData;
                dataAvailable = false;
                lock.unlock();

                dateProgressing(value);
            }
        }
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

    void startInspect() override {
        processingEnabled = true;
    }

    void stopInspect() override {
        processingEnabled = false;
    }

    virtual void dateProgressing(RosModel data) = 0;
};


#endif //APP_COMMUNICATION_SENSOR_H
