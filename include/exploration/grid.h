//
// Created by Looper on 2022/9/30.
//

#ifndef APP_COMMUNICATION_GRID_H
#define APP_COMMUNICATION_GRID_H

#include <vector>
#include <opencv2/opencv.hpp>
#include "simulation.h"

class BoustrophedonLine {
public:
    std::vector<cv::Point> upper_line;
    std::vector<cv::Point> lower_line;
    bool has_two_valid_lines;

    BoustrophedonLine() : has_two_valid_lines(false) {}
};

class BoustrophedonGrid : public std::vector<BoustrophedonLine> {
};

class GridGenerator {
public:
    GridGenerator() {
    }

    void generateStandardGrid(const cv::Mat &room_map, std::vector<cv::Point> &cell_centers, const int cell_size,
                              const bool complete_cell_test = true, const bool start_grid_at_first_free_pixel = true) {
        int min_y = 1000000, max_y = 0, min_x = 1000000, max_x = 0;
        for (int y = 0; y < room_map.rows; y++) {
            for (int x = 0; x < room_map.cols; x++) {
                if (room_map.at<unsigned char>(y, x) == 255) {
                    if (x < min_x)
                        min_x = x;
                    if (x > max_x)
                        max_x = x;
                    if (y < min_y)
                        min_y = y;
                    if (y > max_y)
                        max_y = y;
                }
            }
        }

        const uint16_t half_cell_size = (uint16_t) cell_size / (uint16_t) 2;
        if (!start_grid_at_first_free_pixel) {
            min_x += half_cell_size;
            min_y += half_cell_size;
        }

        // create the grid
        if (complete_cell_test) {
            for (int y = min_y; y <= max_y; y += cell_size) {
                for (int x = min_x; x <= max_x; x += cell_size) {
                    cv::Point cell_center(x, y);
                    if (completeCellTest(room_map, cell_center, cell_size))
                        cell_centers.push_back(cell_center);
                }
            }
        } else {
            // only create cells where the cell center is accessible
            for (int y = min_y; y <= max_y; y += cell_size)
                for (int x = min_x; x <= max_x; x += cell_size)
                    if (room_map.at<unsigned char>(y, x) == 255)
                        cell_centers.push_back(cv::Point(x, y));
        }
    }

    static bool completeCellTest(const cv::Mat &room_map, cv::Point &cell_center, const int cell_size) {
        const int x = cell_center.x;
        const int y = cell_center.y;
        if (room_map.at<unsigned char>(y, x) == 255) {
            return true;
        } else {
            const uint16_t half_cell_size =
                    (uint16_t) cell_size / (uint16_t) 2;
            const bool even_grid_size = ((cell_size % 2) == 0);

            const int upper_bound = even_grid_size ? half_cell_size - 1 : half_cell_size;
            cv::Mat cell_pixels = cv::Mat::zeros(cell_size, cell_size, CV_8UC1);
            int accessible_pixels = 0;
            for (int dy = -half_cell_size; dy <= upper_bound; ++dy) {
                for (int dx = -half_cell_size; dx <= upper_bound; ++dx) {
                    const int nx = x + dx;
                    const int ny = y + dy;
                    if (nx < 0 || nx >= room_map.cols || ny < 0 || ny >= room_map.rows)
                        continue;
                    if (room_map.at<unsigned char>(ny, nx) == 255) {
                        ++accessible_pixels;
                        cell_pixels.at<unsigned char>(half_cell_size + dy, half_cell_size + dx) = 255;
                    }
                }
            }

            if (accessible_pixels > 0) {
                cv::Mat distances;
                cv::distanceTransform(cell_pixels, distances, CV_DIST_L2, 5);
                double max_distance = 0.;
                cv::minMaxLoc(distances, 0, &max_distance, 0, &cell_center);
                cell_center.x += x - half_cell_size;
                cell_center.y += y - half_cell_size;
                double min_squared_center_distance =
                        (x - cell_center.x) * (x - cell_center.x) + (y - cell_center.y) * (y - cell_center.y);
                for (int v = 0; v < distances.rows; ++v) {
                    for (int u = 0; u < distances.cols; ++u) {
                        if ((double) distances.at<float>(v, u) == max_distance) {
                            const double squared_center_distance = (u - half_cell_size) * (u - half_cell_size) +
                                                                   (v - half_cell_size) * (v - half_cell_size);
                            if (squared_center_distance < min_squared_center_distance) {
                                cell_center = cv::Point(x - half_cell_size + u, y - half_cell_size + v);
                                min_squared_center_distance = squared_center_distance;
                            }
                        }
                    }
                }

                return true;
            }
        }
        return false;
    }


    /**
     * @param room_map 仿射变换后的分区片段图，位置为 y 轴方向为图像大小，x 轴中心点为图像的中心位置，0障碍物，255可通行区域，CV_8UC1
     * @param inflated_room_map 仿射变换后的分区片段图，位置为 y 轴方向为图像大小，x 轴中心点为图像的中心位置，图像为外围腐蚀的区域为128
     * @param map_inflation_radius
     * @param grid_points
     * @param min_max_map_coordinates 限定的最大最小范围，都为-1则按照地图尺寸计算
     * @param grid_spacing 机器人转成正方形后的边长，单位 px
     * @param half_grid_spacing 机器人转成正方形后的边长的一半，单位 px
     * @param grid_spacing_horizontal 1
     * @param max_deviation_from_track 机器人对障碍物的额外偏移，单位 px
     */
    static void
    generateBoustrophedonGrid(const cv::Mat &room_map, cv::Mat &inflated_room_map, const int map_inflation_radius,
                              BoustrophedonGrid &grid_points, const cv::Vec4i &min_max_map_coordinates,
                              const int grid_spacing, const int half_grid_spacing,
                              const int grid_spacing_horizontal, int max_deviation_from_track = -1) {

        if (max_deviation_from_track < 0)
            max_deviation_from_track = grid_spacing;

        if (inflated_room_map.rows != room_map.rows || inflated_room_map.cols != room_map.cols)
            cv::erode(room_map, inflated_room_map, cv::Mat(), cv::Point(-1, -1), map_inflation_radius);

        //遍历全图，计算得出区间的上下左右四个边界点
        int min_x = inflated_room_map.cols, max_x = -1, min_y = inflated_room_map.rows, max_y = -1;
        if (min_max_map_coordinates[0] == -1 && min_max_map_coordinates[1] == -1 &&
            min_max_map_coordinates[2] == -1 && min_max_map_coordinates[3] == -1) {
            for (int v = 0; v < inflated_room_map.rows; ++v) {
                for (int u = 0; u < inflated_room_map.cols; ++u) {
                    if (inflated_room_map.at<uchar>(v, u) == 255) {
                        if (min_x > u)
                            min_x = u;
                        if (max_x < u)
                            max_x = u;
                        if (min_y > v)
                            min_y = v;
                        if (max_y < v)
                            max_y = v;
                    }
                }
            }
        } else {
            min_x = min_max_map_coordinates[0];
            max_x = min_max_map_coordinates[1];
            min_y = min_max_map_coordinates[2];
            max_y = min_max_map_coordinates[3];
        }
        //如果房间没有可访问的单元格，因此没有最小/最大坐标，则返回
        if ((min_x == inflated_room_map.cols) || (max_x == -1) || (min_y == inflated_room_map.rows) || (max_y == -1))
            return;

        const int squared_grid_spacing_horizontal = grid_spacing_horizontal * grid_spacing_horizontal;
        int y = min_y;
        for (; y <= max_y + half_grid_spacing; y += grid_spacing) {//使用 max_y + half_grid_spacing 作为循环的结束条件

            if (y > max_y)//在底部最多只能发生一次
                y = max_y;

            BoustrophedonLine line;
            const cv::Point invalid_point(-1, -1);
            cv::Point last_added_grid_point_above(-10000, -10000),
                    last_added_grid_point_below(-10000, -10000);//用于保持水平网格距离
            cv::Point last_valid_grid_point_above(-1, -1), last_valid_grid_point_below(-1, -1);//用于添加最右边可能的点

            //从左往右，逐个像素点判断，将该 Y 坐标下的非障碍物点添加到覆盖线 line 里面
            for (int x = min_x; x <= max_x; x += 1) {

                //将点添加到 grid line 中，如下所示：
                // 1.如果当前点是可到达 --> 该点被添加到 upper_line 中，无效点（-1，-1）被添加到 lower_line
                // 2.如果当前点无法到达：
                // a）y方向周围没有其他点--> upper_line 、lower_line 不变
                // b）上面有通行点、下面没有 --> 有效点添加到 upper_line，无效点（-1，-1）添加到 lower_line
                // c）下面有通行点、上面没有 --> 有效点添加到 lower_line，无效点（-1，-1）添加到 upper_line
                // d）上面下面点都可通行 --> 有效点添加到 upper_line 、lower_line 中

                if (inflated_room_map.at<uchar>(y, x) == 255) {
                    //当前点是可到达
                    if (
                            squaredPointDistance(
                                    last_added_grid_point_above, cv::Point(x, y)
                            ) >= squared_grid_spacing_horizontal
                            ) {
                        line.upper_line.push_back(cv::Point(x, y));
                        line.lower_line.push_back(invalid_point);
                        last_added_grid_point_above = cv::Point(x, y);
                    } else {
                        //记录此点，如果它是最右边的点，则将其添加 upper line
                        last_valid_grid_point_above = cv::Point(x, y);
                    }
                } else {//检查当前点上方或下方的可访问性
                    bool found_above = false;
                    int dy = -1;
                    for (; dy > -max_deviation_from_track; --dy) {
                        if (y + dy >= 0 && inflated_room_map.at<uchar>(y + dy, x) == 255) {
                            found_above = true;
                            break;
                        }
                    }
                    if (found_above) {//检查当前点上方的可访问性
                        //b）上面有通行点、下面没有 --> 有效点添加到 upper_line，无效点（-1，-1）添加到 lower_line
                        if (
                                squaredPointDistance(
                                        last_added_grid_point_above, cv::Point(x, y + dy)
                                ) >= squared_grid_spacing_horizontal
                                ) {
                            line.upper_line.push_back(cv::Point(x, y + dy));
                            line.lower_line.push_back(invalid_point);
                            last_added_grid_point_above = cv::Point(x, y + dy);
                        } else {
                            //记录此点，如果它是最右边的点，则将其添加 upper line
                            last_valid_grid_point_above = cv::Point(x, y + dy);
                        }
                    }

                    bool found_below = false;
                    dy = 1;
                    for (; dy < max_deviation_from_track; ++dy) {
                        if (y + dy < inflated_room_map.rows && inflated_room_map.at<uchar>(y + dy, x) == 255) {
                            found_below = true;
                            break;
                        }
                    }
                    if (found_below) {//检查当前点下方的可访问性
                        if (
                                squaredPointDistance(
                                        last_added_grid_point_below, cv::Point(x, y + dy)
                                ) >= squared_grid_spacing_horizontal
                                ) {
                            if (found_above) {
                                //d）上面下面点都可通行 --> 有效点添加到 upper_line 、lower_line 中
                                line.has_two_valid_lines = true;
                                line.lower_line.back().x = x;
                                line.lower_line.back().y = y + dy;
                            } else {
                                //c）下面有通行点、上面没有 --> 有效点添加到 lower_line，无效点（-1，-1）添加到 upper_line
                                line.upper_line.push_back(invalid_point);
                                line.lower_line.push_back(cv::Point(x, y + dy));
                            }
                            last_added_grid_point_below = cv::Point(x, y + dy);
                        } else {
                            //存储此点，如果它是最右边的点，则将其添加到lower line
                            last_valid_grid_point_below = cv::Point(x, y + dy);
                        }
                    }
                }
            }

            //如果可用，添加最右侧的点
            if (last_valid_grid_point_above.x > -1 &&
                last_valid_grid_point_above.x > last_added_grid_point_above.x) {//有效
                line.upper_line.push_back(last_valid_grid_point_above);
                if (last_valid_grid_point_below.x > -1 && last_valid_grid_point_below.x > last_added_grid_point_below.x)
                    line.lower_line.push_back(last_valid_grid_point_below);
                else
                    line.lower_line.push_back(invalid_point);
            } else {
                if (last_valid_grid_point_below.x > -1 &&
                    last_valid_grid_point_below.x > last_added_grid_point_below.x) {
                    line.upper_line.push_back(invalid_point);
                    line.lower_line.push_back(last_valid_grid_point_below);
                }
            }

            //上面生成的是一条覆盖线，下面则是把当前生成的覆盖直线添加到覆盖线数组里面
            //清除网格线数据
            // 1.如果没有有效点 --> 不要添加 cleaned_line
            // 2.如果 two_valid_lines 为true，则有两个单独的行可供访问
            // 3.否则只有一个有效行，数据可能分布在 upper_line 和 lower_line 上

            BoustrophedonLine cleaned_line;
            if (line.upper_line.size() > 0 && line.lower_line.size() > 0) {//检查行中是否有有效数据
                if (line.has_two_valid_lines) {
                    cleaned_line.has_two_valid_lines = true;
                    for (size_t i = 0; i < line.upper_line.size(); ++i) {
                        if (line.upper_line[i] != invalid_point)
                            cleaned_line.upper_line.push_back(line.upper_line[i]);
                        if (line.lower_line[i] != invalid_point)
                            cleaned_line.lower_line.push_back(line.lower_line[i]);
                    }
                } else {
                    for (size_t i = 0; i < line.upper_line.size(); ++i) {
                        if (line.upper_line[i] != invalid_point)
                            cleaned_line.upper_line.push_back(line.upper_line[i]);
                        else if (line.lower_line[i] != invalid_point)
                            cleaned_line.upper_line.push_back(line.lower_line[i]);
                    }
                }

//                LOG_IF(INFO, DEBUG_EXPLORATION) << min_x << max_x << min_y << max_y
//                          << " 第 " << y << " 列  two_valid_lines : " << cleaned_line.has_two_valid_lines
//                          << "  upper_line size : " << cleaned_line.upper_line.size() << "  lower_line size : "
//                          << cleaned_line.lower_line.size();

                grid_points.push_back(cleaned_line);
            }
        }
    }

    static double squaredPointDistance(const cv::Point &p1, const cv::Point &p2) {
        return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
    }
};


#endif //APP_COMMUNICATION_GRID_H
