#!/bin/bash
robot_slam_dir=/home/admin1/test_ws/install/share/robot_slam
echo "robot_slam_dir: $robot_slam_dir"
data_base_dir=/home/admin1/test_ws/install/share/data_base
echo "data_base_dir: $data_base_dir"
aircore_dir=/home/admin1/AirCore
echo "aircore_dir: $aircore_dir"
source_file_dir=$aircore_dir/files
echo "source_file_dir: $source_file_dir"



robot_slam_launch_dir=$robot_slam_dir/launch
echo "robot_slam_launch_dir: $robot_slam_launch_dir"
robot_slam_params_dir=$robot_slam_dir/params
echo "robot_slam_params_dir: $robot_slam_params_dir"


#costmap
costmap_source_dir=$source_file_dir/costmap_params
echo "costmap_source_dir: $costmap_source_dir"
costmap_param_dir=$robot_slam_params_dir/costmap
echo "costmap_param_dir: $costmap_param_dir"

function update_costmap_common_params_zoo
{
  echo "Upgrading costmap_common_params_zoo"
  rm $costmap_param_dir/costmap_common_params_zoo.yaml
  cp $costmap_source_dir/costmap_common_params_zoo.yaml $costmap_param_dir
}

function update_global_costmap_params
{
  echo "Upgrading global_costmap_params"
  rm $costmap_param_dir/global_costmap_params.yaml
  cp $costmap_source_dir/global_costmap_params.yaml $costmap_param_dir
}

function update_local_costmap_params
{
  echo "Upgrading local_costmap_params"
  rm $costmap_param_dir/local_costmap_params.yaml
  cp $costmap_source_dir/local_costmap_params.yaml $costmap_param_dir
}

#planner
planner_source_dir=$source_file_dir/planner_params
echo "planner_source_dir: $planner_source_dir"
planner_param_dir=$robot_slam_params_dir/planner
echo "planner_param_dir: $planner_param_dir"

function update_global_planner_params
{
  echo "Upgrading global_planner_params"
  rm $planner_param_dir/global_planner_params.yaml
  cp $planner_source_dir/global_planner_params.yaml $planner_param_dir
}

function update_local_planner_params
{
  echo "Upgrading dwa_local_planner_params_zoo"
  rm $planner_param_dir/dwa_local_planner_params_zoo.yaml
  cp $planner_source_dir/dwa_local_planner_params_zoo.yaml $planner_param_dir
}

#inu
function update_inu
{
  echo "Updating inu"
  echo "123456" | sudo -S bash /home/admin1/AirCore/files/package18_2023_03_15/uninstall.sh
  sleep 5
  echo "123456" | sudo -S bash /home/admin1/AirCore/files/package18_2023_03_15/install.sh
}

#carto
carto_source_dir=$source_file_dir/carto_params
echo "carto_source_dir: $carto_source_dir"
carto_params_dir=$robot_slam_params_dir
echo "carto_params_dir: $carto_params_dir"
function update_cartographer_rplidar
{
  echo "Upgrading cartographer_rplidar"
  rm $carto_params_dir/cartographer_rplidar.lua
  cp $carto_source_dir/cartographer_rplidar.lua $carto_params_dir
}

function update_cartographer_localization
{
  echo "Upgrading cartographer_localization"
  rm $carto_params_dir/cartographer_localization.lua
  cp $carto_source_dir/cartographer_localization.lua $carto_params_dir
}

function update_libcartographer
{
  echo "Upgrading libcartographer.so"
  echo "123456" | sudo -S cp $carto_source_dir/libcartographer.so /usr/local/lib/
}

carto_launch_dir=$robot_slam_launch_dir/mapping

function update_cartographer_launch
{
  echo "Upgrading cartographer.launch"
  rm $carto_launch_dir/cartographer.launch
  cp $carto_source_dir/cartographer.launch $carto_launch_dir
}

#env
env_source_dir=$source_file_dir/env
echo "env_source_dir: $env_source_dir"
env_dir=/home/admin1/test_ws/install
echo "env_dir: $env_dir"

function update_env
{
  echo "Updating environment"
  rm $env_dir/_setup_util.py
  cp $env_source_dir/_setup_util.py $env_dir
}

#sensor names
sensor_source_dir=$source_file_dir/sensor_params
echo "sensor_source_dir: $sensor_source_dir"
sensor_param_dir=$robot_slam_params_dir/sensor
echo "sensor_param_dir: $sensor_param_dir"
function update_sensor_params
{
  echo "Updating sensor topic names"
  rm $sensor_param_dir/realworld_sensor_params.yaml
  cp $sensor_source_dir/realworld_sensor_params.yaml $sensor_param_dir
}


#tf params
tf_params_source_dir=$source_file_dir/tf_params
echo "tf_params_source_dir: $tf_params_source_dir"
tf_params_dir=$data_base_dir/tf_params
echo "tf_params_dir: $tf_params_dir"
function update_tf_params
{
  echo "Upgrading tf"
  mkdir $tf_params_dir
  cp $tf_params_source_dir/camera_link1.yaml $tf_params_dir
  cp $tf_params_source_dir/camera_link2.yaml $tf_params_dir
}

#move_base
move_base_source_dir=$source_file_dir/move_base_params
move_base_params_dir=$robot_slam_params_dir
move_base_launch_include_dir=$robot_slam_launch_dir/include
function update_move_base_params
{
  echo "Upgrading move_base_params.yaml"
  rm $move_base_params_dir/move_base_params.yaml
  cp $move_base_source_dir/move_base_params.yaml $move_base_params_dir
}

function update_move_base_launch_include
{
  echo "Upgrading move_base.launch.xml"
  rm $move_base_launch_include_dir/move_base.launch.xml
  cp $move_base_source_dir/move_base.launch.xml $move_base_launch_include_dir
}

echo "Upgrade Aircore"
version=$(rosparam get /ros_version)
major=${version:0:5}
old_version=("0.9.4" "0.9.5" "0.9.6" "0.9.7" "0.9.8" "0.9.9" "1.0.0" "1.0.1" "1.0.2" "2.0.0" "2.0.2" "2.0.3" "2.0.4" "2.0.5")
version_index=0
for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 
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
  echo "Updating 0.9.5"
  update_tf_params
  update_local_planner_params
  update_costmap_common_params_zoo
  update_global_costmap_params
  update_local_costmap_params
fi

if [ $version_index -lt 2 ]
then
  echo "Updating 0.9.6"
  update_libcartographer
  update_cartographer_localization
fi

if [ $version_index -lt 3 ]
then
  echo "Updating 0.9.7"
  update_env
  update_sensor_params
  update_inu
fi

if [ $version_index -lt 4 ]
then
  echo "Updating 0.9.8"
  update_libcartographer
fi

if [ $version_index -lt 5 ]
then
  echo "Updating 0.9.9"
  update_libcartographer
  update_cartographer_rplidar
fi

if [ $version_index -lt 6 ]
then
  echo "Updating 1.0.0"
  update_libcartographer
  update_cartographer_rplidar
  update_cartographer_localization
fi

if [ $version_index -lt 7 ]
then
  echo "Updating 1.0.1"
  update_libcartographer
  update_cartographer_rplidar
  update_cartographer_localization
  update_costmap_common_params_zoo
  update_global_costmap_params
  update_local_costmap_params
fi

if [ $version_index -lt 9 ]
then
  echo "Updating 2.0.0"
  update_costmap_common_params_zoo
  update_global_costmap_params
  update_local_costmap_params
  update_global_planner_params
  update_move_base_params
  update_move_base_launch_include
fi

if [ $version_index -lt 10 ]
then
  echo "Updating 2.0.2"
  update_cartographer_rplidar
  update_cartographer_localization
  update_cartographer_launch
fi

if [ $version_index -lt 11 ]
then
  echo "Updating 2.0.3"
  update_move_base_params
fi

  echo "Updating 2.0.4"
  update_cartographer_rplidar
  update_cartographer_localization