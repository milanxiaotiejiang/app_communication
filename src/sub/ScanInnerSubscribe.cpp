//
// Created by lijiang on 2022/1/10.
//

#include <tf/transform_listener.h>

#include "sub/ScanInnerSubscribe.h"
#include "Timer_tool.h"
#include "tool/Variable.h"

ScanInnerSubscribe::ScanInnerSubscribe(ros::NodeHandle &handle, PubInner &pubInner, PubOut &pubOut)
        : handle(handle), pubInner(pubInner), pubOut(pubOut) {
    sub_scan_inner_ = handle.subscribe("/scan", 10, &ScanInnerSubscribe::subscribeCallback, this);
}

ScanInnerSubscribe::~ScanInnerSubscribe() {
}

void ScanInnerSubscribe::subscribeCallback(const sensor_msgs::LaserScan &scan_raw) {
    if (Variable::get_instance()->isScanFlag()) {
        Variable::get_instance()->setScanFlag(false);
        sensor_msgs::LaserScan scan_app;
        //    nav_msgs::Path path;
        //    path.header = scan_raw.header;
        //    path.header.frame_id = "odom";

        tf::TransformListener tf_listener;
        tf::StampedTransform stamped_transform;
        Eigen::Matrix4f transform_matrix;
        scan_app.angle_increment = scan_raw.angle_increment;
        scan_app.angle_min = scan_raw.angle_min;
        scan_app.angle_max = scan_raw.angle_max;
        scan_app.header = scan_raw.header;
        scan_app.range_max = scan_raw.range_max;
        scan_app.range_min = scan_raw.range_min;
        scan_app.scan_time = scan_raw.scan_time;
        scan_app.time_increment = scan_raw.time_increment;

        try {
            tf_listener.waitForTransform("odom", "laser", ros::Time(0.0), ros::Duration(0.5));
            tf_listener.lookupTransform("odom", "laser", ros::Time(0.0), stamped_transform);
        }
        catch (tf::TransformException &ex) {
            ROS_ERROR("%s", ex.what());
            // ros::Duration(0.5).sleep()
        }
        double current_angle = scan_raw.angle_min;
        for (unsigned int i = 0; i < scan_raw.ranges.size(); i++) {
            if (i % 5 == 0) {

                double dist = scan_raw.ranges[i];
                float x_dir, y_dir, x_real, y_real;
                x_dir = float(dist * cos(i * scan_raw.angle_increment));
                y_dir = float(dist * sin(i * scan_raw.angle_increment));
                float theta = tf::getYaw(stamped_transform.getRotation());
                x_real = x_dir * cos(theta) - y_dir * sin(theta);
                y_real = y_dir * cos(theta) + x_dir * sin(theta);
                scan_app.ranges.push_back(
                        x_real + stamped_transform.getOrigin().getX());
                scan_app.intensities.push_back(
                        y_real + stamped_transform.getOrigin().getY());
                //        geometry_msgs::PoseStamped this_pose_stamped;
                //        this_pose_stamped.pose.position.x = scan_app.ranges[i];
                //        this_pose_stamped.pose.position.y = scan_app.intensities[i];
                //        path.poses.push_back(this_pose_stamped);
            }
            pubOut.publishScan(scan_app);
            //    pubOut.publishPath(path);
        }
    }
}
