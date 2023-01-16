//
// Created by Looper on 2022/11/27.
//

#include "task/model/PointProgressVo.h"

PointProgressVo::PointProgressVo() {}

PointProgressVo::PointProgressVo(const std::string &id, float x, float y, int currentStep, int totalStep,
                                 int currentFrequency, int totalFrequency, const WorkStatus &workStatus, int mMode,
                                 bool isCleaning) : id(id), x(x), y(y), currentStep(currentStep), totalStep(totalStep),
                                                    currentFrequency(currentFrequency), totalFrequency(totalFrequency),
                                                    work_status(workStatus), m_mode(mMode), is_cleaning(isCleaning) {}
