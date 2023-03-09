//
// Created by Looper on 2023/3/8.
//

#ifndef APP_COMMUNICATION_BLOB_H
#define APP_COMMUNICATION_BLOB_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <ros/node_handle.h>
#include <std_msgs/String.h>
#include "ros/ros.h"
#include "glog/logging.h"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

/**
 rostopic pub -1 /tt_blob std_msgs/String "data: '{\"blobColor\":0,\"filterByArea\":true,\"filterByCircularity\":true,\"filterByColor\":true,\"filterByConvexity\":true,\"filterByInertia\":true,\"maxArea\":5000.0,\"maxCircularity\":3.140000104904175,\"maxConvexity\":3.140000104904175,\"maxInertiaRatio\":0.800000011920929,\"maxThreshold\":256.0,\"minArea\":50.0,\"minCircularity\":0.5,\"minConvexity\":0.5,\"minDistBetweenBlobs\":10.0,\"minInertiaRatio\":0.30000001192092896,\"minRepeatability\":2,\"minThreshold\":0.0,\"thresholdStep\":1.0}'"
 */
class BlobParams {
public:
    float thresholdStep{1};
    float minThreshold{0};
    float maxThreshold{256};
    size_t minRepeatability{2};
    float minDistBetweenBlobs{10};

    bool filterByColor{true};
    uchar blobColor{0};

    bool filterByArea{true};
    float minArea{50};
    float maxArea{5000};

    bool filterByCircularity{true};
    float minCircularity{0.5};
    float maxCircularity{3.14};

    bool filterByConvexity{true};
    float minConvexity{0.5};
    float maxConvexity{3.14};

    bool filterByInertia{true};
    float minInertiaRatio{0.3};
    float maxInertiaRatio{0.8};

    friend void to_json(json &j, const BlobParams &blobParams) {
        j = json{
                {"thresholdStep",       blobParams.thresholdStep},
                {"minThreshold",        blobParams.minThreshold},
                {"maxThreshold",        blobParams.maxThreshold},
                {"minRepeatability",    blobParams.minRepeatability},
                {"minDistBetweenBlobs", blobParams.minDistBetweenBlobs},
                {"filterByColor",       blobParams.filterByColor},
                {"blobColor",           blobParams.blobColor},
                {"filterByArea",        blobParams.filterByArea},
                {"minArea",             blobParams.minArea},
                {"maxArea",             blobParams.maxArea},
                {"filterByCircularity", blobParams.filterByCircularity},
                {"minCircularity",      blobParams.minCircularity},
                {"maxCircularity",      blobParams.maxCircularity},
                {"filterByInertia",     blobParams.filterByInertia},
                {"minInertiaRatio",     blobParams.minInertiaRatio},
                {"maxInertiaRatio",     blobParams.maxInertiaRatio},
                {"filterByConvexity",   blobParams.filterByConvexity},
                {"minConvexity",        blobParams.minConvexity},
                {"maxConvexity",        blobParams.maxConvexity}
        };
    }

    friend void from_json(const json &j, BlobParams &blobParams) {
        j.at("thresholdStep").get_to(blobParams.thresholdStep);
        j.at("minThreshold").get_to(blobParams.minThreshold);
        j.at("maxThreshold").get_to(blobParams.maxThreshold);
        j.at("minRepeatability").get_to(blobParams.minRepeatability);
        j.at("minDistBetweenBlobs").get_to(blobParams.minDistBetweenBlobs);
        j.at("filterByColor").get_to(blobParams.filterByColor);
        j.at("blobColor").get_to(blobParams.blobColor);
        j.at("filterByArea").get_to(blobParams.filterByArea);
        j.at("minArea").get_to(blobParams.minArea);
        j.at("maxArea").get_to(blobParams.maxArea);
        j.at("filterByCircularity").get_to(blobParams.filterByCircularity);
        j.at("minCircularity").get_to(blobParams.minCircularity);
        j.at("maxCircularity").get_to(blobParams.maxCircularity);
        j.at("filterByInertia").get_to(blobParams.filterByInertia);
        j.at("minInertiaRatio").get_to(blobParams.minInertiaRatio);
        j.at("maxInertiaRatio").get_to(blobParams.maxInertiaRatio);
        j.at("filterByConvexity").get_to(blobParams.filterByConvexity);
        j.at("minConvexity").get_to(blobParams.minConvexity);
        j.at("maxConvexity").get_to(blobParams.maxConvexity);
    }
};

class Blob {
private:
    ros::NodeHandle handle;
    ros::Subscriber sub_;

    static void subscribeCallback(const std_msgs::String &result) {
        std::string string = result.data;
        json jDecode = json::parse(string);
        auto blob = jDecode.get<BlobParams>();
        detect(blob);
    };

    static void detect(BlobParams blob) {
        auto src = cv::imread("/home/lijiang/Desktop/zt/004.png");
        cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);

        cv::SimpleBlobDetector::Params params;
//        params.thresholdStep = blob.thresholdStep;
        params.minThreshold = blob.minThreshold;
        params.maxThreshold = blob.maxThreshold;

//        params.minRepeatability = blob.minRepeatability;
//        params.minDistBetweenBlobs = blob.minDistBetweenBlobs;

        //颜色 设置blobColor=0选择颜色更暗的斑点，blobColor=255选择颜色更浅的斑点
        params.filterByColor = blob.filterByColor;
        params.blobColor = blob.blobColor;
        //面积 设置minArea=100将过滤掉所有小于100像素的斑点
        params.filterByArea = blob.filterByArea;
        params.minArea = blob.minArea;
        params.maxArea = blob.maxArea;
        //形状（凸）
        params.filterByCircularity = blob.filterByCircularity;
        params.minCircularity = blob.minCircularity;
//        params.maxCircularity = blob.maxCircularity;
        //形状（凸）
        params.filterByConvexity = blob.filterByConvexity;
        params.minConvexity = blob.minConvexity;
//        params.maxConvexity = blob.maxConvexity;
        //凸度 对于一个圆该值是1，对于椭圆它是0和1之间，而对于线段为0。
        params.filterByInertia = blob.filterByInertia;
        params.minInertiaRatio = blob.minInertiaRatio;
        params.maxInertiaRatio = blob.maxInertiaRatio;

        cv::Ptr<cv::SimpleBlobDetector> simpleBlobDetector = cv::SimpleBlobDetector::create(params);
        std::vector<cv::KeyPoint> key_points;
        simpleBlobDetector->detect(src, key_points);
        cv::Mat outImg;
        //绘制结果
        cv::drawKeypoints(src, key_points, outImg, cv::Scalar(0, 0, 255));
        for (const auto &item: key_points) {
            std::cout << " x : " << item.pt.x
                      << ", y : " << item.pt.y
                      << ", octave : " << item.octave
                      << ", response : " << item.response
                      << ", size : " << item.size
                      << ", angle : " << item.angle
                      << std::endl;
            cv::putText(outImg, std::to_string(item.size), item.pt, cv::FONT_HERSHEY_TRIPLEX,
                        0.5, cv::Scalar(128), 1, CV_AA);
        }
        cv::imshow("blob", outImg);
        cv::waitKey();
    }

public:
    Blob(ros::NodeHandle handle) {

        BlobParams blob;
        json jsonResult = blob;
//        LOG(INFO) << jsonResult.dump();
        sub_ = handle.subscribe("tt_blob", 3, subscribeCallback);
    }
};

#endif //APP_COMMUNICATION_BLOB_H
