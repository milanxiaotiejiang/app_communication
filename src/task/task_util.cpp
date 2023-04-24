//
// Created by Looper on 2023/4/23.
//

#include "task/task_util.h"

#include <valarray>

void generateChildPointFlow(const std::vector<RealPoint> &points, std::vector<Cp> &cpList, float resolution_) {
    if (points.empty()) {
        return;
    }

    double min_sq_resolution = resolution_ * resolution_;

    int accumulate = 0;

    auto firstPoint = points[0];
    cpList.emplace_back(accumulate, firstPoint.id, firstPoint.realPosition, firstPoint.realOrientation, false);
    accumulate++;

    double last_x = firstPoint.realPosition.x;
    double last_y = firstPoint.realPosition.y;

    for (unsigned int i = 1; i < points.size(); ++i) {
        auto currentPoint = points[i];
        double loop_x = currentPoint.realPosition.x;
        double loop_y = currentPoint.realPosition.y;
        auto loopPosition = currentPoint.realPosition;
        auto loopOrientation = currentPoint.realOrientation;

        double dist = (loop_x - last_x) * (loop_x - last_x) + (loop_y - last_y) * (loop_y - last_y);
        if (dist > min_sq_resolution) {
            int steps = ceil((std::sqrt(dist)) / resolution_);
            // add a points in-between
            double deltaX = (loop_x - last_x) / steps;
            double deltaY = (loop_y - last_y) / steps;
            for (int j = 1; j < steps; ++j) {
                RealPosition position(last_x + j * deltaX, last_y + j * deltaY, loopPosition.z);
                cpList.emplace_back(accumulate, currentPoint.id, position, loopOrientation, false);
                accumulate++;
            }
        }

        cpList.emplace_back(accumulate, currentPoint.id, loopPosition, loopOrientation, true);
        accumulate++;
        last_x = loop_x;
        last_y = loop_y;
    }
}
