//
// Created by Looper on 2023/3/7.
//

#ifndef APP_COMMUNICATION_NODE_MANAGER_H
#define APP_COMMUNICATION_NODE_MANAGER_H

#include "activate_node.h"

class NodeManager {
public:
    explicit NodeManager(OnceConfirm *onceConfirm);

    virtual ~NodeManager();

    void addActivateNode(ActivateNode *node);

    bool activateNode(NodeChain chain);

    void setNodeSubject(NodeSubject *pSubject);

private:
    ActivateNodeChain *activateNodeChain;
    NodeSubject *nodeSubject;
};


#endif //APP_COMMUNICATION_NODE_MANAGER_H
