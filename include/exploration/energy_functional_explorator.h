//
// Created by Looper on 2022/10/19.
//

#ifndef APP_COMMUNICATION_ENERGY_FUNCTIONAL_EXPLORATOR_H
#define APP_COMMUNICATION_ENERGY_FUNCTIONAL_EXPLORATOR_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <geometry_msgs/Pose2D.h>
#include <geometry_msgs/Polygon.h>
#include <geometry_msgs/Point32.h>

#define PI 3.14159265359
const double PI_2_INV = 1. / (0.5 * PI);

struct EnergyExploratorNode {

    cv::Point center_;
    bool obstacle_;
    bool visited_;
    std::vector<EnergyExploratorNode *> neighbors_;

    int countNonObstacleNeighbors() {
        int non_obstacle_neighbors = 0;
        for (std::vector<EnergyExploratorNode *>::iterator neighbor = neighbors_.begin();
             neighbor != neighbors_.end(); ++neighbor)
            if ((*neighbor)->obstacle_ == false)
                ++non_obstacle_neighbors;
        return non_obstacle_neighbors;
    }

};

struct cv_Point_cmp {

    bool operator()(const cv::Point &lhs, const cv::Point &rhs) const {
        return ((lhs.y < rhs.y) || (lhs.y == rhs.y && lhs.x < rhs.x));
    }

};

class EnergyFunctionalExplorator {

protected:
    double E(const EnergyExploratorNode &location,
             const EnergyExploratorNode &neighbor,
             const double cell_size_in_pixel,
             const double previous_travel_angle);

public:
    EnergyFunctionalExplorator();

    void getExplorationPath(const cv::Mat &room_map, std::vector<geometry_msgs::Pose2D> &path,
                            std::vector<std::vector<geometry_msgs::Pose2D>> &complex_pose_path,
                            const float map_resolution,
                            const cv::Point starting_position, const cv::Point2d map_origin,
                            const double grid_spacing_in_pixel);
};


#endif //APP_COMMUNICATION_ENERGY_FUNCTIONAL_EXPLORATOR_H
