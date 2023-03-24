#!/bin/bash
echo "Upgrade Aircore"
version=$(rosparam get /ros_version)
major=${version:0:5}
old_version=("0.9.4" "0.9.5" "0.9.6" "0.9.7" "0.9.8")
version_index=0
for i in 1 2 3 4
do
  echo "${old_version[$i]}"
  if [[ $major = ${old_version[$i]} ]]
  then 
    version_index=$i
  fi
done
echo "current major version is $major"
echo "version_index is $version_index"
if [ $version_index -lt 1 ]
then
    echo "this is a 2301A version"
    echo "Upgrading tf"
    mkdir /home/admin1/test_ws/install/share/data_base/tf_params/
    cp /home/admin1/AirCore/camera_link1.yaml /home/admin1/test_ws/install/share/data_base/tf_params/
    cp /home/admin1/AirCore/camera_link2.yaml /home/admin1/test_ws/install/share/data_base/tf_params/
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
fi
if [ $version_index -lt 2 ]
then
    echo "this is a 2302B version"
    echo "Upgrading libcartographer.so"
    echo "123456" | sudo -S cp /home/admin1/AirCore/libcartographer.so /usr/local/lib/
    echo "Upgrading cartographer_localization.lua"
    rm /home/admin1/test_ws/install/share/robot_slam/params/cartographer_localization.lua
    cp /home/admin1/AirCore/cartographer_localization.lua /home/admin1/test_ws/install/share/robot_slam/params/
fi
echo "Updating 0.9.7"
echo "Updating environment"
rm /home/admin1/test_ws/install/_setup_util.py
cp /home/admin1/AirCore/_setup_util.py /home/admin1/test_ws/install/
echo "Updating sensor topic names"
rm /home/admin1/test_ws/install/share/robot_slam/params/sensor/realworld_sensor_params.yaml
cp /home/admin1/AirCore/realworld_sensor_params.yaml /home/admin1/test_ws/install/share/robot_slam/params/sensor/
echo "Updating inu"
echo "123456" | sudo -S bash /home/admin1/AirCore/package18_2023_03_15/uninstall.sh
sleep 5
echo "123456" | sudo -S bash /home/admin1/AirCore/package18_2023_03_15/install.sh
echo "Update complete!!!"
