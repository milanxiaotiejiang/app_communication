#include "error_log/err_core.h"
#include "ros/package.h"
#include "tool/Variable.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "schedule/Cron.h"
#include <sys/vfs.h>
#include <unistd.h>
#include "yaml-cpp/yaml.h"
#include "tool/write_file.hpp"
#include "manager/VersionManager.h"
#include "manager/NoticeManager.h"
#include "net/WsServerManager.h"

//
Error_Core::Error_Core(ros::NodeHandle handle, PubInner pubInner, PubOut pubOut)
        : handle(handle),
          pubInner(std::move(pubInner)),
          pubOut(std::move(pubOut)) {
    error_status_ = handle.subscribe(APP_ERROR, 1000, &Error_Core::subscribeErrCallback, this);
    std::string basePath = ros::package::getPath("data_base") + "/config/";
    std::string path = basePath + "error_infos_core.yaml";
    m_config = YAML::LoadFile(path);
}

Error_Core::~Error_Core() {
}

int checkFolderExistCore(std::string const &name) {
    struct stat fileStatus;
    if (stat(name.c_str(), &fileStatus) == -1)
        return -1;
    return 0;
}

void Error_Core::subscribeErrCallback(const std_msgs::String &errmsg) {
    string msg = errmsg.data;
    std::size_t found = msg.find("app_taskstart");
    if (found != std::string::npos) {
        InitCoreLog();
    }
    WriteCoreLog(errmsg.data);
}

void Error_Core::InitCoreLog() {
    string fileName;
    string sss;
    fileName.append(ros::package::getPath("data_base"));

    /*   std::string noticeFilePath;
       noticeFilePath.append(fileName);
       noticeFilePath.append("/config/notice_core.txt");

       if (!sh::File::exists(noticeFilePath)) {
           unique_ptr <sh::File> cFilePtr(new sh::File(noticeFilePath));
           if (!cFilePtr->create(noticeFilePath)) {
               LOG(ERROR) << noticeFilePath << " create fail !";
           }
       }*/

    //////////////////////////
    if (checkFolderExistCore(fileName + "/config/err_core") == -1) {
        string sp = fileName + "/config/err_core";
        mkdir(sp.c_str(), 0777);
    }
    ////////////////////////////////
    std::string st = croncpp::Cron<croncpp::LocalClock, croncpp::NullLock>::get_timestring();
    fileName.append("/config/err_core/Error_Core_" + st + ".txt");
    //设置清扫计时器
    // sh::File *fff = new sh::File(fileName);

    std::shared_ptr<sh::File> fff = make_shared<sh::File>(fileName);
    currFilepath = fileName;
    cout << "Error_Core file  " << fileName << endl;
    if (!fff->open(std::ios::in)) {
        if (!fff->create(fileName)) {
            cout << "fail to create Error_Core file" << endl;
            //   parseError(pubOut, params.getId(), 10001, "创建定时器文件失败11");

            return;
        }
        fff->close();
    } else {
        fff->close();
    }

    ///////////////////////////////////////////
    /*   ofstream outfile;
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
           return;
       } else {
           return;
       }*/
}
/////////////////////

/////////////////////

void Error_Core::WriteCoreLog(string ercode) {
    std::string basePath = ros::package::getPath("data_base") + "/config/";

    time_t t;
    t = time(NULL);


    bool isSuccess = false;


    ofstream outfile;
    std::string noticeTitle;
    std::string noticeMessage;
    std::string solution;

    std::size_t found = ercode.find("app_taskstart");
    if (found != std::string::npos) {
        outfile.open(currFilepath, ios::app); //文件的物理地址，文件的打开方式, 如果没有会自动创建
        //    std::string st = croncpp::Cron<croncpp::LocalClock, croncpp::NullLock>::get_timestring();
        if (outfile.is_open()) {

            outfile << "task_time: " << t << "\n";
            outfile << "task_start: " << ercode << "\n";

            outfile.close();
            isSuccess = true;
        }
    }

    int err_infos__num = m_config["error_infos"].size();
    //存入
    int i = 0;
    for (; i < err_infos__num; i++) {

        if (m_config["error_infos"][i]["err_code"].as<std::string>() == (ercode)) {
            outfile.open(currFilepath, ios::app); //文件的物理地址，文件的打开方式, 如果没有会自动创建
            //    std::string st = croncpp::Cron<croncpp::LocalClock, croncpp::NullLock>::get_timestring();
            if (outfile.is_open()) {
                noticeTitle = m_config["error_infos"][i]["err_msg"].as<std::string>();
                noticeMessage = m_config["error_infos"][i]["err_sym"].as<std::string>();
                solution = m_config["error_infos"][i]["err_level"].as<std::string>();
                outfile << "\n";
                outfile << "err_time: " << t << "\n";
                outfile << "err_code: " << ercode << "\n";
                outfile << "err_msg: " << noticeTitle << "\n";
                outfile << "err_sym: " << noticeMessage << "\n";
                outfile << "err_level: " << solution << "\n";
                outfile.close();
                isSuccess = true;
                break;
            }
        }
    }
    if (i == err_infos__num) {
        outfile.open(currFilepath, ios::app); //文件的物理地址，文件的打开方式, 如果没有会自动创建
        //    std::string st = croncpp::Cron<croncpp::LocalClock, croncpp::NullLock>::get_timestring();
        if (outfile.is_open()) {

            outfile << "\n";
            outfile << "err_time: " << t << "\n";
            outfile << "err_code: " << ercode << "\n";
            outfile << "err_msg: " << "temp msg" << "\n";
            outfile << "err_sym: " << "temp msg" << "\n";
            outfile << "err_level: " << "temp msg" << "\n";
            outfile.close();
            //  isSuccess = true;
            //  break;
        }

    }




    /*   if (isSuccess) {

           std::string noticeFilePath;
           noticeFilePath.append(basePath);
           noticeFilePath.append("notice.txt");

           std::string content;
           std::unique_ptr<sh::File> noticeFilePtr(new sh::File(noticeFilePath));
           if (noticeFilePtr->open(std::ios::in)) {
               content = noticeFilePtr->readAll();
               noticeFilePtr->close();
           }

           vector <Notice> list;
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
           WriteToCloud(noticeCode,noticeTime);
       */
}
