//
// Created by Looper on 2023/3/7.
//

#include "future/node/node_control_subscribe.h"
#include "future/node/node_control.h"
#include "task/manager/NodeWorkModeManager.h"

/**
 * Topic名称	msg类型	消息内容
/save_map	                std_msgs/Int32	                        接收保存地图消息，需要data为1
/update_map	                std_msgs/Int32	                        接收更新地图消息，需要data为1
/start_carto_mapping	    std_msgs/Int32	                        接收以建图模式启动carto节点消息，需要data为1
/start_carto_localization	std_msgs/Int32	                        接收以定位模式启动carto节点消息，需要data为1
/shutdown_carto	            std_msgs/Int32	                        接收关闭carto定位的消息，需要data为1
/restart_carto	            std_msg/Int32	                        接收重启carto定位的消息，需要data为1
/initialpose	            geometry_msgs/PoseWithCovarianceStamped	接收重定位消息，需要大致初始位姿
/clear_current_pose	        std_msg/Int32	                        接收重置初始位姿的消息，需要data为1
/update_finish	            std_msgs/Int32	                        发布更新地图信息，1更新完成
/build_map_finish	        std_msgs/Int32	                        发布建图信息，1建图完成
/carto_heart_beat	        std_msgs/Int32	                        发布心跳包，1为正常
 */

NodeControlSubscribe::NodeControlSubscribe(ros::NodeHandle handle) {
    sub_node_control_ = handle.subscribe("/node_control", 1, &NodeControlSubscribe::nodeControlSubscribeCallback, this);
    sub_initial_pose_ = handle.subscribe("/initialpose", 1, &NodeControlSubscribe::initialPoseSubscribeCallback, this);
}

void NodeControlSubscribe::nodeControlSubscribeCallback(const std_msgs::Int32 &flag_result) {
    auto flag = flag_result.data;
    LOG(INFO) << "NodeControlSubscribe : " << flag;
    if (flag == WorkMode::SLEEPING) {
        NodeControl::instance().changeSleepMode();
    } else if (flag == WorkMode::WORKING) {
        NodeControl::instance().changeWorkMode();
    } else if (flag == WorkMode::MAPPING) {
        NodeControl::instance().changeMapMode();
    }
}

void
NodeControlSubscribe::initialPoseSubscribeCallback(const geometry_msgs::PoseWithCovarianceStampedConstPtr &pose) {
    auto init_pose = pose->pose.pose;
    LOG(INFO) << "给定一个指定的位置来重定位carto,x : " << init_pose.position.x << " y: " << init_pose.position.y;
    //关闭cartographer
    NodeControl::instance().shutdownCartoNodeOnly();
    //设定初始位姿
    NodeControl::instance().paramPose("/set_initial_pose", init_pose);
}
