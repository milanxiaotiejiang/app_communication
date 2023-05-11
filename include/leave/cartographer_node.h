//
// Created by Looper on 2023/4/11.
//

#ifndef APP_COMMUNICATION_CARTOGRAPHER_NODE_H
#define APP_COMMUNICATION_CARTOGRAPHER_NODE_H

#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include "task/async_task_call.h"

/**
 * Topic名称	msg类型	消息内容
/save_map	                std_msgs/Int32	                        接收保存地图消息，需要data为1
/update_map	                std_msgs/Int32	                        接收更新地图消息，需要data为1
/start_carto_mapping	    std_msgs/Int32	                        接收以建图模式启动carto节点消息，需要data为1
/start_carto_localization	std_msgs/Int32	                        接收以定位模式启动carto节点消息，需要data为1
/shutdown_carto	            std_msgs/Int32	                        接收关闭carto定位的消息，需要data为1
/restart_carto	            std_msg/Int32	                        接收重启carto定位的消息，需要data为1
/initial_pose	            geometry_msgs/PoseWithCovarianceStamped	接收重定位消息，需要大致初始位姿
/clear_current_pose	        std_msg/Int32	                        接收重置初始位姿的消息，需要data为1

/update_finish	            std_msgs/Int32	                        发布更新地图信息，1更新完成
/build_map_finish	        std_msgs/Int32	                        发布建图信息，1建图完成
/carto_heart_beat	        std_msgs/Int32	                        发布心跳包，1为正常

 /current_cartographer_pose,当前位姿，作为重启后的定位依据
 /cartographer_work_mode，当前模式，0定位，1建图，2睡眠
 */

class CartographerPublisher {
private:
    ros::Publisher save_map;
    ros::Publisher update_map;
    ros::Publisher start_carto_mapping;
    ros::Publisher start_carto_localization;
    ros::Publisher shutdown_carto;
    ros::Publisher restart_carto;
    ros::Publisher initial_pose;
    ros::Publisher clear_current_pose;
    ros::Publisher control_move_base;
public:
    static auto &instance() {
        static CartographerPublisher obj;
        return obj;
    }

    void initialize(ros::NodeHandle handle);

    void publishSaveMap() const;

    void publishUpdateMap() const;

    void publishStartCartoMapping() const;

    void publishStartCartoLocalization() const;

    void publishShutdownCarto() const;

    void publishRestartCarto() const;

    void publishInitialPose(const geometry_msgs::PoseWithCovarianceStamped &pose) const;

    void publishClearCurrentPose() const;

    void publishControlMoveBase(bool open) const;
};

class CartographerSubscribe {
private:
    AsyncTaskCall *asyncTaskCall;

    ros::Subscriber update_finish;
    ros::Subscriber build_map_finish;
    ros::Subscriber carto_heart_beat;
    ros::Subscriber current_cartographer_pose;

    void updateFinishCallback(const std_msgs::Int32 &carto_result);

    void buildMapFinishCallback(const std_msgs::Int32 &carto_result);

    void cartoHeartBeatCallback(const std_msgs::Int32 &carto_result);

    void currentCartographerPoseCallback(const geometry_msgs::PoseWithCovarianceStamped &carto_result);

public:
    static auto &instance() {
        static CartographerSubscribe obj;
        return obj;
    }

    void initialize(ros::NodeHandle handle);

    void setAsyncTaskCall(AsyncTaskCall *asyncTaskCall) {
        CartographerSubscribe::asyncTaskCall = asyncTaskCall;
    }

    void coverResult();
};

#endif //APP_COMMUNICATION_CARTOGRAPHER_NODE_H
