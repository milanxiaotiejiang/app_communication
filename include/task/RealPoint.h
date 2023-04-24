//
// Created by Looper on 2022/11/5.
//

#ifndef APP_COMMUNICATION_REALPOINT_H
#define APP_COMMUNICATION_REALPOINT_H

#include <utility>
#include <ostream>

#include "model/WorkStatus.h"

class RealPosition {
public:
    double x;
    double y;
    double z;

    RealPosition() = default;

    RealPosition(double x, double y, double z) : x(x), y(y), z(z) {}

    friend std::ostream &operator<<(std::ostream &os, const RealPosition &position) {
        os << "x: " << position.x << " y: " << position.y << " z: " << position.z;
        return os;
    }
};

class RealOrientation {
public:
    double x;
    double y;
    double z;
    double w;

    RealOrientation() = default;

    RealOrientation(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {}

    friend std::ostream &operator<<(std::ostream &os, const RealOrientation &orientation) {
        os << "x: " << orientation.x << " y: " << orientation.y << " z: " << orientation.z << " w: " << orientation.w;
        return os;
    }
};

class RealProgress {
public:
    int currentStep;
    int totalStep;
    int currentFrequency;
    int totalFrequency;

    RealProgress() = default;

    RealProgress(int currentStep, int totalStep, int currentFrequency, int totalFrequency) : currentStep(currentStep),
                                                                                             totalStep(totalStep),
                                                                                             currentFrequency(
                                                                                                     currentFrequency),
                                                                                             totalFrequency(
                                                                                                     totalFrequency) {}

    friend std::ostream &operator<<(std::ostream &os, const RealProgress &progress) {
        os << "totalStep: " << progress.totalStep << " totalFrequency: " << progress.totalFrequency
           << " currentStep: " << progress.currentStep << " currentFrequency: " << progress.currentFrequency;
        return os;
    }
};

class RealError {
public:
    int errorCode{};
    std::string errorMessage;

    RealError() = default;

    RealError(int errorCode, const std::string &errorMessage) : errorCode(errorCode), errorMessage(errorMessage) {}
};

//class SpecialInfo {
//public:
//    bool clean_water_level_check_failed_{false};
//    bool dirty_water_level_check_failed_{false};
//    bool motor_error_{false};
//
//    SpecialInfo() = default;
//};

class RealPoint {
public:
    int id{0};//序列点位ID，一般按排序顺序设置
    std::string taskId;//运行中的任务ID

    bool arrive{false};//当前点为是否达到指定目标
    int timeout = 0;//运行中超时时间

    bool renew{false};//新旧任务标志位
    std::string oldTaskId;//旧任务，有值就是CombinationID，没值就是全覆盖
    long newTaskId{0};//新任务，可从数据库查找到的

    std::string name;//任务名称
    int rate{1};//任务次数
    int mode{};//任何模式，注意新旧的区分

    bool knife{false};//风刀开关

    RealPosition realPosition{0, 0, 0};//geometry_msgs::PoseStamped pose 的 position
    RealOrientation realOrientation{0, 0, 0, 0};//geometry_msgs::PoseStamped pose 的 orientation
    RealProgress realProgress{0, 0, 0, 0};//进度控制，由全覆盖路径和次数 rate 生成
    RealError realError{0, ""};//错误处理，int 类型的 code，string 类型的 message

    WorkStatus work_status;//清洁模式状态，应当和风刀 knife 并行
    bool inClean{false};//未知

    RealPoint() = default;

    friend std::ostream &operator<<(std::ostream &os, const RealPoint &point) {
        os << "id: " << point.id << " realProgress: " << point.realProgress;
        return os;
    }
};

struct Cp {
    int id;
    int pId;
    RealPosition realPosition{0, 0, 0};
    RealOrientation realOrientation{0, 0, 0, 0};
    bool last;

    Cp(int id, int pId, const RealPosition &realPosition, const RealOrientation &realOrientation, bool last) : id(id),
                                                                                                               pId(pId),
                                                                                                               realPosition(
                                                                                                                       realPosition),
                                                                                                               realOrientation(
                                                                                                                       realOrientation),
                                                                                                               last(last) {}
};

#endif //APP_COMMUNICATION_REALPOINT_H
