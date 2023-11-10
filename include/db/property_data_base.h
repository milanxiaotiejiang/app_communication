//
// Created by Looper on 2023/4/6.
//

#ifndef APP_COMMUNICATION_PROPERTY_DATA_BASE_H
#define APP_COMMUNICATION_PROPERTY_DATA_BASE_H

#include "db/SqliteDataBase.h"

class PropertyDataBase {
private:
    PropertyDataBase() = default;

    PropertyDataBase(PropertyDataBase &) = delete;

    PropertyDataBase &operator=(const PropertyDataBase &) = delete;

public:
    ~PropertyDataBase() = default;

private:
    PropertyStorage propertyStorage = SqliteDataBase::initPropertyStorage();

    long consumableId;
public:
    static auto &instance() {
        static PropertyDataBase obj;
        return obj;
    }

    void initProperty();

    Consumable loadConsumable();

    void updateConsumable(long sweep_increment,
                          long mop_increment,
                          long vacuum_increment,
                          long push_increment,
                          long aromatherapy_increment,
                          long disinfect_increment);

    void resetConsumable(bool sweep_reset,
                         bool mop_reset,
                         bool vacuum_reset,
                         bool push_reset,
                         bool aromatherapy_reset,
                         bool disinfect_reset);
};


#endif //APP_COMMUNICATION_PROPERTY_DATA_BASE_H
