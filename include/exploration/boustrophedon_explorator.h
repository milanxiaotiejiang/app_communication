//
// Created by Looper on 2022/9/30.
//

#ifndef APP_COMMUNICATION_BOUSTROPHEDON_EXPLORATOR_H
#define APP_COMMUNICATION_BOUSTROPHEDON_EXPLORATOR_H

#include <opencv2/opencv.hpp>
#include <geometry_msgs/Pose2D.h>
#include <geometry_msgs/PoseStamped.h>
#include <Eigen/Dense>
#include "segmentation/meanshift2d.h"
#include "A_star_pathplanner.h"

class GeneralizedPolygon {
protected:
    std::vector<cv::Point> vertices_;
    cv::Point center_;
    cv::Point bounding_box_center_;
    int max_x_, min_x_, max_y_, min_y_;
    double area_;

public:
    GeneralizedPolygon(const std::vector<cv::Point> &vertices, const double map_resolution) {
        vertices_ = vertices;

        max_x_ = 0;
        min_x_ = 100000;
        max_y_ = 0;
        min_y_ = 100000;
        for (size_t point = 0; point < vertices_.size(); ++point) {
            if (vertices_[point].x > max_x_)
                max_x_ = vertices_[point].x;
            if (vertices_[point].y > max_y_)
                max_y_ = vertices_[point].y;
            if (vertices_[point].x < min_x_)
                min_x_ = vertices_[point].x;
            if (vertices_[point].y < min_y_)
                min_y_ = vertices_[point].y;
        }

        bounding_box_center_.x = (min_x_ + max_x_) / 2;
        bounding_box_center_.y = (min_y_ + max_y_) / 2;

        MeanShift2D ms;
        cv::Mat room = cv::Mat::zeros(max_y_ + 10, max_x_ + 10, CV_8UC1);
        cv::drawContours(room, std::vector<std::vector<cv::Point> >(1, vertices), -1, cv::Scalar(255), CV_FILLED);

//        cv::imshow("room", room);
//        cv::waitKey();

        //返回灰度值不为0的像素数
        area_ = cv::countNonZero(room);
        cv::Mat distance_map;// 欧式距离 因此 type 为 CV_32FC1
        cv::distanceTransform(room, distance_map, CV_DIST_L2, 5);

//        cv::imshow("distance_map", distance_map);
//        cv::waitKey();

        double min_val = 0., max_val = 0.;
        //找出最大值和最小值，其中返回值必须为double类型。
        cv::minMaxLoc(distance_map, &min_val, &max_val);

        std::vector<cv::Vec2d> room_cells;
        for (int v = 0; v < distance_map.rows; ++v)
            for (int u = 0; u < distance_map.cols; ++u)
                if (distance_map.at<float>(v, u) > max_val * 0.95f)
                    room_cells.push_back(cv::Vec2d(u, v));

        cv::Vec2d room_center = ms.findRoomCenter(room, room_cells, map_resolution);

        center_.x = room_center[0];
        center_.y = room_center[1];
    }

    std::vector<cv::Point> getVertices() const {
        return vertices_;
    }

    cv::Point getCenter() const {
        return center_;
    }

    cv::Point getBoundingBoxCenter() const {
        return bounding_box_center_;
    }

    double getArea() const {
        return area_;
    }

    void drawPolygon(cv::Mat &image, const cv::Scalar &color) const {
        // draw polygon in an black image with necessary size
        cv::Mat black_image = cv::Mat(max_y_ + 10, max_x_ + 10, CV_8UC1, cv::Scalar(0));
        cv::drawContours(black_image, std::vector<std::vector<cv::Point> >(1, vertices_), -1, color, CV_FILLED);

        image = black_image.clone();
    }

    void getMinMaxCoordinates(int &min_x, int &max_x, int &min_y, int &max_y) {
        min_x = min_x_;
        max_x = max_x_;
        min_y = min_y_;
        max_y = max_y_;
    }

};

struct BoustrophedonCell {
    typedef std::set<boost::shared_ptr<BoustrophedonCell> > BoustrophedonCellSet;
    typedef std::set<boost::shared_ptr<BoustrophedonCell> >::iterator BoustrophedonCellSetIterator;

    int label_;                // label id of the cell 用来表示不同区间
    double area_;            // area of the cell, in [pixel^2] 表示区间的面积大小
    cv::Rect bounding_box_;        // bounding box of the cell
    BoustrophedonCellSet neighbors_;        // pointer to neighboring cells 使用set的方式存储该区间相邻的区间

    BoustrophedonCell(const int label, const double area, const cv::Rect &bounding_box) {
        label_ = label;
        area_ = area;
        bounding_box_ = bounding_box;
    }

};

class BoustrophedonExplorer {
protected:
    AStarPlanner path_planner_;
    bool isShow;

    static const uchar BORDER_PIXEL_VALUE = 25;

    void computeCellDecompositionWithRotation(const cv::Mat &room_map, const float map_resolution,
                                              const double min_cell_area,
                                              const int min_cell_width, const double rotation_offset, cv::Mat &R,
                                              cv::Rect &bbox, cv::Mat &rotated_room_map,
                                              std::vector<GeneralizedPolygon> &cell_polygons,
                                              std::vector<cv::Point> &polygon_centers);

    void computeCellDecomposition(const cv::Mat &room_map, const float map_resolution, const double min_cell_area,
                                  const int min_cell_width, std::vector<GeneralizedPolygon> &cell_polygons,
                                  std::vector<cv::Point> &polygon_centers);

    void computeBoustrophedonPath(const cv::Mat &room_map, const float map_resolution, const GeneralizedPolygon &cell,
                                  std::vector<cv::Point2f> &fov_middlepoint_path,
                                  std::vector<std::vector<cv::Point2f>> &complex_middle_path,
                                  cv::Point &robot_pos,
                                  const int grid_spacing_as_int, const int half_grid_spacing_as_int,
                                  const double path_eps, const int max_deviation_from_track,
                                  const int grid_obstacle_offset = 0);

    void computeRectangularAmbulatoryPlanePath(const cv::Mat &room_map, const float map_resolution,
                                               const GeneralizedPolygon &cell,
                                               std::vector<cv::Point2f> &fov_middlepoint_path,
                                               std::vector<std::vector<cv::Point2f>> &complex_middle_path,
                                               cv::Point &robot_pos,
                                               const int grid_spacing_as_int, const int half_grid_spacing_as_int,
                                               const double path_eps, const int max_deviation_from_track,
                                               const int grid_obstacle_offset = 0);

    int mergeCells(cv::Mat &cell_map, cv::Mat &cell_map_labels, const double min_cell_area, const int min_cell_width);

    void mergeCellsSelection(cv::Mat &cell_map, cv::Mat &cell_map_labels,
                             std::map<int, boost::shared_ptr<BoustrophedonCell> > &cell_index_mapping,
                             const double min_cell_area, const int min_cell_width);

    void mergeTwoCells(cv::Mat &cell_map, cv::Mat &cell_map_labels, const BoustrophedonCell &minor_cell,
                       BoustrophedonCell &major_cell,
                       std::map<int, boost::shared_ptr<BoustrophedonCell> > &cell_index_mapping);

    void downsamplePath(const std::vector<cv::Point> &original_path, std::vector<cv::Point> &downsampled_path,
                        cv::Point &cell_robot_pos, const double path_eps);

    void downsamplePathReverse(const std::vector<cv::Point> &original_path, std::vector<cv::Point> &downsampled_path,
                               cv::Point &robot_pos, const double path_eps);

public:
    void
    getExplorationPath(const cv::Mat &room_map, std::vector<geometry_msgs::Pose2D> &path,
                       std::vector<std::vector<geometry_msgs::Pose2D>> &complex_pose_path,
                       const float map_resolution,
                       const cv::Point &starting_position, const cv::Point2d &map_origin,
                       const double grid_spacing_in_pixel,
                       const double grid_obstacle_offset, const double path_eps,
                       const double min_cell_area, const int max_deviation_from_track,
                       int tsp_solver);
};

#endif //APP_COMMUNICATION_BOUSTROPHEDON_EXPLORATOR_H

/**
y_start previous_number_of_segments previous_obstacles_end_x
 10      1+1  =2                      360



y_start number_of_segments current_obstacles_start_x   current_obstacles_end_x
 11         5              93 129 154 216 381          124 140 204 250
 12         7              96 109 117 129 154 229 381  103 112 120 140 204 250
 13         7              96 109 117 130 154 229 381  103 112 120 140 204 250
 14         6              96 109 117 154 229 381      103 112 120 204 250
 15         6              96 109 117 154 229 381      103 112 120 204 250
 16         6              96 109 117 154 229 381      103 112 120 204 250
 17         6              96 109 117 154 229 381      103 112 120 204 250
 18         5              96 113 154 229 381          103 120 204 250
*/