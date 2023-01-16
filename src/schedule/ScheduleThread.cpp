#include "schedule/ScheduleThread.h"
#include "model/TimerInfo.h"
#include "net/WsServerManager.h"
#include "sub/json/TaskStrategy.h"
#include "glog/logging.h"
#include "ros/package.h"
#include "tool/Variable.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "task/TaskCenter.h"
#include "task/manager/manual.h"
#include "manager/CombinationManager.h"
#include "manager/ViewPartManager.h"
//
PubInner *pubInnerStatic = nullptr;

ScheduleThread::ScheduleThread(ros::NodeHandle handle, PubInner pubInner, PubOut pubOut) : handle(handle),
                                                                                           pubInner(pubInner),
                                                                                           pubOut(pubOut) {
    sub_json_ = handle.subscribe(APP_SCHEDULE, 1, &ScheduleThread::subscribeCallback, this);
    pubInnerStatic = &pubInner; // new PubInner(handle);
}

ScheduleThread::~ScheduleThread() {
}

std::string fixWeek(std::string strin) {
    //处理cron中默认dayofweek，有的默认从1开始，有的默认从0开始。从0开始就加一
    std::string ss = strin;//"0 0 9 * * 1,2,3,4,5";
    std::size_t found = ss.find("*");
    std::size_t f2 = ss.find("*", found + 1, 1);
    std::string s1 = ss.substr(0, f2 + 1);
    // std::cout<<"   s1   "<<s1<<"  "<<f2<<endl;
    std::string s2 = ss.substr(f2 + 2);
    std::string s3 = s2.substr(0, 1);
    std::string s5 = "*";
    std::size_t found1 = 0;
    // std::cout<<"   s2   "<<s2<<endl;
    while (found1 < s2.size()) {

        s3 = s2.substr(found1, 1);

        int a; //=atoi(str.c_str());
        if (s3 != " " && s3 != "*" && s3 != ",") {
            a = (atoi(s3.c_str()) + 1) % 7;
            std::string s4 = std::to_string(a);
            //s1 += " " + s4;  

            if (s5 == "*") s1 += " ";
            // std::cout <<"111   "<<" s5 "<<s5<<"  s3 "<<s3<<"  s1 "<< s1 << endl;
            s1 += s4;
        } else if (s3 == "*") {
            s1 += " " + s3;
            // std::cout <<"222  "<< s1 << endl;
        } else if (s3 == "," || s3 == " ") {
            // std::cout <<"333  "<< s1 << endl;
            s1 += s3;
        }
        found1 += 1;
        s5 = s3;
        // if(found1 < s2.size() && s3 != ",") s1 += ",";
    }
    //  std::cout << s1 << endl;
    return s1;
}

void execTask(TimerInfo &tsk) {
    std::shared_ptr<Task> tk = make_shared<Task>();
    std::string st = croncpp::Cron<croncpp::LocalClock, croncpp::NullLock>::get_timestring();
    LOG(INFO) << "execTask  taskid  " << st;
    tk->setTaskId(st);//(tsk.getTaskId()+tsk.getTimerRule());
    if (tsk.getTaskId() == "") {
        //如果没有模式，默认全覆盖
        tk->setMode(6);
    } else {
        CombinationBrief combination_brief_temp;
        if (CombinationManager::get_instance()->GetCombination(combination_brief_temp, tsk.getTaskId()) != SUCCESS_) {
            LOG(INFO) << "获取组合路径失败";
            return;
        }
        if(combination_brief_temp.getCombinationType() == 0){
            tk->setMode(7);
            Combination cmb;
            cmb.setCombinationID(tsk.getTaskId());
            tk->setCombination(cmb);
        }else{
            tk->setMode(6);
            Combination cmb;
            cmb.setCombinationID(tsk.getTaskId());
            tk->setCombination(cmb);
        }
    }

    tk->setRate(tsk.getRate());
    tk->setLaunchPeople("admin1");
    //任务运行中，不分配任务
    if (ManualManager::instance().runTaskId() != "") {
        LOG(INFO) << "当前有任务在执行，定时清扫被取消";
    } else {
        if (tk->getMode() == 6) {
            cout << "!!!!!!Full path  45  : " << ManualManager::instance().runTaskId() << endl;
            //pub to clean robot to get full plan
            std_msgs::Int32MultiArray msg;
            vector<int> temp;
            temp.push_back(-1);
            cout << "!!!!!!Full path  133  : " << endl;
            msg.data = temp;
            PublishInnerManager::instance().getPubInner()->publishStartPlan(msg);
            cout << "!!!!!!Full path  135  : " << endl;

            extern std::condition_variable fullPathCV;
            extern std::mutex fullPathLock;
            std::unique_lock<std::mutex> lck(fullPathLock);
            fullPathCV.wait_for(lck, std::chrono::milliseconds(7000));
            cout << "!!!!!!Full path  140  : " << endl;
            vector<Point> full_path = Variable::get_instance()->getFullPath();
            if (full_path.size() > 0) {
                std_msgs::String result;
                tk->setFullPath(full_path);
                cout << "!!!!!!Full path  147  : " << endl;
                try {
                    TaskCenter::instance().executeTask(*tk);

                } catch (app::exception const &e) {
                    LOG(INFO) << "定时全局清扫失败!!!";
                }
            }
        } else if (tk->getMode() == 7) {
            try {
                TaskCenter::instance().executeTask(*tk);
            } catch (app::exception const &e) {
                LOG(INFO) << "定时组合路径执行失败!!!";
            }
        } else {
            LOG(INFO) << "定时任务  未知命令   ";
        }

    }
}

void ScheduleThread::startScheduleCheck() {
    int nstate = 0;
    string fileName;
    string sss;
    fileName.append(ros::package::getPath("data_base"));
    fileName.append("/config/timer_info_json.txt");
    //设置清扫计时器
    // sh::File *fff = new sh::File(fileName);

    std::shared_ptr<sh::File> fff = make_shared<sh::File>(fileName);
    cout << "timeinfo file  " << fileName << endl;
    if (!fff->open(std::ios::in)) {
        if (!fff->create(fileName)) {
            cout << "fail to create timeinfo file" << endl;
            //   parseError(pubOut, params.getId(), 10001, "创建定时器文件失败11");

            return;
        }
    } else {
        stimer_list = fff->readAll();
        fff->close();
    }

    while (1) {
        try {
            string sss = stimer_list;

            //gang rebuild
            if (sss.length() > 0) //不为空
            {
                json jdecode = json::parse(sss);
                std::vector<TimerInfo> timer_infos = jdecode.get<std::vector<TimerInfo>>(); //数据内容，结构体格式
                for (int i = 0; i < timer_infos.size(); i++) {

                    std::shared_ptr<TimerInfo> ti = make_shared<TimerInfo>();
                    ti->setTimerRule(timer_infos[i].getTimerRule());
                    ti->setTaskId(timer_infos[i].getTaskId());
                    ti->setRate(timer_infos[i].getRate());
                    ti->setTaskName(timer_infos[i].getTaskName());
                    ti->setIsNever(timer_infos[i].getIsNever());
                    ti->setIsSkip(timer_infos[i].getIsSkip());
                    ti->setEndYear(timer_infos[i].getEndYear());
                    ti->setEndMonth(timer_infos[i].getEndMonth());
                    ti->setEndDay(timer_infos[i].getEndDay());
                    std::string sRule1 = ti->getTimerRule(); //
                    std::string sRule = fixWeek(sRule1);

                    sRule = "0 " + sRule;                              //目前暂时不支持秒级，暂时这样处理
                    std::string endRule = "0 0 0 " + std::to_string(timer_infos[i].getEndDay()) + " " +
                                          std::to_string(timer_infos[i].getEndMonth()) +
                                          " * ";// + std::to_string(timer_infos[i].getEndYear());//
                    //    cout << "截止时间 ！！！！！！    " <<endRule <<endl;
                    if (timer_infos[i].getIsNever()) {
                        endRule = "* * * * * *";
                    }
                    // endRule = "* * * * * *";
                    cron.add_schedule(timer_infos[i].getTimerId(), sRule, endRule, timer_infos[i].getIsNever(),
                                      timer_infos[i].getEndYear(),
                                      [ti](auto &) {
                                          execTask(*ti);
                                      });
                }
                timer_infos.clear();
                timer_infos.shrink_to_fit();
            } else {
                //返回空
//                cout << "timer file is empty " << endl;
                sleep(15);
                continue;
            }
            cron.tick();
            //  std::cout <<"Now Time is: " << GetNowTime() << std::endl;
            sleep(1); //一秒钟轮询一次
        }
        catch (std::exception const &e) {
        }
    }
}

void ScheduleThread::subscribeCallback(const std_msgs::String &result) {
    std::string decode = result.data; // base64消息解码成string
    json jdecode = json::parse(decode);
    cout << "timeinfo file subscribeCallback209   " << endl;
    //
    //
    string fileName;

    fileName.append(ros::package::getPath("data_base"));
    fileName.append("/config/timer_info_json.txt");
    //设置清扫计时器
    // sh::File *fff = new sh::File(fileName);
    std::shared_ptr<sh::File> fff = make_shared<sh::File>(fileName);
    cout << "timeinfo file subscribeCallback " << fileName << endl;
    if (!fff->open(std::ios::in)) {

        cout << "fail to read timeinfo file in subscribeCallback" << endl;
    } else {
        stimer_list = fff->readAll();
        fff->close();

        if (jdecode["method"] == "del") {
            cron.remove_schedule(jdecode["timerid"]);
        }
    }
}
