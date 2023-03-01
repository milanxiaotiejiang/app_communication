//
// Created by Looper on 2023/3/1.
//

#ifndef APP_COMMUNICATION_TASK_H
#define APP_COMMUNICATION_TASK_H

#include <memory>
#include "string"
#include "vector"
#include "RoomVo.h"
#include "WorkStatus.h"

class TaskVo {
public:
    TaskVo();

    TaskVo(long id, const std::string &oMapId, const std::string &name, int rate, int mode,
           const WorkStatus &workStatus, const std::vector<std::vector<PointVo>> &zones, bool partition,
           const std::vector<int> &subregions, const std::string &source, const std::string &launchPeople,
           long launchTime, long updateTime, long createTime);

    TaskVo(long id, const std::string &oMapId, const std::string &name, int rate, int mode, bool partition,
           const std::string &source, const std::string &launchPeople, long launchTime, long updateTime,
           long createTime);

    void setId(long id);

    void setWorkStatus(const WorkStatus &workStatus);

    void setZones(const std::vector<std::vector<PointVo>> &zones);

    void setSubregions(const std::vector<int> &subregions);

private:
    long id;
    std::string o_map_id;
    std::string name;
    int rate;
    int mode;

    WorkStatus workStatus;

    std::vector<std::vector<PointVo>> zones;
    bool partition;
    std::vector<int> subregions;

    std::string source;
    std::string launch_people;
    long launch_time;

    long update_time;
    long create_time;
};


#endif //APP_COMMUNICATION_TASK_H
