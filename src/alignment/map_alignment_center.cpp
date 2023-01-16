//
// Created by Looper on 2022/10/24.
//

#include "alignment/map_alignment_center.h"
#include "segmentation/SegmentationCenter.h"
#include "alignment/alignment_utils.h"

const int merge_step = 3;//merge_step行或列作为一个线段合并的计算单元
const int merge_max_gap = 3;//前后线段之间的首尾距离小于merge_max_gap才满足合并条件
const int merge_len_thresh = 30;//合并的两条线段的长度和大于merge_len_thresh才满足合并条件(避免斜线被合并)

void AlignmentCenter::initialize(ros::NodeHandle handle) {
    cv::Mat map = SegmentationCenter::instance().generateMat();

    const cv::Mat &image = map.clone();
    cv::erode(image, image, cv::Mat());
    cv::medianBlur(image, image, 5);
    cv::Mat distance_map;
    cv::distanceTransform(image, distance_map, CV_DIST_L2, cv::DIST_MASK_PRECISE);

    cv::Mat dx, dy;
    cv::Sobel(distance_map, dx, CV_64F, 1, 0, 5);//求x方向的一阶导数
    cv::Sobel(distance_map, dy, CV_64F, 0, 1, 5);//求y方向的一阶导数

    cv::Mat sx, sy;
    cv::convertScaleAbs(dx, sx);
    cv::convertScaleAbs(dy, sy);

    cv::Mat grad;
//    grad = sx + sy;
    cv::addWeighted(sx, 0.5, sy, 0.5, 0, grad);
    for (int y = 0; y < grad.rows; y++) {
        for (int x = 0; x < grad.cols; x++) {
            if (map.at<unsigned char>(y, x) != 255) {
                map.at<unsigned char>(y, x) = 0;
            }
        }
    }

    int i = grad.type();

    cv::erode(grad, grad, cv::Mat());
    double min_val = 0., max_val = 0.;
    cv::minMaxLoc(grad, &min_val, &max_val);

    grad.convertTo(grad, CV_8U);

//    skiz = cv::dilate(skiz, kernel, iterations = 1)
//    skiz = cv2.erode(skiz, kernel, iterations = 1)
//    skiz = cv2.medianBlur(skiz, 3)
//    cv::threshold(grad, grad, 127, 255, cv::THRESH_BINARY_INV);

    cv::imshow("grad", grad);
    cv::waitKey();
}


void AlignmentCenter::merge_lines(const cv::Mat &mat) {
    LOG(INFO) << "canny w : " << mat.cols << ", h : " << mat.rows << ", channel : " << mat.channels();
    uchar *image_data = mat.data;
    int width = mat.cols;
    int height = mat.rows;

//    for (int x = 0; x < mat.cols; x++) {
//        int line_num = 0;
//        std::vector<std::vector<int>> vec_lines;
//
//        if (x + merge_step >= mat.cols - 1) {
//            continue;
//        }
//
//        for (int y = 0; y < mat.rows; y++) {
//            std::vector<int> vec_line;
//            int last_val = 0;
//            int head_off = 0;
//            int tail_off = 0;
//
//            int value = mat.at<unsigned char>(y, x);
//            int gray_scale = *(image_data + y * mat.cols + x);
//
//            if (gray_scale == 255 && last_val == 0) {
//                head_off = y;
//            }
//            if ((gray_scale == 0 && last_val == 255)) {
//                tail_off = y;
//                if (tail_off - head_off == 1) {
//                    last_val = 0;
//                    continue;
//                }
//                vec_line.push_back(head_off);
//                vec_line.push_back(tail_off);
//                line_num++;
//            }
//            last_val = gray_scale;
//        }
////        vec_lines.push_back(vec_line);
//    }

    for (int w = 0; w < width - merge_step; w++) {

        int line_num = 0;
        std::vector<std::vector<int>> vec_lines;

        for (int x = w; x < w + merge_step; x++) {

            std::vector<int> vec_line;
            int last_val = 0;
            int head_off = 0;
            int tail_off = 0;

            for (int h = 0; h < height; h++) {
                int gray_scale = *(image_data + h * width + x);
                if (gray_scale == 255 && last_val == 0) {
                    head_off = h;
                }
                if ((gray_scale == 0 && last_val == 255)) {
                    tail_off = h;
                    // 长度为1的线段忽略，避免处理后直角处出现断裂
                    if (tail_off - head_off == 1) {
                        last_val = 0;
                        continue;
                    }
                    vec_line.push_back(head_off);
                    vec_line.push_back(tail_off);
                    line_num++;
                }
                last_val = gray_scale;
            }
            vec_lines.push_back(vec_line);
        }

        // 不少于2条线段执行合并操作
        if (line_num >= 2) {
            std::vector<int> heads; //保存线段的头部偏移
            std::vector<int> tails; //保存线段的尾部偏移
            std::vector<int> indexs;
            for (int i = 0; i < vec_lines.size(); i++) {
                for (int j = 0; j < vec_lines[i].size(); j += 2) {
                    heads.push_back(vec_lines[i][j]);
                    tails.push_back(vec_lines[i][j + 1]);
                    indexs.push_back(indexs.size());
                }
            }

            // 对计算单元内所有的线段头部偏移进行从小到大的排序
            alignment::quick_sort(heads, 0, line_num - 1, indexs);
            // 所有的尾部偏移根据头部偏移的排序索引重新排序
            alignment::resort_arr(tails, indexs);

            std::vector<int> merge_heads; //线段合并后的头部偏移数组,同一列需要合并的线段可能不止一条
            std::vector<int> merge_tails; //线段合并后的尾部偏移数组
            int current_merge_head = heads[0]; //当前合并线段的头部偏移
            int current_merge_tail = tails[0]; //当前合并线段的尾部偏移
            for (int i = 1; i < line_num; i++) {
                int merge_len = current_merge_tail - current_merge_head;
                int line_len = tails[i] - heads[i];
                // 合并条件：当前合并线段的尾部偏移和下一条线段的头部偏移最大的距离不超过阈值，
                // 且二者长度和需大于阈值，防止邻近的两条斜线被误合并
                if (current_merge_tail - heads[i] > -merge_max_gap
                    && merge_len + line_len > merge_len_thresh) {
                    current_merge_tail = std::max(current_merge_tail, tails[i]);
                } else {
                    if (merge_len > merge_len_thresh) {
                        merge_heads.push_back(current_merge_head);
                        merge_tails.push_back(current_merge_tail);
                    }
                    current_merge_head = heads[i];
                    current_merge_tail = tails[i];
                }
            }
            // 若最后一条线段也需要合并，则需要手动把合并线段加进来(此处可能会添加重复，不过不影响)
            if (current_merge_tail - current_merge_head > merge_len_thresh) {
                merge_heads.push_back(current_merge_head);
                merge_tails.push_back(current_merge_tail);
            }

            // 遍历计算单元里所有线段，将组成合并线段的各个子线段清除
            for (int k = 0; k < merge_heads.size(); k++) {
                cv::Point merge_head_pt(-1);
                cv::Point merge_tail_pt(-1);
                for (int i = 0; i < vec_lines.size(); i++) {
                    for (int j = 0; j < vec_lines[i].size(); j += 2) {
                        if (vec_lines[i][j] >= merge_heads[k] && vec_lines[i][j + 1] <= merge_tails[k]) {
                            // 记录合并线段的头坐标点
                            if (vec_lines[i][j] == merge_heads[k] && merge_head_pt.x == -1) {
                                merge_head_pt.x = w + i;
                                merge_head_pt.y = merge_heads[k];
                            }
                            // 记录合并线段的尾坐标点
                            if (vec_lines[i][j + 1] == merge_tails[k] && merge_tail_pt.x == -1) {
                                merge_tail_pt.x = w + i;
                                merge_tail_pt.y = merge_tails[k] - 1;
                            }
                            // 将子线段的像素值置0
                            alignment::set_array(image_data, width, cv::Point(w + i, vec_lines[i][j]),
                                                 cv::Point(w + i, vec_lines[i][j + 1] - 1), 0);
                        }
                    }
                }
                // 将合并线段的像素值置255
                alignment::set_array(image_data, width, merge_head_pt, merge_tail_pt, 255);
            }
        }
    }

    // 水平方向合并线段（同竖直方向合并线段同理）
    for (int n = 0; n < height - merge_step; n++) {
        int line_num = 0;
        std::vector<std::vector<int>> vec_lines;
        for (int y = n; y < n + merge_step; y++) {
            std::vector<int> vec_line;
            int last_val = 0;
            int head_off = 0;
            int tail_off = 0;
            for (int x = 0; x < width; x++) {
                int gray_scale = *(image_data + y * width + x);
                if (gray_scale == 255 && last_val == 0) {
                    head_off = x;
                }
                if ((gray_scale == 0 && last_val == 255)) {
                    tail_off = x;
                    if (tail_off - head_off == 1) {
                        last_val = 0;
                        continue;
                    }
                    vec_line.push_back(head_off);
                    vec_line.push_back(tail_off);
                    line_num++;
                }
                last_val = gray_scale;
            }
            vec_lines.push_back(vec_line);
        }

        if (line_num >= 2) {
            std::vector<int> heads;
            std::vector<int> tails;
            std::vector<int> indexs;
            for (int i = 0; i < vec_lines.size(); i++) {
                for (int j = 0; j < vec_lines[i].size(); j += 2) {
                    heads.push_back(vec_lines[i][j]);
                    tails.push_back(vec_lines[i][j + 1]);
                    indexs.push_back(indexs.size());
                }
            }

            alignment::quick_sort(heads, 0, line_num - 1, indexs);
            alignment::resort_arr(tails, indexs);

            std::vector<int> merge_heads;
            std::vector<int> merge_tails;
            int current_merge_head = heads[0];
            int current_merge_tail = tails[0];
            for (int i = 1; i < line_num; i++) {
                int merge_len = current_merge_tail - current_merge_head;
                int line_len = tails[i] - heads[i];
                if (current_merge_tail - heads[i] > -merge_max_gap
                    && merge_len + line_len > merge_len_thresh) {
                    current_merge_tail = std::max(current_merge_tail, tails[i]);
                } else {
                    if (merge_len > merge_len_thresh) {
                        merge_heads.push_back(current_merge_head);
                        merge_tails.push_back(current_merge_tail);
                    }
                    current_merge_head = heads[i];
                    current_merge_tail = tails[i];
                }
            }

            if (current_merge_tail - current_merge_head > merge_len_thresh) {
                merge_heads.push_back(current_merge_head);
                merge_tails.push_back(current_merge_tail);
            }

            for (int k = 0; k < merge_heads.size(); k++) {
                cv::Point merge_head_pt(-1, -1);
                cv::Point merge_tail_pt(-1, -1);
                for (int i = 0; i < vec_lines.size(); i++) {
                    for (int j = 0; j < vec_lines[i].size(); j += 2) {
                        if (vec_lines[i][j] >= merge_heads[k] && vec_lines[i][j + 1] <= merge_tails[k]) {
                            if (vec_lines[i][j] == merge_heads[k] && merge_head_pt.x == -1) {
                                merge_head_pt.x = merge_heads[k];
                                merge_head_pt.y = n + i;
                            }
                            if (vec_lines[i][j + 1] == merge_tails[k] && merge_tail_pt.x == -1) {
                                merge_tail_pt.x = merge_tails[k] - 1;
                                merge_tail_pt.y = n + i;
                            }

                            alignment::set_array(image_data, width, cv::Point(vec_lines[i][j], n + i),
                                                 cv::Point(vec_lines[i][j + 1] - 1, n + i), 0);
                        }
                    }
                }
                alignment::set_array(image_data, width, merge_head_pt, merge_tail_pt, 255);
            }
        }
    }
}
