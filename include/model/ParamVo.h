//
// Created by Looper on 2023/2/22.
//

#ifndef APP_COMMUNICATION_PARAMVO_H
#define APP_COMMUNICATION_PARAMVO_H

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class ParamVo {

private:
    int tof;
    bool silver;
    float speed;
public:
    ParamVo();

    ParamVo(int tof, bool silver, int speed);

    int getTof() const;

    void setTof(int tof);

    bool isSilver() const;

    void setSilver(bool silver);

    float getSpeed() const;

    void setSpeed(float speed);

    friend void to_json(json &j, const ParamVo &b) {
        j = json{
                {"tof",    b.tof},
                {"silver", b.silver},
                {"speed",  b.speed},
        };
    }

    friend void from_json(const json &j, ParamVo &b) {
        j.at("tof").get_to(b.tof);
        j.at("silver").get_to(b.silver);
        j.at("speed").get_to(b.speed);
    }
};


#endif //APP_COMMUNICATION_PARAMVO_H
