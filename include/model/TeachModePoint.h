//
// Created by lijiang on 2022/1/10.
//

#ifndef APP_COMMUNICATION_TEACHMODEPOINT_H
#define APP_COMMUNICATION_TEACHMODEPOINT_H

#include "Point.h"
#include "vector"
#include <geometry_msgs/Pose.h>
#include <tf/transform_listener.h>
#include <thread>

#include "nlohmann/json.hpp"
#include <iostream>

using json = nlohmann::json;
using namespace std;

const int ON_SAVING = 1;
const int START_SAVE = 0;
const int CANCLE_SAVE = -1;

class WayPoint {//单个示教点
private:
    string point_name;
    float pos_x;
    float pos_y;
    float pos_z;
    float ori_x;
    float ori_y;
    float ori_z;
    float ori_w;

public:
    friend void to_json(json &j, const WayPoint &b) {
        j = json{
                {"point_name", b.point_name},
                {"pos_x",      b.pos_x},
                {"pos_y",      b.pos_y},
                {"pos_z",      b.pos_z},
                {"ori_x",      b.ori_x},
                {"ori_y",      b.ori_y},
                {"ori_z",      b.ori_z},
                {"ori_w",      b.ori_w}};
    }

    friend void from_json(const json &j, WayPoint &b) {
        j.at("point_name").get_to(b.point_name);
        j.at("pos_x").get_to(b.pos_x);
        j.at("pos_y").get_to(b.pos_y);
        j.at("pos_z").get_to(b.pos_z);
        j.at("ori_x").get_to(b.ori_x);
        j.at("ori_y").get_to(b.ori_y);
        j.at("ori_z").get_to(b.ori_z);
        j.at("ori_w").get_to(b.ori_w);
    }


    WayPoint();

    virtual ~WayPoint();

    const string &getPointName() const;

    void setPointName(const string &pointName);

    float getPosX() const;

    void setPosX(float posX);

    float getPosY() const;

    void setPosY(float posY);

    float getPosZ() const;

    void setPosZ(float posZ);

    float getOriX() const;

    void setOriX(float oriX);

    float getOriY() const;

    void setOriY(float oriY);

    float getOriZ() const;

    void setOriZ(float oriZ);

    float getOriW() const;

    void setOriW(float oriW);
};

class TeachPathInfo {//单个示教任务
    string teach_path_id;
    vector<Point> teach_point_list;

public:
    friend void to_json(json &j, const TeachPathInfo &b) {
        j = json{
                {"teach_path_id",    b.teach_path_id},
                {"teach_point_list", b.teach_point_list}};
    }

    friend void from_json(const json &j, TeachPathInfo &b) {
        j.at("teach_path_id").get_to(b.teach_path_id);
        j.at("teach_point_list").get_to(b.teach_point_list);
    }

    TeachPathInfo();

    TeachPathInfo(const string &teachPathId, const vector<Point> &teachPointList);

    virtual ~TeachPathInfo();

    const string &getTeachPathId() const;

    void setTeachPathId(const string &teachPathId);

    const vector<Point> &getTeachPointList();

    void setTeachPointList(const vector<Point> &teachPointList);
};

class WayPointTask {//单个示教任务
    string task_id;
    string task_name;
    std::vector<WayPoint> wayPointTask;

public:
    friend void to_json(json &j, const WayPointTask &b) {
        j = json{
                {"task_id",      b.task_id},
                {"task_name",    b.task_name},
                {"wayPointTask", b.wayPointTask}};
    }

    friend void from_json(const json &j, WayPointTask &b) {
        j.at("task_id").get_to(b.task_id);
        j.at("task_name").get_to(b.task_name);
        j.at("wayPointTask").get_to(b.wayPointTask);
    }

    WayPointTask();

    virtual ~WayPointTask();

    const string &getTaskId() const;

    void setTaskId(const string &taskId);

    const vector<WayPoint> &getWayPointList() const;

    void setWayPointTask(const vector<WayPoint> &wayPointTask);

    const string &getTaskName() const;

    void setTaskName(const string &taskName);
};

class ResponseGetTeachTask {
private:
    string teach_path_id;
    string teach_path_name;

public:
    friend void to_json(json &j, const ResponseGetTeachTask &b) {
        j = json{
                {"teach_path_id",   b.teach_path_id},
                {"teach_path_name", b.teach_path_name}};
    }

    friend void from_json(const json &j, ResponseGetTeachTask &b) {
        j.at("teach_path_id").get_to(b.teach_path_id);
        j.at("teach_path_name").get_to(b.teach_path_name);
    }

    ResponseGetTeachTask();

    virtual ~ResponseGetTeachTask();

    const string &getTeachPathId() const;

    void setTeachPathId(const string &teachPathId);

    const string &getTeachPathName() const;

    void setTeachPathName(const string &teachPathName);
};

class ClossTeachMode {//用于处理结束示教命令
    bool force_quit;
    string teach_path_id;
    string teach_path_name;

public:
    friend void to_json(json &j, const ClossTeachMode &b) {
        j = json{
                {"force_quit",      b.force_quit},
                {"teach_path_id",   b.teach_path_id},
                {"teach_path_name", b.teach_path_name}};
    }

    friend void from_json(const json &j, ClossTeachMode &b) {
        j.at("force_quit").get_to(b.force_quit);
        j.at("teach_path_id").get_to(b.teach_path_id);
        j.at("teach_path_name").get_to(b.teach_path_name);
    }


    ClossTeachMode();

    virtual ~ClossTeachMode();

    bool isForceQuit() const;

    void setForceQuit(bool forceQuit);

    const string &getTeachPathId() const;

    void setTeachPathId(const string &teachPathId);

    const string &getTeachPathName() const;

    void setTeachPathName(const string &teachPathName);
};

class TeachModePoint {//示教执行
private:
    int excute_mode;
    bool isStarted;
    bool GetHeartBeat;
    string teach_path_id;
    string teach_path_name;

    std::vector<WayPoint> Waypoints;

public:
    int getExcuteMode() const;

    void setExcuteMode(int excuteMode);

    bool isGetHeartBeat() const;

    void setGetHeartBeat(bool getHeartBeat);

    bool isStarted1() const;

    void setIsStarted(bool isStarted);

    static void open();

    const string &getTeachPathId() const;

    void setTeachPathId(const string &teachPathId);

    const string &getTeachPathName() const;

    void setTeachPathName(const string &teachPathName);


private:
    static TeachModePoint *m_instance_ptr;


public:
    ~TeachModePoint() = default;

    static TeachModePoint *get_instance() {
        if (m_instance_ptr == nullptr) {
            m_instance_ptr = new TeachModePoint;
        }
        return m_instance_ptr;
    }
};


string float2str(float x);


#endif//APP_COMMUNICATION_TEACHMODEPOINT_H
