//
// Created by Looper on 2023/3/6.
//

#ifndef APP_COMMUNICATION_MAP_CONTROL_H
#define APP_COMMUNICATION_MAP_CONTROL_H

#include "string"

class MapControl {
public:
    static auto &instance() {
        static MapControl obj;
        return obj;
    }

    bool initialize();

    bool loadInformation(const std::string &map_id);

    bool backupAndRetrieve(const std::string &map_id);

    bool backupProhibition(const std::string &map_id, bool retrieve);

    bool backupMap(const std::string &map_id, bool retrieve);

};


#endif //APP_COMMUNICATION_MAP_CONTROL_H
