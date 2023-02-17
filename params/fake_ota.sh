#!/bin/bash
echo "Upgrade Aircore"
echo "Upgrading dwa_local_planner_params_zoo"
rm /home/admin1/test_ws/install/share/robot_slam/params/planner/dwa_local_planner_params_zoo.yaml
cp /home/admin1/AirCore/dwa_local_planner_params_zoo.yaml /home/admin1/test_ws/install/share/robot_slam/params/planner/
echo "Upgrading costmap_common_params_zoo"
rm /home/admin1/test_ws/install/share/robot_slam/params/costmap/costmap_common_params_zoo.yaml
cp /home/admin1/AirCore/costmap_common_params_zoo.yaml /home/admin1/test_ws/install/share/robot_slam/params/costmap/
echo "Upgrading global_costmap_params"
rm /home/admin1/test_ws/install/share/robot_slam/params/costmap/global_costmap_params.yaml
cp /home/admin1/AirCore/global_costmap_params.yaml /home/admin1/test_ws/install/share/robot_slam/params/costmap/
echo "Upgrading local_costmap_params"
rm /home/admin1/test_ws/install/share/robot_slam/params/costmap/local_costmap_params.yaml
cp /home/admin1/AirCore/local_costmap_params.yaml /home/admin1/test_ws/install/share/robot_slam/params/costmap/
echo "Upgrading tf"
cp /home/admin1/AirCore/camera_link1.yaml /home/admin1/test_ws/install/share/data_base/tf_params/
cp /home/admin1/AirCore/camera_link2.yaml /home/admin1/test_ws/install/share/data_base/tf_params/
version=$(rosparam get /ros_version)
major=${version:0:5}
version2211B="0.9.1"
version2212A="0.9.2"
version2212B="0.9.3"
version2301A="0.9.4"
version2301B="0.9.5"
echo "current major version is $major"

