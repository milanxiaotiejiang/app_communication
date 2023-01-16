//
// Created by lijiang on 2021/12/18.
//

#include <model/CoreError.h>
#include <iostream>
#include "manager/CoreErrorManager.h"

void CoreErrorManager::sendCoreError(int errorCode, const std::string &errorMessage) {
    auto coreError = CoreError(errorCode, errorMessage);
    std::cout << coreError << std::endl;

}
