//
// Created by lijiang on 2022/1/10.
//

#include "model/TeachModePoint.h"
#include "manager/ViewPartManager.h"
#include "model/ViewPart.h"
#include "tool/Variable.h"
#include "tool/write_file.hpp"
#include <tinyxml.h>

bool is_in_line(const geometry_msgs::Pose point1, const geometry_msgs::Pose point2, const geometry_msgs::Pose point3) {

    if (abs(point2.orientation.w - point1.orientation.w) > 0.1) {
        return false;
    }


    float x1, y1, x2, y2, x, y;
    x1 = point1.position.x;
    y1 = point1.position.y;
    x2 = point2.position.x;
    y2 = point2.position.y;
    x = point3.position.x;
    y = point3.position.y;
    float judge_num = (y1 - y2) * (x - x2) - (x1 - x2) * (y - y2);
    // cout<<"judge_num: "<<judge_num<<endl;
    if (abs(judge_num) < 5 * 1e-3) {
        return true;
    } else
        return false;

}

void TeachModeProcess() {
    bool is_saved = false;

    std::vector<geometry_msgs::Pose> Points;
    geometry_msgs::Pose pose_now, pose_last, pose_last_last;
    //    pose_now = get_current_pose();
    //    pose_last = pose_now;
    //    pose_last_last = pose_now;
    TeachModePoint::get_instance()->setExcuteMode(CANCLE_SAVE);//初始模式
    ros::Rate loop(10.0);
    unsigned int count = 0;//分频使用
    unsigned int count_heartbeat = 0;
    while (ros::ok()) {
        int point_count = 0;
        while (TeachModePoint::get_instance()->getExcuteMode() == ON_SAVING) {
            ros::spinOnce();

            pose_now = Variable::get_instance()->getCurrentPose();
            if (point_count == 0) {
                Points.push_back(pose_now);
                point_count++;
                pose_last = pose_now;
            }
            if (abs(pose_now.position.x - pose_last.position.x) > 0.05 ||
                abs(pose_now.position.y - pose_last.position.y) > 0.05) {
                if (point_count > 1) {
                    if (is_in_line(pose_now, pose_last, pose_last_last)) {
                        cout << "point1 " << pose_last_last.position.x << " , " << pose_last_last.position.y << " and "
                             << "point2 " << pose_last.position.x << " , " << pose_last.position.y << " and "
                             << "point3 " << pose_now.position.x << " , " << pose_now.position.y << "共线" << endl;
                        Points.pop_back();
                    }
                }
                Points.push_back(pose_now);//这一轮判断结束，给上个点以及上上个点赋值
                if (point_count > 0) {
                    if (point_count > 1) {
                        pose_last_last = pose_last;
                    }
                    pose_last = pose_now;
                }
                point_count++;
            }
            is_saved = false;

            if (TeachModePoint::get_instance()->isGetHeartBeat()) {
                count_heartbeat = 0;
                TeachModePoint::get_instance()->setGetHeartBeat(false);
            }
            //            cout<<"count_heartbeat: "<<count_heartbeat<<endl;
            if (count_heartbeat > 10 * 5)//3s内没收到心跳包退出记录并取消保存/心跳包是1hz
            {
                TeachModePoint::get_instance()->setExcuteMode(CANCLE_SAVE);
                ROS_ERROR("time out to get heart beat, close teach mode！");
            }
            count_heartbeat++;
            count++;
            if (count > 400000000) count = 0;
            loop.sleep();
        }
        if (TeachModePoint::get_instance()->getExcuteMode() == START_SAVE) {
            if (!is_saved) {
                //追加保存路径
                std::vector<WayPointTask> wayPoints_before;
                //save改
                //读文件
                string fileName;
                string sss;
                fileName.append(path::data_base_config_dir());
                fileName.append("teach_point_json.txt");
                std::shared_ptr<sh::File> fff = make_shared<sh::File>(fileName);
                if (fff->open(std::ios::in)) {
                    sss = fff->readAll();
                } else {
                    cout << "fail to open file" << endl;

                    return;
                }

                std_msgs::String result;
                if (sss.length() > 0)//文件不为空
                {
                    json jdecode = json::parse(sss);
                    wayPoints_before = jdecode.get<std::vector<WayPointTask>>();//数据内容，结构体格式
                }

                //结构体转json
                std::vector<WayPointTask> wayPointsTask;//最终存储文件
                WayPointTask wayPoints_now;             //本次添加的任务
                std::vector<WayPoint> wayPointsNow;     //本次添加的点列
                WayPoint temp_point;                    //单个路径点

                for (int i = 0; i < Points.size(); i++) {
                    temp_point.setPointName("point_" + to_string((i + 1)));
                    temp_point.setPosX(Points[i].position.x);
                    temp_point.setPosY(Points[i].position.y);
                    temp_point.setPosZ(Points[i].position.z);
                    temp_point.setOriX(Points[i].orientation.x);
                    temp_point.setOriY(Points[i].orientation.y);
                    temp_point.setOriZ(Points[i].orientation.z);
                    temp_point.setOriW(Points[i].orientation.w);
                    wayPointsNow.push_back(temp_point);
                }
                wayPoints_now.setWayPointTask(wayPointsNow);
                wayPoints_now.setTaskId(TeachModePoint::get_instance()->getTeachPathId());
                wayPoints_now.setTaskName(TeachModePoint::get_instance()->getTeachPathName());
                for (const auto &item: wayPoints_before) {
                    wayPointsTask.push_back(item);
                }
                wayPointsTask.push_back(wayPoints_now);

                json params = wayPointsTask;
                string base64;
                base64.append(params.dump());//json转base64 string

                if (!sh::File::saveTextTo(fileName, base64)) {
                    ROS_ERROR("fail to save teach_point_json.txt");
                } else {//将该路径存入ViewPartList
                    ViewPart view_part_temp;
                    view_part_temp.setMode(3);
                    view_part_temp.setName(wayPoints_now.getTaskName());
                    view_part_temp.setPartID(wayPoints_now.getTaskId());
                    view_part_temp.setPathID(wayPoints_now.getTaskId());
                    vector<Point> teach_point_list_temp;
                    Point point_temp;
                    for (auto &item: wayPointsNow) {
                        point_temp.setXandY(item.getPosX(), item.getPosY());
                        teach_point_list_temp.push_back(point_temp);
                    }
                    view_part_temp.setTeach(teach_point_list_temp);
                    switch (ViewPartManager::get_instance()->AddViewPart(view_part_temp)) {//添加ViewPart
                        case FAILD_TO_OPEN_FILE_:
                            ROS_ERROR("fail to open view_part.txt");
                            break;
                        case EXECUTE_FAILED_:
                            ROS_ERROR("fail to add teach_view_part");
                            break;
                        case FAILED_TO_SAVE_FILE_:
                            ROS_ERROR("fail to save view_part.txt");
                            break;
                        case SUCCESS_: {
                            //回复，标准
                            ROS_INFO("success to save teach_view_part");
                            break;
                        }
                        default:
                            ROS_ERROR("unknow error");
                            break;
                    }
                }
                is_saved = true;
                point_count = 0;//清零已记录的点的数量
                while (!Points.empty()) {
                    Points.pop_back();
                }
            }

        } else if (TeachModePoint::get_instance()->getExcuteMode() == CANCLE_SAVE) {//不保存，直接清空
            is_saved = true;
            count_heartbeat = 0;
            count = 0;
            while (!Points.empty()) {
                Points.pop_back();
            }
        }
        loop.sleep();
    }
}


string float2str(float inVal) {
    ostringstream stringStream;
    stringStream << inVal;
    string resStr = stringStream.str();
    return resStr;
}


void TeachModePoint::open() {
    std::thread TeachModeProcessThread(TeachModeProcess);
    TeachModeProcessThread.detach();
}

bool TeachModePoint::isGetHeartBeat() const {
    return GetHeartBeat;
}

void TeachModePoint::setGetHeartBeat(bool getHeartBeat) {
    GetHeartBeat = getHeartBeat;
}

const string &TeachModePoint::getTeachPathId() const {
    return teach_path_id;
}

void TeachModePoint::setTeachPathId(const string &teachPathId) {
    teach_path_id = teachPathId;
}

const string &TeachModePoint::getTeachPathName() const {
    return teach_path_name;
}

void TeachModePoint::setTeachPathName(const string &teachPathName) {
    teach_path_name = teachPathName;
}

int TeachModePoint::getExcuteMode() const {
    return excute_mode;
}

void TeachModePoint::setExcuteMode(int excuteMode) {
    excute_mode = excuteMode;
}


geometry_msgs::Pose get_current_pose() {

    //    tf::StampedTransform stamped_transform;
    //    geometry_msgs::Pose current_pose;
    //    tf::TransformListener tf_listener;
    //    try {
    //        tf_listener.waitForTransform("map", "base_link", ros::Time(0), ros::Duration(0.5));
    //        tf_listener.lookupTransform("map", "base_link", ros::Time(0), stamped_transform);
    //        current_pose.position.x = stamped_transform.getOrigin().getX();
    //        current_pose.position.y = stamped_transform.getOrigin().getY();
    //        current_pose.position.z = stamped_transform.getOrigin().getZ();
    //        current_pose.orientation.x = stamped_transform.getRotation().getX();
    //        current_pose.orientation.y = stamped_transform.getRotation().getY();
    //        current_pose.orientation.z = stamped_transform.getRotation().getZ();
    //        current_pose.orientation.w = stamped_transform.getRotation().getW();
    ////        Variable::get_instance()->setCurrentPose(current_pose);
    //        return current_pose;
    //    }
    //    catch (tf::TransformException &ex) {
    //        ROS_ERROR("%s", ex.what());
    //        ros::Duration(1.0).sleep();
    //        return current_pose;
    //    }

    return Variable::get_instance()->getCurrentPose();
}

WayPoint::WayPoint() {}

WayPoint::~WayPoint() {
}

const string &WayPoint::getPointName() const {
    return point_name;
}

void WayPoint::setPointName(const string &pointName) {
    point_name = pointName;
}

float WayPoint::getPosX() const {
    return pos_x;
}

void WayPoint::setPosX(float posX) {
    pos_x = posX;
}

float WayPoint::getPosY() const {
    return pos_y;
}

void WayPoint::setPosY(float posY) {
    pos_y = posY;
}

float WayPoint::getPosZ() const {
    return pos_z;
}

void WayPoint::setPosZ(float posZ) {
    pos_z = posZ;
}

float WayPoint::getOriX() const {
    return ori_x;
}

void WayPoint::setOriX(float oriX) {
    ori_x = oriX;
}

float WayPoint::getOriY() const {
    return ori_y;
}

void WayPoint::setOriY(float oriY) {
    ori_y = oriY;
}

float WayPoint::getOriZ() const {
    return ori_z;
}

void WayPoint::setOriZ(float oriZ) {
    ori_z = oriZ;
}

float WayPoint::getOriW() const {
    return ori_w;
}

void WayPoint::setOriW(float oriW) {
    ori_w = oriW;
}

WayPointTask::WayPointTask() {}

WayPointTask::~WayPointTask() {
}

const string &WayPointTask::getTaskId() const {
    return task_id;
}

void WayPointTask::setTaskId(const string &taskId) {
    task_id = taskId;
}

const vector<WayPoint> &WayPointTask::getWayPointList() const {
    return wayPointTask;
}

void WayPointTask::setWayPointTask(const vector<WayPoint> &wayPointTask) {
    WayPointTask::wayPointTask = wayPointTask;
}

const string &WayPointTask::getTaskName() const {
    return task_name;
}

void WayPointTask::setTaskName(const string &taskName) {
    task_name = taskName;
}

ClossTeachMode::ClossTeachMode() {}

ClossTeachMode::~ClossTeachMode() {
}

bool ClossTeachMode::isForceQuit() const {
    return force_quit;
}

void ClossTeachMode::setForceQuit(bool forceQuit) {
    force_quit = forceQuit;
}

const string &ClossTeachMode::getTeachPathId() const {
    return teach_path_id;
}

void ClossTeachMode::setTeachPathId(const string &teachPathId) {
    teach_path_id = teachPathId;
}

const string &ClossTeachMode::getTeachPathName() const {
    return teach_path_name;
}

void ClossTeachMode::setTeachPathName(const string &teachPathName) {
    teach_path_name = teachPathName;
}

ResponseGetTeachTask::ResponseGetTeachTask() {}

ResponseGetTeachTask::~ResponseGetTeachTask() {
}

const string &ResponseGetTeachTask::getTeachPathId() const {
    return teach_path_id;
}

void ResponseGetTeachTask::setTeachPathId(const string &teachPathId) {
    teach_path_id = teachPathId;
}

const string &ResponseGetTeachTask::getTeachPathName() const {
    return teach_path_name;
}

void ResponseGetTeachTask::setTeachPathName(const string &teachPathName) {
    teach_path_name = teachPathName;
}

TeachPathInfo::TeachPathInfo() {}

TeachPathInfo::TeachPathInfo(const string &teachPathId, const vector<Point> &teachPointList) : teach_path_id(
        teachPathId),
                                                                                               teach_point_list(
                                                                                                       teachPointList) {}

TeachPathInfo::~TeachPathInfo() {
}

const string &TeachPathInfo::getTeachPathId() const {
    return teach_path_id;
}

void TeachPathInfo::setTeachPathId(const string &teachPathId) {
    teach_path_id = teachPathId;
}

const vector<Point> &TeachPathInfo::getTeachPointList() {
    return teach_point_list;
}

void TeachPathInfo::setTeachPointList(const vector<Point> &teachPointList) {
    teach_point_list = teachPointList;
}
