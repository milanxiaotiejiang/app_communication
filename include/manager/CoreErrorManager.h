//
// Created by lijiang on 2021/12/18.
//

#ifndef APP_COMMUNICATION_COREERRORMANAGER_H
#define APP_COMMUNICATION_COREERRORMANAGER_H

#include "string"

class CoreErrorManager {

    CoreErrorManager() = default;

    CoreErrorManager(CoreErrorManager &) = delete;

    CoreErrorManager &operator=(const CoreErrorManager &) = delete;

    static CoreErrorManager *m_instance_ptr;

public:
    ~CoreErrorManager() = default;

    static CoreErrorManager *get_instance() {
        if (m_instance_ptr == nullptr) {
            m_instance_ptr = new CoreErrorManager;
        }
        return m_instance_ptr;
    }

    void sendCoreError(int errorCode, const std::string &errorMessage);

};


#endif //APP_COMMUNICATION_COREERRORMANAGER_H
