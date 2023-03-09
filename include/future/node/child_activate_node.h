//
// Created by Looper on 2023/3/7.
//

#ifndef APP_COMMUNICATION_CHILD_ACTIVATE_NODE_H
#define APP_COMMUNICATION_CHILD_ACTIVATE_NODE_H

#include "activate_node.h"

const std::string N_RVIZ = "roslaunch robot_slam view_nav.launch > rviz.log 2>&1";

const std::string n_build_mapping = "roslaunch launch_center build_mapping.launch > launch_center_build_mapping.log 2>&1";
const std::string n_submap_to_map = "roslaunch launch_center submap_to_map.launch > launch_center_submap_to_map.log 2>&1";
const std::string n_localization = "roslaunch launch_center localization.launch > launch_center_localization.log 2>&1";
const std::string n_load_map = "roslaunch launch_center load_map.launch > launch_center_load_map.log 2>&1";
const std::string n_navigation = "roslaunch launch_center navigation.launch > launch_center_navigation.log 2>&1";

class RvizActivateNode : public ActivateNode {
public:
    explicit RvizActivateNode(int seconds);

    ~RvizActivateNode() override;

    bool execute(NodeChain chain) override;
};

class BuildMappingActivateNode : public ActivateNode {
public:
    explicit BuildMappingActivateNode(int seconds);

    ~BuildMappingActivateNode() override;

    bool execute(NodeChain chain) override;
};

class SubmapToMapActivateNode : public ActivateNode {
public:
    explicit SubmapToMapActivateNode(int seconds);

    ~SubmapToMapActivateNode() override;

    bool execute(NodeChain chain) override;
};

class LocalizationActivateNode : public ActivateNode {
public:
    explicit LocalizationActivateNode(int seconds);

    ~LocalizationActivateNode() override;

    bool execute(NodeChain chain) override;
};

class LoadMapActivateNode : public ActivateNode {
public:
    explicit LoadMapActivateNode(int seconds);

    ~LoadMapActivateNode() override;

    bool execute(NodeChain chain) override;
};

class NavigationActivateNode : public ActivateNode {
public:
    explicit NavigationActivateNode(int seconds);

    ~NavigationActivateNode() override;

    bool execute(NodeChain chain) override;
};

#endif //APP_COMMUNICATION_CHILD_ACTIVATE_NODE_H
