//
// Created by lijiang on 2021/12/23.
//

#ifndef APP_COMMUNICATION_VARIABLE_H
#define APP_COMMUNICATION_VARIABLE_H

#include "model/TeachModePoint.h"
#include "nav_msgs/Path.h"
#include <nav_msgs/OccupancyGrid.h>


class Variable {
private:
    nav_msgs::OccupancyGrid map_App;
    nav_msgs::OccupancyGrid grid_map_App;
    int planner_type;
    bool flag_is_start_teach_mode;
    geometry_msgs::Pose current_pose;
    bool scan_flag;
    WayPoint current_point;
    bool m_move_base_recovery_failure;
    bool m_base_exist;   //是否存在基站
    bool m_face_recognition;   //是否存在基站
    bool m_knob;   //是否存在基站
    bool m_voice;   //是否存在基站

public:
    bool
    isFlagIsStartTeachMode() const;

    void setFlagIsStartTeachMode(bool flagIsStartTeachMode);

    const vector<Point> &getFullPath() const;

    void setFullPath(const vector<Point> &fullPath);


    const nav_msgs::OccupancyGrid &getMapApp() const;

    void setMapApp(const nav_msgs::OccupancyGrid &mapApp);

    const nav_msgs::OccupancyGrid &getGridMapApp() const;

    void setGridMapApp(const nav_msgs::OccupancyGrid &mapApp);

    int getPlannerType() const;

    void setPlannerType(int plannerType);

    const geometry_msgs::Pose &getCurrentPose() const;

    void setCurrentPose(const geometry_msgs::Pose &currentPose);

    bool isScanFlag() const;

    void setScanFlag(bool scanFlag);

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

    const bool &getMoveBaseRecoveryFailure() const {
        return m_move_base_recovery_failure;
    }

    void setMoveBaseRecoveryFailure(const bool &move_base_recovery_failure) {
        m_move_base_recovery_failure = move_base_recovery_failure;
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
