//
// Created by Looper on 2022/10/11.
//

#ifndef APP_COMMUNICATION_HANDLE_EXPLORATION_DISPLAY_H
#define APP_COMMUNICATION_HANDLE_EXPLORATION_DISPLAY_H

#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <geometry_msgs/Pose2D.h>

void planning_pose_path_display(const cv::Mat &map, const cv::Point2d &map_origin,
                                const std::vector<std::vector<geometry_msgs::Pose2D>> &complex_path,
                                float resize, const std::string &winname);

void planning_pose_path_display(const cv::Mat &map, const cv::Point2d &map_origin,
                                std::vector<geometry_msgs::Pose2D> exploration_path,
                                float resize, const std::string &winname);

void planning_point_path_display(const cv::Mat &map, std::vector<cv::Point> point_path,
                                 float resize, const std::string &winname);

void center_point_display(const cv::Mat &map, const cv::Point &point, float resize,
                          const std::string &winname);

void planning_point_segmentation_path_display(const cv::Mat &map, std::vector<int> optimal_order,
                                              std::vector<cv::Point> polygon_centers,
                                              std::map<int, std::vector<cv::Point>> each_map,
                                              std::map<int, std::vector<std::vector<cv::Point>>> each_contours_map,
                                              float resize, const std::string &winname);

void save_planning_point_segmentation_path(const cv::Mat &map, cv::Mat segmented_map, cv::Point start_point,
                                           std::vector<int> optimal_order,
                                           std::vector<cv::Point> polygon_centers,
                                           std::map<int, std::vector<cv::Point>> each_map,
                                           std::map<int, std::vector<std::vector<cv::Point>>> each_contours_map);

void save_dynamic_map(const std::string& save_name);

#endif //APP_COMMUNICATION_HANDLE_EXPLORATION_DISPLAY_H
