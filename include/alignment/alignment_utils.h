//
// Created by Looper on 2022/10/24.
//

#ifndef APP_COMMUNICATION_ALIGNMENT_UTILS_H
#define APP_COMMUNICATION_ALIGNMENT_UTILS_H

#include "vector"
#include <opencv2/opencv.hpp>

namespace alignment {
    template<typename T>
    void quick_sort(std::vector<T> &arr, int left, int right, std::vector<int> &arr_index) {
        int i, j;
        int t1, temp1;
        T temp, t;

        if (left > right)
            return;

        temp = arr[left];
        temp1 = arr_index[left];
        i = left;
        j = right;

        while (i != j) {
            while (arr[j] >= temp && i < j)
                j--;
            while (arr[i] <= temp && i < j)
                i++;
            if (i < j) {
                t = arr[i];
                arr[i] = arr[j];
                arr[j] = t;
                t1 = arr_index[i];
                arr_index[i] = arr_index[j];
                arr_index[j] = t1;
            }
        }
        arr[left] = arr[i];
        arr[i] = temp;

        arr_index[left] = arr_index[i];
        arr_index[i] = temp1;
        quick_sort(arr, left, i - 1, arr_index);
        quick_sort(arr, i + 1, right, arr_index);
    }

    template<typename T>
    void resort_arr(std::vector<T> &arr, std::vector<int> indexs) {
        int len = arr.size();
        T *backup = new T[len];
        for (int i = 0; i < len; i++) {
            backup[i] = arr[i];
        }
        for (int i = 0; i < len; i++) {
            arr[i] = backup[indexs[i]];
        }
    }

    void set_array(uchar *arr, int cols, cv::Point ltop, cv::Point rbottom, int val);
}

#endif //APP_COMMUNICATION_ALIGNMENT_UTILS_H
