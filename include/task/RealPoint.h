//
// Created by Looper on 2022/11/5.
//

#ifndef APP_COMMUNICATION_REALPOINT_H
#define APP_COMMUNICATION_REALPOINT_H

#include <utility>
#include <ostream>

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

class RealPoint {
public:
    int id{0};//序列点位ID，一般按排序顺序设置
    int blockId{};

    RealPosition realPosition{0, 0, 0};//geometry_msgs::PoseStamped pose 的 position
    RealOrientation realOrientation{0, 0, 0, 0};//geometry_msgs::PoseStamped pose 的 orientation
//    RealProgress realProgress{0, 0, 0, 0};//进度控制，由全覆盖路径和次数 rate 生成

    long timeout = 0;

    RealPoint() = default;

    int currentStep{0};

    bool core_move{false};
    bool open_gate{false};

//    friend std::ostream &operator<<(std::ostream &os, const RealPoint &point) {
//        os << "id: " << point.id << " blockId: " << point.blockId << " realProgress: " << point.realProgress;
//        return os;
//    }
    friend std::ostream &operator<<(std::ostream &os, const RealPoint &point) {
        os << "id: " << point.id << " blockId: " << point.blockId;
        return os;
    }
};

#endif //APP_COMMUNICATION_REALPOINT_H
