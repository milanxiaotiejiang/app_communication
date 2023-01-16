#include "tool/read_partion_file.h"

using namespace std;

std::string path =
        ros::package::getPath("data_base") + "/config/devide_area.yaml";
std::string name_path =
        ros::package::getPath("data_base") + "/config/devide_area_name.yaml";
std::string map_path =
        ros::package::getPath("robot_slam") + "/maps/mymap.yaml";

bool get_partion_line(vector<Partion_line> &segmentationPoints) {
    YAML::Node config = YAML::LoadFile(path);
    int line_num = config["partition"].size();
    //存入prohibition_position
    for (int i = 1; i < line_num; i++) {

        Point start(atof(config["partition"][i][0].as<std::string>().c_str()),
                    atof(config["partition"][i][1].as<std::string>().c_str()));
        Point end(atof(config["partition"][i][2].as<std::string>().c_str()),
                  atof(config["partition"][i][3].as<std::string>().c_str()));
        Partion_line temp_line(start, end);
        segmentationPoints.push_back(temp_line);
    }
    return true;
}

bool set_partion_line(vector<Partion_line> &segmentationPoints) {
    YAML::Node config = YAML::LoadFile(path);
    ofstream fout(path);

    config.reset();
    config["partition"][0][0] = -100;
    config["partition"][0][1] = -100;

    for (int i = 1; i <= segmentationPoints.size(); i++) {
        config["partition"][i][0] = segmentationPoints[i - 1].getStart().getX();
        config["partition"][i][1] = segmentationPoints[i - 1].getStart().getY();
        config["partition"][i][2] = segmentationPoints[i - 1].getAnEnd().getX();
        config["partition"][i][3] = segmentationPoints[i - 1].getAnEnd().getY();
    }

    fout << config;//将修改写进yaml文件
    fout.close();
    return true;
}

void createBasicName(int max) {
    //加载文件
    YAML::Node config = YAML::LoadFile(name_path);
    //查找info节点
    auto infoNode = config["info"];

    bool unCreate = false;

    if (infoNode.IsDefined() && infoNode.IsMap()) {
        auto map = infoNode.as<std::map<int, string>>();
        auto value = map[max - 1];
        if (!value.empty()) {
            unCreate = true;
        }
    }

    if (!unCreate) {
        ofstream fout(name_path);
        config.reset();
        infoNode.reset();
        //构造map对象
        std::map<int, string> map;
        for (int i = 1; i <= max - 1; i++) {
            map[i] = std::to_string(i);
        }
        infoNode = map;
        config["info"] = infoNode;
        fout << config;//将修改写进yaml文件
        fout.close();
    }

}

string getNameForID(int index) {
    //加载文件
    YAML::Node config = YAML::LoadFile(name_path);
    //查找info节点
    auto infoNode = config["info"];
    //判断是否符合
    if (infoNode.IsDefined() && infoNode.IsMap()) {
        auto map = infoNode.as<std::map<int, string>>();
        auto value = map[index];
        return value;
    }
    return "";
}

bool setNameForID(const Name_info &name_info) {
    //加载文件
    YAML::Node config = YAML::LoadFile(name_path);
    //查找info节点
    auto infoNode = config["info"];
    if (infoNode.IsDefined() && infoNode.IsMap()) {
        auto map = infoNode.as<std::map<int, string>>();
        auto value = map[name_info.getId()];
        if (!value.empty()) {
            map[name_info.getId()] = name_info.getName();

            ofstream fout(name_path);
            config.reset();
            infoNode.reset();
            infoNode = map;
            config["info"] = infoNode;
            fout << config;//将修改写进yaml文件
            fout.close();
            return true;
        }
    }
    return false;
}

void clearBasicName(){
      //加载文件
    YAML::Node config = YAML::LoadFile(name_path);
    //查找info节点
    auto infoNode = config["info"];
    ofstream fout(name_path);
    config.reset();
    infoNode.reset();
    config["info"] = infoNode;
    fout << config;//将修改写进yaml文件
    fout.close();
}

bool get_map_orgin(vector<int> &origin) {
    YAML::Node config = YAML::LoadFile(map_path);
    origin.push_back(atoi(config["origin"][0].as<std::string>().c_str()));
    origin.push_back(atoi(config["origin"][0].as<std::string>().c_str()));
    return true;
}
// bool get_devided_area(vector<float> &partion) {
//     YAML::Node config = YAML::LoadFile(path);

//     for (int j = 0; j < 4; j++) {
//         partion.push_back(atof(config["partition"][1][0][j].as<std::string>().c_str()));
//     }
//     return true;
// }
// bool get_devided_area(vector<vector<float>> &devided_area) {
//     YAML::Node config = YAML::LoadFile(path);
//     int prohibition_num;//障碍物个数
//     prohibition_num = config["partition"].size();
//     vector<int> point_num;
//     for (int i = 0; i < prohibition_num; i++)//第i个障碍物包含的点的个数
//     {
//         point_num.push_back(config["partition"][i].size());
//     }
//     int new_prohibition_num =
//     prohibition_num;//两个new变量用于保存修改后的文件的障碍物个数以及每个障碍中的点数
//     vector<int> new_point_num = point_num;

//     //存入prohibition_position
//     for (int i = 1; i < prohibition_num; i++) {
//         vector<float> temp1;
//         for (int j = 0; j < point_num[i]; j++) {
//             temp1.push_back(atof(config["partition"][i][j][0].as<std::string>().c_str()));
//             temp1.push_back(atof(config["partition"][i][j][1].as<std::string>().c_str()));
//         }
//         devided_area.push_back(temp1);
//     }
//     return true;
// }
