//
// Created by io on 23-11-7.
//

#ifndef APP_COMMUNICATION_HTTP_HANDLER_H
#define APP_COMMUNICATION_HTTP_HANDLER_H

#include <string>
#include "net/base/BaseResult.h"
#include "model/MapInfo.h"

class HttpHandler {

public:
    static BaseResult<MapImageResponse> imageProgressing(const std::string &map_id);

};


#endif //APP_COMMUNICATION_HTTP_HANDLER_H
