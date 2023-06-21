#include "dirent.h"
#include "yaml-cpp/yaml.h"
#include <fstream>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Twist.h>
#include <iostream>
#include <sstream>
#include <std_msgs/String.h>
#include <vector>

#include "nlohmann/json.hpp"
#include <cmath>
#include <fstream>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/OccupancyGrid.h>
#include <ros/ros.h>
#include <signal.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>
#include <sensor_msgs/LaserScan.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

#include "net/base/BaseMethod.h"
#include "net/base/BaseResult.h"
#include "net/base/Entrance.h"
#include "model/Notice.h"
#include "model/TeachModePoint.h"

#include "prohibition.h"
#include "sub/self_check.h"
#include "tool/Switch.h"
#include "tool/write_file.hpp"
#include <actionlib/client/simple_action_client.h>
#include <sub/BeforeJsonSubscribe.h>
#include <sub/JsonSubscribe.h>
#include <sub/JsonSubscribeCloud.h>
#include <sub/MapInnerSubscribe.h>
#include <sub/MoveBaseRecoveryFailureSubscribe.h>
#include <tool/Variable.h>

#include "manager/CombinationManager.h"
#include "manager/NoticeManager.h"
#include "manager/ViewPartManager.h"
#include "manager/InternalEventPubManager.h"
#include "net/WsServerManager.h"
#include "net/MessageBusManager.h"
#include "net/AiServerManager.h"

#include "net/UdpManager.h"
#include "glog/logging.h"
#include "sys/stat.h"
#include "sys/types.h"
#include "schedule/ScheduleThread.h"
#include "sub/DSVersionSubscribe.h"
#include "net/poly/Msg.hpp"
#include "net/poly/Queue.hpp"
#include "manager/PublishInnerManager.h"
#include "manager/PublishOutManager.h"
#include "schedule/schedule_manager_singleton.h"
#include "segmentation/SegmentationCenter.h"
#include "exploration/ExplorationCenter.h"
#include "alignment/map_alignment_center.h"
#include "client/linux/handler/exception_handler.h"
#include "third_party/lss/linux_syscall_support.h"
#include "client/linux/handler/minidump_descriptor.h"
#include "task/TaskCenter.h"
#include "future/timer_call.h"
#include "sub/BiasDetectSubscribe.h"
#include "task/manager/NodeWorkModeManager.h"
#include "clean_history/CleanHistoryCenter.h"
#include "model/FullClean.h"
#include "manager/FullCleanManager.h"
#include "sys/wait.h"
#include "leave/map_control.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "leave/ParamManager.h"
#include "leave/auto_maintenance_mode.h"
#include "db/task_data_base.h"
#include "cppfs/fs.h"
#include "cppfs/FileHandle.h"
#include "cppfs/FileIterator.h"
#include "ctime"
#include "time.h"
#include "future/node/node_control.h"
#include "db/property_data_base.h"

google_breakpad::ExceptionHandler *exceptionHandler = nullptr;
std::string unique_identification = boost::uuids::to_string(boost::uuids::random_generator()());
std::string current_program_string = "";
std::string glog_info_time_pid_string = "";

std::string getenv_rec(const std::string &name);

void judgeEnvironment();

void initLog(char *const *argv);

void initDump();

void initTest(int i, char **pString);

void initNodeParams(const ros::NodeHandle &nh);

void release();