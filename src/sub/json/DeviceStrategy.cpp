
#include "sub/json/DeviceStrategy.h"
#include "task/manager/MechanismManager.h"
#include "task/manager/manual.h"
#include "task/subscribe/zoo_inner_status.h"
#include "leave/ParamManager.h"
#include "db/segmentation_data_base.h"
#include "db/task_data_base.h"

/**
 * @brief Get the Device Status Strategy::date Progressing object获取机器当前状态
 * 
 * @param pubInner 
 * @param pubOut 
 * @param jdecode 
 */

DeviceStatus GetDeviceStatusStrategy::handler(std::string method) {
    //追加几行，每当有连接时候获取下版本号
    std_msgs::Int32 version;
    version.data = 0;
    PublishInnerManager::instance().publishDSVersion(version);
    std_msgs::Int32 version1;
    version1.data = 1;
    PublishInnerManager::instance().publishDSVersion(version1);

    WorkStatus workStatus(ZooInnerStatus::instance().getSweepStatus(),
                          ZooInnerStatus::instance().getMopStatus(),
                          0,
                          ZooInnerStatus::instance().getPushStatus(),
                          ZooInnerStatus::instance().getAromStatus(),
                          0);

    //获取地图ID和地图名称
    MapPo &mapPo = SegmentationDataBase::instance().getDbMap();
    MapInfo map(mapPo.id, mapPo.name);

    int machineCode = AsyncMachine::instance().getMachineCode();
    std::string machineMessage = AsyncMachine::instance().getMachineMessage(machineCode);

    DeviceStatus ds(ZooInnerStatus::instance().getRsoc(), 140000, 15000, map,
                    ZooInnerStatus::instance().getUrgencyStopStatus(), false, false, false,
                    workStatus, machineMessage, machineCode);

    return ds;
}

DeviceStatusV2 GetDeviceStatusStrategyV2::handler(std::string params) {
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

    //获取地图ID和地图名称
    MapPo &mapPo = SegmentationDataBase::instance().getDbMap();
    MapInfo map(mapPo.id, mapPo.name);

    //获取work_status_code
    //int t_work_status_code = DeviceStatusManager::get_instance()->getWorkStatusCode();

    //int arom_status = DeviceStatusManager::get_instance()->getWorkStatus().getAromatherapyStatus();

    bool has_facerecognition = Variable::get_instance()->getface_recognition();
    bool has_basestation = Variable::get_instance()->getBaseExist();
    bool hasknob = Variable::get_instance()->getknob();
    bool hasvoice = Variable::get_instance()->getvoice();
    std::string ss = VersionManager::instance().getAirCodeVersion();
    std::size_t found3 = ss.find_last_of("_");
    std::string fff = ss.substr(0, ss.length() - 4);
    std::string rosv = fff;//VersionManager::instance().getAirCodeVersion();
    std::string ecusv = VersionManager::instance().getDsSoftVersion();
    std::string ecuhv = VersionManager::instance().getDsHardVersion();
    std::string padv = VersionManager::instance().getAppPadVersion();

    int machineCode = AsyncMachine::instance().getMachineCode();
    std::string machineMessage = AsyncMachine::instance().getMachineMessage(machineCode);

    DeviceStatusV2 ds(ZooInnerStatus::instance().getRsoc(), 140000, 1500, map,
                      t_is_urgency_stop, t_drawer_status, t_dust_box_status, t_water_box_status,
                      workStatusUpgrade, machineMessage, machineCode,
                      "1.0.0", has_facerecognition, has_basestation, hasknob, hasvoice, rosv, ecusv, ecuhv, padv,
                      1);

    return ds;
}

std::string ChangeWorkModeStrategy::handler(WorkStatus params) {
    MechanismManager::instance().controlWorkStatus(params, false);
    return "";
}

std::string ChangeAromStatusStrategy::handler(bool params) {
    std_msgs::Int32 arom_status;
    arom_status.data = params;
    PublishInnerManager::instance().publishAromStatus(arom_status);
    return "";
}

int GetHotWindModeStrategy::handler(std::string params) {
    return ParamManager::instance().getDry();
}

int SetHotWindModeStrategy::handler(int params) {
    ParamManager::instance().setDry(params);
    return ParamManager::instance().getDry();
}

void AutomaticOilingStrategy::handler() {
    PublishInnerManager::instance().publishOil();
}

std::string SetBaseStationStrategy::handler(bool params) {
    ParamManager::instance().setBaseStation(params);
    return "";
}

bool GetBaseStationStrategy::handler(std::string params) {
    return ParamManager::instance().isBaseStation();
}

std::string SetRainSnowStrategy::handler(bool params) {
    if (params) {
        //开启“雨雪天模式”时：如果没有雨雪天任务，不可切换；如果有雨雪天任务，允许切换；
        MapPo map = SegmentationDataBase::instance().getDbMap();
        const TaskVo &rainSnowTask = TaskDataBase::instance().loadRainSnowTask(map.id);
        if (rainSnowTask.getId() == -1) {
            throw app::exception(make_error_code(error::the_rain_snow_task_is_not_set));
        }
    }
    ParamManager::instance().setRainSnow(params);
    return "";
}

bool GetRainSnowStrategy::handler(std::string params) {
    return ParamManager::instance().getRainSnow();
}

std::string SetCollectDustStrategy::handler(bool params) {
    ParamManager::instance().setCollectDust(params);
    return "";
}

bool GetCollectDustStrategy::handler(std::string params) {
    return ParamManager::instance().getCollectDust();
}

std::string SetAutoOilStrategy::handler(bool params) {
    ParamManager::instance().setAutoOil(params);
    return "";
}

bool GetAutoOilStrategy::handler(std::string params) {
    return ParamManager::instance().getAutoOil();
}

std::string SetMaintenanceStartTimeStrategy::handler(long params) {
    ParamManager::instance().setMaintenanceStartTime(params);
    return "";
}

long GetMaintenanceStartTimeStrategy::handler(std::string params) {
    return ParamManager::instance().getMaintenanceStartTime();
}
