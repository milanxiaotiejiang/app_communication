//
// Created by lijiang on 2021/12/23.
//

#ifndef APP_COMMUNICATION_VARIABLE_H
#define APP_COMMUNICATION_VARIABLE_H

#include "nav_msgs/Path.h"
#include <nav_msgs/OccupancyGrid.h>


class Variable {
private:
    nav_msgs::OccupancyGrid map_App;
    geometry_msgs::Pose current_pose;
    bool m_base_exist;   //是否存在基站
    bool m_face_recognition;   //是否存在基站
    bool m_knob;   //是否存在基站
    bool m_voice;   //是否存在基站

public:
    const nav_msgs::OccupancyGrid &getMapApp() const;

    void setMapApp(const nav_msgs::OccupancyGrid &mapApp);

    const geometry_msgs::Pose &getCurrentPose() const;

    void setCurrentPose(const geometry_msgs::Pose &currentPose);

    bool getBaseExist() const {
        return m_base_exist;
    }

    void setBaseExist(bool base_exist) {
        m_base_exist = base_exist;
    }

    bool getface_recognition() const {
        return m_face_recognition;
    }

    void setface_recognition(bool face_recognition) {
        m_face_recognition = face_recognition;
    }

    bool getknob() const {
        return m_knob;
    }

    void setknob(bool knob) {
        m_knob = knob;
    }

    bool getvoice() const {
        return m_voice;
    }

    void setvoice(bool voice) {
        m_voice = voice;
    }

private:
    static Variable *m_instance_ptr;

public:
    ~Variable() = default;

    static Variable *get_instance() {
        if (m_instance_ptr == nullptr) {
            m_instance_ptr = new Variable;
        }
        return m_instance_ptr;
    }
};


#endif//APP_COMMUNICATION_VARIABLE_H
