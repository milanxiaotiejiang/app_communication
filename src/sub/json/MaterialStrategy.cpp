//
// Created by mi on 2022/8/1.
//

#include "sub/json/MaterialStrategy.h"
#include "net/base/VersionSubscribe.h"
#include "task/manager/MechanismManager.h"
#include "leave/MaintenanceMode.h"

ConsumableVo GetConsumableStrategy::handler(string params) {
    const Consumable &consumable = PropertyDataBase::instance().loadConsumable();
    ConsumableVo consumableVo(consumable.sweep_expected,
                              consumable.mop_expected,
                              consumable.vacuum_expected,
                              consumable.push_expected,
                              consumable.aromatherapy_expected,
                              consumable.disinfect_expected,

                              consumable.sweep_use,
                              consumable.mop_use,
                              consumable.vacuum_use,
                              consumable.push_use,
                              consumable.aromatherapy_use,
                              consumable.disinfect_use);
    return consumableVo;
}

ConsumableVo ResetConsumableStrategy::handler(ResetConsumableVo params) {
    PropertyDataBase::instance().resetConsumable(
            params.isSweep(),
            params.isMop(),
            params.isVacuum(),
            params.isPush(),
            params.isAromatherapy(),
            params.isDisinfect()
    );
    const Consumable &consumable = PropertyDataBase::instance().loadConsumable();
    ConsumableVo consumableVo(consumable.sweep_expected,
                              consumable.mop_expected,
                              consumable.vacuum_expected,
                              consumable.push_expected,
                              consumable.aromatherapy_expected,
                              consumable.disinfect_expected,

                              consumable.sweep_use,
                              consumable.mop_use,
                              consumable.vacuum_use,
                              consumable.push_use,
                              consumable.aromatherapy_use,
                              consumable.disinfect_use);
    return consumableVo;
}

string HotWindModeStrategy::handler(int params) {
    if (params > 0) {
        HotWindNoteSingleton::instance().openHotWind();
    } else {
        HotWindNoteSingleton::instance().closeHotWind();
    }
    return "";
}

bool HotWindModeStatusStrategy::handler(string params) {
    return HotWindNoteSingleton::instance().isHotWind();
}

string MaintenanceModeStrategy::handler(int params) {
    MaintenanceModeSingleton::instance().operateMaintenanceMode(params > 0);
    return "";
}

bool MaintenanceModeStatusStrategy::handler(string params) {
    return MaintenanceModeSingleton::instance().isMaintenanceMode();
}
