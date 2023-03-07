//
// Created by Looper on 2023/3/7.
//

#include <list>
#include "future/node/activate_node.h"
#include "glog/logging.h"

ActivateNode::ActivateNode(int seconds) : seconds(seconds) {}

void ActivateNode::setNodeSubject(NodeSubject *nodeSubject) {
    ActivateNode::nodeSubject = nodeSubject;
}

ActivateNodeChain::ActivateNodeChain() = default;

ActivateNodeChain::~ActivateNodeChain() {
    std::list<ActivateNode *>::iterator it;
    for (it = this->activateNodeList.begin(); it != this->activateNodeList.end(); ++it) {
        delete (*it);
        (*it) = NULL;
    }
    delete this->onceConfirm;
    this->onceConfirm = nullptr;
}

void ActivateNodeChain::addActivateNode(ActivateNode *node) {
    activateNodeList.push_back(node);
}

void ActivateNodeChain::setOnceConfirm(OnceConfirm *confirm) {
    onceConfirm = confirm;
}

bool ActivateNodeChain::execute(NodeChain chain) {
    std::list<ActivateNode *>::iterator it;
    for (it = this->activateNodeList.begin(); it != this->activateNodeList.end(); ++it) {
        bool successful = (*it)->execute(chain);
        LOG(ERROR) << (*it)->log(successful);
        chain.setPrevious(successful);
    }
    if (chain.isPrevious()) {
        return onceConfirm->confirm(chain);
    }
    return false;
}
