//
// Created by mi on 2022/8/1.
//

#ifndef APP_COMMUNICATION_MATERIALSTATUS_H
#define APP_COMMUNICATION_MATERIALSTATUS_H

#include "nlohmann/json.hpp"

using json = nlohmann::json;

const int MATERIAL_STATUS_VERSION = 1;

const long SOFT_BRUSH_EXPECTED_DURATION = 56 * 60 * 60;
const long CARPET_BRUSH_EXPECTED_DURATION = 112 * 60 * 60;
const long PUSH_BRUSH_EXPECTED_DURATION = 56 * 60 * 60;
const long FAN_FILTER_EXPECTED_DURATION = 56 * 60 * 60;

class MaterialDuration {
private:
    long expected_duration;
    long use_duration;
public:
    MaterialDuration(long expectedDuration, long useDuration);

    long getExpectedDuration() const;

    void setExpectedDuration(long expectedDuration);

    long getUseDuration() const;

    void setUseDuration(long useDuration);

    friend void to_json(json &j, const MaterialDuration &model) {
        j = json{
                {"expected_duration", model.expected_duration},
                {"use_duration",      model.use_duration},
        };
    }

    friend void from_json(const json &j, MaterialDuration &model) {
        j.at("expected_duration").get_to(model.expected_duration);
        j.at("use_duration").get_to(model.use_duration);
    }
};

class MaterialStatus {
private:
    MaterialDuration soft_brush;
    MaterialDuration carpet_brush;
    MaterialDuration push_brush;
    MaterialDuration fan_filter;
public:
    MaterialStatus(const MaterialDuration &softBrush, const MaterialDuration &carpetBrush,
                   const MaterialDuration &pushBrush, const MaterialDuration &fanFilter);

    const MaterialDuration &getSoftBrush() const;

    void setSoftBrush(const MaterialDuration &softBrush);

    const MaterialDuration &getCarpetBrush() const;

    void setCarpetBrush(const MaterialDuration &carpetBrush);

    const MaterialDuration &getPushBrush() const;

    void setPushBrush(const MaterialDuration &pushBrush);

    const MaterialDuration &getFanFilter() const;

    void setFanFilter(const MaterialDuration &fanFilter);

    friend void to_json(json &j, const MaterialStatus &model) {
        j = json{
                {"soft_brush",   model.soft_brush},
                {"carpet_brush", model.carpet_brush},
                {"push_brush",   model.push_brush},
                {"fan_filter",   model.fan_filter}
        };
    }

    friend void from_json(const json &j, MaterialStatus &model) {
        j.at("soft_brush").get_to(model.soft_brush);
        j.at("carpet_brush").get_to(model.carpet_brush);
        j.at("push_brush").get_to(model.push_brush);
        j.at("fan_filter").get_to(model.fan_filter);
    }
};

#endif //APP_COMMUNICATION_MATERIALSTATUS_H
