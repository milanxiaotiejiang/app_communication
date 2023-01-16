//
// Created by mi on 2022/9/5.
//

#ifndef APP_COMMUNICATION_SELFCHECKSTATUS_H
#define APP_COMMUNICATION_SELFCHECKSTATUS_H

#include "nlohmann/json.hpp"
#include "SelfCheckParam.h"

using json = nlohmann::json;

class SelfCheckStatus {
    /*
              本类主要用于时时存储各个传感器状态信息而设立。
          */

    // 各个传感器状态信息。
    bool m_camera_check;
    bool m_laser_check;
    bool m_camera_obstacle;
    bool m_laserscan_obstacle;
    bool m_fall_tof_front_left_check;
    float m_fall_tof_front_left_value;
    bool m_fall_tof_front_right_check;
    float m_fall_tof_front_right_value;
    bool m_fall_tof_left_check;
    float m_fall_tof_left_value;
    bool m_fall_tof_right_check;
    float m_fall_tof_right_value;
    bool m_welt_tof_front_check;
    float m_welt_tof_front_value;
    bool m_welt_tof_after_check;
    float m_welt_tof_after_value;
    bool m_supersonic_front_left_check;
    float m_supersonic_front_left_value;
    bool m_supersonic_front_right_check;
    float m_supersonic_front_right_value;
    bool m_supersonic_corner_left_check;
    float m_supersonic_corner_left_value;
    bool m_supersonic_corner_right_check;
    float m_supersonic_corner_right_value;
    bool m_supersonic_left_check;
    float m_supersonic_left_value;
    bool m_supersonic_right_check;
    float m_supersonic_right_value;
    bool m_inflation_obstacle;             //用于修改膨胀层
    bool m_supersonic_front_left_obsacle;  // 前左
    bool m_supersonic_front_right_obsacle; //前右
    bool m_supersonic_corner_left_obsacle; //角右
    bool m_supersonic_corner_right_obsacle;//角右

public:
    SelfCheckStatus() {
    }

    ~SelfCheckStatus() {}

    void self_check_reset();

    //传入信息时调用的方法。
    void set_camera(bool b) {
        m_camera_check = b;
    }

    void set_laser(bool b) {
        m_laser_check = b;
    }

    void set_camera_obstacle(bool b) {
        m_camera_obstacle = b;
    }

    const bool get_laserscan_obstacle() const {
        return m_laserscan_obstacle;
    }

    const bool get_camera_obstacle() const {
        return m_camera_obstacle;
    }

    const bool get_supersonic_front_right_obstacle() const {
        return m_supersonic_front_right_obsacle;
    }

    const bool get_supersonic_front_left_obstacle() const {
        return m_supersonic_front_left_obsacle;
    }

    const bool get_supersonic_corner_right_obstacle() const {
        return m_supersonic_corner_right_obsacle;
    }

    const bool get_supersonic_corner_left_obstacle() const {
        return m_supersonic_corner_left_obsacle;
    }

    void set_laserscan_obstacle(bool b) {
        m_laserscan_obstacle = b;
    }

    void set_fall_tof_front_left(float value) {
        m_fall_tof_front_left_check = (TOF_MIN1 <= value && value <= TOF_MAX1);
        m_fall_tof_front_left_value = value;
    }

    void set_fall_tof_front_right(float value) {
        m_fall_tof_front_right_check = (TOF_MIN2 <= value && value <= TOF_MAX2);
        m_fall_tof_front_right_value = value;
    }

    void set_fall_tof_left(float value) {
        m_fall_tof_left_check = (TOF_MIN3 <= value && value <= TOF_MAX3);
        m_fall_tof_left_value = value;
    }

    void set_fall_tof_right(float value) {
        m_fall_tof_right_check = (TOF_MIN4 <= value && value <= TOF_MAX4);
        m_fall_tof_right_value = value;
    }

    void set_welt_tof_front(float value) {
        m_welt_tof_front_check = (TOF_MIN5 <= value && value <= TOF_MAX5);
        m_welt_tof_front_value = value;
    }

    void set_welt_tof_after(float value) {
        m_welt_tof_after_check = (TOF_MIN6 <= value && value <= TOF_MAX6);
        m_welt_tof_after_value = value;
    }

    void set_supersonic_front_left(float value) {
        m_supersonic_front_left_check = (SPS_MIN1 <= value && value <= SPS_MAX1);
        m_supersonic_front_left_value = value;
        m_supersonic_front_left_obsacle = (SPS_OBS_MIN1 <= value && value <= SPS_OBS_MAX1);
    }

    void set_supersonic_front_right(float value) {
        m_supersonic_front_right_check = (SPS_MIN2 <= value && value <= SPS_MAX2);
        m_supersonic_front_right_value = value;
        m_supersonic_front_right_obsacle = (SPS_OBS_MIN2 <= value && value <= SPS_OBS_MAX2);
    }

    void set_supersonic_corner_left(float value) {
        m_supersonic_corner_left_check = (SPS_MIN3 <= value && value <= SPS_MAX3);
        m_supersonic_corner_left_value = value;
        m_supersonic_corner_right_obsacle = (SPS_OBS_MIN3 <= value && value <= SPS_OBS_MAX3);
    }

    void set_supersonic_corner_right(float value) {
        m_supersonic_corner_right_check = (SPS_MIN4 <= value && value <= SPS_MAX4);
        m_supersonic_corner_right_value = value;
        m_supersonic_corner_right_obsacle = (SPS_OBS_MIN4 <= value && value <= SPS_OBS_MAX4);
    }

    void set_supersonic_left(float value) {
        m_supersonic_left_check = (SPS_MIN3 <= value && value <= SPS_MAX3);
        m_supersonic_left_value = value;
    }

    void set_supersonic_right(float value) {
        m_supersonic_right_check = (SPS_MIN4 <= value && value <= SPS_MAX4);
        m_supersonic_right_value = value;
    }

    const bool get_inflation_obstacle() const {
        return m_inflation_obstacle;
    }

    void set_inflation_obstacle(const bool &inflation_obstacle) {
        m_inflation_obstacle = inflation_obstacle;
    }

    //发布消息时所用到的方法。
    friend void to_json(json &j, const SelfCheckStatus &b) {
        j = json{
                {"fall_tof_front_left_check",     b.m_fall_tof_front_left_check},
                {"fall_tof_front_left_value",     b.m_fall_tof_front_left_value},
                {"fall_tof_front_right_check",    b.m_fall_tof_front_right_check},
                {"fall_tof_front_right_value",    b.m_fall_tof_front_right_value},
                {"fall_tof_left_check",           b.m_fall_tof_left_check},
                {"fall_tof_left_value",           b.m_fall_tof_left_value},
                {"fall_tof_right_check",          b.m_fall_tof_right_check},
                {"fall_tof_right_value",          b.m_fall_tof_right_value},
                {"welt_tof_after_check",          b.m_welt_tof_after_check},
                {"welt_tof_after_value",          b.m_welt_tof_after_value},
                {"welt_tof_front_check",          b.m_welt_tof_front_check},
                {"welt_tof_front_value",          b.m_welt_tof_front_value},
                {"supersonic_front_left_check",   b.m_supersonic_front_left_check},
                {"supersonic_front_left_value",   b.m_supersonic_front_left_value},
                {"supersonic_front_right_check",  b.m_supersonic_front_right_check},
                {"supersonic_front_right_value",  b.m_supersonic_front_right_value},
                {"supersonic_corner_left_check",  b.m_supersonic_corner_left_check},
                {"supersonic_corner_left_value",  b.m_supersonic_corner_left_value},
                {"supersonic_corner_right_check", b.m_supersonic_corner_right_check},
                {"supersonic_corner_right_value", b.m_supersonic_corner_right_value},
                {"supersonic_left_check",         b.m_supersonic_left_check},
                {"supersonic_left_value",         b.m_supersonic_left_value},
                {"supersonic_right_check",        b.m_supersonic_right_check},
                {"supersonic_right_value",        b.m_supersonic_right_value},
                {"camera_check",                  b.m_camera_check},
                {"laser_check",                   b.m_laser_check},
        };
    }

    friend void from_json(const json &j, SelfCheckStatus &b) {
        j.at("fall_tof_front_left_check").get_to(b.m_fall_tof_front_left_check);
        j.at("fall_tof_front_left_value").get_to(b.m_fall_tof_front_left_value);
        j.at("fall_tof_front_right_check").get_to(b.m_fall_tof_front_right_check);
        j.at("fall_tof_front_right_value").get_to(b.m_fall_tof_front_right_value);
        j.at("fall_tof_left_check").get_to(b.m_fall_tof_left_check);
        j.at("fall_tof_left_value").get_to(b.m_fall_tof_left_value);
        j.at("fall_tof_right_check").get_to(b.m_fall_tof_right_check);
        j.at("fall_tof_right_value").get_to(b.m_fall_tof_right_value);
        j.at("welt_tof_after_check").get_to(b.m_welt_tof_after_check);
        j.at("welt_tof_after_value").get_to(b.m_welt_tof_after_value);
        j.at("welt_tof_front_check").get_to(b.m_welt_tof_front_check);
        j.at("welt_tof_front_value").get_to(b.m_welt_tof_front_value);
        j.at("supersonic_front_left_check").get_to(b.m_supersonic_front_left_check);
        j.at("supersonic_front_left_value").get_to(b.m_supersonic_front_left_value);
        j.at("supersonic_front_right_check").get_to(b.m_supersonic_front_right_check);
        j.at("supersonic_front_right_value").get_to(b.m_supersonic_front_right_value);
        j.at("supersonic_corner_left_check").get_to(b.m_supersonic_corner_left_check);
        j.at("supersonic_corner_left_value").get_to(b.m_supersonic_corner_left_value);
        j.at("supersonic_corner_right_check").get_to(b.m_supersonic_corner_right_check);
        j.at("supersonic_corner_right_value").get_to(b.m_supersonic_corner_right_value);
        j.at("supersonic_left_check").get_to(b.m_supersonic_left_check);
        j.at("supersonic_left_value").get_to(b.m_supersonic_left_value);
        j.at("supersonic_right_check").get_to(b.m_supersonic_right_check);
        j.at("supersonic_right_value").get_to(b.m_supersonic_right_value);
        j.at("camera_check").get_to(b.m_camera_check);
        j.at("laser_check").get_to(b.m_laser_check);
    }
};

#endif //APP_COMMUNICATION_SELFCHECKSTATUS_H
