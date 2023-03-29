//
// Created by lijiang on 2021/12/18.
//

#include "sub/json/TTStrategy.h"
#include "tool/DangerousThreadPool.h"

string AppAlongCleanStrategy::handler(vector<float> params) {
    //发布贴边命令
    geometry_msgs::Twist clean_msg;//用于发布贴边消息
    clean_msg.linear.x = params[0];//存放目标起点坐标
    clean_msg.linear.y = params[1];
    clean_msg.angular.x = params[2];//存放x
    clean_msg.angular.y = params[3];//存放y
    clean_msg.angular.z = 4.0;//4.0表示贴边模式，3.0 表示全覆盖，2.0表示工作，1表示返回基站，0表示停止
    clean_msg.linear.z = 1.0;//表示工作状态

    ROS_INFO("start point:%f,%f    end point:%f,%f ", clean_msg.linear.x, clean_msg.linear.y,
             clean_msg.angular.x, clean_msg.angular.y);
    return "";
}

string PowerReductionStrategy::handler(string params) {
    NoticeManager::instance().setPlannerType(NoticeManager::instance().getPlannerType() + 1);
    return "";
}

string TTErrorCheck::handler(string params) {
    DangerousThreadPool dangerousThreadPool(1);
    dangerousThreadPool.enqueue([]() {
//        Py_Initialize();
//        PyRun_SimpleString("PyRun_SimpleString");
//        PyRun_SimpleString("import sys");
//        string dirs = "sys.path.append('/home/admin1/test/')";
//        PyRun_SimpleString(dirs.c_str());
//        PyObject *module = PyString_FromString("SystemErrorCheck");
//        PyObject *pyModule = PyImport_Import(module);
//        if (pyModule) {
//            PyObject *pyFUn = PyObject_GetAttrString(pyModule, "systemCheck");
//            if (pyFUn && PyCallable_Check(pyFUn)) {
//                PyObject *argss = PyTuple_New(1);
//                PyObject *arg = PyString_FromString("from c++");
//                PyTuple_SetItem(argss, 0, arg);
//                PyObject *pyRes = PyObject_Call(pyFUn, argss, NULL);
//                if (pyRes) {
//                    string ret = PyString_AsString(pyRes);
//                    LOG(ERROR) << "pyRes " << ret;
//                }
//            }
//        }
//        Py_Finalize();

        std::system("python3 /home/admin1/test/SystemErrorCheck_Robot.py");  // "ls -l > test.txt"
    });

    return "";
}
