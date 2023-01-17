cmake_minimum_required(VERSION 3.0.2)
project(app_communication)
SET(CMAKE_BUILD_TYPE Debug)
## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages

## --------------------------------------------
SET(MAJOR_VERSION 1)
SET(MINOR_VERSION 0)
SET(PATCH_VERSION 2)

#说明要生成的是deb包
SET(CPACK_GENERATOR "DEB")

##############设置debian/control文件中的内容###############

#设置版本信息
SET(CPACK_PACKAGE_VERSION_MAJOR "${MAJOR_VERSION}")
SET(CPACK_PACKAGE_VERSION_MINOR "${MINOR_VERSION}")
SET(CPACK_PACKAGE_VERSION_PATCH "${PATCH_VERSION}")

#设置安装包的包名，打好的包将会是packagename-version-linux.deb，如果不设置，默认是工程名
set(CPACK_PACKAGE_NAME "com.maxrock.robot.communication")

#设置程序名，就是程序安装后的名字
set(CPACK_DEBIAN_PACKAGE_NAME "communication")

#设置架构
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")

#设置依赖
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.3.1-6)")

#设置description
SET(CPACK_PACKAGE_DESCRIPTION "pro-cc service")

#设置联系方式
SET(CPACK_PACKAGE_CONTACT "765151629@qq.com")

#设置维护人
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "milanxiaotiejiang")

include(CPack)

## --------------------------------------------

set(CMAKE_CXX_STANDARD 14)
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

find_package(catkin REQUIRED COMPONENTS
        roscpp
        rospy
        roslib
        std_msgs
        tf
        actionlib
        costmap_2d
        geometry_msgs
        move_base_msgs
        nav_msgs
        zoo_bringup
        clean_msgs
        message_generation
        eigen_conversions
        )

catkin_package(
        INCLUDE_DIRS include
        LIBRARIES app_communication
        CATKIN_DEPENDS roscpp rospy std_msgs clean_msgs
        CATKIN_DEPENDS message_runtime
        #  DEPENDS system_lib
)

find_package(OpenCV REQUIRED)
find_package(eventpp)
find_package(SqliteOrm REQUIRED)

file(GLOB GLOG_LIBRARIES /usr/lib/x86_64-linux-gnu/libglog.so)
set(GLOG_INCLUDE_DIRS /usr/include)
#set(ZLIB_LIBRARIES /usr/local/lib/libz.so)
set(PYTHON_INCLUDE_DIRS "/usr/include/python2.7")
set(BREAK_PAD_LIBRARIES /usr/local/lib/libbreakpad_client.a)

link_directories(/usr/local/lib)

message("glog_include_dirs=${GLOG_INCLUDE_DIRS}")
message("glog_libraries=${GLOG_LIBRARIES}")
message("zlib_libraries=${ZLIB_LIBRARIES}")
message("break_pad_libraries=${BREAK_PAD_LIBRARIES}")

include_directories(/usr/local/include/yaml-cpp)
include_directories(${GLOG_INCLUDE_DIRS})
include_directories(include ${catkin_INCLUDE_DIRS})
include_directories(include ${OpenCV_INCLUDE_DIRS})
include_directories(include ${PYTHON_INCLUDE_DIRS})
include_directories(include /usr/local/include/breakpad)

add_executable(rec_app_node
        src/rec_app_node.cpp
        src/prohibition.cpp
        src/tool/read_partion_file.cpp
        src/tool/Switch.cpp
        src/tool/Timer_tool.cpp
        src/tool/Variable.cpp
        src/tool/Tool_Function.cpp
        src/tool/Quaternion.cpp
        src/tool/Msg.cpp
        src/tool/Queue.cpp

        include/model/Entrance.h
        include/model/BaseMethod.h
        include/model/BaseResult.h
        include/model/BasePublish.h
        include/model/NetModel.h
        include/model/VersionInfo.h
        include/model/VersionSubscribe.h


        src/model/MethodAlongWall.cpp
        src/model/Task.cpp
        src/model/WorkStatus.cpp
        src/model/DeviceStatus.cpp
        src/model/DeviceStatusV2.cpp
        src/model/MapInfo.cpp
        src/model/CleanContinuity.cpp
        src/model/Notice.cpp
        src/model/TaskPoint.cpp
        src/model/MapBan.cpp
        src/model/MethodMap.cpp
        src/model/MethodExecuteTask.cpp
        src/model/CoreError.cpp
        src/model/ShowWorkStatus.cpp
        src/model/ShowCloudStatus.cpp
        src/model/point.cpp
        src/model/TeachModePoint.cpp
        src/model/Continuity_info.cpp
        src/model/Polygon_info.cpp
        src/model/CleanPolygon.cpp
        src/model/CleanHistory.cpp
        src/model/Combination.cpp
        src/model/ViewPart.cpp
        src/model/RegisterOperation.cpp
        src/model/RequestModel.cpp
        src/model/Location.cpp
        src/model/Project.cpp
        src/model/Partion.cpp
        src/model/MaterialStatus.cpp
        src/model/UpgradeModel.cpp
        src/model/KnobStatus.cpp
        src/model/RoomVo.cpp

        src/manager/NoticeManager.cpp
        src/manager/CoreErrorManager.cpp
        src/manager/CleanHistoryManager.cpp
        src/manager/CombinationManager.cpp
        src/manager/ViewPartManager.cpp
        src/manager/VersionManager.cpp
        src/manager/UpgradeManager.cpp
        include/manager/PublishOutManager.h
        include/manager/PublishInnerManager.h

        src/pub/PubInner.cpp
        src/pub/PubOut.cpp
        src/sub/AbstractSubscribe.cpp
        src/sub/JsonSubscribe.cpp
        src/sub/JsonSubscribeCloud.cpp
        src/sub/BeforeJsonSubscribe.cpp
        src/sub/MapInnerSubscribe.cpp
        src/sub/OdomSubscribe.cpp
        src/sub/FullPathSubscribe.cpp
        src/sub/ScanInnerSubscribe.cpp
        src/sub/OdomInnerSubscribe.cpp
        src/sub/self_check_class_type.cpp
        src/sub/MovebaseRecoveryFailureSubscribe.cpp
        src/sub/DSVersionSubscribe.cpp
        src/sub/BiasDetectSubscribe.cpp
        ## src/sub/CurrentInnerSubscribe.cpp

        include/sub/json/MessageStrategy.h
        include/sub/json/MessageContext.h
        include/sub/json/TimerStrategy.h
        include/model/TimerInfo.h
        src/model/TimerInfo.cpp
        include/model/OtaInfo.h
        src/model/OtaInfo.cpp
        src/sub/json/DeviceStrategy.cpp
        src/sub/json/TTStrategy.cpp
        src/sub/json/MapStrategy.cpp
        src/sub/json/TaskStrategy.cpp
        src/sub/json/StatusStrategy.cpp
        src/sub/json/ModeStrategy.cpp
        src/sub/json/TeachModeStrategy.cpp
        src/sub/json/GetCleanHistoryStrategy.cpp
        src/sub/json/CombinationStrategy.cpp
        src/sub/json/ViewPartStrategy.cpp
        src/sub/json/TimerStrategy.cpp
        src/sub/json/LocationStrategy.cpp
        src/sub/json/ProjectStrategy.cpp
        src/sub/json/MaterialStrategy.cpp
        src/sub/json/NoticeStrategy.cpp
        src/sub/json/otaStrategy.cpp
        src/sub/json/CloudDeviceStrategy.cpp
        src/sub/json/KnobControlStrategy.cpp
        src/sub/json/FullCleaningModeStrategy.cpp
        src/segmentation/SegmentationStrategy.cpp
        src/exploration/ExplorationStrategy.cpp

        src/net/WsServerManager.cpp
        include/net/UdpManager.h
        include/net/MessageBusManager.h
        include/net/NonCopyable.h
        include/net/MessageBus.h
        include/net/Any.h
        include/net/function_traits.h

        include/BlockingCollection.h
        include/tool/DangerousThreadPool.h
        include/tool/ZLibString.hpp
        src/tool/CalcArea.cpp
        #        include/net/UdpManager.h
        src/tool/CThread.cpp
        src/tool/base64.cpp
        #        include/net/WebSocketManager.h
        #        include/net/WebClientSubscribeLocally.h
        #        include/net/WebClientSubscriber.h
        #        include/net/WebServer.h
        include/schedule/Cron.h
        include/schedule/CronClock.h
        include/schedule/CronData.h
        include/schedule/CronRandomization.h
        include/schedule/CronSchedule.h
        include/schedule/DateTime.h
        include/schedule/TaskCron.h
        include/schedule/TaskQueue.h
        include/schedule/TimeTypes.h
        include/schedule/ScheduleThread.h
        include/error_log/Error_log.h
        include/error_log/err_core.h
        include/BaseThrowable.h

        src/schedule/CronClock.cpp
        src/schedule/CronData.cpp
        src/schedule/CronRandomization.cpp
        src/schedule/CronSchedule.cpp
        src/schedule/ScheduleThread.cpp
        src/schedule/TaskCron.cpp
        src/error_log/err_core.cpp
        src/error_log/Error_log.cpp

        include/db/SqliteDataBase.h
        include/segmentation/fast_math.h
        src/db/DBRoom.cpp
        src/db/segmentation_data_base.cpp
        src/segmentation/contains.cpp
        src/segmentation/CvUtils.cpp
        src/segmentation/Room.cpp
        src/segmentation/abstract_voronoi_segmentation.cpp
        src/segmentation/VoronoiSegmentation.cpp
        src/segmentation/wavefront_region_growing.cpp
        src/segmentation/handle_segmentation_display.cpp
        src/segmentation/meanshift2d.cpp
        src/segmentation/SegmentationCenter.cpp
        src/segmentation/SegmentationSubscribe.cpp
        src/exploration/ExplorationCenter.cpp
        src/exploration/boustrophedon_explorator.cpp
        src/exploration/room_rotator.cpp
        include/exploration/histogram.h
        include/exploration/grid.h
        src/exploration/tsp/nearest_neighbor_TSP.cpp
        src/exploration/A_star_pathplanner.cpp
        src/exploration/node.cpp
        include/exploration/timer.h
        include/exploration/distance_matrix.h
        src/exploration/tsp/genetic_TSP.cpp
        src/exploration/handle_exploration_display.cpp
        src/exploration/pose/tracked_pose_subscribe.cpp
        src/exploration/pose/amcl_pose_subscribe.cpp
        src/segmentation/map_attribute.cpp
        include/exploration/tsp/tsp_solver_defines.h
        src/exploration/energy_functional_explorator.cpp
        src/alignment/map_alignment_center.cpp
        src/alignment/alignment_utils.cpp
        include/exploration/lru_cache.h

        src/task/task_dispatcher.cpp
        src/task/async_task_call.cpp
        src/task/task_generator.cpp
        src/task/TaskCenter.cpp
        src/task/point_routine.cpp
        src/task/point_generator.cpp
        src/task/point_planner.cpp
        include/task/RealPoint.h
        include/task/RealTask.h
        src/future/async_call.cpp
        src/exploration/radian_angle_conversion.cpp
        include/task/status/state_machine.h

        include/future/async_defer.h
        include/future/try.h
        include/future/future.h
        include/future/scheduler.h
        src/future/timer.cpp
        src/future/thread_pool.cpp
        src/future/event_loop.cpp
        src/future/timer_call.cpp
        src/exploration/line.cpp
        src/task/subscribe/ZooRobotStatusSubscribe.cpp
        src/task/manager/NodeWorkModeManager.cpp
        include/task/subscribe/zoo_inner_status.h
        src/task/subscribe/FlagResultSubscribe.cpp
        src/task/subscribe/StationManager.cpp
        src/task/manager/MechanismManager.cpp
        src/task/manager/manual.cpp
        include/task/simulation.h
        src/task/call/head_tail_call.cpp
        src/task/call/timely_call.cpp
        src/task/model/PointProgressVo.cpp
        src/task/manager/PointProgressPublish.cpp
        include/task/subscribe/async_machine.h
        src/task/manager/SwitchModePublish.cpp
        src/task/manager/NativeSystemManager.cpp
        src/clean_history/clean_history_model.cpp
        src/clean_history/CleanHistoryDataBase.cpp
        src/clean_history/CleanHistoryCenter.cpp 
        src/manager/FullPlanWorkStatusManage.cpp
        src/task/model/CombinationPoseVo.cpp
        src/model/FullClean.cpp
        src/manager/FullCleanManager.cpp
        src/manager/DynamicReconfigureManager.cpp include/manager/DynamicReconfigureManager.h)


target_link_libraries(rec_app_node
        yaml-cpp
        ${catkin_LIBRARIES}
        ${OpenCV_LIBRARIES}
        ${GLOG_LIBRARIES}
        python2.7
        #        ${ZLIB_LIBRARIES}
        eventpp::eventpp
        sqlite_orm::sqlite_orm sqlite3
        ${BREAK_PAD_LIBRARIES}
        )


install(PROGRAMS
        DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
        )

install(TARGETS rec_app_node
        RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
        )

install(DIRECTORY include/
        DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
        FILES_MATCHING PATTERN "*.h"
        PATTERN ".svn" EXCLUDE
        )


set(other_dir params)#params package.xml


foreach (_dir ${other_dir})
    install(DIRECTORY ${_dir}/
            DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/${_dir})
endforeach ()

#https://blog.csdn.net/Koupoo/article/details/110749240
                                                              
