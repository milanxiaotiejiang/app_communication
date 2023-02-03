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


/**
 *
# Applies a closing operation to neglect inaccessible areas and map errors/artifacts if the map_correction_closing_neighborhood_size parameter is larger than 0.
# The parameter then specifies the iterations (or neighborhood size) of that closing operation.
# int
#  闭合邻域大小的地图校正
map_correction_closing_neighborhood_size: 2

# min area a boustrophedon cell (i.e. a boustrophedon region of the map) must have to be determined for the path generation
# [pixel^2]
# double
min_cell_area: 200.0      #10.0

# distance between two points when generating the (line) path
# [pixel], where one pixel is the size of one cell in the navigation grid map
# double
path_eps: 6.0

# the additional offset of the grid to obstacles, i.e. allows to displace the grid by more than the standard half_grid_size from obstacles
# [m]
# double
grid_obstacle_offset: 0.251     #0.101        #0.251

# maximal allowed shift off the ideal boustrophedon track to both sides for avoiding obstacles on track
# setting max_deviation_from_track=grid_spacing is usually a good choice
# for negative values (e.g. max_deviation_from_track: -1) max_deviation_from_track is automatically set to grid_spacing
# [pixel]
max_deviation_from_track: 0


#Voronoi分割：120.0-1.53（表示由临界线分隔的区域允许具有的最大/最小面积）
room_area_factor_upper_limit_voronoi: 1000000.0        #120.0
room_area_factor_lower_limit_voronoi: 0.1              #1.53

#parameters for the voronoi segmentation that specify the neighborhood for critical Point extraction and the distance between critical Points.
voronoi_neighborhood_index: 280         #larger value sets a larger neighborhood for searching critical points --> int
max_iterations: 150                     #设置搜索邻域的最大迭代次数，也用于vrf分割-->int
min_critical_point_distance_factor: 0.5 #1.6#消除其中一个临界点之前两个临界点之间的最小距离因子-->双
max_area_for_merging: 12.5              #应与其周围房间合并的房间的最大面积[m²]，也用于voronoi随机场分割
 */
class PlanPo {
public:
    std::string map_id;
    double robot_radius;//机器人半径 0.1-0.3
    int map_correction_closing_neighborhood_size;//外围区域闭合邻域大小 0-10
    double grid_obstacle_offset;//障碍物的额外偏移 0-0.3
    double path_eps;//路径规划时两点间距  1-10
    double min_cell_area;//最小规划面积 30-500
    double max_deviation_from_track;//轨道最大允许偏移量 -1-5
    int range_near_base_station;//基站范围 0-10

    double room_area_factor_lower_limit;//临界线分隔的区域允许具有的最小面积 0.1-20
    double room_area_factor_upper_limit;//临界线分隔的区域允许具有的最大面积 100-1000000
    int neighborhood_index;//搜索临界点 70-600
    int max_iterations;//搜索邻域的最大迭代次数 60-240
    double min_critical_point_distance_factor;//消除临界点与之前两个临界点之间的最小距离 0-1.3
    double max_area_for_merging;//与其周围房间合并的房间的最大面积 3-1000

    int distance_from_obstacles;//与障碍物的间距 0-10
    int number_extension;//生成贴边轮廓的个数 1-3
    int multiple_contour_spacing;//多个贴边轮廓的间距 -3-3
    int random_number_generation_ratio;//可达点的计算比例 50-200
    int boundary_min_area;//贴边范围的最小面积

    PlanPo();

    PlanPo(const std::string &mapId, double robotRadius, int mapCorrectionClosingNeighborhoodSize,
           double gridObstacleOffset, double pathEps, double minCellArea, double maxDeviationFromTrack,
           int rangeNearBaseStation, double roomAreaFactorLowerLimit, double roomAreaFactorUpperLimit,
           int neighborhoodIndex, int maxIterations, double minCriticalPointDistanceFactor, double maxAreaForMerging,
           int distanceFromObstacles, int numberExtension, int multipleContourSpacing, int randomNumberGenerationRatio,
           int boundaryMinArea);
};


#endif //APP_COMMUNICATION_SEGMENTATION_MODEL_H
