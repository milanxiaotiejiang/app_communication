//
// Created by Looper on 2023/4/6.
//

#include "db/property_data_base.h"

void PropertyDataBase::initProperty() {
    propertyStorage.sync_schema();
    std::vector<Consumable> consumableList = propertyStorage.get_all<Consumable>();
    if (consumableList.size() > 1) {
        propertyStorage.remove_all<Consumable>();
        consumableList.clear();
    }
    if (consumableList.empty()) {
        Consumable consumable(
                SWEEP_EXPECTED_DURATION,
                MOP_EXPECTED_DURATION,
                VACUUM_EXPECTED_DURATION,
                PUSH_EXPECTED_DURATION,
                AROMATHERAPY_EXPECTED_DURATION,
                DISINFECT_EXPECTED_DURATION,
                0, 0, 0, 0, 0, 0
        );
        consumableId = propertyStorage.insert(consumable);
    } else {
        consumableId = consumableList[0].id;
    }
}

Consumable PropertyDataBase::loadConsumable() {
    return propertyStorage.get<Consumable>(consumableId);
}

void
PropertyDataBase::updateConsumable(long sweep_increment, long mop_increment, long vacuum_increment, long push_increment,
                                   long aromatherapy_increment, long disinfect_increment) {
    Consumable consumable = propertyStorage.get<Consumable>(consumableId);
    consumable.sweep_use = consumable.sweep_use + sweep_increment;
    consumable.mop_use = consumable.mop_use + mop_increment;
    consumable.vacuum_use = consumable.vacuum_use + vacuum_increment;
    consumable.push_use = consumable.push_use + push_increment;
    consumable.aromatherapy_use = consumable.aromatherapy_use + aromatherapy_increment;
    consumable.disinfect_use = consumable.disinfect_use + disinfect_increment;
    propertyStorage.update(consumable);
}

void PropertyDataBase::resetConsumable(bool sweep_reset, bool mop_reset, bool vacuum_reset, bool push_reset,
                                       bool aromatherapy_reset, bool disinfect_reset) {
    Consumable consumable = propertyStorage.get<Consumable>(consumableId);
    consumable.sweep_use = sweep_reset ? 0 : consumable.sweep_use;
    consumable.mop_use = mop_reset ? 0 : consumable.mop_use;
    consumable.vacuum_use = vacuum_reset ? 0 : consumable.vacuum_use;
    consumable.push_use = push_reset ? 0 : consumable.push_use;
    consumable.aromatherapy_use = aromatherapy_reset ? 0 : consumable.aromatherapy_use;
    consumable.disinfect_use = disinfect_reset ? 0 : consumable.disinfect_use;
    propertyStorage.update(consumable);
}
