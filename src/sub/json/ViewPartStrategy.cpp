/**
 * @file ViewPartStrategy.cpp
 * @author Quan Li (you@domain.com)
 * @brief 路径组合单个零件相关操作
 * @version 0.1
 * @date 2022-03-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "sub/json/ViewPartStrategy.h"
#include "manager/CombinationManager.h"
#include "manager/ViewPartManager.h"
#include "model/TeachModePoint.h"
#include "model/ViewPart.h"
#include "tool/write_file.hpp"

string CombinationPartAddStrategy::handler(ViewPart method) {
    switch (ViewPartManager::get_instance()->AddViewPart(method)) {//添加ViewPart
        case FAILD_TO_OPEN_FILE_:
            throw app::exception(make_error_code(error::open_file_fail));
        case NAME_DUPLICATE_:
            throw app::exception(make_error_code(error::duplicate_path_file_name));
        case EXECUTE_FAILED_:
            throw app::exception(make_error_code(error::add_failed));
        case FAILED_TO_SAVE_FILE_:
            throw app::exception(make_error_code(error::save_failed));
        case SUCCESS_:
            return "";
        default:
            throw app::exception(make_error_code(error::general));
    }
}

vector<ViewPart> CombinationPartListStrategy::handler(string params) {
    ViewPartList view_part_list_temp;
    if (!ViewPartManager::get_instance()->GetViewPartList(view_part_list_temp)) {
        throw app::exception(make_error_code(error::acquisition_file_failed));
    }
    if (view_part_list_temp.GetViewPartList().size() > 0) {
        return view_part_list_temp.GetViewPartList();
    } else {
        return vector<ViewPart>();
    }
}

vector<ViewPartUpgrade> CombinationPartListStrategyV2::handler(string params) {
    std_msgs::String result;
    ViewPartList view_part_list_temp;
    if (!ViewPartManager::get_instance()->GetViewPartList(view_part_list_temp)) {
        throw app::exception(make_error_code(error::acquisition_file_failed));
    }
    if (view_part_list_temp.GetViewPartList().size() > 0) {
        vector<ViewPartUpgrade> list;
        for (const auto &item: view_part_list_temp.GetViewPartList()) {
            WorkStatusUpgrade ws;
            ws.setSweepStatus(item.getWorkStatus().getSweepStatus());
            ws.setDragStatus(item.getWorkStatus().getMopStatus());
            ws.setAbsorbStatus(item.getWorkStatus().getVacuumStatus());
            ws.setPushStatus(item.getWorkStatus().getPushStatus());
            auto viewPart = ViewPartUpgrade();
            viewPart.setMode(item.getMode());
            viewPart.setName(item.getName());
            viewPart.setPartId(item.getPartID());
            viewPart.setPolygon(item.getPolygon());
            viewPart.setRate(item.getRate());
            viewPart.setTeach(item.getTeach());
            viewPart.setPathId(item.getPathID());
            viewPart.setWorkStatus(ws);
            viewPart.setZoned(item.getZoned());
            list.push_back(viewPart);
        }
        return list;
    } else {
        return vector<ViewPartUpgrade>();
    }
}

string CombinationPartDeleteStrategy::handler(string params) {
    if (CombinationManager::get_instance()->SearchPartID(params) == SUCCESS_) {
        throw app::exception(make_error_code(error::file_acquisition_failed));
    }
    switch (ViewPartManager::get_instance()->DelateViewPart(params)) {
        case FAILD_TO_OPEN_FILE_:
            throw app::exception(make_error_code(error::open_file_fail));
        case EXECUTE_FAILED_:
            throw app::exception(make_error_code(error::add_failed));
        case FAILED_TO_SAVE_FILE_:
            throw app::exception(make_error_code(error::save_failed));
        case SUCCESS_: {
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
                throw app::exception(make_error_code(error::failed_to_open_teaching_file));
            }

            std_msgs::String result;
            if (sss.length() > 0)//文件不为空
            {
                json jdecode = json::parse(sss);
                wayPoints_before = jdecode.get<std::vector<WayPointTask>>();//数据内容，结构体格式
            }
            //结构体转json
            vector<WayPointTask>::iterator iter;
            for (iter = wayPoints_before.begin(); iter != wayPoints_before.end(); iter++) {
                if (iter->getTaskId() == params) {
                    wayPoints_before.erase(iter);
                    break;
                }
            }
            json params = wayPoints_before;
            string base64;
            base64.append(params.dump());//json转base64 string

            if (!sh::File::saveTextTo(fileName, base64)) {
                ROS_ERROR("fail to save teach_point_json.txt");
            }
            return "";
        }
        default:
            throw app::exception(make_error_code(error::general));
    }
}

string CombinationPartDeleteForceStrategy::handler(string params) {
    switch (CombinationManager::get_instance()->DelatePartID(params)) {
        case FAILD_TO_OPEN_FILE_:
            throw app::exception(make_error_code(error::open_file_fail));
        case EXECUTE_FAILED_:
            throw app::exception(make_error_code(error::add_failed));
        case FAILED_TO_SAVE_FILE_:
            throw app::exception(make_error_code(error::save_failed));
        case SUCCESS_: {
            switch (ViewPartManager::get_instance()->DelateViewPart(params)) {
                case FAILD_TO_OPEN_FILE_:
                    throw app::exception(make_error_code(error::open_file_fail));
                case EXECUTE_FAILED_:
                    throw app::exception(make_error_code(error::add_failed));
                case FAILED_TO_SAVE_FILE_:
                    throw app::exception(make_error_code(error::save_failed));
                case SUCCESS_: {
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
                        throw app::exception(make_error_code(error::failed_to_open_teaching_file));
                    }

                    std_msgs::String result;
                    if (sss.length() > 0)//文件不为空
                    {
                        json jdecode = json::parse(sss);
                        wayPoints_before = jdecode.get<std::vector<WayPointTask>>();//数据内容，结构体格式
                    }
                    //结构体转json
                    vector<WayPointTask>::iterator iter;
                    for (iter = wayPoints_before.begin(); iter != wayPoints_before.end(); iter++) {
                        if (iter->getTaskId() == params) {
                            wayPoints_before.erase(iter);
                            break;
                        }
                    }
                    json params = wayPoints_before;
                    string base64;
                    base64.append(params.dump());//json转base64 string

                    if (!sh::File::saveTextTo(fileName, base64)) {
                        ROS_ERROR("fail to save teach_point_json.txt");
                    }
                    return "";
                }
                default:
                    throw app::exception(make_error_code(error::general));
            }
            break;
        }
        default:
            throw app::exception(make_error_code(error::general));
    }
}

string CombinationPartUpdateStrategy::handler(ViewPart params) {
    switch (ViewPartManager::get_instance()->ResetViewPart(params, params.getPartID())) {
        case FAILD_TO_OPEN_FILE_:
            throw app::exception(make_error_code(error::open_file_fail));
        case EXECUTE_FAILED_:
            throw app::exception(make_error_code(error::add_failed));
        case FAILED_TO_SAVE_FILE_:
            throw app::exception(make_error_code(error::save_failed));
        case SUCCESS_: {
            return "";
        }
        default:
            throw app::exception(make_error_code(error::general));
    }
}
