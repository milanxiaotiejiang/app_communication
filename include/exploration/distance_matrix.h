//
// Created by Looper on 2022/10/8.
//

#ifndef APP_COMMUNICATION_DISTANCE_MATRIX_H
#define APP_COMMUNICATION_DISTANCE_MATRIX_H

#include <vector>
#include <opencv2/opencv.hpp>
#include "A_star_pathplanner.h"
#include "timer.h"
#include "glog/logging.h"

class DistanceMatrix {
protected:

    bool abort_computation_;

public:

    DistanceMatrix() : abort_computation_(false) {
    }

    void abortComputation() {
        abort_computation_ = true;
    }

    void constructDistanceMatrix(cv::Mat &distance_matrix, const cv::Mat &original_map,
                                 const std::vector<cv::Point> &points,
                                 double downsampling_factor, double robot_radius, double map_resolution,
                                 AStarPlanner &path_planner,
                                 std::vector<std::vector<std::vector<cv::Point> > > *paths = nullptr) {
        LOG(INFO) << "DistanceMatrix::constructDistanceMatrix: Constructing distance matrix...";
        Timer tim;

        /**
         * 一个二维的距离矩阵，该矩阵保存各个区间的两两距离，在进行距离计算时，只需计算二维矩阵对角右上方的两点距离，计算完毕即可得出所有的区间之间的最短距离。
         */
        distance_matrix.create((int) points.size(), (int) points.size(), CV_64F);

        if (points.size() > 500)
            downsampling_factor *= 0.5;

//        const double one_by_downsampling_factor = 1. / downsampling_factor;
        cv::Mat downsampled_map;
        path_planner.downsampleMap(original_map, downsampled_map, downsampling_factor, robot_radius, map_resolution);

        if (points.size() > 500)
            std::cout
                    << "0         10        20        30        40        50        60        70        80        90        100"
                    << std::endl;
        for (int i = 0; i < points.size(); i++) {
            if (points.size() > 500 && i % (std::max(1, (int) points.size() / 100)) == 0)
                std::cout << "." << std::flush;
            for (int j = 0; j < points.size(); j++) {
                if (j != i) {
                    if (j > i) {
                        if (abort_computation_)
                            return;

                        // 将两点之间连一条直线
                        cv::LineIterator it(original_map, points[i], points[j]);
                        // 计算该直线上是否穿过障碍物
                        bool direct_connection = true;
                        for (int k = 0; k < it.count && direct_connection; k++, ++it)
                            if (**it < 250)
                                direct_connection = false;

                        if (direct_connection) {// 没有穿过障碍物，则直线距离即为两点之间的最短距离
                            const double length = cv::norm(points[i] - points[j]);
                            distance_matrix.at<double>(i, j) = length;
                            distance_matrix.at<double>(j, i) = length;

                            if (paths != nullptr) {
                                cv::LineIterator it2(original_map, points[i], points[j]);
                                std::vector<cv::Point> current_path(it2.count);
                                for (int k = 0; k < it2.count; k++, ++it2)
                                    current_path[k] = it2.pos();

                                paths->at(i).at(j) = current_path;
                                paths->at(j).at(i) = current_path;
                            }
                        } else {// 穿过障碍物
                            if (paths != nullptr) {
                                // A*算法计算出两点之间的最短距离
                                std::vector<cv::Point> current_path;
                                double length = path_planner.planPath(original_map, downsampled_map, points[i],
                                                                      points[j], downsampling_factor, 0.,
                                                                      map_resolution, 0, nullptr, &current_path);
                                distance_matrix.at<double>(i, j) = length;
                                distance_matrix.at<double>(j, i) = length;

                                for (std::vector<cv::Point>::iterator point = current_path.begin();
                                     point != current_path.end(); ++point) {
                                    point->x = point->x / downsampling_factor;
                                    point->y = point->y / downsampling_factor;
                                }

                                paths->at(i).at(j) = current_path;
                                paths->at(j).at(i) = current_path;
                            } else {
                                double length = path_planner.planPath(original_map, downsampled_map, points[i],
                                                                      points[j], downsampling_factor, 0.,
                                                                      map_resolution);
                                if (j == 9) {
                                    double length2 = path_planner.planPath(original_map, downsampled_map, points[i],
                                                                           points[j], downsampling_factor, 0.,
                                                                           map_resolution);
                                }
                                distance_matrix.at<double>(i, j) = length;
                                distance_matrix.at<double>(j, i) = length;
                            }
                        }
                    }
                } else {
                    distance_matrix.at<double>(i, j) = 0;
                }
            }
        }
        std::cout << "\n";

        std::cout
                << "Distance matrix data ================================================================================================================================ ";

        for (int y = 0; y < distance_matrix.rows; y++) {
            for (int x = 0; x < distance_matrix.cols; x++) {
                const std::string &basicString = std::to_string((int) distance_matrix.at<double>(y, x));
                if (basicString.length() == 2) {
                    std::cout << " ";
                } else if (basicString.length() == 1) {
                    std::cout << "  ";
                }
                std::cout << basicString;
                std::cout << "  ";
            }
            std::cout << " " << std::endl;
        }

        LOG(INFO) << "Distance matrix created in " << tim.getElapsedTimeInMilliSec() << " ms";
    }

    void cleanDistanceMatrix(const cv::Mat &distance_matrix, cv::Mat &distance_matrix_cleaned,
                             std::map<int, int> &cleaned_index_to_original_index_mapping) {
        cleaned_index_to_original_index_mapping.clear();
        for (int i = 0; i < distance_matrix.rows; ++i)
            cleaned_index_to_original_index_mapping[i] = i;
        distance_matrix_cleaned = distance_matrix.clone();

        if (distance_matrix.rows < 1)
            return;

        const double max_length = 1e90;

        std::vector<bool> remove_entry(distance_matrix.rows, false);

        cv::Mat distance_matrix_temp = distance_matrix.clone();
        while (true) {
            std::vector<int> infinite_length_entries(distance_matrix_temp.rows, 0);
            for (int i = 0; i < distance_matrix_temp.rows; ++i)
                for (int j = 0; j < distance_matrix_temp.cols; ++j)
                    if (distance_matrix_temp.at<double>(i, j) > max_length)
                        infinite_length_entries[i]++;

            std::multimap<int, int> number_infinite_entries_to_row_index_mapping;
            for (size_t i = 0; i < infinite_length_entries.size(); ++i)
                number_infinite_entries_to_row_index_mapping.insert(
                        std::pair<int, int>(infinite_length_entries[i], (int) i));

            bool mark_line = false;
            int mark_index = -1;
            std::multimap<int, int>::reverse_iterator number_infinite_entries_to_row_index_mapping_last = number_infinite_entries_to_row_index_mapping.rbegin();
            if (number_infinite_entries_to_row_index_mapping_last->first > 0) {
                mark_line = true;
                mark_index = number_infinite_entries_to_row_index_mapping_last->second;
                remove_entry[mark_index] = true;
            }
            if (mark_line == true) {
                for (int j = 0; j < distance_matrix_temp.cols; ++j)
                    distance_matrix_temp.at<double>(mark_index, j) = -1.;
                for (int i = 0; i < distance_matrix_temp.rows; ++i)
                    distance_matrix_temp.at<double>(i, mark_index) = -1.;
            } else
                break;
        }

        int number_entries_to_be_removed = 0;
        for (size_t i = 0; i < remove_entry.size(); ++i)
            if (remove_entry[i])
                number_entries_to_be_removed++;

        if (number_entries_to_be_removed > 0) {
            LOG(INFO) << "  DistanceMatrix::cleanDistanceMatrix: Need to remove " << number_entries_to_be_removed
                      << " elements out of " << distance_matrix.rows << " elements from the distance matrix.";

            const int new_size = distance_matrix.rows - number_entries_to_be_removed;
            if (new_size == 0) {
                LOG(INFO)
                        << "  DistanceMatrix::cleanDistanceMatrix: Warning: Would need to remove all elements of distance_matrix. Aborting.";
                return;
            }
            distance_matrix_cleaned.create(new_size, new_size, CV_64F);
            cleaned_index_to_original_index_mapping.clear();

            int new_index = 0;
            for (size_t i = 0; i < remove_entry.size(); ++i) {
                if (!remove_entry[i]) {
                    cleaned_index_to_original_index_mapping[new_index] = (int) i;

                    int new_j = 0;
                    for (size_t j = 0; j < remove_entry.size(); ++j) {
                        if (!remove_entry[j]) {
                            distance_matrix_cleaned.at<double>(new_index, new_j) = distance_matrix.at<double>(i, j);
                            new_j++;
                        }
                    }
                    new_index++;
                }
            }
            if (new_index != new_size)
                LOG(INFO)
                        << "##################################################\nDistanceMatrix::cleanDistanceMatrix: Warning: new_index != new_size.\n##################################################";
        }
    }

    void computeCleanedDistanceMatrix(const cv::Mat &original_map, const std::vector<cv::Point> &points,
                                      double downsampling_factor, double robot_radius, double map_resolution,
                                      AStarPlanner &path_planner,
                                      cv::Mat &distance_matrix,
                                      std::map<int, int> &cleaned_index_to_original_index_mapping, int &start_node) {
        LOG(INFO) << "DistanceMatrix::computeCleanedDistanceMatrix: Constructing distance matrix...";
        cv::Mat distance_matrix_raw;
        constructDistanceMatrix(distance_matrix_raw, original_map, points, downsampling_factor, robot_radius,
                                map_resolution, path_planner);

        cleanDistanceMatrix(distance_matrix_raw, distance_matrix, cleaned_index_to_original_index_mapping);

        int new_start_node = 0;
        for (std::map<int, int>::iterator it = cleaned_index_to_original_index_mapping.begin();
             it != cleaned_index_to_original_index_mapping.end(); ++it)
            if (it->second == start_node)
                new_start_node = it->first;
        start_node = new_start_node;
    }
};


#endif //APP_COMMUNICATION_DISTANCE_MATRIX_H
