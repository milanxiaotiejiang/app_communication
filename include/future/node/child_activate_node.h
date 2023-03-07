//
// Created by Looper on 2023/3/7.
//

#ifndef APP_COMMUNICATION_CHILD_ACTIVATE_NODE_H
#define APP_COMMUNICATION_CHILD_ACTIVATE_NODE_H

#include "activate_node.h"

const std::string N_RVIZ = "roslaunch robot_slam view_nav.launch > rviz.log 2>&1";
const std::string N_DUMP = "rosrun app_communication dump_upload";

class RvizActivateNode : public ActivateNode {
public:
    explicit RvizActivateNode(int seconds);

    ~RvizActivateNode() override;

    bool execute(NodeChain chain) override;
};

class DumpActivateNode : public ActivateNode {
public:
    explicit DumpActivateNode(int seconds);

    ~DumpActivateNode() override;

    bool execute(NodeChain chain) override;
};


#endif //APP_COMMUNICATION_CHILD_ACTIVATE_NODE_H
