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
#include "net/WsServerManager.h"
#include "net/base/RequestModel.h"
#include <chrono>

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
    std::string topic;

    ros::Subscriber subscriber;

    bool keepTransfer;
    bool inThread;

    std::thread sensor_thread;
    std::thread keep_thread;

    std::mutex mtx;
    std::condition_variable cv;

    std::atomic<bool> dataAvailable;
    std::atomic<bool> processingEnabled;
    std::atomic<bool> finished;

    std::chrono::steady_clock::time_point last_received;

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

    void keep_transfer_thread_func() {
        while (true) {
            if (processingEnabled) {
                {
                    std::lock_guard<std::mutex> lock(mtx);
                    dateProgressing(sensorData);

                    RosModel value;
                    sensorData = value;
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }

protected:
    RosModel sensorData;

    bool deliveryCenter;

    virtual void subscribeCallback(const RosModel &data) {
        if (processingEnabled) {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - last_received).count();
            if (elapsed >= 1) {  // 如果距离上一次接收数据已经过去1秒
                if (keepTransfer) {
                    std::lock_guard<std::mutex> lock(mtx);
                    sensorData = data;
                } else {
                    if (inThread)
                        dataTransmission(data);
                    else
                        dateProgressing(data);
                }

                last_received = now;  // 更新时间戳
            }
        }
    }

public:

    Sensor() = default;

    Sensor(ros::NodeHandle handle, std::string topic, bool keepTransfer, bool inThread, bool deliveryCenter) :
            topic(topic), keepTransfer(keepTransfer), inThread(inThread), deliveryCenter(deliveryCenter) {
        subscriber = handle.subscribe(topic, 1, &Sensor::subscribeCallback, this);
        dataAvailable = false;
        processingEnabled = false;
        finished = false;
        if (keepTransfer) {
            keep_thread = std::thread(&Sensor::keep_transfer_thread_func, this);
            keep_thread.detach();
        } else {
            if (inThread) {
                sensor_thread = std::thread(&Sensor::sensor_inspect_thread_func, this);
                sensor_thread.detach();
            }
        }
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
