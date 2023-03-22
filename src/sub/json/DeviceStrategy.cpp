//
// Created by lijiang on 2021/12/18.
//
// Modified by liquan on 2022/4/18

// Modified by siyu.zhu on 2022/05/26.

#include "sub/json/DeviceStrategy.h"
#include "task/manager/MechanismManager.h"
#include "task/manager/manual.h"
#include "task/subscribe/zoo_inner_status.h"
#include "leave/ParamManager.h"

/**
 * @brief Get the Device Status Strategy::date Progressing object获取机器当前状态
 * 
 * @param pubInner 
 * @param pubOut 
 * @param jdecode 
 */

DeviceStatus GetDeviceStatusStrategy::handler(string method) {
    //追加几行，每当有连接时候获取下版本号
    std_msgs::Int16 version;
    version.data = 0;
    PublishInnerManager::instance().publishDSVersion(version);
    std_msgs::Int16 version1;
    version1.data = 1;
    PublishInnerManager::instance().publishDSVersion(version1);

    WorkStatus workStatus(ZooInnerStatus::instance().getSweepStatus(),
                          ZooInnerStatus::instance().getMopStatus(),
                          0,
                          ZooInnerStatus::instance().getPushStatus(),
                          ZooInnerStatus::instance().getAromStatus(),
                          0);

    //获取地图ID和地图名称
    MapInfo map;
    map.setMapId(1);
    map.setMapName("11");

    int machineCode = AsyncMachine::instance().getMachineCode();
    std::string machineMessage = AsyncMachine::instance().getMachineMessage(machineCode);

    DeviceStatus ds(ZooInnerStatus::instance().getRsoc(), 140000, 15000, map,
                    ZooInnerStatus::instance().getUrgencyStopStatus(), false, false, false,
                    workStatus, machineMessage, machineCode);

    return ds;
}

DeviceStatusV2 GetDeviceStatusStrategyV2::handler(string params) {
    /*
    int sweep_status{-1};//清扫
    int mop_status{-1};//湿拖
    int vacuum_status{-1};//尘吸
    int push_status{-1};//尘推
    int aromatherapy_status{-1};//香薰
    int disinfect_status{-1};//消杀
     */
    WorkStatus workStatus(ZooInnerStatus::instance().getSweepStatus(),
                          ZooInnerStatus::instance().getMopStatus(),
                          0,
                          ZooInnerStatus::instance().getPushStatus(),
                          ZooInnerStatus::instance().getAromStatus(),
                          0);

    WorkStatusUpgrade workStatusUpgrade(ZooInnerStatus::instance().getMopStatus(),
                                        0,
                                        ZooInnerStatus::instance().getVacuumStatus(),
                                        ZooInnerStatus::instance().getPushStatus(),
                                        ZooInnerStatus::instance().getAromStatus());

    //获取剩余电量
    //int t_RSOC = DeviceStatusManager::get_instance()->getRSOC();
    auto t_RSOC = ZooInnerStatus::instance().getRsoc();
    auto t_is_urgency_stop = ZooInnerStatus::instance().getUrgencyStopStatus();
    auto t_drawer_status = ZooInnerStatus::instance().getDrawerStatus();
    //获取尘箱状态
    bool t_dust_box_status = false;
    //获取水箱状态
    bool t_water_box_status = false;

    //获取地图ID和地图名称，假数据
    MapInfo map;
    map.setMapId(1);
    map.setMapName("11");

    //获取work_status_code
    //int t_work_status_code = DeviceStatusManager::get_instance()->getWorkStatusCode();

    //int arom_status = DeviceStatusManager::get_instance()->getWorkStatus().getAromatherapyStatus();

    bool has_facerecognition = Variable::get_instance()->getface_recognition();
    bool has_basestation = Variable::get_instance()->getBaseExist();
    bool hasknob = Variable::get_instance()->getknob();
    bool hasvoice = Variable::get_instance()->getvoice();
    string ss = VersionManager::instance().getAirCodeVersion();
    std::size_t found3 = ss.find_last_of("_");
    string fff = ss.substr(0, ss.length() - 4);
    string rosv = fff;//VersionManager::instance().getAirCodeVersion();
    string ecusv = VersionManager::instance().getDsSoftVersion();
    string ecuhv = VersionManager::instance().getDsHardVersion();
    string padv = VersionManager::instance().getAppPadVersion();

    int machineCode = AsyncMachine::instance().getMachineCode();
    std::string machineMessage = AsyncMachine::instance().getMachineMessage(machineCode);

    DeviceStatusV2 ds(ZooInnerStatus::instance().getRsoc(), 140000, 1500, map,
                      t_is_urgency_stop, t_drawer_status, t_dust_box_status, t_water_box_status,
                      workStatusUpgrade, machineMessage, machineCode,
                      "1.0.0", has_facerecognition, has_basestation, hasknob, hasvoice, rosv, ecusv, ecuhv, padv,
                      1);

    return ds;
}

string ChangeWorkModeStrategy::handler(WorkStatus params) {
    MechanismManager::instance().controlWorkStatus(params);
    return "";
}

string ChangeAromStatusStrategy::handler(bool params) {
    std_msgs::Int16 arom_status;
    arom_status.data = (int16_t) params;
    PublishInnerManager::instance().publishAromStatus(arom_status);
    return "";
}

string SelfCleanStrategy::handler(string params) {
    //操作,发布指令，打开自清洁
    std_msgs::Int16 clean_mode;
    clean_mode.data = ((int16_t) 4);
    PublishInnerManager::instance().publishPushMode(clean_mode);
    return "";
}

string OpenMachineDrawerStrategy::handler(string params) {
    //操作,发布指令，打开自清洁
    std_msgs::Int16 drawer_cmd;
    drawer_cmd.data = ((int16_t) 1);
    PublishInnerManager::instance().publishDrawerCmd(drawer_cmd);
    return "";
}

string LightBeltModeStrategy::handler(int params) {
    int16_t light_mode = (int16_t) params;
    //操作,发布指令，打开自清洁
    std_msgs::Int16 light_cmd;
    light_cmd.data = ((int16_t) light_mode);
    PublishInnerManager::instance().publishLightCmd(light_cmd);
    return "";
}

string PlayerRecruitVoiceStrategy::handler(int params) {
    //操作,发布指令，播放音乐
    std_msgs::Int32 player_cmd;
    player_cmd.data = ((int32_t) 1);
    PublishInnerManager::instance().publishMusic(player_cmd);
    return "";
}

int GetHotWindModeStrategy::handler(string params) {
    return ParamManager::instance().getDry();
}

int SetHotWindModeStrategy::handler(int params) {
    ParamManager::instance().setDry(params);
    return ParamManager::instance().getDry();
}

void AutomaticOilingStrategy::handler() {
    std_msgs::Int32 message;
    message.data = 1;
    PublishInnerManager::instance().publishOil(message);
}
