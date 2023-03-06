#!/bin/bash
echo "Upgrade Aircore"
version=$(rosparam get /ros_version)
major=${version:0:5}
version2211B="0.9.1"
version2212A="0.9.2"
version2212B="0.9.3"
version2301A="0.9.4"
version2301B="0.9.5"
version23O2B="0.9.6"
echo "current major version is $major"
if [ $major = $version2301A ]
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
echo "Upgrading libcartographer.so"
echo "123456" | sudo -S cp /home/admin1/AirCore/libcartographer.so /usr/local/lib/
echo "Upgrading cartographer_localization.lua"
rm /home/admin1/test_ws/install/share/robot_slam/params/cartographer_localization.lua
cp /home/admin1/AirCore/cartographer_localization.lua /home/admin1/test_ws/install/share/robot_slam/params/
echo "Updating inu"
echo "123456" | sudo -S bash /home/admin1/AirCore/package18_2023_02_28/uninstall.sh
sleep 5
echo "123456" | sudo -S bash /home/admin1/AirCore/package18_2023_02_28/install.sh
echo "Update complete!!!"
