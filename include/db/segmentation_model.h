//
// Created by Looper on 2022/9/13.
//

#ifndef APP_COMMUNICATION_SEGMENTATION_MODEL_H
#define APP_COMMUNICATION_SEGMENTATION_MODEL_H

#include <ostream>
#include "string"

class MapPo {
public:
    std::string id;
    std::string name;
    std::string path;

    MapPo();

    MapPo(std::string id, std::string name, std::string path);
};

class RoomPo {
public:
    int id{};//UUID name

    std::string o_map_id;

    std::string name;

    int value{};//rand() % 52224 + 13056 映射到地图中
    double area{};//面积
    double center_x{};//中心点x
    double center_y{};//中心点y
    double perimeter{};//周长
    std::string neighbor_room_ids;//邻居id 3,4

    RoomPo();

    RoomPo(std::string oMapId, std::string name, int value, double area, double centerX, double centerY,
           double perimeter, std::string neighborRoomIds);

    friend std::ostream &operator<<(std::ostream &os, const RoomPo &room);
};


#endif //APP_COMMUNICATION_SEGMENTATION_MODEL_H
