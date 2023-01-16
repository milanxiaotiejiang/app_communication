//
// Created by Looper on 2022/10/8.
//

#ifndef APP_COMMUNICATION_NODE_H
#define APP_COMMUNICATION_NODE_H

class NodeAstar {
protected:
    int xPos_;
    int yPos_;
    int level_;
    int priority_;

public:
    NodeAstar(int xp, int yp, int d, int p);

    int getxPos() const;

    int getyPos() const;

    int getLevel() const;

    int getPriority() const;

    void updatePriority(const int &xDest, const int &yDest);

    void nextLevel(const int &i);

    const int &estimate(const int &xDest, const int &yDest) const;
};


#endif //APP_COMMUNICATION_NODE_H
