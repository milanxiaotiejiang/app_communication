//
// Created by Looper on 2022/10/24.
//

#include "alignment/alignment_utils.h"

void alignment::set_array(uchar *arr, int cols, cv::Point ltop, cv::Point rbottom, int val) {
    // 直线绘制
    if (ltop.x == rbottom.x || ltop.y == rbottom.y) {
        for (int i = ltop.y; i <= rbottom.y; i++) {
            for (int j = ltop.x; j <= rbottom.x; j++) {
                *(arr + i * cols + j) = val;
            }
        }
    }
        // 斜线绘制,绘制斜线时须在X和Y两个方向进行遍历,否则对于近乎水平或竖直的斜线绘制会有问题
    else {
        float x_slope = float(rbottom.y - ltop.y) / (rbottom.x - ltop.x);
        for (int i = ltop.x; i <= rbottom.x; i++) {
            int yoffset = (i - ltop.x) * x_slope + ltop.y;
            *(arr + yoffset * cols + i) = val;
        }
        float y_slope = float(rbottom.x - ltop.x) / (rbottom.y - ltop.y);
        for (int j = ltop.y; j <= rbottom.y; j++) {
            int xoffset = (j - ltop.y) * y_slope + ltop.x;
            *(arr + j * cols + xoffset) = val;
        }
    }
}
