//
// Created by admin1 on 2023/7/17.
//

#ifndef APP_COMMUNICATION_ROSLASERSCAN_H
#define APP_COMMUNICATION_ROSLASERSCAN_H

#include <vector>
#include <ostream>
#include "RosBasic.h"

class RosLaserScan {
private:
    RosHeader header;
    float angle_min;
    float angle_max;
    float angle_increment;
    float time_increment;
    float scan_time;
    float range_min;
    float range_max;
    std::vector<float> ranges;
    std::vector<float> intensities;
    int rangesLength;
    int intensitiesLength;
public:
    RosLaserScan() {}

    RosLaserScan(const RosHeader &header, float angleMin, float angleMax, float angleIncrement, float timeIncrement,
                 float scanTime, float rangeMin, float rangeMax, const std::vector<float> &ranges,
                 const std::vector<float> &intensities, int rangesLength, int intensitiesLength) : header(header),
                                                                                                   angle_min(angleMin),
                                                                                                   angle_max(angleMax),
                                                                                                   angle_increment(
                                                                                                           angleIncrement),
                                                                                                   time_increment(
                                                                                                           timeIncrement),
                                                                                                   scan_time(scanTime),
                                                                                                   range_min(rangeMin),
                                                                                                   range_max(rangeMax),
                                                                                                   ranges(ranges),
                                                                                                   intensities(
                                                                                                           intensities),
                                                                                                   rangesLength(
                                                                                                           rangesLength),
                                                                                                   intensitiesLength(
                                                                                                           intensitiesLength) {}

    friend void to_json(json &j, const RosLaserScan &model) {
        j = json{
                {"header",            model.header},
                {"angle_min",         model.angle_min},
                {"angle_max",         model.angle_max},
                {"angle_increment",   model.angle_increment},
                {"time_increment",    model.time_increment},
                {"scan_time",         model.scan_time},
                {"range_min",         model.range_min},
                {"range_max",         model.range_max},
//                {"ranges",          model.ranges},
//                {"intensities",     model.intensities},
                {"rangesLength",      model.rangesLength},
                {"intensitiesLength", model.intensitiesLength},
        };
    }

    friend void from_json(const json &j, RosLaserScan &model) {
        j.at("header").get_to(model.header);
        j.at("angle_min").get_to(model.angle_min);
        j.at("angle_max").get_to(model.angle_max);
        j.at("angle_increment").get_to(model.angle_increment);
        j.at("time_increment").get_to(model.time_increment);
        j.at("scan_time").get_to(model.scan_time);
        j.at("range_min").get_to(model.range_min);
        j.at("range_max").get_to(model.range_max);
//        j.at("ranges").get_to(model.ranges);
//        j.at("intensities").get_to(model.intensities);
        j.at("rangesLength").get_to(model.rangesLength);
        j.at("intensitiesLength").get_to(model.intensitiesLength);
    }

    friend std::ostream &operator<<(std::ostream &os, const RosLaserScan &scan) {
        os << "header: " << scan.header << " angle_min: " << scan.angle_min << " angle_max: " << scan.angle_max
           << " angle_increment: " << scan.angle_increment << " time_increment: " << scan.time_increment
           << " scan_time: " << scan.scan_time << " range_min: " << scan.range_min << " range_max: " << scan.range_max
           << " ranges: " << scan.ranges.size() << " intensities: " << scan.intensities.size();
        return os;
    }
};

#endif //APP_COMMUNICATION_ROSLASERSCAN_H
