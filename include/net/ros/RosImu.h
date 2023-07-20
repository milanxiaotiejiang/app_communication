//
// Created by admin1 on 2023/7/17.
//

#ifndef APP_COMMUNICATION_ROSIMU_H
#define APP_COMMUNICATION_ROSIMU_H

#include <boost/array.hpp>
#include <ostream>
#include "nlohmann/json.hpp"
#include "RosBasic.h"

using json = nlohmann::json;

class RosVector3 {
private:
    double x;
    double y;
    double z;

public:
    RosVector3() {}

    RosVector3(double x, double y, double z) : x(x), y(y), z(z) {}

    friend void to_json(json &j, const RosVector3 &model) {
        j = json{
                {"x", model.x},
                {"y", model.y},
                {"z", model.z},
        };
    }

    friend void from_json(const json &j, RosVector3 &model) {
        j.at("x").get_to(model.x);
        j.at("y").get_to(model.y);
        j.at("z").get_to(model.z);
    }

    friend std::ostream &operator<<(std::ostream &os, const RosVector3 &vector3) {
        os << "x: " << vector3.x << " y: " << vector3.y << " z: " << vector3.z;
        return os;
    }
};

class RosImu {
private:
    RosHeader header;
    RosOrientation orientation;
    std::vector<double> orientation_covariance;
    RosVector3 angular_velocity;
    std::vector<double> angular_velocity_covariance;
    RosVector3 linear_acceleration;
    std::vector<double> linear_acceleration_covariance;

public:
    RosImu() {}

    RosImu(const RosHeader &header, const RosOrientation &orientation,
           const std::vector<double> &orientationCovariance, const RosVector3 &angularVelocity,
           const std::vector<double> &angularVelocityCovariance, const RosVector3 &linearAcceleration,
           const std::vector<double> &linearAccelerationCovariance) : header(header), orientation(orientation),
                                                                      orientation_covariance(orientationCovariance),
                                                                      angular_velocity(angularVelocity),
                                                                      angular_velocity_covariance(
                                                                              angularVelocityCovariance),
                                                                      linear_acceleration(linearAcceleration),
                                                                      linear_acceleration_covariance(
                                                                              linearAccelerationCovariance) {}

    friend void to_json(json &j, const RosImu &model) {
        j = json{
                {"header",                         model.header},
                {"orientation",                    model.orientation},
                {"orientation_covariance",         model.orientation_covariance},
                {"angular_velocity",               model.angular_velocity},
                {"angular_velocity_covariance",    model.angular_velocity_covariance},
                {"linear_acceleration",            model.linear_acceleration},
                {"linear_acceleration_covariance", model.linear_acceleration_covariance},
        };
    }

    friend void from_json(const json &j, RosImu &model) {
        j.at("header").get_to(model.header);
        j.at("orientation").get_to(model.orientation);
        j.at("orientation_covariance").get_to(model.orientation_covariance);
        j.at("angular_velocity").get_to(model.angular_velocity);
        j.at("angular_velocity_covariance").get_to(model.angular_velocity_covariance);
        j.at("linear_acceleration").get_to(model.linear_acceleration);
        j.at("linear_acceleration_covariance").get_to(model.linear_acceleration_covariance);
    }

    friend std::ostream &operator<<(std::ostream &os, const RosImu &imu) {
        os << "header: " << imu.header << " orientation: " << imu.orientation << " orientation_covariance: "
           << imu.orientation_covariance.size() << " angular_velocity: " << imu.angular_velocity
           << " angular_velocity_covariance: " << imu.angular_velocity_covariance.size() << " linear_acceleration: "
           << imu.linear_acceleration << " linear_acceleration_covariance: " << imu.linear_acceleration_covariance.size();
        return os;
    }
};

#endif //APP_COMMUNICATION_ROSIMU_H
