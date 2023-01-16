//
// Created by mi on 2022/8/1.
//

#include "model/MaterialStatus.h"

MaterialDuration::MaterialDuration(long expectedDuration, long useDuration) : expected_duration(expectedDuration),
                                                                              use_duration(useDuration) {}

long MaterialDuration::getExpectedDuration() const {
    return expected_duration;
}

void MaterialDuration::setExpectedDuration(long expectedDuration) {
    expected_duration = expectedDuration;
}

long MaterialDuration::getUseDuration() const {
    return use_duration;
}

void MaterialDuration::setUseDuration(long useDuration) {
    use_duration = useDuration;
}

MaterialStatus::MaterialStatus(const MaterialDuration &softBrush, const MaterialDuration &carpetBrush,
                               const MaterialDuration &pushBrush, const MaterialDuration &fanFilter) : soft_brush(
        softBrush), carpet_brush(carpetBrush), push_brush(pushBrush), fan_filter(fanFilter) {}

const MaterialDuration &MaterialStatus::getSoftBrush() const {
    return soft_brush;
}

void MaterialStatus::setSoftBrush(const MaterialDuration &softBrush) {
    soft_brush = softBrush;
}

const MaterialDuration &MaterialStatus::getCarpetBrush() const {
    return carpet_brush;
}

void MaterialStatus::setCarpetBrush(const MaterialDuration &carpetBrush) {
    carpet_brush = carpetBrush;
}

const MaterialDuration &MaterialStatus::getPushBrush() const {
    return push_brush;
}

void MaterialStatus::setPushBrush(const MaterialDuration &pushBrush) {
    push_brush = pushBrush;
}

const MaterialDuration &MaterialStatus::getFanFilter() const {
    return fan_filter;
}

void MaterialStatus::setFanFilter(const MaterialDuration &fanFilter) {
    fan_filter = fanFilter;
}