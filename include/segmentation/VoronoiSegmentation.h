//
// Created by Looper on 2022/9/15.
//

#ifndef APP_COMMUNICATION_VORONOISEGMENTATION_H
#define APP_COMMUNICATION_VORONOISEGMENTATION_H

#include "abstract_voronoi_segmentation.h"

class VoronoiSegmentation : public AbstractVoronoiSegmentation {
public:

    VoronoiSegmentation();

    /**
     *
     * @param map_to_be_labeled
     * @param segmented_map
     * @param rooms
     * @param map_resolution_from_subscription 地图分辨率 0.05
     * 下列两个房间面积系数->设置房间的面积限制--------->，单位为[m^2]
     * @param room_area_factor_lower_limit 房间面积系数下限 0.1
     * @param room_area_factor_upper_limit 房间面积系数上限 1000000
     * 下列四个为 voronoi 分割的参数，用于指定临界点提取的邻域和临界点之间的距离。
     * @param neighborhood_index 邻域指数 280 用于 Voronoi 分割的邻域大小，值越大，搜索临界点的邻域越大
     * @param max_iterations 最大迭代 150 搜索邻居的最大迭代次数，也用于vrf分割
     * @param min_critical_point_distance_factor 最小临界点距离因子 0.5 消除其中一个临界点之前，两个临界点之间的最小距离因子
     * @param max_area_for_merging 最大合并面积 12.5 应与其周围房间合并的房间的最大面积[m^2]，也用于voronoi随机场分割
     */
    void segmentMap(const cv::Mat &map_to_be_labeled, cv::Mat &segmented_map, std::vector<Room> &rooms,
                    double map_resolution_from_subscription,
                    double room_area_factor_lower_limit, double room_area_factor_upper_limit,
                    int neighborhood_index, int max_iterations,
                    double min_critical_point_distance_factor, double max_area_for_merging);
};


#endif //APP_COMMUNICATION_VORONOISEGMENTATION_H
