#include "rec_app.h"
#include "simulation.h"
#include "leave/ParamManager.h"

/**
 * 单元测试示例代码
 */
#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

int Factorial(int number) {
    return number <= 1 ? number : Factorial(number - 1) * number;
}

//TEST_CASE() {
//    REQUIRE(Factorial(3) == 7);
//}


int ignore_area;//面积小于此百分比面积的分区区域将被忽略

NoticeManager *NoticeManager::m_instance_ptr = nullptr;
Variable *Variable::m_instance_ptr = nullptr;
TeachModePoint *TeachModePoint::m_instance_ptr = nullptr;
ViewPartManager *ViewPartManager::m_instance_ptr = nullptr;
CombinationManager *CombinationManager::m_instance_ptr = nullptr;
FullCleanManager *FullCleanManager::m_instance_ptr = nullptr;

internal_event::InternalEventPubManager *internal_event::InternalEventPubManager::instance_ = nullptr;

ScheduleThread *sThd = nullptr;

MessageBus *MessageBusManager::getMessageBus() const { return messageBus; }

ThreadPool pool(3);

int main(int argc, char **argv) {

    judgeEnvironment();
    initLog(argv);
    initDump();
    initTest(argc, argv);

    //初始化ros节点
    ros::init(argc, argv, "rec_app_node");
    LOG(INFO) << sys_gettid() << " start to listening!";
    //Li Quan 新清洁历史
    clean_history_db::CleanHistoryCenter::instance().initialize();
    pool.init();

    async::TimerInitCall::instance().initialize();
    UdpManager::instance().start();

    ros::NodeHandle handle;
    handle.param("/path_planning_node/ignore_area", ignore_area, std::int32_t(8));
    PubInner pubInner(handle);
    PubOut pubOut(handle);
    PublishInnerManager::instance().setPubInner(&pubInner);
    PublishOutManager::instance().setPubOut(&pubOut);
    internal_event::InternalEventPubManager::get_instance()->setPubOut(&pubOut);

    SegmentationCenter::instance().initialize();
    ExplorationCenter::instance().initialize(handle);
//    AlignmentCenter::instance().initialize(handle);
    TaskCenter::instance().initialize(handle);

    ParamManager::instance().loadDefaultParam();

    //启动订阅话题的callback
    JsonSubscribe jsonSubscribe(handle);
    JsonSubscribeCloud jsonSubscribeCloud(handle, pubInner, pubOut);
    BeforeJsonSubscribe beforeJsonSubscribe(handle, pubInner, pubOut);
    MapInnerSubscribe mapInnerSubscribe(handle, pubInner, pubOut);
    DSVersionSubscribe dsVersionSubscribe(handle, pubInner, pubOut);

    SelfCheckSubscribe selfCheckSubscribe(handle, pubInner, pubOut);
    MoveBaseRecoveryFailureSubscribe moveBaseRecoveryFailureSubscribe(handle);

    SegmentationSubscribe SegmentationSubscribe(handle);
    NoticeManager::get_instance()->setPubOut(&pubOut);

    std_msgs::String test;
    /////////////////////////////////
    ////////////////////////////////////////////

    UpgradeManager::instance().updateCleanHistoryPrincipal();
    UpgradeManager::instance().updateCombinationBase64();
    UpgradeManager::instance().updateViewPartBase64();
    UpgradeManager::instance().updateCleanHistoryBase64();
    UpgradeManager::instance().updateTimeInfoBase64();
    UpgradeManager::instance().updateTeachPoint();
    UpgradeManager::instance().updateCombinationPrincipalWork();
    UpgradeManager::instance().checkWhetherFileExists();
    UpgradeManager::instance().removePolygonViewPart();

    ros::NodeHandle nh;
    initNodeParams(nh);

    ros::Publisher pub_current = nh.advertise<std_msgs::Int32>("/current_flag", 10);
    WsServerManager::instance().startWebSocket(pubInner, pubOut);

    string last_task;
    nh.param<string>("last_task", last_task, "");//上次执行的任务
    restartAfterCrash(last_task);
    ///////////////////////////////////////////////
    sThd = new ScheduleThread(handle);
    sThd->start();
    sThd->detach();

    ros::MultiThreadedSpinner spinner;
    spinner.spin();

    release();
    return 0;
}

std::string getenv_rec(const std::string &name) {
    if (name != "HOME") {
        return nullptr;
    }
    DIR *pAdmin = opendir("/home/admin1");
    bool isRealEnvironment = pAdmin != nullptr;
    if (pAdmin != nullptr) {
        closedir(pAdmin);
    }
    return isRealEnvironment ? "/home/admin1" : "/home/lijiang";
}

void judgeEnvironment() {
    std::string home = getenv_rec("HOME");
    Environment::instance().isRealEnvironment = (string(home) == "/home/admin1");
}

void SignalHandle(const char *data, int size) {
    std::string str = std::string(data, size);
    LOG(ERROR) << str;
}

void initLog(char *const *argv) {
    // sudo apt-get install libgoogle-glog-dev
    std::string logDirStr = string(getenv_rec("HOME")) + "/app_log";
    mkdir(logDirStr.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);

    std::string cartoLogDirStr = string(getenv_rec("HOME")) + "/carto_log";
    mkdir(cartoLogDirStr.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);

    FLAGS_logtostderr = false; //设置日志消息是否转到标准输出而不是日志文件(false)
    FLAGS_stderrthreshold = google::ERROR; //严重性级别在该门限值以上的日志信息除了写入日志文件以外，还要输出到stderr。
    // 各严重性级别对应的数值：INFO—0，WARNING—1，ERROR—2，FATAL—3  默认值为2.
    FLAGS_minloglevel = google::INFO; //严重性级别在该门限值以上的日志信息才进行记录。默认值为0.
    FLAGS_log_dir = logDirStr; //设置日志文件保存目录,这个目录必须是已经存在的,否则不能生成日志文件.
    FLAGS_log_prefix = true; //设置日志前缀是否应该添加到每行输出
    FLAGS_logbufsecs = 0; //设置可以缓冲日志的最大秒数，0指实时输出
    FLAGS_max_log_size = 10; //设置最大日志文件大小（以MB为单位）
    FLAGS_stop_logging_if_full_disk = true; //设置是否在磁盘已满时避免日志记录到磁盘
    google::InitGoogleLogging(argv[0]); // 全局初始化glog，argv[0]是程序名
    google::SetStderrLogging(google::GLOG_INFO); // 设置glog的输出级别，这里的含义是输出INFO级别以上的信息
    google::SetLogDestination(google::GLOG_INFO, string(logDirStr + "/info_").c_str());
    google::SetLogDestination(google::GLOG_WARNING, string(logDirStr + "/warn_").c_str());
    google::SetLogDestination(google::GLOG_ERROR, string(logDirStr + "/error_").c_str());

    auto filenameExtension = unique_identification + "_";
    google::SetLogFilenameExtension(filenameExtension.data());

    FLAGS_colorlogtostderr = true; // 开启终端颜色区分
    google::InstallFailureSignalHandler();
    google::InstallFailureWriter(&SignalHandle);

    //    LOG(INFO) << "This is my first glog INFO ";
    //    LOG(WARNING) << "This is my first glog WARNING";
    //    LOG(ERROR) << "This is my first glog ERROR 1";

    LOG(INFO) << "current process id is " << getpid() << "  log uuid : " << unique_identification;
}

/**
 * 指令形式
//    cd /home/lijiang/app_ws/build/app_communication/PATH/home/lijiang/app_ws/devel/lib/app_communication
//    cp rec_app_node /home/lijiang/app_dump/
//    cd /home/lijiang/app_dump
//    dump_syms ./rec_app_node > rec_app_node.sym
//    head -n1 rec_app_node.sym
//    cp rec_app_node /home/lijiang/app_dump/
//    mkdir -p symbols/rec_app_node/E49C79F8412998A8DA848C21ABC7E8BA0
//    mv rec_app_node.sym symbols/rec_app_node/E49C79F8412998A8DA848C21ABC7E8BA0
//    minidump_stackwalk b0b3ee65-051a-414a-84065a83-9c8461c2.dmp symbols > b0b3ee65-051a-414a-84065a83-9c8461c2.txt
 */
static bool dumpCallback(const google_breakpad::MinidumpDescriptor &descriptor, void *context, bool succeeded) {
    std::string crash_file_path = descriptor.path();
    unsigned long start = crash_file_path.find("app_dump/") + 9;
    auto crash_file = crash_file_path.substr(start);
    LOG(ERROR) << sys_gettid() << " " << "Dump path : " << crash_file_path << " " << succeeded;

    auto home = string(getenv_rec("HOME"));

    std::string parse_crash = "parse_crash.sh";
    std::string rec_app_node = "rec_app_node";
    std::string dump_upload = "dump_upload";

    //find parse_crash.sh
    std::string parse_crash_dir = home + "/AirCore/app/install/share/app_communication/scripts/";
    std::string clion_parse_crash_dir = home + "/app_ws/src/app_communication/scripts/";
    std::string real_parse_crash_dir;
    if (access((parse_crash_dir + parse_crash).c_str(), F_OK) == 0) {
        real_parse_crash_dir = parse_crash_dir;
    } else {
        if (access((clion_parse_crash_dir + parse_crash).c_str(), F_OK) == 0) {
            real_parse_crash_dir = clion_parse_crash_dir;
        }
    }

    //find rec_app_node
    std::string program_installation_dir = home + "/AirCore/app/install/lib/app_communication/";
    std::string clion_program_installation_dir = home + "/app_ws/devel/lib/app_communication/";
    std::string real_program_installation_dir;
    if (access((program_installation_dir + rec_app_node).c_str(), F_OK) == 0) {
        real_program_installation_dir = program_installation_dir;
    } else {
        if (access((clion_program_installation_dir + rec_app_node).c_str(), F_OK) == 0) {
            real_program_installation_dir = clion_program_installation_dir;
        }
    }

    //find dump_upload
    std::string dump_upload_dir = home + "/AirCore/app/install/lib/app_communication/";
    std::string clion_dump_upload_dir = home + "/app_ws/devel/lib/app_communication/";
    std::string real_dump_upload_dir;
    if (access((dump_upload_dir + rec_app_node).c_str(), F_OK) == 0) {
        real_dump_upload_dir = dump_upload_dir;
    } else {
        if (access((clion_dump_upload_dir + rec_app_node).c_str(), F_OK) == 0) {
            real_dump_upload_dir = clion_dump_upload_dir;
        }
    }

    if (!real_parse_crash_dir.empty() && !real_program_installation_dir.empty()) {
        auto CMD = real_parse_crash_dir + parse_crash + " " + real_program_installation_dir + " " + crash_file;
        LOG(INFO) << "CMD : " << CMD;
        std::system(CMD.c_str());

        if (!real_dump_upload_dir.empty()) {
            std::string dump_upload_executable_file = real_dump_upload_dir + dump_upload;
            //子进程的返回值为0,父进程的返回值则是新建的进程ID
            pid_t pid;
            if ((pid = fork()) < 0) {
                LOG(ERROR) << "fork error";
            } else if (pid == 0) {
                LOG(INFO) << "fork success, this is son process" << " " << getpid();

                if (execl(dump_upload_executable_file.data(),
                          dump_upload_executable_file.data(),
                          (crash_file).c_str(),
                          unique_identification.c_str(),
                          (char *) 0)
                        ) {
                    LOG(INFO) << "execle error";
                }
            }

            LOG(INFO) << "son process" << " " << pid;

            if (waitpid(pid, nullptr, 0) != pid) {
                LOG(ERROR) << "wait error";
            }
        }


    }

    return succeeded;
}

static bool filterCallback(void *context) {
    return true;
}

void initDump() {
    std::string dumpDirStr = string(getenv_rec("HOME")) + "/app_dump";

    LOG(INFO) << "dumpDirStr  " << dumpDirStr;
    mkdir(dumpDirStr.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
    google_breakpad::MinidumpDescriptor descriptor(dumpDirStr);
    exceptionHandler = new google_breakpad::ExceptionHandler(descriptor,//minidump文件写入的目录
                                                             filterCallback,//可选，在写minidump文件之前，会先调用filter回调。根据它返回true/false来决定是否需要写minidump文件。
                                                             dumpCallback,//可选，在写minidump文件之后调用的回调函数
                                                             NULL,
                                                             true,//如果为ture，不管怎样当未捕捉异常被抛出时都会写入minidump文件，如果为false则必须明确调用了 WriteMinidump 才会写入minidump 文件
                                                             -1);//如果为-1，则使用同线程模式（in-precess），如果有一个有效的值，则使用跨线程模式（out-of-process)

//    volatile int *a = (int *) (NULL);
//    *a = 1;
}

void initTest(int argc, char **argv) {
    if (!Environment::instance().isRealEnvironment) {
        Catch::Session().run(argc, argv);
    }
}

//本函数用于rec_app_node 节点发生崩溃后继续执行任务
//当下达任务时，TaskManager::addOnceTask函数会记录一个task对象到last_task并存入参数服务器
//当任务正常结束时，服务器中参数被清空
//当节点发生崩溃时，进入主循环之前调用此函数，解析参数服务器中的last_task参数
//如果为空则无事发生，如果非空则继续执行该任务
void restartAfterCrash(string &last_task) {
    if (last_task.empty()) {
        return;
    } else {
        LOG(INFO) << "last_task  " << last_task;
//        string decode = base64_decode(last_task);//转化成base64
        LOG(INFO) << "after decode";
        json jdecode = json::parse(last_task);
        LOG(INFO) << "after parse";
        auto last_task = jdecode.get<Task>();
        //全局清扫
        TaskCenter::instance().executeTask(last_task);
    }
}

void initNodeParams(const ros::NodeHandle &nh) {
    bool base_exist;
    nh.param<bool>("base_exist", base_exist, true); //是否存在基站，默认存在
    Variable::get_instance()->setBaseExist(base_exist);
    bool face_recognition;
    nh.param<bool>("face_recognition", face_recognition, false); //是否存在人脸识别，默认不存在
    Variable::get_instance()->setface_recognition(face_recognition);
    bool has_knob;
    nh.param<bool>("has_knob", has_knob, false); //是否存在旋钮，默认不存在
    Variable::get_instance()->setknob(has_knob);
    bool has_voice;
    nh.param<bool>("has_voice", has_voice, false); //是否存在语音识别，默认不存在
    Variable::get_instance()->setvoice(face_recognition);
    string ros_version;
    nh.param<string>("ros_version", ros_version, "0.0.0.1"); //版本号
    VersionManager::instance().setAirCodeVersion(ros_version);
    int machine_version;
    nh.param<int>("machine_version", machine_version, 630); //版本号
    VersionManager::instance().setMachineVersion(machine_version);
    int self_check_version;
    nh.param<int>("self_check_version", self_check_version, 1); //版本号
    VersionManager::instance().setSelfCheckVersion(self_check_version);
}

void release() {
    LOG(ERROR) << "Shutdown Shutdown Shutdown ...";
    if (exceptionHandler != nullptr)
        delete exceptionHandler;
    google::ShutdownGoogleLogging(); // 全局关闭glog
    UdpManager::instance().stop();
    WsServerManager::instance().stopWebSocket();
    TaskCenter::instance().uninstall();
    ExplorationCenter::instance().uninstall();
}