//
// Created by mi on 2022/8/10.
//

#include "sub/json/otaStrategy.h"
//#include <manager/TaskPubManager.h>
#include <std_msgs/String.h>
#include "glog/logging.h"
#include "manager/VersionManager.h"

int checkFolderExistOta(std::string const &name) {
    struct stat fileStatus;
    if (stat(name.c_str(), &fileStatus) == -1)
        return -1;
    return 0;
}

string otaStrategy::handler(OtaInfo params) {
    ///Ewen change begin
    string otaType = params.getota_module();
    LOG(INFO) << "otaStrategy::handler entry:" << otaType;
    json js;
    const string CODE = "error_code";
    int ret = -1;
    //取值：“core", "pad", "ecu"，"nebula"
    if (otaType == "core") { // core
        ret = handleCore(params);
    } else if (otaType == "pad") { // pad
        ret = handlePad(params);
    } else if (otaType == "ecu") { //下位机
        ret = handleEcu(params);
    } else {
        LOG(ERROR) << "Unknown module recv, ignore";
    }
    js[CODE] = ret;
    LOG(INFO) << "otaStrategy::handler exit return:" << js.dump();
    ///Ewen change end
    return js.dump();
}

int otaStrategy::handleCore(OtaInfo &params) {
    std_msgs::String path;
    ///path.data = params.getota_path();
    string ota_path = params.getota_path();
    std::size_t found = ota_path.find_last_of("_");
    std::size_t found2 = ota_path.find_last_of(".");
    string ver = ota_path.substr(found + 1, ota_path.length() - found - 5); // 0.8.6.1
    string ros_version = VersionManager::instance().getAirCodeVersion(); // 0.8.6.1_600
    std::size_t found3 = ros_version.find_last_of("_");
    string ros_version2 = ros_version.substr(0, ros_version.length() - 4);
    LOG(INFO) << "core target version: " << ver << " ,current:" << ros_version2;
    if (ver == ros_version2) {
        LOG(INFO) << "No need upgrade: core version:" << ros_version;
        return 100;
    }
    string ota_folder = "/home/admin1/t5/";
    if (checkFolderExistOta(ota_folder) == -1) {
        string sp = ota_folder;
        mkdir(sp.c_str(), 0777);
        LOG(INFO) << "No core ota folder create it";
    }
    // To avoid pc reboot deadlock
    string last_ota_folder = ota_folder + ver;
    bool need_reboot = true;
    if (checkFolderExistOta(last_ota_folder) != -1) {
        need_reboot = false;
        LOG(INFO) << "ota folder exist, don't reboot";
    }
    string cmd = "rm -rf /home/admin1/t5/*";
    int ret = system(cmd.c_str());
    if (ret == -1) {
        LOG(ERROR) << "rm core ota t5 directory failed:" << ret;
        return -1;
    }

    string cmd2 = "tar -xvf " + ota_path + " -C /home/admin1/t5";
    ret = system(cmd2.c_str());
    if (ret == -1) {
        LOG(ERROR) << "tar xvf core package failed:" << ret;
        return -1;
    }
    // Check the ota package if contain right folder name
    if (checkFolderExistOta(last_ota_folder) == -1) {
        LOG(ERROR) << "ota package is wrong, no version folder exist, do nothing" << last_ota_folder;
        return -2;
    }

    string supwd = "123456";
    string ota_core_path = ota_folder + ver;
    string comd = "dpkg -i " + ota_core_path + "/*.deb";
    string strcmd = "echo " + supwd + "|sudo -S  " + comd;
    ret = system(strcmd.c_str());
    LOG(INFO) << "Start install:" << strcmd << " ret:" << ret;
    if (ret == -1) {
        LOG(ERROR) << "Install core deb package failed:" << ret;
    }

    const string shell_cmd =
            "bash " + ota_core_path +
            "/AirCore/app/install/share/app_communication/params/ota.sh " +
            ota_core_path + " " + ros_version2;
    ret = system(shell_cmd.c_str());
    if (ret == -1) {
        LOG(ERROR) << "run ota shell failed:" << ret;
        return ret;
    }
    if (need_reboot) {
        strcmd = "echo " + supwd + "|sudo -S reboot";
        LOG(INFO) << "Reboot sys for OTA after 10s" << ret;
        ros::Duration(10).sleep();
        ret = system(strcmd.c_str());
        if (ret == -1) {
            LOG(ERROR) << "Reboot sys failed" << ret;
            return -3;
        }
    }
    return 0;
}

int otaStrategy::handlePad(OtaInfo &params) {
    int result = 0;
    string padv = VersionManager::instance().getAppPadVersion();
    LOG(INFO) << "Current pad version:" << padv;
    if (params.getota_version() == padv) {
        LOG(INFO) << "No need OTA, pad version = target version:" << padv;
        return 100;
    }
    long noticeTime = 0;
    std::string noticeFile;
    std::string noticeMessage;
    string fileName = "/home/admin1/t6/";
    if (checkFolderExistOta(fileName) == -1) {
        string sp = fileName;
        mkdir(sp.c_str(), 0777);
        LOG(INFO) << "otaStrategy::handler no pad ota folder, create it";
    }
    string pad_path = params.getota_path();
    string cmd = "rm -rf /home/admin1/t6/*";
    int ret = system(cmd.c_str());
    if (ret == -1) {
        LOG(ERROR) << "rm t6 directory failed:" << ret;
        result = -1;
    }
    cmd = "tar -xvf " + pad_path + " -C /home/admin1/t6/";
    ret = system(cmd.c_str());
    if (ret == -1) {
        LOG(ERROR) << "tar xvf package failed:" << ret;
        return ret;
    }
    // Temp:wait extract file done
    ros::Duration(5).sleep();
    std::size_t found = pad_path.find_last_of("/\\");
    std::size_t found2 = pad_path.find_last_of(".");
    LOG(INFO) << "ota_path:" << pad_path << "found:" << found << "found2:" << found2;
    string filename = pad_path.substr(found + 1, pad_path.length() - found - 5) + ".apk";
    LOG(INFO) << "ota_file_name:" << filename;
    noticeFile = "http://192.168.8.100:8000/pkgs/" + filename;
    LOG(INFO) << noticeFile;
    // Prepare ota notify file for pad check
    std::string ota_notify = fileName + "ota_notify.json";
    Notice notice_obj(6666, noticeTime, noticeFile, params.getota_desc(), filename);
    json notice_js = notice_obj;

    if (!sh::File::saveTextTo(ota_notify, notice_js.dump())) {
        LOG(ERROR) << "Save ota_notify file failed";
        result = -2;
    }

    LOG(INFO) << "Write notif to:" << ota_notify << " with:" << notice_js.dump();;
    for (int i = 0; i < 10; i++) {
        ros::Duration(2).sleep();
        LOG(INFO) << "Send notice to pad:" << noticeFile << "  " << i;
        NoticeManager::get_instance()->sendNotice(6666, noticeTime, noticeFile, params.getota_desc(),
                                                  filename); //文件名传递给pad
    }
    return result;
}

int otaStrategy::handleEcu(OtaInfo &params) {
    string ecu_version = VersionManager::instance().getDsSoftVersion();
    LOG(INFO) << "Current ecu version:" << ecu_version;
    if (params.getota_version() == ecu_version) {
        LOG(INFO) << "No need OTA, ecu version = target version:" << ecu_version;
        return 100;
    }
    string ecu_ota_folder = "/home/admin1/t7/";
    if (checkFolderExistOta(ecu_ota_folder) == -1) {
        string sp = ecu_ota_folder;
        mkdir(sp.c_str(), 0777);
        LOG(INFO) << "otaStrategy::handler no ecu ota folder, create it";
    }
    string cmd = "rm -rf /home/admin1/t7/*";
    int ret = system(cmd.c_str());
    if (ret == -1) {
        LOG(INFO) << "rm ecu ota left file failed:" << ret;
    }
    string ecu_path = params.getota_path();
    cmd = "tar -xvf " + ecu_path + " -C /home/admin1/t7/";
    LOG(INFO) << "ecu ota tar cmd:" << cmd;
    ret = system(cmd.c_str());
    if (ret == -1) {
        LOG(ERROR) << "tar xvf ecu package failed:" << ret;
        return ret;
    }

    std_msgs::String path;
    std::size_t found = ecu_path.find_last_of("/\\");
    std::size_t found2 = ecu_path.find_last_of(".");
    path.data = ecu_ota_folder + ecu_path.substr(found + 1, ecu_path.length() - found);
    path.data = ecu_ota_folder + ecu_path.substr(found + 1, ecu_path.length() - found - 5) + ".rbl";
    LOG(INFO) << "Notify driver ecu ota start:" << ecu_path.substr(found + 1, ecu_path.length() - found) << "  ";
    PublishInnerManager::instance().getPubInner()->publishOtaLow(path); //发给下位机
    return 0;
}
