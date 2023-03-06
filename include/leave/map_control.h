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

    void changeMap(std::string map_id);

};


#endif //APP_COMMUNICATION_MAP_CONTROL_H
