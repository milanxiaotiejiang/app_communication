//
// Created by lijiang on 2022/1/10.
//

#include "sub/json/TeachModeStrategy.h"

string StartTeachModeStrategy::handler(string params) {
    TeachModePoint::get_instance()->setExcuteMode(ON_SAVING);
    return "";
}

string StopTeachModeStrategy::handler(ClossTeachMode params) {
    string new_name = params.getTeachPathName();
    if (!params.isForceQuit())//如果并非强制退出
    {
        bool can_save = true;//判断是否能保存
        string fileName;
        string sss;
        fileName.append(ros::package::getPath("data_base"));
        fileName.append("/config/teach_point_json.txt");
        std::shared_ptr<sh::File> fff = make_shared<sh::File>(fileName);
        if (fff->open(std::ios::in)) {//打开成功
            sss = fff->readAll();
            if (sss.length() > 0)//文件不为空
            {
                json jdecode = json::parse(sss);
                std::vector<WayPointTask> param = jdecode.get<std::vector<WayPointTask >>();//数据内容，结构体格式
                for (auto &item: param) {
                    if (new_name == item.getTaskName()) {
                        throw app::exception(make_error_code(error::duplicate_file_name));
                    }
                }
            }
            ViewPartList view_part_list_temp;
            if (!ViewPartManager::get_instance()->GetViewPartList(view_part_list_temp)) {
                throw app::exception(make_error_code(error::failed_to_obtain_the_path_file));
            }
            if (view_part_list_temp.GetViewPartList().size() > 0) {
                for (auto &item: view_part_list_temp.GetViewPartList()) {
                    if (new_name == item.getName()) {
                        can_save = false;
                        throw app::exception(make_error_code(error::failed_same_name_as_the_combined_path));
                    }
                }
            }
            if (can_save) {
                TeachModePoint::get_instance()->setTeachPathName(params.getTeachPathName());
                TeachModePoint::get_instance()->setTeachPathId(params.getTeachPathId());
                TeachModePoint::get_instance()->setExcuteMode(START_SAVE);
            }
        } else {
            throw app::exception(make_error_code(error::failed_to_open_teaching_file));
        }


    } else {
        TeachModePoint::get_instance()->setExcuteMode(CANCLE_SAVE);//强制退出，不保存路径
    }

    return "";
}

string HeartBeatofTeachModeStrategy::handler(string params) {
    TeachModePoint::get_instance()->setGetHeartBeat(true);
    return "";
}

vector<ResponseGetTeachTask> GetTeachModeListStrategy::handler(string params) {
    string fileName;
    string sss;
    fileName.append(ros::package::getPath("data_base"));
    fileName.append("/config/teach_point_json.txt");
    std::shared_ptr<sh::File> fff = make_shared<sh::File>(fileName);
    if (fff->open(std::ios::in)) {
        sss = fff->readAll();
    } else {
        throw app::exception(make_error_code(error::open_file_fail));
    }

    std_msgs::String result;
    if (sss.length() > 0)//文件不为空
    {
        json jdecode = json::parse(sss);

        std::vector<WayPointTask> param = jdecode.get<std::vector<WayPointTask>>();//数据内容，结构体格式
        //回复
        std::vector<ResponseGetTeachTask> response;
        for (int i = 0; i < param.size(); i++) {
            ResponseGetTeachTask temp_task_info;
            temp_task_info.setTeachPathId(param[i].getTaskId());
            temp_task_info.setTeachPathName(param[i].getTaskName());
            response.push_back(temp_task_info);
        }
        return response;
    } else {
        return std::vector<ResponseGetTeachTask>();
    }
}

vector<Point> GetTeachModeDetialStrategy::handler(string params) {
    string fileName;
    string sss;
    fileName.append(ros::package::getPath("data_base"));
    fileName.append("/config/teach_point_json.txt");
    std::shared_ptr<sh::File> fff = make_shared<sh::File>(fileName);
    if (fff->open(std::ios::in)) {
        sss = fff->readAll();
    } else {
        throw app::exception(make_error_code(error::open_file_fail));
    }

    std_msgs::String result;
    if (sss.length() > 0)//文件不为空
    {
        json jdecode = json::parse(sss);

        std::vector<WayPointTask> param = jdecode.get<std::vector<WayPointTask>>();//数据内容，结构体格式
        //回复
        std::vector<Point> response;
        for (int i = 0; i < param.size(); i++) {
            if (param[i].getTaskId() == params) {
                Point temp_point;
                for (const auto &item: param[i].getWayPointList()) {
                    temp_point.setXandY(item.getPosX(), item.getPosY());
                    response.push_back(temp_point);
                }
            }
        }
        return response;
    } else {
        return std::vector<Point>();
    }
}

bool DeleteTeachModeStrategy::is_exist(const string &str, vector<string> str_list) {
    for (const auto &item: str_list) {
        if (str == item) return true;
    }
    return false;
}

string DeleteTeachModeStrategy::handler(vector<string> params) {
    for (auto &item: params) {
        switch (CombinationManager::get_instance()->DelatePartID(item)) {
            case FAILD_TO_OPEN_FILE_:
                ROS_INFO("打开组合路径文件失败");
                break;
            case EXECUTE_FAILED_:
                ROS_INFO("删除组合路径文件中的partid失败");
                break;
            case FAILED_TO_SAVE_FILE_:
                ROS_INFO("保存组合路径失败");
                break;
            case SUCCESS_: {
                switch (ViewPartManager::get_instance()->DelateViewPart(item)) {
                    case FAILD_TO_OPEN_FILE_:
                        ROS_INFO("打开ViewPartList文件失败");
                        break;
                    case EXECUTE_FAILED_:
                        ROS_INFO("删除ViewPart失败");
                        break;
                    case FAILED_TO_SAVE_FILE_:
                        ROS_INFO("保存ViewPartList文件失败");
                        break;
                    case SUCCESS_: {
                        //回复，标准
                        ROS_INFO("删除ViewPart成功");
                        break;
                    }
                    default:
                        ROS_INFO("未知错误");
                        break;
                }
                break;
            }
            default:
                ROS_INFO("未知错误");
                break;
        }
    }
    std::vector<WayPointTask> wayPoints_before;
    //save改
    //读文件
    string fileName;
    string sss;
    fileName.append(ros::package::getPath("data_base"));
    fileName.append("/config/teach_point_json.txt");
    std::shared_ptr<sh::File> fff = make_shared<sh::File>(fileName);
    if (fff->open(std::ios::in)) {
        sss = fff->readAll();
    } else {
        throw app::exception(make_error_code(error::open_file_fail));
    }

    std_msgs::String result;
    if (sss.length() > 0)//文件不为空
    {
        json jdecode = json::parse(sss);
        wayPoints_before = jdecode.get<std::vector<WayPointTask>>();//数据内容，结构体格式
    }

    //结构体转json
    std::vector<WayPointTask> wayPointsTask;//最终存储文件
    for (const auto &item: wayPoints_before) {
        if (!(is_exist(item.getTaskId(), params))) {
            wayPointsTask.push_back(item);
        }
    }
    json wayparams = wayPointsTask;
    string base64;
    base64.append(wayparams.dump());//json转base64 string

    if (!sh::File::saveTextTo(fileName, base64)) {
        ROS_ERROR("fail to save teach_point_json.txt");
    }
    return "";
}
