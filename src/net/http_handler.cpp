//
// Created by io on 23-11-7.
//

#include "net/http_handler.h"
#include <opencv2/opencv.hpp>
#include "cppfs/fs.h"
#include "cppfs/FileHandle.h"
#include "cppfs/FilePath.h"
#include "db/path.h"
#include "alignment/rrmap2.h"
#include "tool/base64.h"

BaseResult<MapImageResponse> HttpHandler::imageProgressing(const std::string &map_id) {

    std::string mapPath = path::robot_slam_map_dir() + map_id + path::separator() + path::mymap_pgm;
    auto mapPgmPath = cppfs::FilePath(mapPath);
    if (!cppfs::fs::open(mapPgmPath.fullPath()).exists())
        return BaseResult<MapImageResponse>(0, 1, "文件不存在");

    cv::Mat map = cv::imread(mapPgmPath.fullPath(), cv::ImreadModes::IMREAD_GRAYSCALE);
    cv::normalize(map, map, 0, 255, cv::NORM_MINMAX);
//    map.convertTo(map, CV_8U);

    cv::rotate(map, map, cv::RotateFlags::ROTATE_90_COUNTERCLOCKWISE);

    //map中只包含 0 / 255
    for (int y = 0; y < map.rows; y++) {
        for (int x = 0; x < map.cols; x++) {
            if (map.at<unsigned char>(y, x) < 254) {
                map.at<unsigned char>(y, x) = 0;
            } else {
                map.at<unsigned char>(y, x) = 255;
            }
        }
    }

    auto cols = map.cols;//width
    auto rows = map.rows;//height

    std::vector<int8_t> mapArray;
    for (int y = 0; y < map.rows; y++) {
        for (int x = 0; x < map.cols; x++) {
            if (map.at<unsigned char>(y, x) == 255) {
                MMapExtend::writeByteToByteArray(mapArray, 15);
            } else {
                MMapExtend::writeByteToByteArray(mapArray, 0);
            }
        }
    }

    std::stringstream input;
    for (int8_t b: mapArray) {
        input << b;
    }

    std::stringstream compressed;
    boost::iostreams::filtering_streambuf<boost::iostreams::input> outbuf;
    outbuf.push(boost::iostreams::gzip_compressor());
    outbuf.push(input);
    boost::iostreams::copy(outbuf, compressed);

    std::string compressedString = compressed.str();
    const std::string &base64Encode = base64_encode(compressedString);

    MapImageResponse imageResponse;
    imageResponse.image = base64Encode;
    imageResponse.width = cols;
    imageResponse.height = rows;

    return BaseResult<MapImageResponse>(0, imageResponse);
}
