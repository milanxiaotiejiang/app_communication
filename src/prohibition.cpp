#include "prohibition.h"
#include "yaml-cpp/yaml.h"
#include <fstream>
#include <iostream>
#include <ros/package.h>
#include <vector>

using namespace std;

std::string partition_path = ros::package::getPath("data_base") + "/config/devide_area.yaml";

int set_prohibition(float *point, int num) {

    string path;
    path.append(ros::package::getPath("data_base"));
    path.append("/config/prohibition_areas.yaml");
    // path.append("/home/admin1/test_ws/src/data_base/config/prohibition_areas.yaml");

    YAML::Node config = YAML::LoadFile(path);

    int prohibition_num;//障碍物个数
    prohibition_num = config["prohibition_areas"].size();
    vector<int> point_num;
    vector<vector<vector<float>>> prohibition_position;
    for (int i = 0; i < prohibition_num; i++)//第i个障碍物包含的点的个数
    {
        point_num.push_back(config["prohibition_areas"][i].size());
    }
    int new_prohibition_num = prohibition_num;//两个new变量用于保存修改后的文件的障碍物个数以及每个障碍中的点数
    vector<int> new_point_num = point_num;

    //获取yaml文件的所有信息
    for (int i = 0; i < prohibition_num; i++) {
        vector<vector<float>> temp1;
        //        cout<<"第"<<i+1<<"个障碍:"<<endl;
        for (int j = 0; j < point_num[i]; j++) {
            vector<float> temp2;
            //            cout<<"     第"<<j+1<<"个点:"<<config["prohibition_areas"][0][0].size()<<endl;
            temp2.push_back(atof(config["prohibition_areas"][i][j][0].as<std::string>().c_str()));
            temp2.push_back(atof(config["prohibition_areas"][i][j][1].as<std::string>().c_str()));
            cout << "     " << atof(config["prohibition_areas"][i][j][0].as<std::string>().c_str()) << "," << atof(config["prohibition_areas"][i][j][1].as<std::string>().c_str()) << endl;
            temp1.push_back(temp2);
        }
        prohibition_position.push_back(temp1);
    }

    ofstream fout(path);

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

int reset_prohibition(void) {
    string path;
    path.append(ros::package::getPath("data_base"));
    path.append("/config/prohibition_areas.yaml");
    // path.append("/home/admin1/test_ws/src/data_base/config/prohibition_areas.yaml");
    YAML::Node config = YAML::LoadFile(path);
    ofstream fout(path);
    config.reset();//将yaml文件清空
    //对yaml文件进行操作

    config["prohibition_areas"][0][0][0] = -100;
    config["prohibition_areas"][0][0][1] = -100;//保留一个点，不然文件会无法访问

    fout << config;//将修改写进yaml文件

    fout.close();//文件关闭后yaml内容才会变化

    return 1;
}

bool get_prohibition(vector<vector<float>> &prohibition_position) {
    string path;
    path.append(ros::package::getPath("data_base"));
    path.append("/config/prohibition_areas.yaml");
    // path.append("/home/admin1/test_ws/src/data_base/config/prohibition_areas.yaml");
    YAML::Node config = YAML::LoadFile(path);
    int prohibition_num;//障碍物个数
    prohibition_num = config["prohibition_areas"].size();
    vector<int> point_num;
    for (int i = 0; i < prohibition_num; i++)//第i个障碍物包含的点的个数
    {
        point_num.push_back(config["prohibition_areas"][i].size());
    }
    int new_prohibition_num = prohibition_num;//两个new变量用于保存修改后的文件的障碍物个数以及每个障碍中的点数
    vector<int> new_point_num = point_num;

    //存入prohibition_position
    for (int i = 1; i < prohibition_num; i++) {
        vector<float> temp1;
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


// int set_partition(vector<float> &point, int num) {

//     YAML::Node config = YAML::LoadFile(partition_path);
//     ofstream fout(partition_path);

//     int partition_num;//障碍物个数

//     config["partition"][0][0][0] = -100;
//     config["partition"][0][0][1] = -100;
//     //对yaml文件进行操作
//     for (int i = 0; i < point.size(); i++) {
//         config["partition"][1][0][i] = point[i];
//         config["partition"][1][0][i] = point[i];
//     }

//     fout << config;//将修改写进yaml文件

//     fout.close();//文件关闭后yaml内容才会变化
//     return 1;
// }

// int reset_partition(void) {
//     YAML::Node config = YAML::LoadFile(partition_path);
//     ofstream fout(partition_path);
//     config.reset();//将yaml文件清空
//     //对yaml文件进行操作

//     config["partition"][0][0][0] = -100;
//     config["partition"][0][0][1] = -100;//保留一个点，不然文件会无法访问

//     fout << config;//将修改写进yaml文件

//     fout.close();//文件关闭后yaml内容才会变化

//     return 1;
// }

// bool get_partition(vector<vector<float>> &partition) {
//     YAML::Node config = YAML::LoadFile(partition_path);
//     int partition_num;//障碍物个数
//     partition_num = config["partition"].size();
//     vector<int> point_num;
//     for (int i = 0; i < partition_num; i++)//第i个障碍物包含的点的个数
//     {
//         point_num.push_back(config["partition"][i].size());
//     }
//     int new_partition_num = partition_num;//两个new变量用于保存修改后的文件的障碍物个数以及每个障碍中的点数
//     vector<int> new_point_num = point_num;

//     //存入partition_path
//     for (int i = 1; i < partition_num; i++) {
//         vector<float> temp1;
//         //        cout<<"第"<<i<<"个障碍:"<<endl;
//         for (int j = 0; j < point_num[i]; j++) {
//             temp1.push_back(atof(config["partition"][i][j][0].as<std::string>().c_str()));
//             temp1.push_back(atof(config["partition"][i][j][1].as<std::string>().c_str()));
//         }
//         partition.push_back(temp1);
//     }
//     return true;
// }


bool write_into_file(char const *fileName, char const *content) {
    ofstream out;
    out.open(fileName);
    if (!out.is_open()) {
        cout << "file not exit" << endl;
        return false;
    } else {
        out << content;
        cout << "write succeed" << endl;
        out.close();
        return true;
    }

    return false;
}

string read_from_file(char const *fileName) {
    char content[256];
    string line;
    ifstream in;
    in.open(fileName);
    if (!in.is_open()) {
        cout << "file not exit" << endl;
    } else {
        in.getline(content, 256);
        line = content;
    }

    return line;
}