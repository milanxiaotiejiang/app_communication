//
// Created by noodles on 24-5-20.
//

#ifndef APP_COMMUNICATION_MODULE_H
#define APP_COMMUNICATION_MODULE_H


class Module {
private:
    Module() = default;

    Module(Module &) = delete;

    Module &operator=(const Module &) = delete;

public:
    ~Module() = default;

public:
    static auto &instance() {
        static Module obj;
        return obj;
    }

    bool module_property{false};
    bool module_udp{true};
    bool module_auto_maintenance{false};
    bool module_version{false};
    bool module_old_self_check{false};
    bool module_ai{false};
    bool module_elevator{false};
    bool module_mechanism{false};


    bool dependence_imu{false};
    int number_of_tasks{1};

};


#endif //APP_COMMUNICATION_MODULE_H
