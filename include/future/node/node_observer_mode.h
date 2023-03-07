//
// Created by Looper on 2023/3/7.
//

#ifndef APP_COMMUNICATION_NODE_OBSERVER_MODE_H
#define APP_COMMUNICATION_NODE_OBSERVER_MODE_H

#include <list>

class NodeObserver {
public:
    virtual void update() = 0;
};

class NodeSubject {
public:
    virtual void attach(NodeObserver *observer) = 0;

    virtual void detach(NodeObserver *observer) = 0;

    virtual void notify() = 0;
};

class AbnormalObserver : public NodeObserver {
private:
    NodeSubject *nodeSubject;
public:
    explicit AbnormalObserver(NodeSubject *nodeSubject);

    void update() override;
};

class AbnormalSubject : public NodeSubject {
private:
    std::list<NodeObserver *> nodeObserverList;
public:
    void attach(NodeObserver *observer) override;

    void detach(NodeObserver *observer) override;

    void notify() override;
};

#endif //APP_COMMUNICATION_NODE_OBSERVER_MODE_H
