//
// Created by Looper on 2022/9/30.
//

#include <nav_msgs/OccupancyGrid.h>
#include "exploration/boustrophedon_explorator.h"
#include "glog/logging.h"
#include "exploration/room_rotator.h"
#include "exploration/grid.h"
#include "exploration/tsp/nearest_neighbor_TSP.h"
#include "exploration/tsp/genetic_TSP.h"
#include "exploration/tsp/tsp_solver_defines.h"
#include "exploration/cv_extend.h"
#include "exploration/voronoi/voronoi.hpp"

static bool DISPLAY_TRAJECTORY = false;
static bool DISPLAY_TRAJECTORY_RESULT = false;

/**
 *
 * @param room_map
 * @param path
 * @param map_resolution 栅格地图的分辨率,一直默认为0.05
 * @param starting_position 起始位置，后续将动态设置为机器人的当前位置
 * @param map_origin 地图坐标原点，后续将解析地图的yaml动态赋值
 * @param grid_spacing_in_pixel 机器人转成正方形后的边长，单位 px
 * @param grid_obstacle_offset 机器人对障碍物的额外偏移，单位 m
 * @param path_eps 创建的覆盖路径中两点之间的最小距离，单位 px
 * @param min_cell_area 用来过滤小面积区域
 * @param max_deviation_from_track 为避免轨道上的障碍物，最大允许偏离轨道两侧的理想距离
 */
void BoustrophedonExplorer::getExplorationPath(const cv::Mat &room_map, std::vector<geometry_msgs::Pose2D> &pose_path,
                                               std::vector<std::vector<geometry_msgs::Pose2D>> &complex_pose_path,
                                               const float map_resolution, const cv::Point &starting_position,
                                               const cv::Point2d &map_origin, const double grid_spacing_in_pixel,
                                               const double grid_obstacle_offset, const double path_eps,
                                               const double min_cell_area, const int max_deviation_from_track,
                                               int tsp_solver) {

    LOG(INFO) << "Planning the boustrophedon path trough the room.";

    const int grid_spacing_as_int = (int) std::floor(grid_spacing_in_pixel);
    const int half_grid_spacing_as_int = (int) std::floor(0.5 * grid_spacing_in_pixel);
    const int min_cell_width = half_grid_spacing_as_int + 2. * grid_obstacle_offset / map_resolution;

    //定义图像的旋转相关属性（旋转矩阵，旋转中心点、尺寸大小和旋转角度）
    cv::Mat R;
    cv::Rect bbox;
    cv::Mat rotated_room_map;
    //定义每一个区间的多边形点数组、中心点数组
    std::vector<GeneralizedPolygon> cell_polygons;//用来确定区间在地图上的位置
    std::vector<cv::Point> polygon_centers;//用来确定区间跟区间之间的距离
    computeCellDecompositionWithRotation(room_map, map_resolution, min_cell_area, min_cell_width,
                                         0., R, bbox, rotated_room_map, cell_polygons, polygon_centers);

    if (cell_polygons.size() != polygon_centers.size()) {
        LOG(ERROR) << "cell_polygons size != polygon_centers size .";
        return;
    }
    for (int i = 0; i < cell_polygons.size(); ++i) {
        GeneralizedPolygon &generalizedPolygon = cell_polygons[i];
        cv::Point_<int> &point = polygon_centers[i];

        std::string msg;
        msg.append("index=").append(std::to_string(i)).append(" ");
        msg.append("X=").append(std::to_string(point.x)).append(" ");
        msg.append("Y=").append(std::to_string(point.y)).append(" ");
        msg.append("CenterX=").append(std::to_string(generalizedPolygon.getCenter().x)).append(" ");
        msg.append("CenterY=").append(std::to_string(generalizedPolygon.getCenter().y)).append(" ");
        msg.append("BoxCenterX=").append(std::to_string(generalizedPolygon.getBoundingBoxCenter().x)).append(" ");
        msg.append("BoxCenterY=").append(std::to_string(generalizedPolygon.getBoundingBoxCenter().y)).append(" ");
        msg.append("Vertices Size=").append(std::to_string(generalizedPolygon.getVertices().size())).append(" ");
        msg.append("Area=").append(std::to_string((int) generalizedPolygon.getArea())).append(" ");

//        LOG(INFO) << msg;
    }

    LOG(INFO) << "Found the cells in the given map.";

    std::vector<cv::Point> starting_point_vector(1, starting_position);
    //线性图像变换
    cv::transform(starting_point_vector, starting_point_vector, R);
    const cv::Point rotated_starting_point = starting_point_vector[0];

    int start_cell_index = 0;
    for (std::vector<GeneralizedPolygon>::iterator cell = cell_polygons.begin(); cell != cell_polygons.end(); ++cell)
        if (cv::pointPolygonTest(cell->getVertices(), rotated_starting_point, false) >= 0)
            start_cell_index = cell - cell_polygons.begin();

    if (DISPLAY_TRAJECTORY) {
        auto polygon_centers_map = rotated_room_map.clone();
        for (int i = 0; i < polygon_centers.size(); i++) {
            auto point = polygon_centers[i];
            cv::putText(polygon_centers_map, std::to_string(i), point, cv::FONT_HERSHEY_TRIPLEX,
                        0.8, cv::Scalar(128), 1, CV_AA);
            cv::circle(polygon_centers_map, point, 3, cv::Scalar(160), CV_FILLED);
        }
        cv::imshow("polygon_centers_map", polygon_centers_map);
        cv::waitKey();
    }

    std::vector<int> optimal_order;
    // 确定单元格的最佳访问顺序
    if (tsp_solver == TSP_GENETIC) {
        //ROS默认使用的计算TSP遍历顺序的算法是遗传算法，且会先将地图缩放0.25倍后进行计算。默认使用的是GeneticTSPSolver，即用遗传算法来求解区间遍历顺序
        LOG(INFO) << "GeneticTSPSolver .. ";
        GeneticTSPSolver tsp_solver;
        optimal_order = tsp_solver.solveGeneticTSP(rotated_room_map, polygon_centers, 0.25, 0.0, map_resolution,
                                                   start_cell_index, 0);
        if (optimal_order.size() != polygon_centers.size()) {
            LOG(INFO)
                    << "=====================> Genetic TSP failed with 25% resolution, falling back to 100%. <=======================";
            optimal_order = tsp_solver.solveGeneticTSP(rotated_room_map, polygon_centers, 1.0, 0.0,
                                                       map_resolution, start_cell_index, 0);
        }
    } else if (tsp_solver == TSP_NEAREST_NEIGHBOR) {
        // 一种通过计算最临近区域求出TSP近似解的方式，不追求下方的遗传学 TSP 的最优解，只求近似解为止（比下方步缺少一步）
        // 算法原理：每次都取离当前位置最近的区域为下一个清扫区域，到达下一个区域后，再取最近的区域为下一个清扫区域，即遗传学TSP前半段
        LOG(INFO) << "NearestNeighborTSPSolver .. ";
        NearestNeighborTSPSolver neighbor_tsp_solver;
        optimal_order = neighbor_tsp_solver.solveNearestTSP(rotated_room_map, polygon_centers, 0.2, 0.0,
                                                            map_resolution, start_cell_index, 0);
        if (optimal_order.size() != polygon_centers.size()) {
            LOG(INFO)
                    << "=====================> Genetic TSP failed with 25% resolution, falling back to 100%. <=======================";
            optimal_order = neighbor_tsp_solver.solveNearestTSP(rotated_room_map, polygon_centers, 1.0, 0.0,
                                                                map_resolution, start_cell_index, 0);
        }
    }

    if (DISPLAY_TRAJECTORY) {
        auto polygon_centers_map2 = rotated_room_map.clone();
        for (int i = 0; i < optimal_order.size(); i++) {
            auto point = polygon_centers[optimal_order[i]];
            cv::putText(polygon_centers_map2, std::to_string(i), point, cv::FONT_HERSHEY_TRIPLEX,
                        0.8, cv::Scalar(128), 1, CV_AA);
            cv::circle(polygon_centers_map2, point, 3, cv::Scalar(160), CV_FILLED);
        }
        cv::imshow("polygon_centers_map2", polygon_centers_map2);
        cv::waitKey();
    }

    LOG(INFO) << "Starting to get the paths for each cell, number of cells: " << (int) cell_polygons.size();
    LOG(INFO) << "Boustrophedon grid_spacing_as_int = " << grid_spacing_as_int;
    cv::Point robot_pos = rotated_starting_point;

    std::vector<cv::Point2f> fov_middlepoint_path;
    std::vector<std::vector<cv::Point2f>> complex_middle_path;
    for (size_t cell = 0; cell < cell_polygons.size(); ++cell) {
        computeBoustrophedonPath(rotated_room_map, map_resolution, cell_polygons[optimal_order[cell]],
                                 fov_middlepoint_path, complex_middle_path,
                                 robot_pos, grid_spacing_as_int, half_grid_spacing_as_int, path_eps,
                                 max_deviation_from_track, grid_obstacle_offset / map_resolution);
//        computeRectangularAmbulatoryPlanePath(rotated_room_map, map_resolution, cell_polygons[optimal_order[cell]],
//                                              fov_middlepoint_path, complex_middle_path,
//                                              robot_pos, grid_spacing_as_int, half_grid_spacing_as_int, path_eps,
//                                              max_deviation_from_track, grid_obstacle_offset / map_resolution);
    }

    if (fov_middlepoint_path.empty()) {
        LOG(ERROR) << "Warning: there are no accessible points in this room.";
        return;
    }

    RoomRotator room_rotation;
    std::vector<geometry_msgs::Pose2D> fov_poses;
    room_rotation.transformPathBackToOriginalRotation(fov_middlepoint_path, fov_poses, R);
    std::vector<std::vector<geometry_msgs::Pose2D>> complex_path;
    for (const auto &complex_middle: complex_middle_path) {
        std::vector<geometry_msgs::Pose2D> complex;
        room_rotation.transformPathBackToOriginalRotation(complex_middle, complex, R);
        complex_path.push_back(complex);
    }

    if (DISPLAY_TRAJECTORY_RESULT) {
        cv::Mat room_map_path = room_map.clone();
        cv::circle(room_map_path, starting_position, 3, cv::Scalar(160), CV_FILLED);
        for (size_t i = 0; i < fov_poses.size() - 1; ++i) {
            cv::circle(room_map_path, cv::Point(cvRound(fov_poses[i].x), cvRound(fov_poses[i].y)), 1, cv::Scalar(200),
                       CV_FILLED);
            cv::line(room_map_path, cv::Point(cvRound(fov_poses[i].x), cvRound(fov_poses[i].y)),
                     cv::Point(cvRound(fov_poses[i + 1].x), cvRound(fov_poses[i + 1].y)), cv::Scalar(100), 1);
        }
        cv::circle(room_map_path, cv::Point(cvRound(fov_poses.back().x), cvRound(fov_poses.back().y)), 1,
                   cv::Scalar(200), CV_FILLED);
        cv::imshow("room_map_path_intermediate", room_map_path);
        cv::waitKey();
    }

    if (DISPLAY_TRAJECTORY_RESULT) {
        cv::Mat room_map_path = room_map.clone();
        cv::circle(room_map_path, starting_position, 3, cv::Scalar(160), CV_FILLED);
        for (const auto &complex: complex_path) {
            for (size_t i = 0; i < complex.size() - 1; ++i) {
                cv::circle(room_map_path, cv::Point(cvRound(complex[i].x), cvRound(complex[i].y)), 1, cv::Scalar(200),
                           CV_FILLED);
                cv::line(room_map_path, cv::Point(cvRound(complex[i].x), cvRound(complex[i].y)),
                         cv::Point(cvRound(complex[i + 1].x), cvRound(complex[i + 1].y)), cv::Scalar(100), 1);
            }
            cv::circle(room_map_path, cv::Point(cvRound(complex.back().x), cvRound(complex.back().y)), 1,
                       cv::Scalar(200), CV_FILLED);
            cv::imshow("room_map_path_intermediate", room_map_path);
            cv::waitKey();
        }
    }

    for (std::vector<geometry_msgs::Pose2D>::iterator pose = fov_poses.begin(); pose != fov_poses.end(); ++pose) {
        geometry_msgs::Pose2D current_pose;
        current_pose.x = (((room_map.cols - pose->x) * map_resolution) + map_origin.x);
        current_pose.y = (((room_map.rows - pose->y) * map_resolution) + map_origin.y);
        current_pose.theta = pose->theta;
        pose_path.push_back(current_pose);
    }

    for (auto &complex: complex_path) {
        std::vector<geometry_msgs::Pose2D> complex_pose;
        for (std::vector<geometry_msgs::Pose2D>::iterator pose = complex.begin(); pose != complex.end(); ++pose) {
            geometry_msgs::Pose2D current_pose;
            current_pose.x = (((room_map.cols - pose->x) * map_resolution) + map_origin.x);
            current_pose.y = (((room_map.rows - pose->y) * map_resolution) + map_origin.y);
            current_pose.theta = pose->theta;
            complex_pose.push_back(current_pose);
        }
        complex_pose_path.push_back(complex_pose);
    }

}

/**
 * 1. 对地图进行长短边计算，计算得出最佳的旋转矩阵，并将地图进行旋转
 * 2. 调用computeCellDecomposition()函数进行区间分割
 */
void BoustrophedonExplorer::computeCellDecompositionWithRotation(const cv::Mat &room_map, const float map_resolution,
                                                                 const double min_cell_area,
                                                                 const int min_cell_width, const double rotation_offset,
                                                                 cv::Mat &R, cv::Rect &bbox, cv::Mat &rotated_room_map,
                                                                 std::vector<GeneralizedPolygon> &cell_polygons,
                                                                 std::vector<cv::Point> &polygon_centers) {
    RoomRotator room_rotation;
    room_rotation.computeRoomRotationMatrix(room_map, R, bbox, map_resolution, 0, rotation_offset);
    room_rotation.rotateRoom(room_map, rotated_room_map, R, bbox);

    if (DISPLAY_TRAJECTORY) {
        cv::imshow("room_map", room_map);
        cv::waitKey();
        cv::imshow("rotated_room_map", rotated_room_map);
        cv::waitKey();
    }

    computeCellDecomposition(rotated_room_map, map_resolution, min_cell_area, min_cell_width,
                             cell_polygons, polygon_centers);
}

/**
 * 1. 使用牛耕法将地图分割为若干个子区间
 * 2. 对第一步分割好的区间做一个合并操作，将小面积区间合并到相邻面积最大的区间
 * @param room_map 经过预处理以及最佳角度旋转后的栅格地图，该地图是一张二值图，0代表障碍物，255代表可通行区域
 * @param map_resolution 栅格地图的分辨率
 * @param min_cell_area 用来过滤小面积区域
 * @param min_cell_width 用来过滤窄区域
 * @param cell_polygons 保存返回的分区结果，生成的区间以多边形点的形式保存
 * @param polygon_centers 保存每一个分区的中心点坐标，用来计算区间之间的距离
 */
void BoustrophedonExplorer::computeCellDecomposition(const cv::Mat &room_map, const float map_resolution,
                                                     const double min_cell_area,
                                                     const int min_cell_width,
                                                     std::vector<GeneralizedPolygon> &cell_polygons,
                                                     std::vector<cv::Point> &polygon_centers) {
    cv::Mat cell_map = room_map.clone();

    //找到出现白色像素的最小y值，以设置 y_start 、previous_number_of_segments
    //从上到下，从左到右，遍历地图数据，当找到房间的第一行时，记录下当时的 Y 坐标 y_start，以及该行的 previous_number_of_segments 和障碍点的 X 坐标列表 previous_obstacles_end_x
    size_t y_start = 0;
    bool found = false, obstacle = false;
    int previous_number_of_segments = 0;
    std::vector<int> previous_obstacles_end_x;//跟踪障碍物的终点
    for (size_t y = 0; y < room_map.rows; ++y) {
        for (size_t x = 0; x < room_map.cols; ++x) {
            if (!found && room_map.at<uchar>(y, x) == 255) {
                y_start = y;
                found = true;
            } else if (found && !obstacle && room_map.at<uchar>(y, x) == 0) {
                ++previous_number_of_segments;
                obstacle = true;
            } else if (found && obstacle && room_map.at<uchar>(y, x) == 255) {
                obstacle = false;
                previous_obstacles_end_x.push_back(x);
            }
        }

        if (found)
            break;
    }

    //扫描地图并检测关键点
    //从 y_start+1 开始继续遍历，查找到第一个可通行区域的点(255)后，开始查找x轴上的障碍物的起始结束点位，并记录到current_obstacles_start_x、current_obstacles_end_x
    for (size_t y = y_start + 1; y < room_map.rows; ++y) {//从 y_start + 1 开始，因为我们知道 y_start 处的 number_of_segments
        bool isChange = false;
        int number_of_segments = 0;//当前 x 轴方向有多少段障碍物
        std::vector<int> current_obstacles_start_x;
        std::vector<int> current_obstacles_end_x;
        bool obstacle_hit = false;//检查是否碰到障碍物
        bool hit_white_pixel = false;//检查当前命中了白色像素，以在第一个白色像素处开始

        for (size_t x = 0; x < room_map.cols; ++x) {
            if (!hit_white_pixel && room_map.at<uchar>(y, x) == 255) {
                hit_white_pixel = true;
            } else if (hit_white_pixel) {
                if (!obstacle_hit && room_map.at<uchar>(y, x) == 0) {//检查障碍物
                    ++number_of_segments;
                    obstacle_hit = true;
                    current_obstacles_start_x.push_back(x);
                } else if (obstacle_hit && room_map.at<uchar>(y, x) == 255) {//检查是否离开障碍物
                    obstacle_hit = false;
                    current_obstacles_end_x.push_back(x);
                }
            }
        }

        //如果 number_of_segments 数量没有改变，检查 segments 的位置是否改变，以使它们之间有间隙
        bool segment_shift_detected = false;
        if (previous_number_of_segments == number_of_segments &&
            current_obstacles_start_x.size() == previous_obstacles_end_x.size() + 1) {
            for (size_t i = 0; i < previous_obstacles_end_x.size(); ++i)
                if (current_obstacles_start_x[i] > previous_obstacles_end_x[i]) {
                    segment_shift_detected = true;
                    break;
                }
        }

        hit_white_pixel = false;

        //牛耕法会逐行（或逐列）计算该行（或该列）被障碍物分为几段， 根据前后连续两行（或两列）的段数，会触发不同的处理逻辑：
        //1. 当前段数比上一个的段数多，则触发IN事件处理。
        //2. 当前段数比上一个的段数少，则触发OUT事件处理。
        //3. 当前段跟上一个的段数一样，但不连续，则同样当IN事件处理。
        //检查 previous_number_of_segments 是否已更改-->事件发生
        if (previous_number_of_segments < number_of_segments || segment_shift_detected) {//IN event (or shift)
            //再次检查当前切片的关键点
            for (int x = 0; x < room_map.cols; ++x) {
                if (!hit_white_pixel && room_map.at<uchar>(y, x) == 255)
                    hit_white_pixel = true;
                else if (hit_white_pixel && room_map.at<uchar>(y, x) == 0) {
                    //检查障碍物所在单元周围是否有其他障碍物，如果没有出现，则找到一个临界点
                    bool critical_point = true;
                    for (int dx = -1; dx <= 1; ++dx)
                        if (room_map.at<uchar>(y - 1, std::max(0, std::min(x + dx, room_map.cols - 1))) == 0)
                            critical_point = false;

                    //如果找到临界点，则标记分离，注意该算法从临界点开始向左和向右移动，直到碰到障碍物，因为这样可以防止其他障碍物后面的不必要单元与临界点的y值相同
                    if (critical_point) {
                        for (int dx = -1; x + dx >= 0; --dx) {
                            uchar &val = cell_map.at<uchar>(y, x + dx);
                            if (val == 255 && cell_map.at<uchar>(y - 1, x + dx) == 255) {
                                val = BORDER_PIXEL_VALUE;
                                isChange = true;
                            } else if (val == 0)
                                break;
                        }

                        for (int dx = 1; x + dx < room_map.cols; ++dx) {
                            uchar &val = cell_map.at<uchar>(y, x + dx);
                            if (val == 255 && cell_map.at<uchar>(y - 1, x + dx) == 255) {
                                val = BORDER_PIXEL_VALUE;
                                isChange = true;
                            } else if (val == 0)
                                break;
                        }
                    }
                }
            }
        } else if (previous_number_of_segments > number_of_segments) {//OUT event
            //再次检查上一个切片的临界点-->y-1
            for (int x = 0; x < room_map.cols; ++x) {
                if (room_map.at<uchar>(y - 1, x) == 255 && !hit_white_pixel)
                    hit_white_pixel = true;
                else if (hit_white_pixel && room_map.at<uchar>(y - 1, x) == 0) {
                    //检查障碍物所在单元周围是否有其他障碍物，如果没有出现，则找到一个临界点
                    bool critical_point = true;
                    for (int dx = -1; dx <= 1; ++dx)
                        if (room_map.at<uchar>(y, std::max(0, std::min(x + dx, room_map.cols - 1))) == 0)
                            critical_point = false;

                    //如果找到临界点，则标记分离，注意该算法从临界点开始向左和向右移动，直到碰到障碍物，因为这样可以防止其他障碍物后面的不必要单元与临界点的y值相同
                    if (critical_point) {
                        const int ym2 = std::max(0, (int) y - 2);

                        for (int dx = -1; x + dx >= 0; --dx) {
                            uchar &val = cell_map.at<uchar>(y - 1, x + dx);
                            if (val == 255 && cell_map.at<uchar>(ym2, x + dx) == 255) {
                                val = BORDER_PIXEL_VALUE;
                                isChange = true;
                            } else if (val == 0)
                                break;
                        }

                        for (int dx = 1; x + dx < room_map.cols; ++dx) {
                            uchar &val = cell_map.at<uchar>(y - 1, x + dx);
                            if (val == 255 && cell_map.at<uchar>(ym2, x + dx) == 255) {
                                val = BORDER_PIXEL_VALUE;
                                isChange = true;
                            } else if (val == 0)
                                break;
                        }
                    }
                }
            }
        }

        if (isChange && DISPLAY_TRAJECTORY) {
            cv::imshow("compute map", cell_map);
            cv::waitKey();
        }

        //保存找到的线段数和障碍物终点
        previous_number_of_segments = number_of_segments;
        previous_obstacles_end_x = current_obstacles_end_x;
    }

    if (DISPLAY_TRAJECTORY) {
        cv::imshow("cell_map", cell_map);
        cv::waitKey();
    }

    cv::Mat cell_map_labels;
    const int number_of_cells = mergeCells(cell_map, cell_map_labels, min_cell_area, min_cell_width);


    std::vector<std::vector<cv::Point> > cells;
    for (int i = 1; i <= number_of_cells; ++i) {
        cv::Mat cell_copy(cell_map_labels == i);
        if (DISPLAY_TRAJECTORY) {
            cv::imshow("cell_copy", cell_copy);
            cv::waitKey();
        }
        std::vector<std::vector<cv::Point> > cellsi;
        // 只检测最外层轮廓   压缩水平方向、垂直方向和对角线方向的像素，只保留该方向的终点坐标
        cv::findContours(cell_copy, cellsi, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
        cells.insert(cells.end(), cellsi.begin(), cellsi.end());
    }

    for (size_t cell = 0; cell < cells.size(); ++cell) {
        GeneralizedPolygon current_cell(cells[cell], map_resolution);
        if (current_cell.getArea() >= min_cell_area) {
            cell_polygons.push_back(current_cell);
            polygon_centers.push_back(current_cell.getCenter());
        } else {
//            LOG(WARNING) << "WARN: BoustrophedonExplorer::computeCellDecomposition: dropped cell " << cell + 1
//                         << " with area=" << current_cell.getArea()
//                         << ". This should only happen for small unconnected cells.";
        }
    }
}

void BoustrophedonExplorer::computeBoustrophedonPath(const cv::Mat &room_map, const float map_resolution,
                                                     const GeneralizedPolygon &cell,
                                                     std::vector<cv::Point2f> &fov_middlepoint_path,
                                                     std::vector<std::vector<cv::Point2f>> &complex_middle_path,
                                                     cv::Point &robot_pos,//当前点，在每一次执行完都会修改该点数据
                                                     const int grid_spacing_as_int, const int half_grid_spacing_as_int,
                                                     const double path_eps, const int max_deviation_from_track,
                                                     const int grid_obstacle_offset) {

    //cv::Mat &room_map 地图原始数据

    cv::Mat cell_map;//分区后的片段图，位置为 y 轴方向为图像大小，x 轴方向为在原图中大小
    cell.drawPolygon(cell_map, cv::Scalar(255));

    cv::Point cell_center = cell.getBoundingBoxCenter();

    cv::Mat R_cell;//
    cv::Rect cell_bbox;
    cv::Mat rotated_cell_map;//仿射变换后的分区片段图，位置为 y 轴方向为图像大小，x 轴中心点为图像的中心位置
    RoomRotator cell_rotation;
    cell_rotation.computeRoomRotationMatrix(cell_map, R_cell, cell_bbox, map_resolution, &cell_center);
    cell_rotation.rotateRoom(cell_map, rotated_cell_map, R_cell, cell_bbox);

    cv::Mat inflated_room_map;//原始地图腐蚀之后的地图
    cv::Mat rotated_inflated_room_map;//仿射变换后的原始腐蚀图
    explorationErode(room_map, inflated_room_map, half_grid_spacing_as_int + grid_obstacle_offset);

    cell_rotation.rotateRoom(inflated_room_map, rotated_inflated_room_map, R_cell, cell_bbox);

    cv::Mat rotated_inflated_cell_map = rotated_cell_map.clone();//仿射变换后的分区片段图，位置为 y 轴方向为图像大小，x 轴中心点为图像的中心位置，图像为外围腐蚀的区域为128
    for (int v = 0; v < rotated_inflated_cell_map.rows; ++v)
        for (int u = 0; u < rotated_inflated_cell_map.cols; ++u)
            if (rotated_inflated_cell_map.at<uchar>(v, u) != 0 && rotated_inflated_room_map.at<uchar>(v, u) == 0)
                rotated_inflated_cell_map.at<uchar>(v, u) = 128;

    if (DISPLAY_TRAJECTORY) {
        cv::imshow("rotated_cell_map_with_inflation", rotated_inflated_cell_map);
        cv::waitKey();
    }

    BoustrophedonGrid grid_lines;
    GridGenerator::generateBoustrophedonGrid(rotated_cell_map, rotated_inflated_cell_map, -1, grid_lines,
                                             cv::Vec4i(-1, -1, -1, -1), //cv::Vec4i(min_x, max_x, min_y, max_y),
                                             grid_spacing_as_int, half_grid_spacing_as_int, 1,
                                             max_deviation_from_track);

    if (DISPLAY_TRAJECTORY) {
        cv::Mat rotated_cell_map_disp = rotated_cell_map.clone();
        for (size_t i = 0; i < grid_lines.size(); ++i) {
            for (size_t j = 0; j + 1 < grid_lines[i].upper_line.size(); ++j) {
                cv::circle(rotated_cell_map_disp, grid_lines[i].upper_line[j], 1, cv::Scalar(64), CV_FILLED);
                cv::line(rotated_cell_map_disp, grid_lines[i].upper_line[j], grid_lines[i].upper_line[j + 1],
                         cv::Scalar(128), 1);
            }
            for (size_t j = 0; j + 1 < grid_lines[i].lower_line.size(); ++j) {
                cv::circle(rotated_cell_map_disp, grid_lines[i].lower_line[j], 1, cv::Scalar(64), CV_FILLED);
                cv::line(rotated_cell_map_disp, grid_lines[i].lower_line[j], grid_lines[i].lower_line[j + 1],
                         cv::Scalar(196), 1);
            }
        }
        cv::imshow("rotated_cell_map", rotated_cell_map_disp);
        cv::waitKey();
    }

    if (grid_lines.size() == 0)
        return;

    std::vector<cv::Point> outer_corners(4);
    outer_corners[0] = grid_lines[0].upper_line[0];        // upper left corner
    outer_corners[1] = grid_lines[0].upper_line.back();    // upper right corner
    outer_corners[2] = grid_lines.back().upper_line[0];    // lower left corner
    outer_corners[3] = grid_lines.back().upper_line.back();    // lower right corner
    cv::Mat R_cell_inv;
    cv::invertAffineTransform(R_cell, R_cell_inv);//反转旋转矩阵，将确定的点重新映射到原始单元格
    cv::transform(outer_corners, outer_corners, R_cell_inv);
    double min_corner_dist = path_planner_.planPath(room_map, robot_pos, outer_corners[0], 1.0, 0.0, map_resolution);
    int min_corner_index = 0;
    for (int i = 1; i < 4; ++i) {
        double dist = path_planner_.planPath(room_map, robot_pos, outer_corners[i], 1.0, 0.0, map_resolution);
        if (dist < min_corner_dist) {
            min_corner_dist = dist;
            min_corner_index = i;
        }
    }

    bool start_from_upper_path = min_corner_index < 2;//true --> upper
    bool start_from_left = min_corner_index % 2 == 0;//用于确定路径应该从哪一侧开始，并检查路径的结束位置

    if (DISPLAY_TRAJECTORY) {
        cv::Mat room_map_disp = room_map.clone();
        for (size_t i = 0; i < outer_corners.size(); i += 2)
            cv::line(room_map_disp, outer_corners[i], outer_corners[i + 1], cv::Scalar(128), 1);
        cv::circle(room_map_disp, robot_pos, 3, cv::Scalar(160), CV_FILLED);
        if (start_from_upper_path) {
            if (start_from_left)
                cv::circle(room_map_disp, outer_corners[0], 3, cv::Scalar(64), CV_FILLED);
            else
                cv::circle(room_map_disp, outer_corners[1], 3, cv::Scalar(64), CV_FILLED);
        } else {
            if (start_from_left)
                cv::circle(room_map_disp, outer_corners[2], 3, cv::Scalar(64), CV_FILLED);
            else
                cv::circle(room_map_disp, outer_corners[3], 3, cv::Scalar(64), CV_FILLED);
        }
        cv::imshow("rotated_room_map", room_map_disp);
        cv::waitKey();
    }

    //第一条直线从左往右取点，下一条直线从右往左取点，再下一条从左往右取点，这样依次循环执行，生成弓字形的覆盖路径点

    cv::Point cell_robot_pos;
    bool start = true;
    std::vector<cv::Point> current_fov_path;
    bool first = true;
    if (start_from_upper_path) {

        for (BoustrophedonGrid::iterator line = grid_lines.begin(); line != grid_lines.end(); ++line) {
            if (start) {
                if (start_from_left)
                    cell_robot_pos = line->upper_line[0];
                else
                    cell_robot_pos = line->upper_line.back();
                start = false;
            }
            if (first) {
                first = false;
                current_fov_path.push_back(cell_robot_pos);
            }

            if (start_from_left) {
                //使用 Astar-path 获取水平线之间的过渡点
                std::vector<cv::Point> astar_path;
                path_planner_.planPath(rotated_inflated_cell_map, cell_robot_pos, line->upper_line[0], 1.0, 0.0,
                                       map_resolution, 0, &astar_path);
                downsamplePath(astar_path, current_fov_path, cell_robot_pos, path_eps);

                downsamplePath(line->upper_line, current_fov_path, cell_robot_pos, path_eps);

                if (line->has_two_valid_lines)
                    downsamplePathReverse(line->lower_line, current_fov_path, cell_robot_pos, path_eps);
                else
                    start_from_left = false;
            } else {
                std::vector<cv::Point> astar_path;
                path_planner_.planPath(rotated_inflated_cell_map, cell_robot_pos, line->upper_line.back(), 1.0, 0.0,
                                       map_resolution, 0, &astar_path);
                downsamplePath(astar_path, current_fov_path, cell_robot_pos, path_eps);

                downsamplePathReverse(line->upper_line, current_fov_path, cell_robot_pos, path_eps);

                if (line->has_two_valid_lines)
                    downsamplePath(line->lower_line, current_fov_path, cell_robot_pos, path_eps);
                else
                    start_from_left = true;
            }
        }
    } else {
        for (BoustrophedonGrid::reverse_iterator line = grid_lines.rbegin(); line != grid_lines.rend(); ++line) {
            if (start) {
                if (start_from_left)
                    cell_robot_pos = line->upper_line[0];
                else
                    cell_robot_pos = line->upper_line.back();
                start = false;
            }
            if (first) {
                first = false;
                current_fov_path.push_back(cell_robot_pos);
            }

            if (start_from_left) {
                std::vector<cv::Point> astar_path;
                path_planner_.planPath(rotated_inflated_cell_map, cell_robot_pos, line->upper_line[0], 1.0, 0.0,
                                       map_resolution, 0, &astar_path);
                downsamplePath(astar_path, current_fov_path, cell_robot_pos, path_eps);

                downsamplePath(line->upper_line, current_fov_path, cell_robot_pos, path_eps);

                if (line->has_two_valid_lines)
                    downsamplePathReverse(line->lower_line, current_fov_path, cell_robot_pos, path_eps);
                else
                    start_from_left = false;
            } else {
                std::vector<cv::Point> astar_path;
                path_planner_.planPath(rotated_inflated_cell_map, cell_robot_pos, line->upper_line.back(), 1.0, 0.0,
                                       map_resolution, 0, &astar_path);
                downsamplePath(astar_path, current_fov_path, cell_robot_pos, path_eps);

                downsamplePathReverse(line->upper_line, current_fov_path, cell_robot_pos, path_eps);

                if (line->has_two_valid_lines)
                    downsamplePath(line->lower_line, current_fov_path, cell_robot_pos, path_eps);
                else
                    start_from_left = true;
            }
        }
    }

    if (DISPLAY_TRAJECTORY) {
        cv::Mat rotated_cell_fov_path_disp = rotated_cell_map.clone();
        for (size_t i = 1; i < current_fov_path.size(); ++i) {
            cv::circle(rotated_cell_fov_path_disp, current_fov_path[i], 1, cv::Scalar(196), 1);
            cv::line(rotated_cell_fov_path_disp, current_fov_path[i - 1], current_fov_path[i], cv::Scalar(128), 1);
//            cv::imshow("rotated_cell_fov_path", rotated_cell_fov_path_disp);
//            cv::waitKey();
        }
        cv::imshow("rotated_cell_fov_path", rotated_cell_fov_path_disp);
        cv::waitKey();
    }

    std::vector<cv::Point2f> fov_middlepoint_path_part;
    for (std::vector<cv::Point>::iterator point = current_fov_path.begin(); point != current_fov_path.end(); ++point)
        fov_middlepoint_path_part.push_back(cv::Point2f(point->x, point->y));
    cv::transform(fov_middlepoint_path_part, fov_middlepoint_path_part, R_cell_inv);

    fov_middlepoint_path.insert(fov_middlepoint_path.end(), fov_middlepoint_path_part.begin(),
                                fov_middlepoint_path_part.end());

    complex_middle_path.push_back(fov_middlepoint_path_part);

    if (DISPLAY_TRAJECTORY) {
        cv::Mat cell_fov_path_disp = cell_map.clone();
        for (size_t i = 1; i < fov_middlepoint_path.size(); ++i) {
            cv::circle(cell_fov_path_disp, fov_middlepoint_path[i], 1, cv::Scalar(196), 1);
            cv::line(cell_fov_path_disp, fov_middlepoint_path[i - 1], fov_middlepoint_path[i], cv::Scalar(128), 1);
//            cv::imshow("cell_fov_path", cell_fov_path_disp);
//            cv::waitKey();
        }
        cv::imshow("cell_fov_path", cell_fov_path_disp);
        cv::waitKey();
    }

    std::vector<cv::Point> current_pos_vector(1, cell_robot_pos);
    cv::transform(current_pos_vector, current_pos_vector, R_cell_inv);
    robot_pos = current_pos_vector[0];
}

void BoustrophedonExplorer::computeRectangularAmbulatoryPlanePath(const cv::Mat &room_map, const float map_resolution,
                                                                  const GeneralizedPolygon &cell,
                                                                  std::vector<cv::Point2f> &fov_middlepoint_path,
                                                                  std::vector<std::vector<cv::Point2f>> &complex_middle_path,
                                                                  cv::Point &robot_pos, const int grid_spacing_as_int,
                                                                  const int half_grid_spacing_as_int,
                                                                  const double path_eps,
                                                                  const int max_deviation_from_track,
                                                                  const int grid_obstacle_offset) {
    cv::Mat cell_map;//分区后的片段图，位置为 y 轴方向为图像大小，x 轴方向为在原图中大小
    cell.drawPolygon(cell_map, cv::Scalar(255));

    cv::Point cell_center = cell.getBoundingBoxCenter();

    cv::Mat R_cell;//
    cv::Rect cell_bbox;
    cv::Mat rotated_cell_map;//仿射变换后的分区片段图，位置为 y 轴方向为图像大小，x 轴中心点为图像的中心位置
    RoomRotator cell_rotation;
    cell_rotation.computeRoomRotationMatrix(cell_map, R_cell, cell_bbox, map_resolution, &cell_center);
    cell_rotation.rotateRoom(cell_map, rotated_cell_map, R_cell, cell_bbox);

    cv::Mat inflated_room_map;//原始地图腐蚀之后的地图
    cv::Mat rotated_inflated_room_map;//仿射变换后的原始腐蚀图
    explorationErode(room_map, inflated_room_map, half_grid_spacing_as_int + grid_obstacle_offset);

    cell_rotation.rotateRoom(inflated_room_map, rotated_inflated_room_map, R_cell, cell_bbox);

    cv::Mat rotated_inflated_cell_map = rotated_cell_map.clone();//仿射变换后的分区片段图，位置为 y 轴方向为图像大小，x 轴中心点为图像的中心位置，图像为外围腐蚀的区域为128
    for (int v = 0; v < rotated_inflated_cell_map.rows; ++v)
        for (int u = 0; u < rotated_inflated_cell_map.cols; ++u)
            if (rotated_inflated_cell_map.at<uchar>(v, u) != 0 && rotated_inflated_room_map.at<uchar>(v, u) == 0)
                rotated_inflated_cell_map.at<uchar>(v, u) = 128;

    if (DISPLAY_TRAJECTORY) {
        cv::imshow("rotated_cell_map_with_inflation", rotated_inflated_cell_map);
        cv::waitKey();
    }

    cv::Mat R_cell_inv;
    cv::invertAffineTransform(R_cell, R_cell_inv);//反转旋转矩阵，将确定的点重新映射到原始单元格

    // use voronoi
    nav_msgs::OccupancyGrid room_gridmap;
    room_gridmap.info.width = rotated_inflated_cell_map.cols;
    room_gridmap.info.height = rotated_inflated_cell_map.rows;
    room_gridmap.data.resize(rotated_inflated_cell_map.cols * rotated_inflated_cell_map.rows);
    for (int x = 0; x < rotated_inflated_cell_map.cols; x++)
        for (int y = 0; y < rotated_inflated_cell_map.rows; y++)
            room_gridmap.data[y * rotated_inflated_cell_map.cols + x] = rotated_inflated_cell_map.at<int8_t>(y, x) ?
                                                                        0 : 100;
    VoronoiMap vm(room_gridmap.data.data(), room_gridmap.info.width, room_gridmap.info.height, grid_spacing_as_int, 2);
    std::vector<cv::Point> current_fov_path;
    auto mat = rotated_inflated_cell_map.clone();
    vm.generatePath(mat, current_fov_path, cv::Mat(), 1, 1);

    cv::Point cell_robot_pos = current_fov_path[current_fov_path.size() - 1];


    // 通过腐蚀+边界查找实现回字形规划路径
//    cv::Point cell_robot_pos;
//    std::vector<cv::Point> current_fov_path;
//
//    auto occupancyGrid = rotated_inflated_cell_map.clone();
//    cv::Mat half_element = cv::getStructuringElement(cv::MORPH_RECT,
//                                                     cv::Size(half_grid_spacing_as_int, half_grid_spacing_as_int),
//                                                     cv::Point(-1, -1));
//    cv::erode(occupancyGrid, occupancyGrid, half_element);
//    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,
//                                                cv::Size(grid_spacing_as_int, grid_spacing_as_int),
//                                                cv::Point(-1, -1));
//    while (true) {
//        std::vector<std::vector<cv::Point>> contours;
//        cv::findContours(occupancyGrid, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
//        if (contours.empty()) {
//            break;  // No more contours found, exit the loop
//        }
//        for (const auto &contour: contours) {
////            std::vector<cv::Point> list;
////            cv::approxPolyDP(contour, list, 0.01, false);
//            for (const auto &point: contour) {
//                current_fov_path.push_back(point);
//                cell_robot_pos = point;
//            }
//        }
//        cv::erode(occupancyGrid, occupancyGrid, element);
//    }

    std::vector<cv::Point2f> fov_middlepoint_path_part;
    for (std::vector<cv::Point>::iterator point = current_fov_path.begin(); point != current_fov_path.end(); ++point)
        fov_middlepoint_path_part.push_back(cv::Point2f(point->x, point->y));
    cv::transform(fov_middlepoint_path_part, fov_middlepoint_path_part, R_cell_inv);

    fov_middlepoint_path.insert(fov_middlepoint_path.end(), fov_middlepoint_path_part.begin(),
                                fov_middlepoint_path_part.end());

    complex_middle_path.push_back(fov_middlepoint_path_part);

    if (DISPLAY_TRAJECTORY) {
        cv::Mat cell_fov_path_disp = cell_map.clone();
        for (size_t i = 1; i < fov_middlepoint_path.size(); ++i) {
            cv::circle(cell_fov_path_disp, fov_middlepoint_path[i], 1, cv::Scalar(196), 1);
            cv::line(cell_fov_path_disp, fov_middlepoint_path[i - 1], fov_middlepoint_path[i], cv::Scalar(128), 1);
            cv::imshow("cell_fov_path", cell_fov_path_disp);
            cv::waitKey();
        }
        cv::imshow("cell_fov_path", cell_fov_path_disp);
        cv::waitKey();
    }

    std::vector<cv::Point> current_pos_vector(1, cell_robot_pos);
    cv::transform(current_pos_vector, current_pos_vector, R_cell_inv);
    robot_pos = current_pos_vector[0];
}

int BoustrophedonExplorer::mergeCells(cv::Mat &cell_map, cv::Mat &cell_map_labels, const double min_cell_area,
                                      const int min_cell_width) {
    cell_map.convertTo(cell_map_labels, CV_32SC1, 256, 0);
    for (int v = 0; v < cell_map_labels.rows; ++v)
        for (int u = 0; u < cell_map_labels.cols; ++u)
            if (cell_map_labels.at<int>(v, u) == BORDER_PIXEL_VALUE * 256)
                cell_map_labels.at<int>(v, u) = -1;

    std::map<int, boost::shared_ptr<BoustrophedonCell> > cell_index_mapping;
    int label_index = 1;
    for (int v = 0; v < cell_map_labels.rows; ++v) {
        for (int u = 0; u < cell_map_labels.cols; ++u) {
            if (cell_map_labels.at<int>(v, u) != 65280)
                continue;

            //见 ExplorationCenter.cpp 漫水填充法
            cv::Rect bounding_box;
            const double area = cv::floodFill(cell_map_labels, cv::Point(u, v),
                                              label_index, &bounding_box, 0, 0, 4);
            cell_index_mapping[label_index] = boost::shared_ptr<BoustrophedonCell>(
                    new BoustrophedonCell(label_index, area, bounding_box));
            label_index++;
            if (label_index == INT_MAX)
                LOG(WARNING) << "WARN: BoustrophedonExplorer::mergeCells: label_index exceeds range of int.";
        }
    }

    LOG(INFO) << "BoustrophedonExplorer::mergeCells: found " << label_index - 1 << " cells before merging.";

    //配对响应的邻居
    for (int v = 1; v < cell_map_labels.rows - 1; ++v) {
        for (int u = 1; u < cell_map_labels.cols - 1; ++u) {
            if (cell_map_labels.at<int>(v, u) == -1) {
                const int label_left = cell_map_labels.at<int>(v, u - 1);
                const int label_right = cell_map_labels.at<int>(v, u + 1);
                if (label_left > 0 && label_right > 0) {
                    cell_index_mapping[label_left]->neighbors_.insert(cell_index_mapping[label_right]);
                    cell_index_mapping[label_right]->neighbors_.insert(cell_index_mapping[label_left]);
                }
                const int label_up = cell_map_labels.at<int>(v - 1, u);
                const int label_down = cell_map_labels.at<int>(v + 1, u);
                if (label_up > 0 && label_down > 0) {
                    cell_index_mapping[label_up]->neighbors_.insert(cell_index_mapping[label_down]);
                    cell_index_mapping[label_down]->neighbors_.insert(cell_index_mapping[label_up]);
                }
            }
        }
    }

    if (DISPLAY_TRAJECTORY) {
        for (const auto &item: cell_index_mapping) {
            int first = item.first;

            boost::shared_ptr<BoustrophedonCell> second = item.second;
            int label = second->label_;
            double area = second->area_;
            cv::Rect &boundingBox = second->bounding_box_;
            BoustrophedonCell::BoustrophedonCellSet &set = second->neighbors_;

            std::string msg;
            msg.append("房间 ");
            msg.append(std::to_string(first));
            msg.append(" label=");
            msg.append(std::to_string(label));
            msg.append(" area=");
            msg.append(std::to_string((int) area));
            msg.append(" width=");
            msg.append(std::to_string((int) boundingBox.width));

            msg.append(" 邻居 ");
            for (const auto &item: set) {
                msg.append(std::to_string(item->label_)).append(" ");
            }
            LOG(INFO) << msg;
        }
        cv::imshow("merge before", cell_map);
        cv::waitKey();
    }

    mergeCellsSelection(cell_map, cell_map_labels, cell_index_mapping, min_cell_area, min_cell_width);

    if (DISPLAY_TRAJECTORY) {
        cv::imshow("merge after", cell_map);
        cv::waitKey();
    }

    int new_cell_label = 1;
    for (std::map<int, boost::shared_ptr<BoustrophedonCell> >::iterator itc = cell_index_mapping.begin();
         itc != cell_index_mapping.end(); ++itc, ++new_cell_label)
        for (int v = 0; v < cell_map_labels.rows; ++v)
            for (int u = 0; u < cell_map_labels.cols; ++u)
                if (cell_map_labels.at<int>(v, u) == itc->second->label_)
                    cell_map_labels.at<int>(v, u) = new_cell_label;

    LOG(INFO) << "INFO: BoustrophedonExplorer::mergeCells: " << cell_index_mapping.size()
              << " cells remaining after merging.";
    return cell_index_mapping.size();
}


void BoustrophedonExplorer::mergeCellsSelection(cv::Mat &cell_map, cv::Mat &cell_map_labels,
                                                std::map<int, boost::shared_ptr<BoustrophedonCell> > &cell_index_mapping,
                                                const double min_cell_area, const int min_cell_width) {
    std::multimap<double, boost::shared_ptr<BoustrophedonCell> > area_to_region_id_mapping;
    for (std::map<int, boost::shared_ptr<BoustrophedonCell> >::iterator itc = cell_index_mapping.begin();
         itc != cell_index_mapping.end(); ++itc)
        area_to_region_id_mapping.insert(
                std::pair<double, boost::shared_ptr<BoustrophedonCell> >(itc->second->area_, itc->second)
        );

    //将 min_cell_area 以下的小单元格与其最大的相邻单元格合并
    for (std::multimap<double, boost::shared_ptr<BoustrophedonCell> >::iterator it = area_to_region_id_mapping.begin();
         it != area_to_region_id_mapping.end();) {

        if (it->first >= min_cell_area && it->second->bounding_box_.width >= min_cell_width &&
            it->second->bounding_box_.height >= min_cell_width) {
            ++it;
            continue;
        }

        if (it->second->neighbors_.size() == 0) {
//            LOG(WARNING) << "BoustrophedonExplorer::mergeCells: skipping small cell without neighbors.";
            ++it;
            continue;
        }

        //确定最大的相邻单元
        const BoustrophedonCell &small_cell = *(it->second);
        std::multimap<double, boost::shared_ptr<BoustrophedonCell>, std::greater<double>> area_sorted_neighbors;
        for (BoustrophedonCell::BoustrophedonCellSetIterator itn = small_cell.neighbors_.begin();
             itn != small_cell.neighbors_.end(); ++itn)
            area_sorted_neighbors.insert(std::pair<double, boost::shared_ptr<BoustrophedonCell> >((*itn)->area_, *itn));

        BoustrophedonCell &large_cell = *(area_sorted_neighbors.begin()->second);

        //合并单元格
        mergeTwoCells(cell_map, cell_map_labels, small_cell, large_cell, cell_index_mapping);

        //更新 area_to_region_id_mapping
        area_to_region_id_mapping.clear();
        for (std::map<int, boost::shared_ptr<BoustrophedonCell> >::iterator itc = cell_index_mapping.begin();
             itc != cell_index_mapping.end(); ++itc)
            area_to_region_id_mapping.insert(
                    std::pair<double, boost::shared_ptr<BoustrophedonCell> >(itc->second->area_, itc->second));
        it = area_to_region_id_mapping.begin();
    }

    //用最大相邻区域标签的标签标记剩余边界像素
    for (int v = 1; v < cell_map.rows - 1; ++v) {
        for (int u = 1; u < cell_map.cols - 1; ++u) {
            if (cell_map.at<uchar>(v, u) == BORDER_PIXEL_VALUE) {
                std::set<int> neighbor_labels;
                for (int dv = -1; dv <= 1; ++dv) {
                    for (int du = -1; du <= 1; ++du) {
                        const int &val = cell_map_labels.at<int>(v + dv, u + du);
                        if (val > 0)
                            neighbor_labels.insert(val);
                    }
                }
                if (neighbor_labels.size() > 0) {
                    for (std::multimap<double, boost::shared_ptr<BoustrophedonCell> >::reverse_iterator it = area_to_region_id_mapping.rbegin();
                         it != area_to_region_id_mapping.rend(); ++it) {
                        if (neighbor_labels.find(it->second->label_) != neighbor_labels.end()) {
                            cell_map_labels.at<int>(v, u) = it->second->label_;
                            break;
                        }
                    }
                } else
                    LOG(WARNING) << "BoustrophedonExplorer::mergeCells: border pixel has no labeled neighbors.";
            }
        }
    }
}

void BoustrophedonExplorer::mergeTwoCells(cv::Mat &cell_map, cv::Mat &cell_map_labels,
                                          const BoustrophedonCell &minor_cell, BoustrophedonCell &major_cell,
                                          std::map<int, boost::shared_ptr<BoustrophedonCell> > &cell_index_mapping) {
    //从 cell_map 中删除两者间的边框
    for (int v = 0; v < cell_map.rows; ++v)
        for (int u = 0; u < cell_map.cols; ++u)
            if (cell_map.at<uchar>(v, u) == BORDER_PIXEL_VALUE &&
                ((cell_map_labels.at<int>(v, u - 1) == minor_cell.label_ &&
                  cell_map_labels.at<int>(v, u + 1) == major_cell.label_) ||
                 (cell_map_labels.at<int>(v, u - 1) == major_cell.label_ &&
                  cell_map_labels.at<int>(v, u + 1) == minor_cell.label_) ||
                 (cell_map_labels.at<int>(v - 1, u) == minor_cell.label_ &&
                  cell_map_labels.at<int>(v + 1, u) == major_cell.label_) ||
                 (cell_map_labels.at<int>(v - 1, u) == major_cell.label_ &&
                  cell_map_labels.at<int>(v + 1, u) == minor_cell.label_))) {
                cell_map.at<uchar>(v, u) = 255;
                cell_map_labels.at<int>(v, u) = major_cell.label_;
                major_cell.area_ += 1;
            }

    //更新 cell_map_labels 中的标签
    for (int v = 0; v < cell_map_labels.rows; ++v)
        for (int u = 0; u < cell_map_labels.cols; ++u)
            if (cell_map_labels.at<int>(v, u) == minor_cell.label_)
                cell_map_labels.at<int>(v, u) = major_cell.label_;

    // 更新 major_cell
    major_cell.area_ += minor_cell.area_;
    for (BoustrophedonCell::BoustrophedonCellSetIterator itn = major_cell.neighbors_.begin();
         itn != major_cell.neighbors_.end(); ++itn)
        if ((*itn)->label_ == minor_cell.label_) {
            major_cell.neighbors_.erase(itn);
            break;
        }

    for (BoustrophedonCell::BoustrophedonCellSetIterator itn = minor_cell.neighbors_.begin();
         itn != minor_cell.neighbors_.end(); ++itn)
        if ((*itn)->label_ != major_cell.label_)
            major_cell.neighbors_.insert(*itn);

    //清除对minor_cell的所有引用
    cell_index_mapping.erase(minor_cell.label_);
    for (std::map<int, boost::shared_ptr<BoustrophedonCell> >::iterator itc = cell_index_mapping.begin();
         itc != cell_index_mapping.end(); ++itc)
        for (BoustrophedonCell::BoustrophedonCellSetIterator itn = itc->second->neighbors_.begin();
             itn != itc->second->neighbors_.end(); ++itn)
            if ((*itn)->label_ == minor_cell.label_) {
                (*itn)->label_ = major_cell.label_;
                break;
            }
}

/**
 * 从左往右取点
 * @param original_path
 * @param downsampled_path
 * @param robot_pos
 * @param path_eps
 */
void BoustrophedonExplorer::downsamplePath(const std::vector<cv::Point> &original_path,
                                           std::vector<cv::Point> &downsampled_path,
                                           cv::Point &robot_pos, const double path_eps) {
    for (size_t path_point = 0; path_point < original_path.size(); ++path_point) {
        if (cv::norm(robot_pos - original_path[path_point]) >= path_eps) {
            downsampled_path.push_back(original_path[path_point]);
            robot_pos = original_path[path_point];
        }
    }
    if (original_path.size() > 0) {
        downsampled_path.push_back(original_path.back());
        robot_pos = original_path.back();
    }
    if (original_path.size() > 0 && downsampled_path.size() == 0) {
        downsampled_path.push_back(original_path[0]);
    }
}

/**
 * 从右往左取点
 * @param original_path
 * @param downsampled_path
 * @param robot_pos
 * @param path_eps
 */
void BoustrophedonExplorer::downsamplePathReverse(const std::vector<cv::Point> &original_path,
                                                  std::vector<cv::Point> &downsampled_path,
                                                  cv::Point &robot_pos, const double path_eps) {
    for (size_t path_point = original_path.size() - 1;; --path_point) {
        if (cv::norm(robot_pos - original_path[path_point]) >= path_eps) {
            downsampled_path.push_back(original_path[path_point]);
            robot_pos = original_path[path_point];
        }
        if (path_point == 0)
            break;
    }
    if (original_path.size() > 0) {
        downsampled_path.push_back(original_path[0]);
        robot_pos = original_path[0];
    }
}