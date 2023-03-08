#ifndef APP_COMMUNICATION_INTERNAL_EVENT_H
#define APP_COMMUNICATION_INTERNAL_EVENT_H

#include <string>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace internal_event {


    static const int MANUAL_BACK_TO_BASE = 1;
    static const int URGENCY_STOP = 2;
    static const int MANUAL_NEW_TASK = 3;
    static const int NEBULAR_OPERATION = 4;
    static const int CANCEL_URGENCY_STOP = 7;
    static const int LOW_BATTERY_BACK_CHARGE = 8;
    static const int ENTER_MANUAL_CLEAN_MODE = 9;
    static const int CLEAN_WATER_LEVEL_CHECK_FAILED = 10;
    static const int DIRTY_WATER_LEVEL_CHECK_FAILED = 11;
    static const int MOTOR_ERROR_RECOVERY_SCCEED = 12;
    static const int MOTOR_ERROR_RECOVERY_FAILED = 13;
    static const int LASER_ERROR_RECOVERY_FAILED = 16;

    class StartEvent {//开始和结束事件
    private:
        std::string event_id_;
        std::string event_desc_;

    public:
        StartEvent() {}

        StartEvent(std::string event_id, std::string event_desc) : event_id_(event_id), event_desc_(event_desc) {}

        ~StartEvent() {}

        friend void to_json(json &j, const StartEvent &b) {
            j = json{
                    {"event_id",   b.event_id_},
                    {"event_desc", b.event_desc_}
            };
        }

        friend void from_json(const json &j, StartEvent &b) {
            j.at("event_id").get_to(b.event_id_);
            j.at("event_desc").get_to(b.event_desc_);
        }

    };

    class WorkStatusUpdate {//状态更新事件
    private:
        std::string event_id_;
        int event_code_;

    public:
        WorkStatusUpdate() {}

        WorkStatusUpdate(std::string event_id, int event_code) : event_id_(event_id), event_code_(event_code) {}

        ~WorkStatusUpdate() {}

        friend void to_json(json &j, const WorkStatusUpdate &b) {
            j = json{
                    {"event_id",   b.event_id_},
                    {"event_code", b.event_code_}
            };
        }

        friend void from_json(const json &j, WorkStatusUpdate &b) {
            j.at("event_id").get_to(b.event_id_);
            j.at("event_code").get_to(b.event_code_);
        }
    };

    class AlarmEvent {//告警事件
    private:
        std::string event_id_;
        int event_code_;
        std::string event_code2_;
        std::string event_desc_;
        std::string event_params_;

    public:
        AlarmEvent() {}

        AlarmEvent(const std::string &eventId, int eventCode, const std::string &eventCode2,
                   const std::string &eventDesc,
                   const std::string &eventParams) : event_id_(eventId), event_code_(eventCode),
                                                     event_code2_(eventCode2),
                                                     event_desc_(eventDesc), event_params_(eventParams) {}

        virtual ~AlarmEvent() {
        }

        friend void to_json(json &j, const AlarmEvent &b) {
            j = json{
                    {"event_id",     b.event_id_},
                    {"event_code",   b.event_code_},
                    {"event_code2",  b.event_code2_},
                    {"event_desc",   b.event_desc_},
                    {"event_params", b.event_params_}
            };
        }

        friend void from_json(const json &j, AlarmEvent &b) {
            j.at("event_id").get_to(b.event_id_);
            j.at("event_code").get_to(b.event_code_);
            j.at("event_code2").get_to(b.event_code2_);
            j.at("event_desc").get_to(b.event_desc_);
            j.at("event_params").get_to(b.event_params_);
        }
    };

    class OperEvent {//操作事件
    private:
        std::string event_id_;
        int event_code_;


    public:
        OperEvent() {}

        OperEvent(const std::string &eventId, int eventCode) : event_id_(eventId), event_code_(eventCode) {}

        virtual ~OperEvent() {
        }

        friend void to_json(json &j, const OperEvent &b) {
            j = json{
                    {"event_id",   b.event_id_},
                    {"event_code", b.event_code_}};
        }

        friend void from_json(const json &j, OperEvent &b) {
            j.at("event_id").get_to(b.event_id_);
            j.at("event_code").get_to(b.event_code_);
        }
    };
}


#endif//APP_COMMUNICATION_INTERNAL_EVENT_H