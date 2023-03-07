//
// Created by Looper on 2023/3/7.
//

#ifndef APP_COMMUNICATION_ACTIVATE_NODE_H
#define APP_COMMUNICATION_ACTIVATE_NODE_H

#include "future/thread_pool.h"
#include "node_chain.h"
#include "once_confirm.h"
#include "list"

#define become_silent false

class ActivateNode {
private:
    int seconds{};

    std::mutex wait_mutex;
    std::condition_variable err_cond;
    bool error_monitor = false;

protected:
    template<typename F, typename... Args>
    bool asyncExecute(async::ThreadPool *pool, F &&f, Args &&... args) {
        auto func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);

        pool->execute([this, t = std::move(func)]() {
            t();

            std::unique_lock<std::mutex> guard(wait_mutex);
            error_monitor = true;
            err_cond.notify_one();
        });

        std::unique_lock<std::mutex> lck(wait_mutex);
        err_cond.wait_for(lck, std::chrono::seconds(seconds), [this]() {
            return error_monitor;
        });

        return !error_monitor;
    }

public:
    explicit ActivateNode(int seconds);

    virtual ~ActivateNode() = default;;

    virtual bool execute(NodeChain chain) = 0;

    virtual std::string log(bool isSuccessful) {
        std::string name = typeid(*this).name();
        if (isSuccessful)
            return name + " Node started successfully !!!";
        else
            return name + " Node startup failed !!!";
    };
};

class ActivateNodeChain {
private:
    std::list<ActivateNode *> activateNodeList;
    OnceConfirm *onceConfirm{};

public:
    ActivateNodeChain();

    virtual ~ActivateNodeChain();

    void addActivateNode(ActivateNode *node);

    void setOnceConfirm(OnceConfirm *confirm);

    bool execute(NodeChain chain);
};


#endif //APP_COMMUNICATION_ACTIVATE_NODE_H
