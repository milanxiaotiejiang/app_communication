//
// Created by mi on 2022/8/1.
//

#include "sub/json/MaterialStrategy.h"
#include "net/base/VersionSubscribe.h"

VersionSubscribe<MaterialStatus> GetMaterialStrategy::handler(string params) {
    MaterialDuration soft_brush(SOFT_BRUSH_EXPECTED_DURATION, 1000);
    MaterialDuration carpet_brush(CARPET_BRUSH_EXPECTED_DURATION, 1001);
    MaterialDuration push_brush(PUSH_BRUSH_EXPECTED_DURATION, 1002);
    MaterialDuration fan_filter(FAN_FILTER_EXPECTED_DURATION, 1003);
    MaterialStatus materialStatus(soft_brush, carpet_brush, push_brush, fan_filter);
    //回复，带参数，包括分配的id
    VersionSubscribe<MaterialStatus> response(MATERIAL_STATUS_VERSION, materialStatus);
    return response;
}
