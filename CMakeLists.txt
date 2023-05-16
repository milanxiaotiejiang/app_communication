cmake_minimum_required(VERSION 3.0.2)
project(app_communication)
SET(CMAKE_BUILD_TYPE Debug)

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
        replan_msgs
#        back_charge_msgs
        )

catkin_package(
        INCLUDE_DIRS include
        LIBRARIES app_communication
        CATKIN_DEPENDS roscpp rospy std_msgs clean_msgs message_runtime
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

include_directories(include /usr/local/include/yaml-cpp)
include_directories(${GLOG_INCLUDE_DIRS})
include_directories(include ${catkin_INCLUDE_DIRS})
include_directories(include ${OpenCV_INCLUDE_DIRS})
include_directories(include ${PYTHON_INCLUDE_DIRS})
include_directories(include /usr/local/include/breakpad)

message("cmake_current_source_dir=${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB HEADER_FILES_LEVEL_1 "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB SOURCE_FILES_LEVEL_1 "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

file(GLOB HEADER_FILES_LEVEL_2 "${CMAKE_CURRENT_SOURCE_DIR}/include/*/*.h")
file(GLOB SOURCE_FILES_LEVEL_2 "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cpp")

file(GLOB HEADER_FILES_LEVEL_3 "${CMAKE_CURRENT_SOURCE_DIR}/include/*/*/*.h")
file(GLOB SOURCE_FILES_LEVEL_3 "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*/*.cpp")

file(GLOB HEADER_FILES_LEVEL_4 "${CMAKE_CURRENT_SOURCE_DIR}/include/*/*/*/*.h")
file(GLOB SOURCE_FILES_LEVEL_4 "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*/*/*.cpp")


file(GLOB HEADER_CPP_FS "${CMAKE_CURRENT_SOURCE_DIR}/include/cppfs/*.h")
file(GLOB SOURCE_CPP_FS "${CMAKE_CURRENT_SOURCE_DIR}/src/cppfs/*.cpp")
file(GLOB HEADER_CPP_FS_POSIX "${CMAKE_CURRENT_SOURCE_DIR}/include/cppfs/posix/*.h")
file(GLOB SOURCE_CPP_FS_POSIX "${CMAKE_CURRENT_SOURCE_DIR}/src/cppfs/posix/*.cpp")
file(GLOB HEADER_CPP_FS_LINUX "${CMAKE_CURRENT_SOURCE_DIR}/include/cppfs/linux/*.h")
file(GLOB SOURCE_CPP_FS_LINUX "${CMAKE_CURRENT_SOURCE_DIR}/src/cppfs/linux/*.cpp")

add_executable(rec_app_node
        ${HEADER_FILES_LEVEL_1}
        ${SOURCE_FILES_LEVEL_1}
        ${HEADER_FILES_LEVEL_2}
        ${SOURCE_FILES_LEVEL_2}
        ${HEADER_FILES_LEVEL_3}
        ${SOURCE_FILES_LEVEL_3}
        ${HEADER_FILES_LEVEL_4}
        ${SOURCE_FILES_LEVEL_4}
        ${TEST_FILES}
        include/cppfs/basen.hpp
        )

add_executable(dump_upload
        dump/dump_upload.cpp
        include/cppfs/basen.hpp
        ${HEADER_CPP_FS}
        ${SOURCE_CPP_FS}
        ${HEADER_CPP_FS_POSIX}
        ${SOURCE_CPP_FS_POSIX}
        ${HEADER_CPP_FS_LINUX}
        ${SOURCE_CPP_FS_LINUX}
        )


target_link_libraries(rec_app_node
        yaml-cpp
        ${catkin_LIBRARIES}
        ${OpenCV_LIBRARIES}
        ${GLOG_LIBRARIES}
        python2.7
        #                ${ZLIB_LIBRARIES}
        eventpp::eventpp
        sqlite_orm::sqlite_orm sqlite3
        ${BREAK_PAD_LIBRARIES}
        )

target_link_libraries(dump_upload
        ${catkin_LIBRARIES}
        )


install(PROGRAMS
        DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
        )

install(TARGETS rec_app_node
        RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
        )

install(TARGETS dump_upload
        RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
        )

install(DIRECTORY include/
        DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
        FILES_MATCHING PATTERN "*.h"
        PATTERN ".svn" EXCLUDE
        )


set(other_dir
        params
        scripts
        )


foreach (_dir ${other_dir})
    install(DIRECTORY ${_dir}/
            DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/${_dir})
endforeach ()

#https://blog.csdn.net/Koupoo/article/details/110749240
                                                              
