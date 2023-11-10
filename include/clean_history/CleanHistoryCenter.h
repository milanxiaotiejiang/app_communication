//
// Created by liquan on 22-12-1.
//

#ifndef APP_COMMUNICATION_CLEANHISTORYCENTER_H
#define APP_COMMUNICATION_CLEANHISTORYCENTER_H

#include <mutex>
#include <tuple>
#include "clean_history/CleanHistoryDataBase.h"
#include "task/RealTask.h"
#include "task/status/state_machine.h"
#include "BaseThrowable.h"

namespace clean_history_db {
    class CleanHistoryCenter {
    private:
        CleanHistoryCenter() = default;

        CleanHistoryCenter(CleanHistoryCenter &) = delete;

        CleanHistoryCenter &operator=(const CleanHistoryCenter &) = delete;

    public:
        ~CleanHistoryCenter() = default;

    private:
        CleanHistory current_history_;
        std::mutex history_update_mutex_;
    public:
        static auto &instance() {
            static CleanHistoryCenter obj;
            return obj;
        }

        //初始化
        bool initialize();

        void removeCleanHistory();

        //下任务时新增一条历史
        bool addCleanHistory(const RealTask &task);

        //下任务失败结束该条历史
        bool launchFailed(const RealTask &task, const app::exception &e);

        //设置清洁模式
        void upDateByRealTask(const RealTask &real_task);

        //执行任务
        bool executeTask(const RealTask &real_task);

        //更新当前做的事情
        bool setCurrentFlow(int doing);

        //添加一次急停
        bool addUrgencyStop();

        //取消急停
        bool cancelUrgencyStop();

        //手动返回基站
        bool manualBack();

        //添加一次暂停
        bool addPause();

        //低电量返回基站
        bool lowPowerBack();

        //水箱返回基站
        bool equipmentErrorBack(bool clean_water_level_check_failed_,
                                bool dirty_water_level_check_failed_,
                                bool motor_error_,
                                bool mop_error_);

        //强制返回基站
        bool forceBack();

        //设置出站
        bool setOutStation(int state);

        //设值结束睡眠模式是否成功
        bool setEndSleep(int state);

        //设置返回摆渡点尝试次数
        bool setBackBaseRetries(int retries);

        // 清洁中更新清洁面积和清洁时间
        bool updateCleanHistory(const RealBlock &realBlock, const RealPoint &realPoint);

        //设置返回摆渡点是否成功
        bool setBackBasePointArrived(int state);

        //设置是否到达基站
        bool setStationArrived(int state);

        //设置回充重试次数
        bool setRechargeRetries(int retries);

        //设置清洁机构关闭是否成功
        bool setCloseMechanism(int state);

        //设置清洁机构打开是否成功
        bool setOpenMechanism(int state);

        //重置当前任务
        bool resetCurrentTask();

        //完成这条清洁历史
        bool complete();

        bool specialComplete();

        bool enterManualCleanMode();

        bool laserInterrupt();

        void updateProperty(const WorkStatus &workStatus, long cleanTime);

        //错误完成历史
        bool errorComplete(int error_code, std::string error_string, std::string error_code2);

        bool successComplete(int error_code, std::string error_string, std::string error_code2);

        std::tuple<int, std::string, std::string> generateErrorMessageFromLastFlow(int flow);

        //生成错误日志
        std::string make_error_msg(int error_code);

        std::tuple<int, std::string, std::string> generateErrorMessageFromException(const app::exception &e);

        std::vector<CleanHistory> getAllCleanHistories() {
            std::unique_lock<std::mutex> lock(history_update_mutex_);
            return CleanHistoryDataBase::instance().getAll();
        }

        //获取当前清洁时间
        long getCurrentCleanTime() {
            std::unique_lock<std::mutex> lock(history_update_mutex_);
            if (!current_history_.task_id_.empty() && current_history_.execute_time_ > 0) {
                //更新执行时间
                long current_time;
                time_t timep;
                time(&timep);
                current_time = timep * 1000;//毫秒
                current_history_.clean_time_ = (current_time - current_history_.execute_time_) / 1000 / 60;
                CleanHistoryDataBase::instance().updateHistory(current_history_);
                return current_history_.clean_time_;
            }
            return 0;
        }
    };
}


#endif //APP_COMMUNICATION_CLEANHISTORYCENTER_H
