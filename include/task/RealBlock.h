//
// Created by Looper on 2023/5/22.
//

#ifndef APP_COMMUNICATION_REALBLOCK_H
#define APP_COMMUNICATION_REALBLOCK_H

#include <utility>
#include <ostream>

#include "model/WorkStatus.h"
#include "task/RealPoint.h"
#include "task/status/state_machine.h"

class RealError {
public:
    int errorCode{};
    std::string errorMessage;

    RealError() = default;

    RealError(int errorCode, const std::string &errorMessage) : errorCode(errorCode), errorMessage(errorMessage) {}
};

const int FLOW_SEIZE_SEAT = -10;
const int FLOW_OPEN_MECHANISM = -11;
const int FLOW_CLOSE_MECHANISM = -12;
const int FLOW_OUT_STATION = -13;
const int FLOW_END_SLEEP = -14;
const int FLOW_IN_BASE_POINT = -15;
const int FLOW_IN_STATION = -16;

const int FLOW_ELEVATOR_PRE = -17;
const int FLOW_ELEVATOR_POST = -18;

const int FLOW_READY_BACK = -19;

const int FLOW_ERROR_UNRECOVERABLE = -20;
const int FLOW_ERROR_LIFT = -21;
const int FLOW_ELECTRIC_MOVE = -22;

class RealBlock {
public:
    int id{0};//序列点位ID，一般按排序顺序设置

    std::string taskId{""};//运行中的任务ID

    long newTaskId{0};//新任务，可从数据库查找到的

    std::string name{""};//任务名称
    int rate{};
    int mode{};//任何模式，注意新旧的区分

    bool knife{false};//风刀开关
    WorkStatus work_status;//清洁模式状态，应当和风刀 knife 并行

    bool arrive{false};//当前点为是否达到指定目标
    bool retry{false};;
    long timeout = 0;//运行中超时时间

    int totalStep{0};
    int totalFrequency{0};
    int currentFrequency{0};//当前次数
    bool inClean{true};//未知

    RealError realError{0, ""};//错误处理，int 类型的 code，string 类型的 message

    std::vector<RealPoint> plannerPoints;

    int timely_step{0};
    int already_step{0};

    int current_step{0};
    int goal_step{0};
    int current_goal{0};

    float totalDistance;

    bool mustArrive{false};

    friend std::ostream &operator<<(std::ostream &os, const RealBlock &block) {
        os << "id: " << block.id;
        return os;
    }
};

#endif //APP_COMMUNICATION_REALBLOCK_H
