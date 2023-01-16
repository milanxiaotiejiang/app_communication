//
// Created by admin1 on 22-11-25.
//

#ifndef APP_COMMUNICATION_ZOO_INNER_STATUS_H
#define APP_COMMUNICATION_ZOO_INNER_STATUS_H


class ZooInnerStatus {
private:

    int sweep_status;

    int mop_status;

    int push_status;

    int self_clean_status;

    int vacuum_status;

    int RSOC;

    int tank_status;

    int urgency_stop_status;

    int drawer_status;

    int is_charging;

    int arom_status;

    int knob_available;

    int knob_task;

    int clean_water_level;

    int dirty_water_level;

    bool is_first_switch_mode{true};
public:
    static auto &instance() {
        static ZooInnerStatus obj;
        return obj;
    }


    int getSweepStatus() const {
        return sweep_status;
    }

    void setSweepStatus(int sweepStatus) {
        sweep_status = sweepStatus;
    }

    int getMopStatus() const {
        return mop_status;
    }

    void setMopStatus(int mopStatus) {
        mop_status = mopStatus;
    }

    int getPushStatus() const {
        return push_status;
    }

    void setPushStatus(int pushStatus) {
        push_status = pushStatus;
    }

    int getSelfCleanStatus() const {
        return self_clean_status;
    }

    void setSelfCleanStatus(int selfCleanStatus) {
        self_clean_status = selfCleanStatus;
    }

    int getVacuumStatus() const {
        return vacuum_status;
    }

    void setVacuumStatus(int vacuumStatus) {
        vacuum_status = vacuumStatus;
    }

    int getRsoc() const {
        return RSOC;
    }

    void setRsoc(int rsoc) {
        RSOC = rsoc;
    }

    int getTankStatus() const {
        return tank_status;
    }

    void setTankStatus(int tankStatus) {
        tank_status = tankStatus;
    }

    int getUrgencyStopStatus() const {
        return urgency_stop_status;
    }

    void setUrgencyStopStatus(int urgencyStopStatus) {
        urgency_stop_status = urgencyStopStatus;
    }

    int getDrawerStatus() const {
        return drawer_status;
    }

    void setDrawerStatus(int drawerStatus) {
        drawer_status = drawerStatus;
    }

    int getIsCharging() const {
        return is_charging;
    }

    void setIsCharging(int isCharging) {
        is_charging = isCharging;
    }

    int getAromStatus() const {
        return arom_status;
    }

    void setAromStatus(int aromStatus) {
        arom_status = aromStatus;
    }

    int getKnobAvailable() const {
        return knob_available;
    }

    void setKnobAvailable(int knobAvailable) {
        knob_available = knobAvailable;
    }

    int getKnobTask() const {
        return knob_task;
    }

    void setKnobTask(int knobTask) {
        knob_task = knobTask;
    }

    void setIsFirstSwitchMode(bool isFirstSwitchMode) {
        is_first_switch_mode = isFirstSwitchMode;
    }

    bool getIsFirstSwitchMode() {
        return is_first_switch_mode;
    }

    void setCleanWaterLevel(int level) {
        clean_water_level = level;
    }

    int getCleanWaterLevel() {
        return clean_water_level;
    }

    void setDirtyWaterLevel(int level) {
        dirty_water_level = level;
    }

    int getDirtyWaterLevel() {
        return dirty_water_level;
    }
};


#endif //APP_COMMUNICATION_ZOO_INNER_STATUS_H
