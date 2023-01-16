//
// Created by Looper on 2022/10/8.
//

#include "exploration/node.h"

#include <cmath>

const int dir_ = 8;

NodeAstar::NodeAstar(int xp, int yp, int d, int p) {
    xPos_ = xp;
    yPos_ = yp;
    level_ = d;
    priority_ = p;
}

int NodeAstar::getxPos() const {
    return xPos_;
}

int NodeAstar::getyPos() const {
    return yPos_;
}

int NodeAstar::getLevel() const {
    return level_;
}

int NodeAstar::getPriority() const {
    return priority_;
}

void NodeAstar::updatePriority(const int &xDest, const int &yDest) {
    priority_ = level_ + estimate(xDest, yDest); // * 10; //A*
}

void NodeAstar::nextLevel(const int &i) // i: direction
{
    level_ += (dir_ == 8 ? (i % 2 == 0 ? 10 : 14) : 10);
}

const int &NodeAstar::estimate(const int &xDest, const int &yDest) const {
    static int xd, yd, d;
    xd = xDest - xPos_;
    yd = yDest - yPos_;

    // Euclidian Distance
    d = static_cast<int>(sqrt(xd * xd + yd * yd));

    // Manhattan distance
//	d = abs(xd) + abs(yd);

    // Chebyshev distance
//	d=std::max(abs(xd), abs(yd));

    return (d);
}

