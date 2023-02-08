//
// Created by admin1 on 2023/1/4.
//

#ifndef APP_COMMUNICATION_DYNAMICPARAMETER_H
#define APP_COMMUNICATION_DYNAMICPARAMETER_H
#include "glog/logging.h"
#include <dynamic_reconfigure/Reconfigure.h>
#include <ros/ros.h>

template <typename T>
//动态参数，支持double int bool
class Parameter {
public:

  Parameter(std::string nh_name, std::string param_name)
      : nh_(nh_name), param_name_(param_name) {
    LOG(INFO) << "add dynamic param" << std::endl;
    LOG(INFO) << "param namespace: " << nh_.getNamespace() << std::endl;
    LOG(INFO) << "param name: " << param_name_ << std::endl;
    std::string type = typeid(T).name();
    LOG(INFO) << "param type: " << type << std::endl;
    client_ = nh_.serviceClient<dynamic_reconfigure::Reconfigure>(
        "set_parameters");
  }

  bool setParameter(const T param) {
    if (setParameterFunction(param)){
      LOG(INFO) << "param "<<nh_.getNamespace()<<"/"<<param_name_<<" is set to"<< param<< std::endl;
      return true;
    }
    else
    {
      LOG(ERROR) << "param "<<nh_.getNamespace()<<"/"<<param_name_<<" set failed"<< param<< std::endl;
      return false;
    }
  }

private:
  ros::NodeHandle nh_;
  std::string param_name_;
  ros::ServiceClient client_;



  bool setParameterFunction(const bool param) {
    dynamic_reconfigure::Reconfigure bool_reconfigure;
    dynamic_reconfigure::BoolParameter new_bool;
    new_bool.name = param_name_;
    new_bool.value = param;
    bool_reconfigure.request.config.bools.push_back(new_bool);
    try {
        return client_.call(bool_reconfigure);
    } catch (...) {
      ROS_ERROR("Something went wrong in the service call to %s/%s dynamic_reconfigure",nh_.getNamespace().c_str(),param_name_.c_str());
      return false;
    }
  }

  bool setParameterFunction(const int param) {
    dynamic_reconfigure::Reconfigure int_reconfigure;
    dynamic_reconfigure::IntParameter new_int;
    new_int.name = param_name_;
    new_int.value = param;
    int_reconfigure.request.config.ints.push_back(new_int);
    try {
      return client_.call(int_reconfigure);
    } catch (...) {
      ROS_ERROR("Something went wrong in the service call to %s/%s dynamic_reconfigure",nh_.getNamespace().c_str(), param_name_.c_str());
      return false;
    }
  }

  bool setParameterFunction(const double param) {
    dynamic_reconfigure::Reconfigure double_reconfigure;
    dynamic_reconfigure::DoubleParameter new_double;
    new_double.name = param_name_;
    new_double.value = param;
    double_reconfigure.request.config.doubles.push_back(new_double);
    try {
      return client_.call(double_reconfigure);
    } catch (...) {
      ROS_ERROR("Something went wrong in the service call to %s/%s dynamic_reconfigure",nh_.getNamespace().c_str(), param_name_.c_str());
      return false;
    }
  }
};

#endif // APP_COMMUNICATION_DYNAMICPARAMETER_H