//
// Created by Looper on 2023/2/22.
//

#ifndef APP_COMMUNICATION_PARAMMANAGER_H
#define APP_COMMUNICATION_PARAMMANAGER_H

class ParamManager {
public:
    static auto &instance() {
        static ParamManager obj;
        return obj;
    }

    void loadDefaultParam();

    int getTof();

    void setTof(int tof);

    void setSilver(bool silver);

    bool getSilver();

    int getDry();

    void setDry(int dry);

    bool getEnergy();

    void setEnergy(bool energy);

    bool getTxtUpgrade();

    void setTxtUpgrade(bool txt_upgrade);

    bool getRainSnow();

    void setRainSnow(bool rain_snow);

    bool isBaseStation();

    void setBaseStation(bool has);

    bool getCollectDust();

    void setCollectDust(bool collect_dust);

    bool getAutoOil();

    void setAutoOil(bool auto_oil);

    long getMaintenanceStartTime();

    void setMaintenanceStartTime(long maintenance_start_time);
};


#endif //APP_COMMUNICATION_PARAMMANAGER_H
