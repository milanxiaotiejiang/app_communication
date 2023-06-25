//
// Created by Looper on 2022/9/14.
//

#include "segmentation/CvUtils.h"
#include "BaseThrowable.h"

namespace CvUtils {
    double sideInLine(const cv::Point &ps, const cv::Point &pe, const cv::Point &p) {
        //Tmp = (y1 – y2) * x + (x2 – x1) * y + x1 * y2 – x2 * y1
        return (ps.y - pe.y) * p.x + (pe.x - ps.x) * p.y + ps.x * pe.y - pe.x * ps.y;
    }

    typedef struct {
        int rows;//行数
        int cols;//列数
        int type;//类型
    } MatHeader;

    bool write(const std::string &fileName, const cv::Mat &src) {
        if (fileName.empty()) {
            LOG(ERROR) << "文件名为空";
            return false;
        }
        if (src.empty()) {
            LOG(ERROR) << "mat为空";
            return false;
        }

        MatHeader matHeader{src.rows, src.cols, src.type()};

        std::ofstream out(fileName, std::ios::binary);
        if (!out.is_open()) {
            LOG(ERROR) << "打开文件" + fileName + "失败";
            throw app::exception(make_error_code(error::room_mb_file_open_fail));
        }

        //写入文件类型，表示为这个类型的数据，长度为两字节
        char fileType[3] = "mb";
        out.write(fileType, 2 * sizeof(char));

        //写入Mat头
        out.write((char *) &matHeader, sizeof(MatHeader));

        out.write((char *) src.data, src.rows * src.step[0]);

        out.flush();
        out.close();

        return true;
    }

    cv::Mat read(const std::string &fileName) {
        //打开文件
        std::ifstream in(fileName, std::ios::binary);
        if (!in.is_open()) {
            LOG(ERROR) << "打开文件" + fileName + "失败";
            throw app::exception(make_error_code(error::room_mb_file_open_fail));
        }

        char fileType[3] = {'\0', '\0', '\0'};//初始化一个默认值

        //读取前两字节
        in.read(fileType, 2 * sizeof(char));

        //判断是否是"mb"这种文件
        if (strcmp(fileType, "mb") != 0) {
            LOG(ERROR) << fileName + "非Mat类型的数据";
            throw app::exception(make_error_code(error::room_mb_file_read_fail));
        }

        //读取Mat头
        MatHeader matHeader{0, 0, 0};
        in.read((char *) &matHeader, sizeof(MatHeader));

        //初始化一个Mat
        cv::Mat mat(matHeader.rows, matHeader.cols, matHeader.type);

        //写入数据
        in.read((char *) mat.data, mat.rows * mat.step[0]);

        in.close();
        return mat;
    }

    bool savePng(const std::string &fileName, const cv::Mat &src) {
        std::vector<int> compression_params;  //无损压缩参数
        compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
        compression_params.push_back(0);// 无压缩png.
        compression_params.push_back(cv::IMWRITE_PNG_STRATEGY);
        compression_params.push_back(cv::IMWRITE_PNG_STRATEGY_DEFAULT);
        cv::Mat_<float> color_depth;
        double min;
        double max;
        cv::minMaxIdx(src, &min, &max);
        cv::Mat adjMap;
        float scale = 255 / (max - min);
        src.convertTo(adjMap, CV_8UC1, scale, -min * scale);  // beta = -min *scale
        cv::Mat falseColorsMap;
        cv::applyColorMap(adjMap, falseColorsMap, cv::COLORMAP_JET);
        cv::imwrite(fileName, falseColorsMap, compression_params);

        LOG_IF(INFO, DEBUG_SEGMENTATION) << "SegmentationSubscribe " << fileName;

        return true;
    }

    bool savePgm(const std::string &fileName, const cv::Mat &src) {
        cv::imwrite(fileName, src);
        return true;
    }

}