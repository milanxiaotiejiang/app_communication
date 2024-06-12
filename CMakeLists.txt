cmake_minimum_required(VERSION 3.0.2)

project(app_communication)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

SET(CMAKE_BUILD_TYPE Debug)
#SET(CMAKE_BUILD_TYPE "Release")
#SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -fPIC")

## --------------------------------------------
SET(MAJOR_VERSION 1)
SET(MINOR_VERSION 0)
SET(PATCH_VERSION 0)

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
        back_charge_msgs
        ai_msgs
)

catkin_package(
        INCLUDE_DIRS include
        LIBRARIES app_communication
        CATKIN_DEPENDS roscpp rospy std_msgs clean_msgs message_runtime
        #  DEPENDS system_lib
)

find_package(CURL REQUIRED)
if (CURL_FOUND)
    message("We have found the curl library. ${CURL}")
else ()
    message(FATAL_ERROR "cURL library not found. Please install it or specify the library path.")
endif ()

find_package(OpenCV REQUIRED)
find_package(eventpp REQUIRED)
find_package(SqliteOrm REQUIRED)
find_package(cppfs REQUIRED)
find_package(Boost COMPONENTS iostreams)

#file(GLOB GLOG_LIBRARIES /usr/lib/x86_64-linux-gnu/libglog.so)
if (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
    file(GLOB GLOG_LIBRARIES "/usr/lib/x86_64-linux-gnu/libglog.so")
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
    file(GLOB GLOG_LIBRARIES "/usr/lib/aarch64-linux-gnu/libglog.so")
else ()
    message(FATAL_ERROR "Unsupported architecture")
endif ()
set(GLOG_INCLUDE_DIRS /usr/include/glog)
message("glog_include_dirs=${GLOG_INCLUDE_DIRS}")
message("glog_libraries=${GLOG_LIBRARIES}")

set(PYTHON_INCLUDE_DIRS "/usr/include/python2.7")
set(BREAK_PAD_LIBRARIES /usr/local/lib/libbreakpad_client.a)
set(BOOST_IOSTREAMS_DIRS ${Boost_INCLUDE_DIRS}/boost/iostreams)

#file(GLOB CURL_LIBRARIES /usr/lib/x86_64-linux-gnu/libcurl.so)
if (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
    file(GLOB CURL_LIBRARIES "/usr/lib/x86_64-linux-gnu/libcurl.so")
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
    file(GLOB CURL_LIBRARIES "/usr/lib/aarch64-linux-gnu/libcurl.so")
else ()
    message(FATAL_ERROR "Unsupported architecture")
endif ()
message("curl_libraries=${CURL_LIBRARIES}")

link_directories(/usr/local/lib)

message("break_pad_libraries=${BREAK_PAD_LIBRARIES}")
message("boost_iostreams_dirs=${BOOST_IOSTREAMS_DIRS}")
message("opencv_include_dirs=${OpenCV_INCLUDE_DIRS}")

include_directories(include /usr/local/include/yaml-cpp)
include_directories(${GLOG_INCLUDE_DIRS})
include_directories(include ${catkin_INCLUDE_DIRS})
include_directories(include ${OpenCV_INCLUDE_DIRS})
include_directories(include ${PYTHON_INCLUDE_DIRS})
include_directories(include /usr/local/include/breakpad)
include_directories(include ${BOOST_IOSTREAMS_DIRS})


message("cmake_current_source_dir=${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB HEADER_FILES_LEVEL_1 "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB SOURCE_FILES_LEVEL_1 "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

file(GLOB HEADER_FILES_LEVEL_2 "${CMAKE_CURRENT_SOURCE_DIR}/include/*/*.h")
file(GLOB SOURCE_FILES_LEVEL_2 "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cpp")

file(GLOB HEADER_FILES_LEVEL_3 "${CMAKE_CURRENT_SOURCE_DIR}/include/*/*/*.h")
file(GLOB SOURCE_FILES_LEVEL_3 "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*/*.cpp")

file(GLOB HEADER_FILES_LEVEL_4 "${CMAKE_CURRENT_SOURCE_DIR}/include/*/*/*/*.h")
file(GLOB SOURCE_FILES_LEVEL_4 "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*/*/*.cpp")


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
        include/exploration/voronoi/voronoi.hpp
        include/exploration/voronoi/nanoflann.hpp
)

add_executable(dump_upload
        dump/dump_upload.cpp
)


target_link_libraries(rec_app_node
        yaml-cpp
        ${catkin_LIBRARIES}
        ${OpenCV_LIBRARIES}
        #        python2.7
        eventpp::eventpp
        sqlite_orm::sqlite_orm sqlite3
        cppfs::cppfs
        ${BREAK_PAD_LIBRARIES}
        ${Boost_LIBRARIES}
        #        CURL::CURL curl
        ${CURL_LIBRARIES}
        ${GLOG_LIBRARIES}
)

target_link_libraries(dump_upload
        ${catkin_LIBRARIES}
        cppfs::cppfs
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

install(FILES ${CPPFS_LIB}
        DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
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
                                                              
