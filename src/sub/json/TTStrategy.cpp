//
// Created by lijiang on 2021/12/18.
//

#include "sub/json/TTStrategy.h"
#include "tool/DangerousThreadPool.h"

std::string TTErrorCheck::handler(std::string params) {
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
