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

    bool isBaseStation();

    void setBaseStation(bool has);
};


#endif //APP_COMMUNICATION_PARAMMANAGER_H
