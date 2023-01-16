//
// Created by Looper on 2022/10/8.
//

#include "exploration/tsp/nearest_neighbor_TSP.h"
#include "exploration/distance_matrix.h"


NearestNeighborTSPSolver::NearestNeighborTSPSolver() {

}

std::vector<int> NearestNeighborTSPSolver::solveNearestTSP(const cv::Mat &path_length_matrix, const int start_node) {
    std::vector<int> calculated_order;

    if (path_length_matrix.rows > 1) {
        std::vector<bool> visited(path_length_matrix.rows, false);

        int current_node = start_node;
        calculated_order.push_back(current_node);
        visited[current_node] = true;

        do {
            int next_node;
            double min_distance = 1e100;
            for (int current_neighbor = 0; current_neighbor < path_length_matrix.cols; current_neighbor++) {
                if (!visited[current_neighbor]) {
                    const double length = path_length_matrix.at<double>(current_node, current_neighbor);
                    if (length < min_distance && length > 0) {
                        next_node = current_neighbor;
                        min_distance = length;
                    }
                }
            }
            calculated_order.push_back(next_node);
            visited[next_node] = true;
            current_node = next_node;
        } while (calculated_order.size() < path_length_matrix.rows);

    } else {
        calculated_order.push_back(start_node);
    }

    return calculated_order;
}

std::vector<int>
NearestNeighborTSPSolver::solveNearestTSP(const cv::Mat &original_map, const std::vector<cv::Point> &points,
                                          double downsampling_factor, double robot_radius, double map_resolution,
                                          const int start_node, cv::Mat *distance_matrix) {
    LOG(INFO) << "NearestNeighborTSPSolver::solveNearestTSP: Constructing distance matrix...";
    cv::Mat distance_matrix_ref;
    if (distance_matrix != nullptr)
        distance_matrix_ref = *distance_matrix;
    DistanceMatrix distance_matrix_computation;
    distance_matrix_computation.constructDistanceMatrix(distance_matrix_ref, original_map, points, downsampling_factor,
                                                        robot_radius, map_resolution, pathplanner_);

    return solveNearestTSP(distance_matrix_ref, start_node);
}


std::vector<int>
NearestNeighborTSPSolver::solveNearestTSPClean(const cv::Mat &original_map, const std::vector<cv::Point> &points,
                                               double downsampling_factor, double robot_radius, double map_resolution,
                                               const int start_node) {
    cv::Mat distance_matrix_cleaned;
    std::map<int, int> cleaned_index_to_original_index_mapping;
    int new_start_node = start_node;
    DistanceMatrix distance_matrix_computation;
    distance_matrix_computation.computeCleanedDistanceMatrix(original_map, points, downsampling_factor, robot_radius,
                                                             map_resolution, pathplanner_,
                                                             distance_matrix_cleaned,
                                                             cleaned_index_to_original_index_mapping, new_start_node);

    return solveNearestTSPWithCleanedDistanceMatrix(distance_matrix_cleaned, cleaned_index_to_original_index_mapping,
                                                    new_start_node);
}


std::vector<int> NearestNeighborTSPSolver::solveNearestTSPWithCleanedDistanceMatrix(const cv::Mat &distance_matrix,
                                                                                    const std::map<int, int> &cleaned_index_to_original_index_mapping,
                                                                                    const int start_node) {
    // solve TSP and re-index points to original indices
    std::vector<int> optimal_order = solveNearestTSP(distance_matrix, start_node);
    for (size_t i = 0; i < optimal_order.size(); ++i)
        optimal_order[i] = cleaned_index_to_original_index_mapping.at(optimal_order[i]);

    return optimal_order;
}

