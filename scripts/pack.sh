#!/bin/bash

echo "欢迎使用一键打包助手，请准备好你的 AirCore 文件夹 !"

pattern="ros_version: ([0-9]+\.[0-9]+\.[0-9]+\.[0-9]+)_.*"
robot_basic_params_path="$HOME/AirCore/app/install/share/app_communication/params/robot_basic_params.yaml"
headN1=$(head -n1 "$robot_basic_params_path")
echo "查找到第一行数据为: $headN1"

if [[ $headN1 =~ $pattern ]]; then
  version="${BASH_REMATCH[1]}"
else
  echo "版本匹配失败"
  exit 1
fi
echo "获得当前版本号为: $version"

pack_dir=$HOME/$version
core_dir=$HOME/AirCore
target_path=$HOME/$version".tar.xz"
out_path=$HOME/core_$version".tar"

rm -rf "$pack_dir"
mkdir "$pack_dir"
cp -r "$core_dir" $pack_dir

cd

#tar -cJf $version".tar.xz" pack_dir
echo '开始压缩' "$pack_dir" '文件夹...'
# 使用 pv 命令来显示进度条
# tar -c "$pack_dir" | pv -s $(du -sb "$pack_dir" | awk '{print $1}') | xz >"$target_path"
tar -cf - $version | pv -s $(du -sb $version | awk '{print $1}') | xz -z > $version.tar.xz
echo "文件夹压缩完成！"

mv $target_path $out_path
echo '打包完成，安装包路径为' "$out_path" '欢迎再次使用！'