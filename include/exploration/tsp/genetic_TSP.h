//
// Created by Looper on 2022/10/8.
//

#ifndef APP_COMMUNICATION_GENETIC_TSP_H
#define APP_COMMUNICATION_GENETIC_TSP_H

#include <iostream>
#include <iomanip>
#include <string>
#include <math.h>
#include <ctime>
#include <cstdlib>
#include <stdio.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "exploration/A_star_pathplanner.h"
#include "exploration/distance_matrix.h"

class GeneticTSPSolver {
protected:

    AStarPlanner pathplanner_;

    double getPathLength(const cv::Mat &path_length_Matrix, std::vector<int> given_path);

    std::vector<int> mutatePath(const std::vector<int> &parent_path);

    std::vector<int>
    getBestPath(const std::vector<std::vector<int> > paths, const cv::Mat &pathlength_Matrix, bool &changed);

    void distance_matrix_thread(DistanceMatrix &distance_matrix_computation, cv::Mat &distance_matrix,
                                const cv::Mat &original_map, const std::vector<cv::Point> &points,
                                double downsampling_factor,
                                double robot_radius, double map_resolution, AStarPlanner &path_planner);

    bool abort_computation_;

    int min_number_of_generations_;

    int const_generations_number_;

public:
    GeneticTSPSolver(int min_number_of_gens = 2300, int const_generations = 100);

    void abortComputation();

    std::vector<int> solveGeneticTSP(const cv::Mat &path_length_Matrix, const int start_Node);

    std::vector<int>
    solveGeneticTSP(const cv::Mat &original_map, const std::vector<cv::Point> &points, double downsampling_factor,
                    double robot_radius, double map_resolution, const int start_Node, cv::Mat *distance_matrix = 0);

    std::vector<int> solveGeneticTSPClean(const cv::Mat &original_map, const std::vector<cv::Point> &points,
                                          double downsampling_factor, double robot_radius, double map_resolution,
                                          const int start_node);

    std::vector<int> solveGeneticTSPWithCleanedDistanceMatrix(const cv::Mat &distance_matrix,
                                                              const std::map<int, int> &cleaned_index_to_original_index_mapping,
                                                              const int start_node);
};


#endif //APP_COMMUNICATION_GENETIC_TSP_H
