//
// Created by Looper on 2023/3/7.
//

#include "future/node/node_manager.h"

NodeManager::NodeManager(OnceConfirm *onceConfirm) {
    this->activateNodeChain = new ActivateNodeChain();
    this->activateNodeChain->setOnceConfirm(onceConfirm);
}

NodeManager::~NodeManager() {
    delete this->activateNodeChain;
    this->activateNodeChain = nullptr;
}

void NodeManager::addActivateNode(ActivateNode *node) {
    if (nodeSubject != nullptr)
        node->setNodeSubject(nodeSubject);
    this->activateNodeChain->addActivateNode(node);
}

bool NodeManager::activateNode(NodeChain chain) {
    return this->activateNodeChain->execute(chain);
}

void NodeManager::setNodeSubject(NodeSubject *pSubject) {
    this->nodeSubject = pSubject;
}
