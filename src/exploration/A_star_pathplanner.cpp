//
// Created by Looper on 2022/10/8.
//

#include "exploration/A_star_pathplanner.h"
#include "exploration/cv_extend.h"
#include "simulation.h"

const int dir = 8;

static int dx[dir] =
        {1, 1, 0, -1, -1, -1, 0, 1};
static int dy[dir] =
        {0, 1, 1, 1, 0, -1, -1, -1};

static int expanding_counter = 0;

bool operator<(const NodeAstar &a, const NodeAstar &b) {
    return a.getPriority() > b.getPriority();
}

AStarPlanner::AStarPlanner() {
    n = 1;
    m = 1;
}

void AStarPlanner::drawRoute(cv::Mat &map, const cv::Point start_point, const std::string &route, double step_length) {
    if (route.length() > 0) {
        int j;
        char c;
        int x1 = start_point.x;
        int y1 = start_point.y;
        int x2, y2;
        for (int i = 0; i < route.length(); i++) {
            c = route.at(i);
            j = c - '0';
            x2 = x1 + dx[j] * step_length;
            y2 = y1 + dy[j] * step_length;
            const double progress = 0.2 + 0.6 * (double) i / (double) route.length();
            cv::line(map, cv::Point(x1, y1), cv::Point(x2, y2), CV_RGB(0, progress * 255, 0), 1);
            x1 = x2;
            y1 = y2;
        }
    }
}

void AStarPlanner::getRoute(const cv::Point start_point, const std::string &route,
                            double step_length, std::vector<cv::Point> &route_points) {
    if (route.length() > 0) {
        int j;
        char c;
        int x1 = start_point.x;
        int y1 = start_point.y;
        route_points.push_back(cv::Point(x1, y1));
        int x2, y2;
        for (int i = 0; i < route.length(); i++) {
            c = route.at(i);
            j = c - '0';
            x2 = x1 + dx[j] * step_length;
            y2 = y1 + dy[j] * step_length;
            route_points.push_back(cv::Point(x2, y2));
            x1 = x2;
            y1 = y2;
        }
    }
}

void AStarPlanner::downsampleMap(const cv::Mat &map, cv::Mat &downsampled_map, const double downsampling_factor,
                                 const double robot_radius, const double map_resolution) {
    cv::Mat eroded_map;
    int number_of_erosions = (robot_radius / map_resolution);
//    cv::erode(map, eroded_map, cv::Mat(), cv::Point(-1, -1), number_of_erosions);
    explorationErode(map, eroded_map, cv::MORPH_CROSS, number_of_erosions);
    if (downsampling_factor != 1.)
        cv::resize(eroded_map, downsampled_map, cv::Size(0, 0),
                   downsampling_factor, downsampling_factor, cv::INTER_NEAREST);//_LINEAR);
    else
        downsampled_map = eroded_map;
}

std::string AStarPlanner::pathFind(const int &xStart, const int &yStart, const int &xFinish, const int &yFinish,
                                   const cv::Mat &map) {
    static std::priority_queue<NodeAstar> pq[2];
    static int pqi;
    static NodeAstar *n0;
    static NodeAstar *m0;
    static int i, j, x, y, xdx, ydy;
    static char c;
    pqi = 0;

    cv::Mat map_to_calculate_path(cv::Size(m, n), CV_32S);

    for (y = 0; y < map.rows; y++) {
        for (x = 0; x < map.cols; x++) {
            if (map.at<unsigned char>(y, x) == 255) {
                map_to_calculate_path.at<int>(x, y) = 0;
            } else {
                map_to_calculate_path.at<int>(x, y) = 1;
            }
        }
    }

    cv::Mat closed_nodes_map(cv::Size(m, n), CV_32S);
    cv::Mat open_nodes_map(cv::Size(m, n), CV_32S);
    cv::Mat dir_map(cv::Size(m, n), CV_32S);

    for (y = 0; y < closed_nodes_map.rows; y++) {
        for (x = 0; x < closed_nodes_map.cols; x++) {
            closed_nodes_map.at<int>(y, x) = 0;
            open_nodes_map.at<int>(y, x) = 0;
        }
    }

    n0 = new NodeAstar(xStart, yStart, 0, 0);
    n0->updatePriority(xFinish, yFinish);
    pq[pqi].push(*n0);
    open_nodes_map.at<int>(xStart, yStart) = n0->getPriority();

    delete n0;

    while (!pq[pqi].empty()) {
        n0 = new NodeAstar(pq[pqi].top().getxPos(), pq[pqi].top().getyPos(), pq[pqi].top().getLevel(),
                           pq[pqi].top().getPriority());

        x = n0->getxPos();
        y = n0->getyPos();

        pq[pqi].pop();
        open_nodes_map.at<int>(x, y) = 0;
        closed_nodes_map.at<int>(x, y) = 1;

        if (x == xFinish && y == yFinish) {
            std::string path = "";
            while (!(x == xStart && y == yStart)) {
                j = dir_map.at<int>(x, y);
                c = '0' + (j + dir / 2) % dir;
                path = c + path;
                x += dx[j];
                y += dy[j];
            }

            delete n0;
            while (!pq[pqi].empty())
                pq[pqi].pop();
            return path;
        }

        for (i = 0; i < dir; i++) {
            xdx = x + dx[i];
            ydy = y + dy[i];

            expanding_counter++;

            if (!(xdx < 0 || xdx > n - 1 || ydy < 0 || ydy > m - 1 || map_to_calculate_path.at<int>(xdx, ydy) == 1 ||
                  closed_nodes_map.at<int>(xdx, ydy) == 1)) {
                m0 = new NodeAstar(xdx, ydy, n0->getLevel(), n0->getPriority());
                m0->nextLevel(i);
                m0->updatePriority(xFinish, yFinish);

                if (open_nodes_map.at<int>(xdx, ydy) == 0) {
                    open_nodes_map.at<int>(xdx, ydy) = m0->getPriority();
                    pq[pqi].push(*m0);
                    dir_map.at<int>(xdx, ydy) = (i + dir / 2) % dir;
                } else if (open_nodes_map.at<int>(xdx, ydy) > m0->getPriority()) {
                    open_nodes_map.at<int>(xdx, ydy) = m0->getPriority();
                    dir_map.at<int>(xdx, ydy) = (i + dir / 2) % dir;

                    while (!(pq[pqi].top().getxPos() == xdx && pq[pqi].top().getyPos() == ydy)) {
                        pq[1 - pqi].push(pq[pqi].top());
                        pq[pqi].pop();
                    }
                    pq[pqi].pop();

                    if (pq[pqi].size() > pq[1 - pqi].size())
                        pqi = 1 - pqi;
                    while (!pq[pqi].empty()) {
                        pq[1 - pqi].push(pq[pqi].top());
                        pq[pqi].pop();
                    }
                    pqi = 1 - pqi;
                    pq[pqi].push(*m0);
                }
                delete m0;
            }
        }
        delete n0;
    }
    return "";
}

double AStarPlanner::planPath(const cv::Mat &map, const cv::Point &start_point, const cv::Point &end_point,
                              const double downsampling_factor, const double robot_radius, const double map_resolution,
                              const int end_point_valid_neighborhood_radius, std::vector<cv::Point> *route) {
    expanding_counter = 0;
    double step_length = 1. / downsampling_factor;

    double path_length = 0;

    if (start_point.x == end_point.x && start_point.y == end_point.y) {
        return path_length;
    }

    if (start_point.x < 0 || start_point.x >= map.cols || start_point.y < 0 || start_point.y >= map.rows ||
        end_point.x < 0 || end_point.x >= map.cols || end_point.y < 0 || end_point.y >= map.rows) {
        return 1e100;
    }

    cv::Mat downsampled_map;
    downsampleMap(map, downsampled_map, downsampling_factor, robot_radius, map_resolution);

    int start_x = downsampling_factor * start_point.x;
    int start_y = downsampling_factor * start_point.y;
    int end_x = downsampling_factor * end_point.x;
    int end_y = downsampling_factor * end_point.y;

    m = downsampled_map.rows;
    n = downsampled_map.cols;

    route_ = pathFind(start_x, start_y, end_x, end_y, downsampled_map);
    if (route_.empty()) {
        if (end_point_valid_neighborhood_radius > 0) {
            for (int r = 1; r <= end_point_valid_neighborhood_radius; ++r) {
                for (int rdy = -r; rdy <= r; ++rdy) {
                    for (int rdx = -r; rdx <= r; ++rdx) {
                        if ((abs(rdy) != r && abs(rdx) != r) || end_x + rdx < 0 || end_x + rdx >= n || end_y + rdy < 0 ||
                            end_y + rdy >= m)
                            continue;
                        route_ = pathFind(start_x, start_y, end_x + rdx, end_y + rdy, downsampled_map);
                        if (!route_.empty())
                            break;
                    }
                    if (!route_.empty())
                        break;
                }
                if (!route_.empty())
                    break;
            }
        }
        if (route_.empty()) {
            return 1e100;
        }
    }

    if (route_.length() > 0) {
        int j;
        char c;
        int x = start_x;
        int y = start_y;
        const double straight_step = (1. / downsampling_factor);
        const double diagonal_step = (std::sqrt(2.) / downsampling_factor);
        for (int i = 0; i < route_.length(); i++) {
            c = route_.at(i);
            j = c - '0';
            x = x + dx[j];
            y = y + dy[j];
            if (j == 0 || j == 2 || j == 4 || j == 6) {
                path_length += straight_step;
            }
            if (j == 1 || j == 3 || j == 5 || j == 7) {
                path_length += diagonal_step;
            }
        }
    }

    if (route != NULL)
        getRoute(start_point, route_, step_length, *route);

    return path_length;
}


double AStarPlanner::planPath(const cv::Mat &map, const cv::Mat &downsampled_map, const cv::Point &start_point,
                              const cv::Point &end_point, const double downsampling_factor,
                              const double robot_radius, const double map_resolution,
                              const int end_point_valid_neighborhood_radius, cv::Mat *draw_path_map,
                              std::vector<cv::Point> *route) {
    route_ = "";
    double step_length = 1. / downsampling_factor;
    double pathlength = step_length *
                        planPath(downsampled_map, downsampling_factor * start_point, downsampling_factor * end_point,
                                 1., 0., map_resolution, end_point_valid_neighborhood_radius, route);
    if (pathlength > 1e90) {
        pathlength = planPath(map, start_point, end_point, 1., 0., map_resolution,
                              1. / downsampling_factor * end_point_valid_neighborhood_radius, route);
        step_length = 1.;
    }
    if (pathlength > 1e90)
        LOG_IF(INFO, DEBUG_EXPLORATION)
        << "######################### No path found on the originally sized map #######################";
    else {
        if (draw_path_map != nullptr) {
            drawRoute(*draw_path_map, start_point, route_, step_length);
        }
    }

    return pathlength;
}
