//
// Created by Looper on 2022/11/27.
//

#ifndef APP_COMMUNICATION_POINTPROGRESSVO_H
#define APP_COMMUNICATION_POINTPROGRESSVO_H


#include "model/WorkStatus.h"

class PointProgressVo {
private:
    std::string id;
    float x{};
    float y{};
    int currentStep{};
    int totalStep{};
    int currentFrequency{};
    int totalFrequency{};
    WorkStatus work_status;
    int m_mode;
    bool is_cleaning;
public:
    PointProgressVo();

    PointProgressVo(const std::string &id, float x, float y, int currentStep, int totalStep, int currentFrequency,
                    int totalFrequency, const WorkStatus &workStatus, int mMode, bool isCleaning);


    friend void to_json(json &j, const PointProgressVo &vo) {
        j = json{
                {"id",               vo.id},
                {"x",                vo.x},
                {"y",                vo.y},
                {"currentStep",      vo.currentStep},
                {"totalStep",        vo.totalStep},
                {"currentFrequency", vo.currentFrequency},
                {"totalFrequency",   vo.totalFrequency},
                {"work_status",      vo.work_status},
                {"mode",             vo.m_mode},
                {"is_cleaning",      vo.is_cleaning}
        };
    }

    friend void from_json(const json &j, PointProgressVo &vo) {
        j.at("id").get_to(vo.id);
        j.at("x").get_to(vo.x);
        j.at("y").get_to(vo.y);
        j.at("currentStep").get_to(vo.currentStep);
        j.at("totalStep").get_to(vo.totalStep);
        j.at("currentFrequency").get_to(vo.currentFrequency);
        j.at("totalFrequency").get_to(vo.totalFrequency);
        j.at("work_status").get_to(vo.work_status);
        j.at("mode").get_to(vo.m_mode);
        j.at("is_cleaning").get_to(vo.is_cleaning);
    }
};


#endif //APP_COMMUNICATION_POINTPROGRESSVO_H
