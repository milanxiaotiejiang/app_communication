//
// Created by lijiang on 2021/12/24.
//

#include <tool/Variable.h>
#include "sub/FullPathSubscribe.h"
#include "model/TaskPoint.h"
#include "tool/Tool_Function.h"
#include <assert.h>
#include <condition_variable>
#include <mutex>

std::condition_variable fullPathCV;
std::mutex fullPathLock;

FullPathSubscribe::FullPathSubscribe(ros::NodeHandle handle, PubInner pubInner, PubOut pubOut)
        : handle(handle),
          pubInner(std::move(pubInner)),
          pubOut(std::move(pubOut)) {
    path_sub_ = handle.subscribe("/cleaning_path", 10,
                                 &FullPathSubscribe::subscribeCallback, this);


}


FullPathSubscribe::~FullPathSubscribe() {

}

void FullPathSubscribe::subscribeCallback(const nav_msgs::Path &path) {
    vector<Point> Full_path;
    //一条线中间的点不添加，只添加拐点。
    vector<Point> Last_path = Full_path;
    Full_path.clear();
    Point last_last_point;
    Point last_point;
    Point this_point;
    for (int i = 0; i < path.poses.size(); i++) {
        if (i > 0) {
            if (i > 1) {
                last_last_point = last_point;
            }
            last_point = this_point;
        }
        this_point.setX(path.poses[i].pose.position.x);
        this_point.setY(path.poses[i].pose.position.y);
        if (i > 1) {
            if (is_in_line(this_point, last_point, last_last_point))//删除直线中间的点
            {
                //防止出现last和last_last两点之间是连接线的状况
                if (sqrt(pow(last_last_point.getX() - last_point.getX(), 2) +
                         pow(last_last_point.getY() - last_point.getY(), 2)) <
                    1.0) {
                    Full_path.pop_back();
                }
            }
        }
        Full_path.push_back(this_point);
    }
    Variable::get_instance()->setFullPath(Full_path);
    fullPathCV.notify_all();
}

