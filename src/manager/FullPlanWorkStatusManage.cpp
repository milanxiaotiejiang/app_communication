//
// Created by admin1 on 22-11-29.
//

#include "manager/FullPlanWorkStatusManager.h"
#include <ros/package.h>
#include "ros/ros.h"

#include "BaseThrowable.h"

#include <nlohmann/json.hpp>

#include "glog/logging.h"
#include "tool/write_file.hpp"

WorkStatus FullPlanWorkStatusManager::getFullCleanWorkStatus() {
    auto dataBasePath = ros::package::getPath("data_base");
    std::string filePath;
    filePath.append(dataBasePath);
    filePath.append(FULL_CLEANING_MODE);

    if (!sh::File::exists(filePath)) {
        WorkStatus workStatus(0, 0, 0, 1, 0, 0);
        workStatus.setPushStatus(1);
        json currentJson = workStatus;
        if (!sh::File::saveTextTo(filePath, currentJson.dump())) {
            throw app::exception(make_error_code(error::full_cleaning_mode_error));
        }
    }


    std::string content;
    std::unique_ptr<sh::File> uniquePtr(new sh::File(filePath));
    if (uniquePtr->open(std::ios::in)) {
        content = uniquePtr->readAll();
        uniquePtr->close();
    } else {
        throw app::exception(make_error_code(error::full_cleaning_mode_error));
    }


    if (content.empty()) {
        throw app::exception(make_error_code(error::full_cleaning_mode_error));
    }

    auto status = json::parse(content).get<WorkStatus>();
    return status;
}