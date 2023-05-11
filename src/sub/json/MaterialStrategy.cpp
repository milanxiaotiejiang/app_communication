//
// Created by mi on 2022/8/1.
//

#include "sub/json/MaterialStrategy.h"
#include "net/base/VersionSubscribe.h"
#include "task/manager/MechanismManager.h"

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

string MaintenanceModeStrategy::handler(int params) {
    MechanismManager::instance().operateMaintenanceMode(params > 0);
    return "";
}

bool MaintenanceModeStatusStrategy::handler(string params) {
    return MechanismManager::instance().isMaintenanceMode();
}
