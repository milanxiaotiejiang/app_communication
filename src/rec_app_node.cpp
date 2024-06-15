#include "rec_app.h"
#include "simulation.h"
#include "module.h"

/**
 * gdb --args /home/noodles/app_ws/devel/lib/app_communication/rec_app_node __name:=rec_app_node
 */
/**
 * pull requests
 */
/**
 * https://github.com/fnc12/sqlite_orm
 * https://github.com/cginternals/cppfs
 */
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

Variable *Variable::m_instance_ptr = nullptr;

internal_event::InternalEventPubManager *internal_event::InternalEventPubManager::instance_ = nullptr;

int main(int argc, char **argv) {

    current_program_string = argv[0];

    judgeEnvironment();
    initLog(argv);
    initDump();
    initTest(argc, argv);

    //初始化ros节点
    ros::init(argc, argv, "rec_app_node");
    LOG_IF(INFO, DEBUG_FIRING) << "启动 rec_app_node " << sys_gettid() << " start to listening ! ";


    ros::NodeHandle handle;

    initNodeParams(handle);

    //新清洁历史
    clean_history_db::CleanHistoryCenter::instance().initialize();
    if (Module::instance().module_property)
        PropertyDataBase::instance().initProperty();

    async::TimerInitCall::instance().initialize();
    if (Module::instance().module_udp)
        UdpManager::instance().start();

    AsyncMachine::instance().initialize(handle);
    PublishInnerManager::instance().initialize(handle);
    PublishOutManager::instance().initialize(handle);

    ParamManager::instance().loadDefaultParam();

    TaskDataBase::instance().initialize();
    if (!MapControl::instance().initialize(handle)) {
        LOG(ERROR) << "data DB Map load fail !";
        return 0;
    }
    ros::Time::init();

    SensorCenter::instance().initialize(handle);
    if (Module::instance().module_auto_maintenance)
        AutoMaintenanceModeManager::instance().run();
    if (!SegmentationCenter::instance().initialize(handle)) {
        LOG(ERROR) << "data Map File load fail !";
        return 0;
    }
    ExplorationCenter::instance().initialize(handle);
//    AlignmentCenter::instance().initialize(handle);
    TaskCenter::instance().initialize(handle);

    NodeControl::instance().initialize(handle);

    //启动订阅话题的callback
    JsonSubscribe jsonSubscribe(handle);
    BeforeJsonSubscribe beforeJsonSubscribe(handle);
    MapInnerSubscribe mapInnerSubscribe(handle);
    if (Module::instance().module_version)
        DSVersionSubscribe dsVersionSubscribe(handle);
    if (Module::instance().module_old_self_check)
        SelfCheckSubscribe selfCheckSubscribe(handle);

    WsServerManager::instance().startWebSocket();
    if (Module::instance().module_ai)
        AiServerManager::instance().startWebSocket();

    ScheduleManagerSingleton::instance().start(handle);

    ManualManager::instance().restore();

    if (Module::instance().module_elevator)
        ElevatorControlManager::instance().initialize(handle);

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
    return isRealEnvironment ? "/home/admin1" : "/home/noodles";
}

void judgeEnvironment() {
    std::string home = getenv_rec("HOME");
    Environment::instance().isRealEnvironment = (string(home) == "/home/cat");
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

    time_t timestamp_ = std::time(0);
    struct ::tm tm_time;
    localtime_r(&timestamp_, &tm_time);
    std::ostringstream time_pid_stream;
    time_pid_stream.fill('0');
    time_pid_stream << 1900 + tm_time.tm_year
                    << std::setw(2) << 1 + tm_time.tm_mon
                    << std::setw(2) << tm_time.tm_mday
                    << '-'
                    << std::setw(2) << tm_time.tm_hour
                    << std::setw(2) << tm_time.tm_min
                    << std::setw(2) << tm_time.tm_sec
                    << '.'
                    << getpid();
    const string &time_pid_string = time_pid_stream.str();
    glog_info_time_pid_string = "info_" + time_pid_string;

//    auto filenameExtension = unique_identification + "_";
//    google::SetLogFilenameExtension(filenameExtension.data());

    FLAGS_colorlogtostderr = true; // 开启终端颜色区分
//    google::InstallFailureSignalHandler();
//    google::InstallFailureWriter(&SignalHandle);

    //    LOG(INFO) << "This is my first glog INFO ";
    //    LOG(WARNING) << "This is my first glog WARNING";
    //    LOG(ERROR) << "This is my first glog ERROR 1";

    LOG_IF(INFO, DEBUG_FIRING) << "glog file is " << glog_info_time_pid_string;
    Environment::instance().glog_info_time_pid = logDirStr + "/"/* + time_pid_string*/;
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
    std::system("rosparam set /rec_app_node_crash true");

    std::string crash_file_path = descriptor.path();
    unsigned long start = crash_file_path.find("app_dump/") + 9;
    auto crash_file = crash_file_path.substr(start);
    LOG(WARNING) << sys_gettid() << " " << "Dump path : " << crash_file_path << " " << succeeded;

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

    if (!Environment::instance().isRealEnvironment) {
        return succeeded;
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
                LOG_IF(INFO, DEBUG_DUMP) << "fork success, this is son process" << " " << getpid();

                if (execl(dump_upload_executable_file.data(),
                          dump_upload_executable_file.data(),
                          current_program_string.c_str(),
                          (crash_file).c_str(),
                          glog_info_time_pid_string.c_str(),
                          (char *) 0)
                        ) {
                    LOG_IF(INFO, DEBUG_DUMP) << "execle error";
                }
            }

            LOG_IF(INFO, DEBUG_DUMP) << "son process" << " " << pid;

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

    LOG_IF(INFO, DEBUG_FIRING) << "dumpDirStr " << dumpDirStr;
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
    bool update_map;
    nh.param<bool>("update_map", update_map, false); //update_map
    Environment::instance().update_map = update_map;
    bool direct_start_move_base;
    nh.param<bool>("direct_start_move_base", direct_start_move_base, false); //direct_start_move_base
    Environment::instance().direct_start_move_base = direct_start_move_base;
    int explorer_mode;
    nh.param<int>("explorer_mode", explorer_mode, 2);
    Environment::instance().explorer_mode = explorer_mode;
    int dry_accumulation;
    nh.param<int>("dry_accumulation", dry_accumulation, 0);
    Environment::instance().dry_accumulation = dry_accumulation;
    int complex_path_num_splits;
    nh.param<int>("complex_path_num_splits", complex_path_num_splits, 1000);
    Environment::instance().complex_path_num_splits = complex_path_num_splits;
    bool rectangular_ambulatory_plane;
    nh.param<bool>("rectangular_ambulatory_plane", rectangular_ambulatory_plane, true);
    Environment::instance().rectangular_ambulatory_plane = rectangular_ambulatory_plane;
    bool gzip_map;
    nh.param<bool>("gzip_map", gzip_map, true);
    Environment::instance().gzip_map = gzip_map;

    LOG(INFO) << "core version : " << ros_version;

    bool crash = false;
    ros::param::get("/rec_app_node_crash", crash);
    Environment::instance().rec_app_node_crash = crash;
    nh.setParam("/rec_app_node_crash", false);

    std::string nebula_base_url = "https://api.maxrockrobot.com/nebula";
    std::string nebula_account = "robot";
    std::string nebula_secret = "robot1@nebula";
    if (ParamManager::instance().getCloudInteractiveEnvironment()) {
        nebula_base_url = "https://testapi.maxrockrobot.com/nebula";
        nebula_account = "robot";
        nebula_secret = "robot1@test";
    }
    Environment::instance().nebula_base_url = nebula_base_url;
    Environment::instance().nebula_account = nebula_account;
    Environment::instance().nebula_secret = nebula_secret;

    std::string path;
    path.append("/opt/robot/robot_hw_info.yaml");
    if (sh::File::exists(path)) {
        YAML::Node config = YAML::LoadFile(path);
        YAML::Node deviceSecretNode = config["device_secret"];
        YAML::Node deviceNameNode = config["device_name"];
        if (!deviceNameNode.IsNull()) {
            std::string deviceName = deviceNameNode.as<std::string>();
            Environment::instance().device_name = deviceName;
        }
    }


    int inu_firing_launch_interval;
    nh.param<int>("inu_firing_launch_interval", inu_firing_launch_interval, 20);
    Environment::instance().inu_firing_launch_interval = inu_firing_launch_interval;
    int inu_launch_middle_interval;
    nh.param<int>("inu_launch_middle_interval", inu_launch_middle_interval, 2);
    Environment::instance().inu_launch_middle_interval = inu_launch_middle_interval;
    int inu_final_confirmation_interval;
    nh.param<int>("inu_final_confirmation_interval", inu_final_confirmation_interval, 10);
    Environment::instance().inu_final_confirmation_interval = inu_final_confirmation_interval;
    int inu_start_last_stop_server_interval;
    nh.param<int>("inu_start_last_stop_server_interval", inu_start_last_stop_server_interval, 10);
    Environment::instance().inu_start_last_stop_server_interval = inu_start_last_stop_server_interval;

    bool no_station_mapping_mode;
    nh.param<bool>("no_station_mapping_mode", no_station_mapping_mode, false);
    if (no_station_mapping_mode)
        Environment::instance().no_station_mapping_mode = no_station_mapping_mode;


    int the_interval_between_two_messages;
    nh.param<int>("the_interval_between_two_messages", the_interval_between_two_messages, 300);
    Environment::instance().the_interval_between_two_messages = the_interval_between_two_messages;
    int maximum_number_of_retry_attempts_for_errors_to_the_elevator;
    nh.param<int>("maximum_number_of_retry_attempts_for_errors_to_the_elevator",
                  maximum_number_of_retry_attempts_for_errors_to_the_elevator, 2);
    Environment::instance().maximum_number_of_retry_attempts_for_errors_to_the_elevator = maximum_number_of_retry_attempts_for_errors_to_the_elevator;
    int maximum_number_of_retries_for_elevator_logic_errors;
    nh.param<int>("maximum_number_of_retries_for_elevator_logic_errors",
                  maximum_number_of_retries_for_elevator_logic_errors, 4);
    Environment::instance().maximum_number_of_retries_for_elevator_logic_errors = maximum_number_of_retries_for_elevator_logic_errors;
    int maximum_number_of_entering_the_elevator;
    nh.param<int>("maximum_number_of_entering_the_elevator", maximum_number_of_entering_the_elevator, 1);
    Environment::instance().maximum_number_of_entering_the_elevator = maximum_number_of_entering_the_elevator;
    int the_time_interval_for_continuously_lighting_up_floors;
    nh.param<int>("the_time_interval_for_continuously_lighting_up_floors",
                  the_time_interval_for_continuously_lighting_up_floors, 3000);
    Environment::instance().the_time_interval_for_continuously_lighting_up_floors = the_time_interval_for_continuously_lighting_up_floors;
    int the_time_interval_for_continuous_floor_queries;
    nh.param<int>("the_time_interval_for_continuous_floor_queries", the_time_interval_for_continuous_floor_queries,
                  1000);
    Environment::instance().the_time_interval_for_continuous_floor_queries = the_time_interval_for_continuous_floor_queries;
    int the_time_interval_for_continuous_floor_determination;
    nh.param<int>("the_time_interval_for_continuous_floor_determination",
                  the_time_interval_for_continuous_floor_determination, 1000);
    Environment::instance().the_time_interval_for_continuous_floor_determination = the_time_interval_for_continuous_floor_determination;
    int maximum_waiting_time_for_elevator;
    nh.param<int>("maximum_waiting_time_for_elevator", maximum_waiting_time_for_elevator, 60 * 10 * 1000);
    Environment::instance().maximum_waiting_time_for_elevator = maximum_waiting_time_for_elevator;
    int maximum_time_for_entering_and_exiting_the_elevator;
    nh.param<int>("maximum_time_for_entering_and_exiting_the_elevator",
                  maximum_time_for_entering_and_exiting_the_elevator, 60 * 2 * 1000);
    Environment::instance().maximum_time_for_entering_and_exiting_the_elevator = maximum_time_for_entering_and_exiting_the_elevator;
    double entering_inner_steering_speed;
    nh.param<double>("entering_inner_steering_speed", entering_inner_steering_speed, 0.4);
    Environment::instance().entering_inner_steering_speed = entering_inner_steering_speed;
    bool serial_port_send_print;
    nh.param<bool>("serial_port_send_print", serial_port_send_print, false);
    Environment::instance().serial_port_send_print = serial_port_send_print;
    bool serial_port_accept_print;
    nh.param<bool>("serial_port_accept_print", serial_port_accept_print, false);
    Environment::instance().serial_port_accept_print = serial_port_accept_print;
    bool jump_elevator_status_door_state;
    nh.param<bool>("jump_elevator_status_door_state", jump_elevator_status_door_state, true);
    Environment::instance().jump_elevator_status_door_state = jump_elevator_status_door_state;
    int maximum_delay_time;
    nh.param<int>("maximum_delay_time", maximum_delay_time, 9);
    Environment::instance().maximum_delay_time = maximum_delay_time;


    int pre_circulation_error_retry_count_max;
    nh.param<int>("pre_circulation_error_retry_count_max", pre_circulation_error_retry_count_max, 2);
    Environment::instance().pre_circulation_error_retry_count_max = pre_circulation_error_retry_count_max;
    int post_circulation_error_retry_count_max;
    nh.param<int>("post_circulation_error_retry_count_max", post_circulation_error_retry_count_max, 2);
    Environment::instance().post_circulation_error_retry_count_max = post_circulation_error_retry_count_max;

    int pre_circulation_error_retry_timeout;
    nh.param<int>("pre_circulation_error_retry_timeout", pre_circulation_error_retry_timeout, 30 * 1000);
    Environment::instance().pre_circulation_error_retry_timeout = pre_circulation_error_retry_timeout;
    int post_circulation_error_retry_timeout;
    nh.param<int>("post_circulation_error_retry_timeout", post_circulation_error_retry_timeout, 30 * 1000);
    Environment::instance().post_circulation_error_retry_timeout = post_circulation_error_retry_timeout;

    int outside_from_target_distance;
    nh.param<int>("outside_from_target_distance", outside_from_target_distance, 70);
    Environment::instance().outside_from_target_distance = outside_from_target_distance;

    int pre_elevator_in_error_retry_count_max;
    nh.param<int>("pre_elevator_in_error_retry_count_max", pre_elevator_in_error_retry_count_max, 2);
    Environment::instance().pre_elevator_in_error_retry_count_max = pre_elevator_in_error_retry_count_max;
    int post_elevator_in_error_retry_count_max;
    nh.param<int>("post_elevator_in_error_retry_count_max", post_elevator_in_error_retry_count_max, 2);
    Environment::instance().post_elevator_in_error_retry_count_max = post_elevator_in_error_retry_count_max;

    int pre_elevator_in_error_retry_timeout;
    nh.param<int>("pre_elevator_in_error_retry_timeout", pre_elevator_in_error_retry_timeout, 30 * 1000);
    Environment::instance().pre_elevator_in_error_retry_timeout = pre_elevator_in_error_retry_timeout;
    int post_elevator_in_error_retry_timeout;
    nh.param<int>("post_elevator_in_error_retry_timeout", post_elevator_in_error_retry_timeout, 30 * 1000);
    Environment::instance().post_elevator_in_error_retry_timeout = post_elevator_in_error_retry_timeout;

    double inner_white_pixel_ratio;
    nh.param<double>("inner_white_pixel_ratio", inner_white_pixel_ratio, 0.6);
    Environment::instance().inner_white_pixel_ratio = inner_white_pixel_ratio;
    int internal_spatial_analysis_count;
    nh.param<int>("internal_spatial_analysis_count", internal_spatial_analysis_count, 30);
    Environment::instance().internal_spatial_analysis_count = internal_spatial_analysis_count;


    bool module_elevator;
    nh.param<bool>("module_elevator", module_elevator, false);
    Module::instance().module_elevator = module_elevator;
    bool module_ai;
    nh.param<bool>("module_ai", module_ai, false);
    Module::instance().module_ai = module_ai;
    bool module_property;
    nh.param<bool>("module_property", module_property, false);
    Module::instance().module_property = module_property;
    bool module_udp;
    nh.param<bool>("module_udp", module_udp, true);
    Module::instance().module_udp = module_udp;
    bool module_auto_maintenance;
    nh.param<bool>("module_auto_maintenance", module_auto_maintenance, false);
    Module::instance().module_auto_maintenance = module_auto_maintenance;
    bool module_version;
    nh.param<bool>("module_version", module_version, false);
    Module::instance().module_version = module_version;
    bool module_old_self_check;
    nh.param<bool>("module_old_self_check", module_old_self_check, false);
    Module::instance().module_old_self_check = module_old_self_check;
    bool module_mechanism;
    nh.param<bool>("module_mechanism", module_mechanism, false);
    Module::instance().module_mechanism = module_mechanism;


    bool dependence_imu;
    nh.param<bool>("dependence_imu", dependence_imu, false);
    Module::instance().dependence_imu = dependence_imu;

}

void release() {
    LOG(ERROR) << "Shutdown Shutdown Shutdown ...";
    if (exceptionHandler != nullptr)
        delete exceptionHandler;
    google::ShutdownGoogleLogging(); // 全局关闭glog
    UdpManager::instance().stop();
    WsServerManager::instance().stopWebSocket();
    AiServerManager::instance().stopWebSocket();
    TaskCenter::instance().uninstall();
    ExplorationCenter::instance().uninstall();
}