#include "error_log/Error_log.h"
#include "ros/package.h"
#include "tool/Variable.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "schedule/Cron.h"
#include <sys/vfs.h>
#include <unistd.h>

#include "tool/write_file.hpp"
#include "manager/VersionManager.h"
#include "manager/NoticeManager.h"
#include "clean_msgs/cloud_robot_event.h"
//

/*string GetCurrentTime(tm in)
{
    tm *ct = &in;
    int year, month, day, hour, minute, second;// 年月日时分秒。
    year = ct->tm_year + 1900;                 // 年份基础从1900开始的，所以要加上
    month = ct->tm_mon + 1;                    // 月份是0-11，对应1-12月
    day = ct->tm_mday;
    hour = ct->tm_hour;
    minute = ct->tm_min;
    second = ct->tm_sec;

    char temp[100];                            // 创建字符数组。
    sprintf(temp, "%04d_%02d_%02d_%02d_%02d_%02d: ", year, month, day, hour, minute, second);// 时间信息合并。
    string out(temp);                          // 转化为string型
    return move(out);                          // 用move（string）速度快很多。
}*/
int checkFolderExist(std::string const &name) {
    struct stat fileStatus;
    if (stat(name.c_str(), &fileStatus) == -1)
        return -1;
    return 0;
}

void Error_log::InitLog() {
    string fileName;
    string sss;
    fileName.append(ros::package::getPath("data_base"));

    std::string noticeFilePath;
    noticeFilePath.append(fileName);
    noticeFilePath.append("/config/notice.txt");

    if (!sh::File::exists(noticeFilePath)) {
        unique_ptr<sh::File> cFilePtr(new sh::File(noticeFilePath));
        if (!cFilePtr->create(noticeFilePath)) {
            LOG(ERROR) << noticeFilePath << " create fail !";
        }
    }

    //////////////////////////
    if (checkFolderExist(fileName + "/config/err_log") == -1) {
        string sp = fileName + "/config/err_log";
        mkdir(sp.c_str(), 0777);
    }
    ////////////////////////////////
    std::string st = croncpp::Cron<croncpp::LocalClock, croncpp::NullLock>::get_timestring();
    fileName.append("/config/err_log/error_log_" + st + ".txt");
    //设置清扫计时器
    // sh::File *fff = new sh::File(fileName);

    std::shared_ptr<sh::File> fff = make_shared<sh::File>(fileName);
    sFilepath = fileName;
    cout << "error_log file  " << fileName << endl;
    if (!fff->open(std::ios::in)) {
        if (!fff->create(fileName)) {
            cout << "fail to create error_log file" << endl;
            //   parseError(pubOut, params.getId(), 10001, "创建定时器文件失败11");

            return;
        }
    } else {
        fff->close();
    }
    ///////////////////////////////////////////
    ofstream outfile;
    outfile.open(fileName, ios::app); //文件的物理地址，文件的打开方式, 如果没有会自动创建
    if (outfile.is_open()) {
        ////////////////////////////////////////////////////////////////

        string path;
        path.append("/opt/robot/robot_hw_info.yaml");
        if (sh::File::exists(path)) {
            // path.append("/home/admin1/test_ws/src/data_base/config/prohibition_areas.yaml");

            YAML::Node config = YAML::LoadFile(path);
            outfile << "DeviceName: " << config["device_name"] << "\n";
            outfile << "DeviceSecret: " << config["device_secret"] << "\n";

            ////////////////////////////////////////////////////////////////
            fstream fs;

            char rstr[1000] = {0};
            fs.open("/proc/version", ios_base::out | ios_base::in);
            if (fs.is_open()) {
                cout << "open success" << endl;
            } else {
                cout << "open failed" << endl;
            }

            fs.read(rstr, sizeof(rstr)); //读数据

            cout << rstr << endl;
            outfile << "操作系统: " << rstr << "\n";
            fs.close();
            ////////////////////////////////
            outfile << "ROS 1.0 "
                    << "\n";
            outfile << "AirCore： " << VersionManager::instance().getAirCodeVersion() << "\n";
            ////////////////////////////////////////////////////////////////
            struct statfs diskInfo;
            statfs("/", &diskInfo);
            long long unsigned totalBlocks = diskInfo.f_bsize;
            long long unsigned totalSize = (totalBlocks * diskInfo.f_blocks) >> 20;
            long long unsigned freeDisk = (diskInfo.f_bfree * totalBlocks) >> 20;
            std::cout << "磁盘总大小: " << totalSize << " MB"
                      << "\n空闲大小: " << freeDisk << " MB" << std::endl;
            outfile << "磁盘总大小: " << totalSize << " MB"
                    << "\n空闲大小: " << freeDisk << " MB"
                    << "\n";
            ////////////////////////////////////////////////////////////////

            outfile.close();
        }
        // return;
    }
    std::string basePath = ros::package::getPath("data_base") + "/config/";
    std::string path = basePath + "error_infos.yaml";

    m_config = YAML::LoadFile(path);
}

void Error_log::WriteLog(int ercode) {
    std::string basePath = ros::package::getPath("data_base") + "/config/";

    time_t t;
    t = time(NULL);

    int noticeCode = ercode;
    long noticeTime = t;
    std::string noticeTitle;
    std::string noticeMessage;
    std::string solution;
    bool isSuccess = false;

    // struct tm *local;
    ofstream outfile;
    //  std::string path = basePath + "error_infos.yaml";
    //  YAML::Node config = YAML::LoadFile(path);



    int err_infos__num = m_config["error_infos"].size();
    std::cout << "err_infos__num: " << err_infos__num << std::endl;

    //存入
    for (int i = 0; i < err_infos__num; i++) {
        std::cout << "m_config: " << m_config["error_infos"][i]["err_code"].as<std::string>() << std::endl;
        std::cout << "to_string(ercode): " << to_string(ercode) << std::endl;
        if (m_config["error_infos"][i]["err_code"].as<std::string>() == to_string(ercode)) {
            std::cout << "err_infos__num22222: " << err_infos__num << std::endl;
            outfile.open(sFilepath, ios::app); //文件的物理地址，文件的打开方式, 如果没有会自动创建
            //    std::string st = croncpp::Cron<croncpp::LocalClock, croncpp::NullLock>::get_timestring();
            if (outfile.is_open()) {
                noticeTitle = m_config["error_infos"][i]["err_msg"].as<std::string>();
                noticeMessage = m_config["error_infos"][i]["err_sym"].as<std::string>();
                solution = m_config["error_infos"][i]["err_solution"].as<std::string>();
                outfile << "\n";
                outfile << "err_time: " << t << "\n";
                outfile << "err_code: " << ercode << "\n";
                outfile << "err_msg: " << noticeTitle << "\n";
                outfile << "err_sym: " << noticeMessage << "\n";
                outfile << "err_solution: " << solution << "\n";
                outfile.close();
                isSuccess = true;
            }
        }
    }

    if (isSuccess) {

        std::string noticeFilePath;
        noticeFilePath.append(basePath);
        noticeFilePath.append("notice.txt");

        std::string content;
        std::unique_ptr<sh::File> noticeFilePtr(new sh::File(noticeFilePath));
        if (noticeFilePtr->open(std::ios::in)) {
            content = noticeFilePtr->readAll();
            noticeFilePtr->close();
        }

        vector<Notice> list;
        if (!content.empty()) {
            auto jdecode = json::parse(content);
            list = jdecode.get<vector<Notice>>();
        }

        auto notice = Notice(noticeCode, noticeTime, noticeTitle, noticeMessage, solution);
        list.push_back(notice);
        json base_success = list;

        if (!sh::File::saveTextTo(noticeFilePath, base_success.dump())) {
            LOG(ERROR) << noticeFilePath << " save empty text fail !";
        }

        NoticeManager::get_instance()->sendNotice(noticeCode, noticeTime, noticeTitle, noticeMessage, solution);
        WriteToCloud(noticeCode, noticeTime);
    }
}

void Error_log::WriteToCloud(int ntCode, long nttime) {
    clean_msgs::cloud_robot_event event;
    event.event_id = "alarm_event";
    event.event_data = "{\"err_time\":" + to_string(nttime) + ", \"err_code\":" + to_string(ntCode) + "}";
    NoticeManager::get_instance()->getPubOut()->publishCloudEvent(event);


}