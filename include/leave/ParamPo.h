//
// Created by Looper on 2023/2/22.
//

#ifndef APP_COMMUNICATION_PARAMPO_H
#define APP_COMMUNICATION_PARAMPO_H

#include <ostream>

class ParamPo {
private:
    int tof;
    bool silver;
    bool speed;
public:
    ParamPo();

    ParamPo(int tof, bool silver, bool speed);

    int getTof() const;

    void setTof(int tof);

    bool isSilver() const;

    void setSilver(bool silver);

    bool isSpeed() const;

    void setSpeed(bool speed);

    friend std::ostream &operator<<(std::ostream &os, const ParamPo &po);
};


#endif //APP_COMMUNICATION_PARAMPO_H
