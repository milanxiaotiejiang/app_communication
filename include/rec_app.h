/***
 * @brief: prihibition areas
 * @author: jiang
 * @date: 20210904
***/

// #ifndef REC_APP_H
// #define REC_APP_H

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
#include <std_msgs/Int16.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>
#include <sensor_msgs/LaserScan.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

#include "model/BaseMethod.h"
#include "model/BaseResult.h"
#include "model/CurrentExecuteTime.h"
#include "model/Entrance.h"
#include "model/MapBan.h"
#include "model/Notice.h"
#include "model/TeachModePoint.h"

#include "prohibition.h"
#include "sub/ScanInnerSubscribe.h"
#include "sub/self_check.h"
#include "tool/CalcArea.h"
#include "tool/Switch.h"
#include "tool/write_file.hpp"
#include <Timer_tool.h>
#include <actionlib/client/simple_action_client.h>
#include <pub/PubOut.h>
#include <sub/BeforeJsonSubscribe.h>
#include <sub/FullPathSubscribe.h>
#include <sub/JsonSubscribe.h>
#include <sub/JsonSubscribeCloud.h>
#include <sub/MapInnerSubscribe.h>
#include <sub/MoveBaseRecoveryFailureSubscribe.h>
#include <tool/Variable.h>

#include "manager/CleanHistoryManager.h"
#include "manager/CombinationManager.h"
#include "manager/NoticeManager.h"
#include "manager/ViewPartManager.h"
#include "manager/UpgradeManager.h"
#include "manager/InternalEventPubManager.h"
#include "net/WsServerManager.h"
#include "net/MessageBusManager.h"

#include "pub/PubInner.h"
#include "net/UdpManager.h"
#include "glog/logging.h"
#include "sys/stat.h"
#include "sys/types.h"
#include "schedule/ScheduleThread.h"
#include "error_log/Error_log.h"
#include "error_log/err_core.h"
#include "sub/DSVersionSubscribe.h"
#include "tool/Msg.hpp"
#include "tool/Queue.hpp"
#include "tool/ThreadPool.h"
#include "manager/PublishInnerManager.h"
#include "manager/PublishOutManager.h"
#include "segmentation/SegmentationCenter.h"
#include "segmentation/SegmentationSubscribe.h"
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
#include "net/WebSocketManager.h"

google_breakpad::ExceptionHandler *exceptionHandler = nullptr;

void judgeEnvironment();

void initLog(char *const *argv);

void initDump();

void initTest(int i, char **pString);

//重启续扫
void restartAfterCrash(string &last_task);

void initNodeParams(const ros::NodeHandle &nh);

void release();