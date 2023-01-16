//
// Created by milan on 2021/12/15.
//

#ifndef C_PTHREAD_WORKSTATUS_H
#define C_PTHREAD_WORKSTATUS_H

#include <nlohmann/json.hpp>
#include <ostream>

using json = nlohmann::json;

//sweep 扫
//mop 湿拖
//vacuum 吸
//push 尘推
//aromatherapy 香薰
//disinfect xiaosha

class WorkStatus {
private:
    int sweep_status{-1};
    int mop_status{-1};
    int vacuum_status{-1};
    int push_status{-1};
    int aromatherapy_status{-1};
    int disinfect_status{-1};

public:
    virtual ~WorkStatus();

    WorkStatus();

    WorkStatus(int sweepStatus, int mopStatus, int vacuumStatus, int pushStatus, int aromatherapyStatus,
               int disinfectStatus);

    int getSweepStatus() const;

    void setSweepStatus(int sweepStatus);

    int getMopStatus() const;

    void setMopStatus(int mopStatus);

    int getVacuumStatus() const;

    void setVacuumStatus(int vacuumStatus);

    int getPushStatus() const;

    void setPushStatus(int pushStatus);

    int getAromatherapyStatus() const;

    void setAromatherapyStatus(int aromatherapyStatus);

    int getDisinfectStatus() const;

    void setDisinfectStatus(int disinfectStatus);


    friend void to_json(json &j, const WorkStatus &b) {
        j = json{
                {"sweep_status",        b.sweep_status},
                {"mop_status",          b.mop_status},
                {"vacuum_status",       b.vacuum_status},
                {"push_status",         b.push_status},
                {"aromatherapy_status", b.aromatherapy_status},
                {"disinfect_status",    b.disinfect_status}
        };
    }

    friend void from_json(const json &j, WorkStatus &b) {
        j.at("sweep_status").get_to(b.sweep_status);
        j.at("mop_status").get_to(b.mop_status);
        j.at("vacuum_status").get_to(b.vacuum_status);
        j.at("push_status").get_to(b.push_status);
        j.at("aromatherapy_status").get_to(b.aromatherapy_status);
        j.at("disinfect_status").get_to(b.disinfect_status);
    }

    friend std::ostream &operator<<(std::ostream &os, const WorkStatus &status);
};


#endif //C_PTHREAD_WORKSTATUS_H
