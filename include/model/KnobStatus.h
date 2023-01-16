//
// Created by mi on 2022/8/23.
//

#ifndef APP_COMMUNICATION_KNOBSTATUS_H
#define APP_COMMUNICATION_KNOBSTATUS_H

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class KnobControl {
public:
    KnobControl();

    friend void to_json(json &j, const KnobControl &b) {
        j = json{
        };
    }

    friend void from_json(const json &j, KnobControl &b) {
    }
};

class KnobStatus {
private:
    KnobControl control;
    bool isAvailable{false};
public:
    KnobStatus();

    const KnobControl &getControl() const;

    void setControl(const KnobControl &control);

    bool isAvailable1() const;

    void setIsAvailable(bool isAvailable);

    friend void to_json(json &j, const KnobStatus &b) {
        j = json{
                {"control",     b.control},
                {"isAvailable", b.isAvailable}
        };
    }

    friend void from_json(const json &j, KnobStatus &b) {
        j.at("control").get_to(b.control);
        j.at("isAvailable").get_to(b.isAvailable);
    }
};


#endif //APP_COMMUNICATION_KNOBSTATUS_H
