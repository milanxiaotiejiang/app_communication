//
// Created by Looper on 2022/9/15.
//

#ifndef APP_COMMUNICATION_ABSTRACT_VORONOI_SEGMENTATION_H
#define APP_COMMUNICATION_ABSTRACT_VORONOI_SEGMENTATION_H

#include <opencv2/opencv.hpp>
#include "Room.h"

#define PI 3.14159265

struct cv_Point_comp {
    bool operator()(const cv::Point &lhs, const cv::Point &rhs) const {
        return ((lhs.y < rhs.y) || (lhs.y == rhs.y && lhs.x < rhs.x));
    }
};

class AbstractVoronoiSegmentation {
protected:
    bool determineRoomIndexFromRoomID(const std::vector<Room> &rooms, const int room_id, size_t &room_index);

    void mergeRoomPair(std::vector<Room> &rooms, const int target_index, const int room_to_merge_index,
                       cv::Mat &segmented_map, const double map_resolution);

    void drawVoronoi(cv::Mat &img, const std::vector<std::vector<cv::Point2f> > &facets_of_voronoi,
                     const cv::Scalar voronoi_color, const cv::Mat &eroded_map);

    void createVoronoiGraph(cv::Mat &map_for_voronoi_generation);

    void pruneVoronoiGraph(cv::Mat &voronoi_map, std::set<cv::Point, cv_Point_comp> &node_points);

    void mergeRooms(cv::Mat &map_to_merge_rooms, std::vector<Room> &rooms, double map_resolution_from_subscription,
                    double max_area_for_merging);

    void formatRooms(cv::Mat &map_to_merge_rooms, std::vector<Room> &rooms, double map_resolution_from_subscription);

    void standardMergeRoom(cv::Mat &map_to_merge_rooms, std::vector<Room> &rooms,
                           double map_resolution_from_subscription, double sign_area);

public:
    AbstractVoronoiSegmentation();
};


#endif //APP_COMMUNICATION_ABSTRACT_VORONOI_SEGMENTATION_H
