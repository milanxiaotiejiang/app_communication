//
// Created by Looper on 2022/12/1.
//

#ifndef APP_COMMUNICATION_COMBINATIONPOSEVO_H
#define APP_COMMUNICATION_COMBINATIONPOSEVO_H


class CombinationPoseVo {
private:
    float x;
    float y;
    float theta;
    int index;
public:
    CombinationPoseVo();

    CombinationPoseVo(float x, float y, float theta, int index);

    float getX() const;

    void setX(float x);

    float getY() const;

    void setY(float y);

    float getTheta() const;

    void setTheta(float theta);

    int getIndex() const;

    void setIndex(int index);
};


#endif //APP_COMMUNICATION_COMBINATIONPOSEVO_H
