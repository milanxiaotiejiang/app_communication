//
// Created by Looper on 2022/10/8.
//

#ifndef APP_COMMUNICATION_NEAREST_NEIGHBOR_TSP_H
#define APP_COMMUNICATION_NEAREST_NEIGHBOR_TSP_H

#include "exploration/A_star_pathplanner.h"

class NearestNeighborTSPSolver {
protected:

    AStarPlanner pathplanner_;

public:
    NearestNeighborTSPSolver();

    std::vector<int>
    solveNearestTSP(const cv::Mat &path_length_matrix, const int start_node);

    std::vector<int>
    solveNearestTSP(const cv::Mat &original_map, const std::vector<cv::Point> &points, double downsampling_factor,
                    double robot_radius, double map_resolution, const int start_node, cv::Mat *distance_matrix = 0);

    std::vector<int> solveNearestTSPClean(const cv::Mat &original_map, const std::vector<cv::Point> &points,
                                          double downsampling_factor, double robot_radius, double map_resolution,
                                          const int start_node);

    std::vector<int> solveNearestTSPWithCleanedDistanceMatrix(const cv::Mat &distance_matrix,
                                                              const std::map<int, int> &cleaned_index_to_original_index_mapping,
                                                              const int start_node);
};


#endif //APP_COMMUNICATION_NEAREST_NEIGHBOR_TSP_H
