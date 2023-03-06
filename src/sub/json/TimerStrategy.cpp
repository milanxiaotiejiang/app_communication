//
// Created by Paul wang on 2022/05/09.
//

#include "sub/json/TimerStrategy.h"
#include "ros/package.h"
#include "glog/logging.h"
#include "db/path.h"
#include <model/Entrance.h>


string AddTimerStrategy::handler(TimerInfo params) {

    string fileName;
    string sss;
    fileName.append(path::data_base_config_dir());
    fileName.append("timer_info_json.txt");
    //设置清扫计时器
    //sh::File *fff = new sh::File(fileName);
    std::shared_ptr<sh::File> fff = make_shared<sh::File>(fileName);
    if (!fff->open(std::ios::in)) {
        if (!fff->create(fileName)) {
            throw app::exception(make_error_code(error::create_file_timer_fail));
        }
    }
    cout << "begin add timeinfo file" << endl;
    if (fff->open(std::ios::in)) {//打开成功
        sss = fff->readAll();
        std::vector<TimerInfo> timerInfoTask;//最终存储文件
        if (sss.length() > 0)                //文件不为空
        {
            json jdecode_l = json::parse(sss);
            std::vector<TimerInfo> param = jdecode_l.get<std::vector<TimerInfo>>();//数据内容，结构体格式
            for (int i = 0; i < param.size(); i++) timerInfoTask.push_back(param[i]);
        }
        cout << " add timeinfo file  36 " << endl;
        cout << params.getTimerRule() << endl;
        /*  TimerInfo tinf(params.getTimerId(),
                         params.getTimerRule(),
                         //params.getTaskName(),
                         params.getTimerName(),
                         params.getTaskId(),
                         params.getIsExecute(),
                         params.getRate(),
                         params.getIsNever(),
                         params.getIsSkip(),
                         params.getEndYear(),
                         params.getEndMonth(),
                         params.getEndDay());*/
        TimerInfo tinf(params.getTimerId(),
                       params.getTimerRule(),

                       params.getTimerName(),
                       params.getTaskId(),
                       params.getIsExecute(),
                       params.getRate(),
                       params.getTaskName(),
                       params.getIsNever(),
                       params.getIsSkip(),
                       params.getEndYear(),
                       params.getEndMonth(),
                       params.getEndDay());

        timerInfoTask.push_back(tinf);


        json params_l = timerInfoTask;
        string base64;
        base64.append(params_l.dump());//json转base64 string
        if (!sh::File::saveTextTo(fileName, base64)) {
            throw app::exception(make_error_code(error::save_file_timer_failed));
        }
        timerInfoTask.clear();
        timerInfoTask.shrink_to_fit();

        fff->close();
        std_msgs::String ret;
        json js;
        js["method"] = "add";
        js["timerid"] = params.getTimerId();
        ret.data.append(js.dump());

        PublishOutManager::instance().publishAppSchedule(ret);

    } else {
        throw app::exception(make_error_code(error::open_file_timer_fail));
    }
    json jj;
    jj["code"] = 0;
    return jj.dump();

}

string UpdateTimerStrategy::handler(TimerInfo params) {
    string fileName;
    string sss;
    fileName.append(path::data_base_config_dir());
    fileName.append("timer_info_json.txt");

    // *fff = new sh::File(fileName);
    std::shared_ptr<sh::File> fff = make_shared<sh::File>(fileName);
    string base64;
    if (fff->open(std::ios::in)) {
        sss = fff->readAll();
    } else {
        throw app::exception(make_error_code(error::open_file_timer_fail));
    }
    std_msgs::String result;
    if (sss.length() > 0)//不为空
    {
        json jdecode_l = json::parse(sss);
        std::vector<TimerInfo> timer_infos = jdecode_l.get<std::vector<TimerInfo>>();//数据内容，结构体格式

        int timers_num = timer_infos.size();
        bool bb = params.getIsExecute();

        for (int i = 0; i < timer_infos.size(); i++) {
            if (timer_infos[i].getTimerId() == params.getTimerId()) {
                timer_infos[i].setIsExecute(params.getIsExecute());
                timer_infos[i].setRate(params.getRate());
                timer_infos[i].setTaskId(params.getTaskId());
                timer_infos[i].setTimerRule(params.getTimerRule());
                timer_infos[i].setTimerName(params.getTimerName());
                timer_infos[i].setTaskName(params.getTaskName());
                timer_infos[i].setIsNever(params.getIsNever());
                timer_infos[i].setIsSkip(params.getIsSkip());
                timer_infos[i].setEndYear(params.getEndYear());
                timer_infos[i].setEndMonth(params.getEndMonth());
                timer_infos[i].setEndDay(params.getEndDay());
                //
                break;
            }
        }
        fff->close();

        json params_l = timer_infos;
        string base64;
        base64.append(params_l.dump());//json转base64 string

        if (!sh::File::saveTextTo(fileName, base64)) {
            throw app::exception(make_error_code(error::save_file_timer_failed));
        }
        timer_infos.clear();
        timer_infos.shrink_to_fit();
        std_msgs::String ret;


        json js;
        js["method"] = "upd";
        js["timerid"] = params.getTimerId();
        ret.data.append(js.dump());
        PublishOutManager::instance().publishAppSchedule(ret);


    } else {
        throw app::exception(make_error_code(error::open_file_timer_fail));
    }

    //return "";
    json jj;
    jj["code"] = 0;
    return jj.dump();
}

vector<TimerInfo> GetTimerListStrategy::handler(string params) {
    string fileName;
    string sss;
    fileName.append(path::data_base_config_dir());
    fileName.append("timer_info_json.txt");

    //sh::File *fff = new sh::File(fileName);
    std::shared_ptr<sh::File> fff = make_shared<sh::File>(fileName);
    string base64;
    if (fff->open(std::ios::in)) {
        sss = fff->readAll();
    } else {
        throw app::exception(make_error_code(error::open_file_timer_fail));
    }
    std_msgs::String result;
    std::vector<TimerInfo> timer_info;
    if (sss.length() > 0)//不为空
    {
        json jdecode = json::parse(sss);

//        LOG(WARNING) << "GetTimerListStrategy " << jdecode;
        timer_info = jdecode.get<std::vector<TimerInfo>>();//数据内容，结构体格式
    }
    return timer_info;
}

string DeleteTimerStrategy::handler(string params) {
    //    使能/失能清扫计时器
    string fileName;
    string path;
    fileName.append(path::data_base_config_dir());
    fileName.append("timer_info_json.txt");
    cout << "timer_infos 214 " << endl;
    //sh::File *fff = new sh::File(fileName);
    std::shared_ptr<sh::File> fff = make_shared<sh::File>(fileName);
    string base64;
    if (fff->open(std::ios::in)) {
        path = fff->readAll();
    } else {
        cout << "fail to open timeinfo file" << endl;
        throw app::exception(make_error_code(error::open_file_fail));
    }
    std_msgs::String result;
    if (path.length() > 0) {
        json jdecode_l = json::parse(path);
        std::vector<TimerInfo> timer_infos = jdecode_l.get<std::vector<TimerInfo>>();//数据内容，结构体格式

        int i = 0;
        for (vector<TimerInfo>::iterator iter = timer_infos.begin(); iter != timer_infos.end();) {

            if (timer_infos[i].getTimerId() == params) {
                iter = timer_infos.erase(iter);//erase函数的返回指向当前被删除元素的下一个元素的迭代器
                break;
            } else {
                iter++;
                i++;
            }
        }
        fff->close();
        cout << "timer_infos  " << timer_infos.size() << endl;
        json params_l = timer_infos;
        string base64;
        base64.append(params_l.dump());//json转base64 string

        if (!sh::File::saveTextTo(fileName, base64)) {
            throw app::exception(make_error_code(error::save_file_timer_failed));
        }
        timer_infos.clear();
        timer_infos.shrink_to_fit();
        std_msgs::String ret;

        json js;
        js["method"] = "del";
        js["timerid"] = params;
        cout << "280  " << js["timerid"] << endl;
        ret.data.append(js.dump());

        PublishOutManager::instance().publishAppSchedule(ret);

        // return "";
        json jj;
        jj["code"] = 0;
        return jj.dump();
    } else {
        throw app::exception(make_error_code(error::open_file_timer_fail));
    }
}
