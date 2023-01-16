//
// Created by lijiang on 2021/12/18.
//

#include "task/TaskCenter.h"
#include "manager/CleanHistoryManager.h"
#include "task/manager/manual.h"
#include "task/model/PointProgressVo.h"
#include "sub/json/TaskStrategy.h"

std::string Map_path =
        ros::package::getPath("robot_slam") + "/maps/mymap.pgm";


string ExecuteTaskStrategy::handler(Task task) {
    TaskCenter::instance().executeTask(task);
    return "";
}

vector<Task> GetTaskListStrategy::handler(string params) {
    std::vector<Task> task_list;
    for (const auto &item: ManualManager::instance().runTaskList()) {
        Task task(item.getId(), item.getMode(), item.getRate(), item.getWorkStatus(),
                  CleanPolygon(), item.getZoned(), CleanContinuity(), TeachPathInfo(),
                  item.getLaunchPeople(), item.getLaunchTime(), item.getTimeMode(), true);
        if (item.getMode() == 7) {
            task.setCombination(item.getCombination());
        }
        task_list.push_back(task);
    }
    return task_list;
}

vector<TaskUpgrade> GetTaskListStrategyV2::handler(string params) {
//    //操作，获取当前任务状态
//    std::vector<Task> clean_task_list;
//    for (const auto &item: TaskManager::get_instance()->getTaskList()) {
//        clean_task_list.push_back(item);
//    }
//
//    std::vector<TaskUpgrade> taskUpgradeList;
//    for (const auto &item: clean_task_list) {
//        auto workStatus = item.getWorkStatus();
//        WorkStatusUpgrade ws;
//        ws.setSweepStatus(workStatus.getSweepStatus());
//        ws.setDragStatus(workStatus.getMopStatus());
//        ws.setAbsorbStatus(workStatus.getVacuumStatus());
//        ws.setPushStatus(workStatus.getPushStatus());
//
//        TaskUpgrade taskUpgrade(
//                item.getTaskId(), item.getMode(), item.getRate(), ws,
//                item.getPolygon(), item.getZoned(), item.getContinuity(),
//                item.getTeachPath(), item.getLaunchPeople(), item.getLaunchTime(), item.getTimeMode(),
//                item.isInExecute(), item.getCombination(), item.getFullPath()
//        );
//        taskUpgradeList.push_back(taskUpgrade);
//    }
//
//    return taskUpgradeList;
}

bool IsInBasementStrategy::handler(string params) {
    return true;
}

VersionInfo GetRosVersionStrategy::handler(string params) {
    VersionManager::instance().getAirCodeVersion();
    VersionInfo into(
            VersionManager::instance().getAirCodeVersion(),
            VersionManager::instance().getDsHardVersion(),
            VersionManager::instance().getDsSoftVersion(),
            VersionManager::instance().getAppPadVersion()
    );
    return into;
}

deque<PointProgressVo> GetFinishedPointStrategy::handler(string params) {
    //操作，获取当前任务状态
    deque<PointProgressVo> finished_point_list;
    for (const auto &point: ManualManager::instance().runTaskPoint()) {
        PointProgressVo pointProgressVo(point.task_id, point.realPosition.x, point.realPosition.y,
                                        point.realProgress.currentStep, point.realProgress.totalStep,
                                        point.realProgress.currentFrequency, point.realProgress.totalFrequency,
                                        point.work_status, point.mode, point.inClean);
        finished_point_list.push_back(pointProgressVo);
    }
    return finished_point_list;
}

void GetFullPlanStrategy::dateProgressing(int source, json &jdecode) {

    auto command = jdecode.get<RequestModel<BaseMethod<vector<int>>>>();
    auto commandMsg = command.getMsg();
    auto params = commandMsg.getParams();
    //操作
    vector<int> temp = params;

    for (int i = 0; i < temp.size(); i++) {
        cout << temp[i] << endl;
    }

    std_msgs::Int32MultiArray msg;
    msg.data = temp;
    PublishInnerManager::instance().getPubInner()->publishStartPlan(msg);
    int Id = commandMsg.getId();

    extern ThreadPool pool;
    Task param;//数据内容，结构体格式
    auto answerFullPath = [](Task &param, int id) {
        extern std::condition_variable fullPathCV;
        extern std::mutex fullPathLock;
        std::unique_lock<std::mutex> lck(fullPathLock);
        fullPathCV.wait_for(lck, std::chrono::milliseconds(7000));
        vector<Point> full_path = Variable::get_instance()->getFullPath();
        param.setFullPath(full_path);
        //回复给APP用于显示全覆盖路径
        std_msgs::String result;
        BaseResult<Task> success(id, param);//这里尖括号里不写类型会编译不过去
        RequestModel<BaseResult<Task>> requestModel(
                "publish", "/response_json", success
        );
        json jsonResult = requestModel;
        PublishOutManager::instance().getPubOut()->publishJson(jsonResult.dump());                            //回应app
    };
    pool.submit(answerFullPath, param, Id);
}