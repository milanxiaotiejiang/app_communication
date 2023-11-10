//
// Created by Looper on 2023/3/7.
//

#include "future/node/node_observer_mode.h"
#include "simulation.h"
#include "future/node/node_control.h"

AbnormalObserver::AbnormalObserver(NodeSubject *nodeSubject) : nodeSubject(nodeSubject) {}

void AbnormalObserver::update() {
    NodeControl::instance().update();
}

void AbnormalSubject::attach(NodeObserver *observer) {
    nodeObserverList.push_back(observer);
}

void AbnormalSubject::detach(NodeObserver *observer) {
    nodeObserverList.remove(observer);
}

void AbnormalSubject::notify() {
    auto it = nodeObserverList.begin();
    while (it != nodeObserverList.end()) {
        (*it)->update();
        it++;
    }
}
