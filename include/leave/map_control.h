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

    bool store2Use(const std::string &map_id);

    bool use2Store(const std::string &map_id);

    void tt();

};


#endif //APP_COMMUNICATION_MAP_CONTROL_H
