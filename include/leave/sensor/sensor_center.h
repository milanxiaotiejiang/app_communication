//
// Created by Looper on 2023/7/17.
//

#ifndef APP_COMMUNICATION_SENSOR_CENTER_H
#define APP_COMMUNICATION_SENSOR_CENTER_H

#include "ros/ros.h"
#include "leave/sensor/carpet.h"
#include "collision.h"
#include "imu.h"
#include "metal.h"
#include "odometer.h"
#include "radar.h"
#include "silver_bull.h"
#include "tof.h"
#include "ultrasonic.h"
#include "net/ros/RosImu.h"
#include "net/ros/RosBasic.h"
#include "net/ros/RosPointCloud2.h"
#include "net/ros/RosRange.h"
#include "net/ros/RosLaserScan.h"

class SensorCenter {
private:
    SensorCenter() = default;

    SensorCenter(SensorCenter &) = delete;

    SensorCenter &operator=(const SensorCenter &) = delete;

    std::atomic<bool> sensorSelfMode;

//    std::shared_ptr<Carpet> carpet;
//    std::shared_ptr<Collision> collision;
//    std::shared_ptr<Imu> imu;
//    std::shared_ptr<Metal> metal;
//    std::shared_ptr<Odometer> odometer;
//    std::shared_ptr<Radar> radar;
//    std::shared_ptr<SilverBullUp> silverBullUp;
//    std::shared_ptr<SilverBullDown> silverBullDown;
//    std::shared_ptr<TofLeft> tofLeft;
//    std::shared_ptr<TofRight> tofRight;
//    std::shared_ptr<Ultrasonic1> ultrasonic1;
//    std::shared_ptr<Ultrasonic2> ultrasonic2;
//    std::shared_ptr<Ultrasonic3> ultrasonic3;
//    std::shared_ptr<Ultrasonic4> ultrasonic4;

    std::vector<std::shared_ptr<SensorBase>> sensors;

    int carpetData = 0;//地毯
    std::vector<int> collisionData;//碰撞
    RosImu imuData;//imu
    int metalData = 0;//金属
    RosOdom odomData;
    RosLaserScan scanData;//雷达
    RosPointCloud2 silverUpData;
    RosPointCloud2 silverDownData;
    RosRange tofLeftData;
    RosRange tofRightData;
    RosRange ultrasonic1Data;
    RosRange ultrasonic2Data;
    RosRange ultrasonic3Data;
    RosRange ultrasonic4Data;

    std::thread polymerization_thread;

    void sensor_polymerization_thread_func();

public:
    ~SensorCenter() = default;

    static auto &instance() {
        static SensorCenter obj;
        return obj;
    }

    void initialize(const ros::NodeHandle &handle);

    bool isSensorSelfMode();

    void startInspect();

    void stopInspect();

    void setCarpetData(int carpetData);

    void setCollisionData(const std::vector<int> &collisionData);

    void setImuData(const RosImu &imuData);

    void setMetalData(int metalData);

    void setOdomData(const RosOdom &odomData);

    void setScanData(const RosLaserScan &scanData);

    void setSilverUpData(const RosPointCloud2 &silverUpData);

    void setSilverDownData(const RosPointCloud2 &silverDownData);

    void setTofLeftData(const RosRange &tofLeftData);

    void setTofRightData(const RosRange &tofRightData);

    void setUltrasonic1Data(const RosRange &ultrasonic1Data);

    void setUltrasonic2Data(const RosRange &ultrasonic2Data);

    void setUltrasonic3Data(const RosRange &ultrasonic3Data);

    void setUltrasonic4Data(const RosRange &ultrasonic4Data);
};


#endif //APP_COMMUNICATION_SENSOR_CENTER_H
