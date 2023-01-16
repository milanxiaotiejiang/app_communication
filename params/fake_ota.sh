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
version=$(rosparam get /ros_version)
major=${version:0:5}
version2211B="0.9.1"
version2212A="0.9.2"
echo "current major version is $major"
if [ $major = $version2211B ]
then
    echo "this is a 2211B version"
    echo "Updating combination_list"
    rm /home/admin1/test_ws/install/share/data_base/config/combination_list_principal_json_work.txt
    cp /home/admin1/AirCore/combination_list_principal_json_work.txt /home/admin1/test_ws/install/share/data_base/config/
    echo "Updating station_areas"
    rm /home/admin1/test_ws/install/share/data_base/config/station_areas.yaml
    cp /home/admin1/AirCore/station_areas.yaml /home/admin1/test_ws/install/share/data_base/config/
    echo "Updating inu"
    echo "123456" | sudo -S bash /home/admin1/AirCore/package18_12_21/uninstall.sh
    sleep 5
    echo "123456" | sudo -S bash /home/admin1/AirCore/package18_12_21/install.sh
    sleep 20
    echo "123456" | sudo -S rm  /opt/ros/melodic/share/inudev_ros_nodelet/launch/inudev_ros_nodelet.launch
    echo "123456" | sudo -S cp  /home/admin1/AirCore/inudev_ros_nodelet.launch /opt/ros/melodic/share/inudev_ros_nodelet/launch
    echo "Upgrading cleanHistory"
    rm /home/admin1/test_ws/install/share/data_base/config/cleanHistory.sqlite
fi
if [ $major = $version2212A ]
then
    echo "this is a 2212B version"
    echo "Upgrading cleanHistory"
    rm /home/admin1/test_ws/install/share/data_base/config/cleanHistory.sqlite
fi
