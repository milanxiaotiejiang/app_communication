#include "prohibition.h"
#include "yaml-cpp/yaml.h"
#include "db/path.h"
#include <fstream>
#include <iostream>
#include <ros/package.h>
#include <vector>

std::string partition_path = path::data_base_config_dir() + "devide_area.yaml";

int set_prohibition(float *point, int num) {

    YAML::Node config = YAML::LoadFile(path::prohibition_areas_path());

    int prohibition_num;//障碍物个数
    prohibition_num = config["prohibition_areas"].size();
    std::vector<int> point_num;
    std::vector<std::vector<std::vector<float>>> prohibition_position;
    for (int i = 0; i < prohibition_num; i++)//第i个障碍物包含的点的个数
    {
        point_num.push_back(config["prohibition_areas"][i].size());
    }
    int new_prohibition_num = prohibition_num;//两个new变量用于保存修改后的文件的障碍物个数以及每个障碍中的点数
    std::vector<int> new_point_num = point_num;

    //获取yaml文件的所有信息
    for (int i = 0; i < prohibition_num; i++) {
        std::vector<std::vector<float>> temp1;
        //        cout<<"第"<<i+1<<"个障碍:"<<endl;
        for (int j = 0; j < point_num[i]; j++) {
            std::vector<float> temp2;
            //            cout<<"     第"<<j+1<<"个点:"<<config["prohibition_areas"][0][0].size()<<endl;
            temp2.push_back(atof(config["prohibition_areas"][i][j][0].as<std::string>().c_str()));
            temp2.push_back(atof(config["prohibition_areas"][i][j][1].as<std::string>().c_str()));
            temp1.push_back(temp2);
        }
        prohibition_position.push_back(temp1);
    }

    std::ofstream fout(path::prohibition_areas_path());

    config.reset();//将yaml文件清空

    //对yaml文件进行操作
    for (int i = 0; i < new_prohibition_num; i++) {
        for (int j = 0; j < new_point_num[i]; j++) {
            config["prohibition_areas"][i][j][0] = prohibition_position[i][j][0];
            config["prohibition_areas"][i][j][1] = prohibition_position[i][j][1];
        }
    }
    for (int i = 0; i < num; i += 2) {
        config["prohibition_areas"][new_prohibition_num][i / 2][0] = point[i];
        config["prohibition_areas"][new_prohibition_num][i / 2][1] = point[i + 1];//新的障碍物从第一个点开始写起
    }
    fout << config;//将修改写进yaml文件

    fout.close();//文件关闭后yaml内容才会变化
    return 1;
}

int reset_prohibition(std::string prohibition_path) {
    YAML::Node config = YAML::LoadFile(prohibition_path);
    std::ofstream fout(prohibition_path);
    config.reset();//将yaml文件清空
    //对yaml文件进行操作

    config["prohibition_areas"][0][0][0] = -100;
    config["prohibition_areas"][0][0][1] = -100;//保留一个点，不然文件会无法访问

    fout << config;//将修改写进yaml文件
    fout.close();//文件关闭后yaml内容才会变化
    return 1;
}

bool get_prohibition(std::vector<std::vector<float>> &prohibition_position) {
    YAML::Node config = YAML::LoadFile(path::prohibition_areas_path());
    int prohibition_num;//障碍物个数
    prohibition_num = config["prohibition_areas"].size();
    std::vector<int> point_num;
    for (int i = 0; i < prohibition_num; i++)//第i个障碍物包含的点的个数
    {
        point_num.push_back(config["prohibition_areas"][i].size());
    }
    int new_prohibition_num = prohibition_num;//两个new变量用于保存修改后的文件的障碍物个数以及每个障碍中的点数
    std::vector<int> new_point_num = point_num;

    //存入prohibition_position
    for (int i = 1; i < prohibition_num; i++) {
        std::vector<float> temp1;
        //        cout<<"第"<<i<<"个障碍:"<<endl;
        if (point_num[i] > 2) {
            temp1.push_back(1.0);
        } else {
            temp1.push_back(2.0);
        }
        for (int j = 0; j < point_num[i]; j++) {
            temp1.push_back(atof(config["prohibition_areas"][i][j][0].as<std::string>().c_str()));
            temp1.push_back(atof(config["prohibition_areas"][i][j][1].as<std::string>().c_str()));
        }
        prohibition_position.push_back(temp1);
    }
    return true;
}
