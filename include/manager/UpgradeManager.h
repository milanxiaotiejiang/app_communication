//
// Created by Looper on 2023/4/17.
//

#ifndef APP_COMMUNICATION_UPGRADEMANAGER_H
#define APP_COMMUNICATION_UPGRADEMANAGER_H

#include "simulation.h"
#include "db/segmentation_data_base.h"
#include "model/TimerInfo.h"
#include <cppfs/fs.h>
#include <cppfs/FileHandle.h>


class UpgradeManager {
private:
    UpgradeManager() = default;

    UpgradeManager(UpgradeManager &) = delete;

    UpgradeManager &operator=(const UpgradeManager &) = delete;

public:
    ~UpgradeManager() = default;

public:
    static auto &instance() {
        static UpgradeManager obj;
        return obj;
    }

    std::map<std::string, long> taskMaps;

    void upgradeTask() {

        if (ParamManager::instance().getTxtUpgrade()) {
            return;
        }

        MapPo map = SegmentationDataBase::instance().getDbMap();

        const cv::Mat room_map = SegmentationCenter::instance().generateMat();
        int rows = room_map.rows;
        int cols = room_map.cols;

        auto map_origin = MapAttribute::instance().getMapOrigin();

        geometry_msgs::Pose2D robot_position = MapAttribute::instance().getRobotPositionPose();
//        const cv::Point &start_point = poseTransferPoint(robot_position.x, robot_position.y);


        //任务

        CombinationBriefList combination_brief_list_temp;
        auto startegyResult = CombinationManager::get_instance()->GetCombinationBriefList(combination_brief_list_temp);
        if (startegyResult != SUCCESS_) {
            return;
        }
        if (combination_brief_list_temp.getCombinationBriefList().size() == 0) {
            return;
        }
        std::vector<CombinationBrief> briefList = combination_brief_list_temp.getCombinationBriefList();
        for (const auto &item: briefList) {

            if (item.getCombinationType() == 1) {
                TaskVo taskVo(-1, map.id, item.getName(), item.getRate(),
                              SqliteDataBase::ModeToInt(TaskMode::Cover), item.isPrincipal(),
                              false, false, SqliteDataBase::SourceToString(TaskSource::App), "", 0, 0, 0, false);

                taskVo.setWorkStatus(item.getWorkStatus());
                long taskId = TaskDataBase::instance().addTask(map.id, taskVo);
                taskMaps[item.getCombinationID()] = taskId;
//                CombinationManager::get_instance()->DelateCombination(item.getCombinationID());

            } else if (item.getCombinationType() == 0) {
                CombinationBrief combination_brief_temp;
                ViewPartList view_part_list_temp;
                if (ViewPartManager::get_instance()->GetViewPartList(view_part_list_temp) !=
                    SUCCESS_) {//访问ViewPartList成功
                    continue;
                } else if (CombinationManager::get_instance()->GetCombination(combination_brief_temp,
                                                                              item.getCombinationID()) !=
                           SUCCESS_) {//访问当前组合成功
                    continue;
                } else {
                    CombinationDetail combination_detail_temp(combination_brief_temp);
                    std::vector<string> part_not_mached = combination_brief_temp.toDetail(combination_detail_temp,
                                                                                          view_part_list_temp);//用于验证是否有未匹配的iewpart
                    if (part_not_mached.size() > 0) {
                        for (auto &item: part_not_mached) {
                            CombinationManager::get_instance()->DelatePartID(item);
                        }

                    } else {//正常
                        LOG(INFO) << "glog file is " << glog_info_time_pid_string;

                        TaskVo taskVo(-1, map.id, combination_detail_temp.getName(), combination_detail_temp.getRate(),
                                      SqliteDataBase::ModeToInt(TaskMode::Zoned), combination_detail_temp.isPrincipal(),
                                      false, false, SqliteDataBase::SourceToString(TaskSource::App), "", 0, 0, 0,
                                      false);
                        std::vector<ZoneVo> zones;
                        const std::vector<ViewPart> &partList = combination_detail_temp.getViewPartList();
                        for (const auto &part: partList) {
                            std::vector<PointVo> pointVos;
                            std::vector<Point> zoned = part.getZoned();
                            for (const auto z: zoned) {
                                auto x = z.getX();
                                auto y = z.getY();
                                PointVo pointVo(cols - (y - map_origin.x) / map_resolution_from_subscription,
                                                rows - (x - map_origin.y) / map_resolution_from_subscription);
                                pointVos.push_back(pointVo);
                            }
                            ZoneVo zoneVo;
                            zoneVo.setPoints(pointVos);
                            zones.push_back(zoneVo);
                        }
                        taskVo.setZones(zones);
                        taskVo.setWorkStatus(combination_detail_temp.getWorkStatus());
                        long taskId = TaskDataBase::instance().addTask(map.id, taskVo);
                        taskMaps[item.getCombinationID()] = taskId;
//                        CombinationManager::get_instance()->DelateCombination(item.getCombinationID());
                    }
                }
            }

        }

    }

    void upgradeTimer() {

        if (ParamManager::instance().getTxtUpgrade()) {
            return;
        }

        MapPo map = SegmentationDataBase::instance().getDbMap();

        //定时任务

        string fileName;
        string sss;
        fileName.append(path::data_base_config_dir());
        fileName.append("timer_info_json.txt");

        //sh::File *fff = new sh::File(fileName);
        std::shared_ptr<sh::File> fff = std::make_shared<sh::File>(fileName);
        string base64;
        if (fff->open(std::ios::in)) {
            sss = fff->readAll();
        } else {
            return;
        }
        std_msgs::String result;
        std::vector<TimerInfo> timer_info;
        if (sss.length() > 0)//不为空
        {
            json jdecode = json::parse(sss);

//        LOG(WARNING) << "GetTimerListStrategy " << jdecode;
            timer_info = jdecode.get<std::vector<TimerInfo>>();//数据内容，结构体格式
        }

        if (!timer_info.empty()) {
            for (auto &timer: timer_info) {
                if (!timer.getTaskId().empty()) {
                    long taskId = taskMaps[timer.getTaskId()];
                    if (taskId != 0) {
                        TimerVo timerVo(0, timer.getTimerRule(), taskId, timer.getTimerName(),
                                        timer.getTaskName(), timer.getIsExecute(), timer.getRate(), timer.getIsNever(),
                                        timer.getIsSkip(),
                                        timer.getEndYear(), timer.getEndMonth(), timer.getEndDay());
                        TaskDataBase::instance().addTimer(map.id, timerVo);
                    }

                    timer.setOld(true);
                }
            }

            json params_l = timer_info;
            string base64;
            base64.append(params_l.dump());//json转base64 string
            sh::File::saveTextTo(fileName, base64);
        }


    }

    /**
     * cleanHistory.sqlite
     * Property.sqlite
     * Task.sqlite
     * move.mp3
     * out.mp3
     * pad_version_info.txt
     * param_app.yaml
     */
    void deleteExcessive() {

        cppfs::FileHandle fh = cppfs::fs::open(path::data_base_config_dir());
        if (fh.isDirectory()) {
            const std::vector<std::string> &files = fh.listFiles();
            for (const auto &item: files) {
                if (item != "cleanHistory.sqlite" && item != "Property.sqlite" && item != "Task.sqlite" &&
                    item != "move.mp3" && item != "out.mp3" && item != "pad_version_info.txt" &&
                    item != "param_app.yaml" && item != "prohibition_areas.yaml" &&
                    item != "timer_info_json.txt" && item != "combination_list_principal_json_work.txt" &&
                    item != "view_part_principal_json.txt") {
                    cppfs::FileHandle file = cppfs::fs::open(path::data_base_config_dir() + item);
                    if (file.isDirectory()) {
                        file.removeDirectoryRec();
                    } else if (file.isFile()) {
                        file.remove();
                    }
                }
            }
        }

        ParamManager::instance().setTxtUpgrade(true);
    }

};

#endif //APP_COMMUNICATION_UPGRADEMANAGER_H
