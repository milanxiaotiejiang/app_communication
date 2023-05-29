//
// Created by Looper on 2023/4/6.
//

#ifndef APP_COMMUNICATION_CONSUMABLE_H
#define APP_COMMUNICATION_CONSUMABLE_H

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class ConsumableVo {
private:
    long sweep_expected;
    long mop_expected;
    long vacuum_expected;
    long push_expected;
    long aromatherapy_expected;
    long disinfect_expected;

    long sweep_use;
    long mop_use;
    long vacuum_use;
    long push_use;
    long aromatherapy_use;
    long disinfect_use;

public:
    ConsumableVo();

    ConsumableVo(long sweepExpected, long mopExpected, long vacuumExpected, long pushExpected,
                 long aromatherapyExpected, long disinfectExpected, long sweepUse, long mopUse, long vacuumUse,
                 long pushUse, long aromatherapyUse, long disinfectUse);

    long getSweepExpected() const;

    void setSweepExpected(long sweepExpected);

    long getMopExpected() const;

    void setMopExpected(long mopExpected);

    long getVacuumExpected() const;

    void setVacuumExpected(long vacuumExpected);

    long getPushExpected() const;

    void setPushExpected(long pushExpected);

    long getAromatherapyExpected() const;

    void setAromatherapyExpected(long aromatherapyExpected);

    long getDisinfectExpected() const;

    void setDisinfectExpected(long disinfectExpected);

    long getSweepUse() const;

    void setSweepUse(long sweepUse);

    long getMopUse() const;

    void setMopUse(long mopUse);

    long getVacuumUse() const;

    void setVacuumUse(long vacuumUse);

    long getPushUse() const;

    void setPushUse(long pushUse);

    long getAromatherapyUse() const;

    void setAromatherapyUse(long aromatherapyUse);

    long getDisinfectUse() const;

    void setDisinfectUse(long disinfectUse);

    friend void to_json(json &j, const ConsumableVo &b) {
        j = json{
                {"sweep_expected",        b.sweep_expected},
                {"mop_expected",          b.mop_expected},
                {"vacuum_expected",       b.vacuum_expected},
                {"push_expected",         b.push_expected},
                {"aromatherapy_expected", b.aromatherapy_expected},
                {"disinfect_expected",    b.disinfect_expected},
                {"sweep_use",             b.sweep_use},
                {"mop_use",               b.mop_use},
                {"vacuum_use",            b.vacuum_use},
                {"push_use",              b.push_use},
                {"aromatherapy_use",      b.aromatherapy_use},
                {"disinfect_use",         b.disinfect_use},
        };
    }

    friend void from_json(const json &j, ConsumableVo &b) {
        j.at("sweep_expected").get_to(b.sweep_expected);
        j.at("mop_expected").get_to(b.mop_expected);
        j.at("vacuum_expected").get_to(b.vacuum_expected);
        j.at("push_expected").get_to(b.push_expected);
        j.at("aromatherapy_expected").get_to(b.aromatherapy_expected);
        j.at("disinfect_expected").get_to(b.disinfect_expected);
        j.at("sweep_use").get_to(b.sweep_use);
        j.at("mop_use").get_to(b.mop_use);
        j.at("vacuum_use").get_to(b.vacuum_use);
        j.at("push_use").get_to(b.push_use);
        j.at("aromatherapy_use").get_to(b.aromatherapy_use);
        j.at("disinfect_use").get_to(b.disinfect_use);
    }
};

class ResetConsumableVo {
private:
    bool sweep;
    bool mop;
    bool vacuum;
    bool push;
    bool aromatherapy;
    bool disinfect;
public:
    ResetConsumableVo();

    ResetConsumableVo(bool sweep, bool mop, bool vacuum, bool push, bool aromatherapy, bool disinfect);

    bool isSweep() const;

    void setSweep(bool sweep);

    bool isMop() const;

    void setMop(bool mop);

    bool isVacuum() const;

    void setVacuum(bool vacuum);

    bool isPush() const;

    void setPush(bool push);

    bool isAromatherapy() const;

    void setAromatherapy(bool aromatherapy);

    bool isDisinfect() const;

    void setDisinfect(bool disinfect);

    friend void to_json(json &j, const ResetConsumableVo &b) {
        j = json{
                {"sweep",        b.sweep},
                {"mop",          b.mop},
                {"vacuum",       b.vacuum},
                {"push",         b.push},
                {"aromatherapy", b.aromatherapy},
                {"disinfect",    b.disinfect},
        };
    }

    friend void from_json(const json &j, ResetConsumableVo &b) {
        j.at("sweep").get_to(b.sweep);
        j.at("mop").get_to(b.mop);
        j.at("vacuum").get_to(b.vacuum);
        j.at("push").get_to(b.push);
        j.at("aromatherapy").get_to(b.aromatherapy);
        j.at("disinfect").get_to(b.disinfect);
    }
};

#endif //APP_COMMUNICATION_CONSUMABLE_H
