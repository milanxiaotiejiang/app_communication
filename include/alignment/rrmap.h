//
// Created by Looper on 2023/5/17.
//

#ifndef APP_COMMUNICATION_RRMAP_H
#define APP_COMMUNICATION_RRMAP_H

#include <cstdint>
#include "climits"
#include "vector"
#include "stdexcept"
#include "boost/iostreams/filtering_streambuf.hpp"
#include "boost/iostreams/copy.hpp"
#include "boost/iostreams/filter/gzip.hpp"
#include "segmentation/SegmentationCenter.h"
#include "segmentation/map_attribute.h"

struct MPoint {
    int16_t x;
    int16_t y;
};

struct MLine {
    MPoint pointStart;
    MPoint pointEnd;
};

struct MZone {
    MPoint p0;
    MPoint p1;
    MPoint p2;
    MPoint p3;
};

struct RRBlock {
    // 8
    int16_t blockType;
    int16_t blockHeaderLength;
    int32_t blockDataLength;
};

struct MMapResource {
    // 28 + 166934 = 116962
    RRBlock rrBlock;            //  blockType = 2  blockHeaderLength = 28  blockDataLength = 116934
    int32_t unknown;            //
    int32_t top;                // 337
    int32_t left;               // 320
    int32_t imgHeight;          // 437
    int32_t imgWidth;           // 382
};

struct MMapCharger {
    // 8 + 12 = 20
    RRBlock rrBlock;            // blockType = 1  blockHeaderLength = 8  blockDataLength = 12
    int32_t chargerX;           // 25616
    int32_t chargerY;           // 25201
    int32_t chargerA;
};

struct MMapRobot {
    //  8 + 12 = 20
    RRBlock rrBlock;            // blockType = 8  blockHeaderLength = 8  blockDataLength = 12
    int32_t robotX;             // 25607
    int32_t robotY;             // 25379
    int32_t robotA;             // -87
};

struct MMapPath {
    // 20 + 616 = 636
    RRBlock rrBlock;            // blockType = 3  blockHeaderLength = 20  blockDataLength = 616
    // head
    int32_t pairs;              // 616
    int32_t pointLength;        // 154
    int32_t pointSize;          // 4
    int32_t angle;              // 0
    // data
    std::vector<MPoint> points;// 共 154 个，即 pairs/4
};

struct MMapProhibition {
    // 12 + 0 = 12
    RRBlock rrBlock;            // blockType = 9  blockHeaderLength = 12  blockDataLength = 0
    int32_t number;             // 0
    // data
    std::vector<MZone> zones;
};

struct MMapVirtually {
    // 12 + 72 = 84
    RRBlock rrBlock;            // blockType = 10  blockHeaderLength = 12  blockDataLength = 72
    int32_t number;             // 9
    // data
    std::vector<MLine> virtuallys;
};

struct MMapZone {
    // 12 + 0 = 12
    RRBlock rrBlock;            // blockType = 12  blockHeaderLength = 12  blockDataLength = 0
    int32_t number;             // 0
    // data
    std::vector<MZone> zones;
};

struct RRMapUnknown1 {
    // 12 + 28 = 40
    RRBlock rrBlock;            // blockType = 15  blockHeaderLength = 12  blockDataLength = 28
};

struct RRMapUnknown2 {
    // 12 + 24 = 36
    RRBlock rrBlock;            // blockType = 15  blockHeaderLength = 12  blockDataLength = 24
};

struct RRMap1024 {
    // 8 + 20 = 28
    RRBlock rrBlock;            // blockType = 1024  blockHeaderLength = 8  blockDataLength = 20
};

struct RRMap {
    // head 20
    int8_t default_0;           // 1 114
    int8_t default_1;           // 1 114
    int16_t mapHeaderLength;    // 2 20 (default_0 + default_1 + mapHeaderLength + mapDataLength + majorVersion + minorVersion + mapIndex + mapSequence = 20)
    int32_t mapDataLength;      // 4
    int16_t majorVersion;       // 2 1
    int16_t minorVersion;       // 2 0
    int32_t mapIndex;           // 4 132
    int32_t mapSequence;        // 4 2289

    // 166982 （20 + 116962）
    MMapResource rrMapSize;

    // 167002 （166982 + 20）
    MMapCharger rrMapCharger;

    // 167022 （167002 + 20）
    MMapRobot rrMapRobot;

    // 167658 （167022 + 636）
    MMapPath rrMapPath;

    // 167670 (167658 + 12)
    MMapProhibition rrMapArea;

    // 167754 (167670 + 84)
    MMapVirtually rrMapWall;

    // 167766 (167754 + 12)
    MMapZone rrMapZone;

    // 167806 (167766 + 40)
    RRMapUnknown1 rrMapUnknown1;

    // 167842 (167806 + 36)
    RRMapUnknown2 rrMapUnknown2;

    // 167870 (167842 + 28)
    RRMap1024 rrMap1024;

    static int8_t convert8(int value) {
        if (value >= INT8_MIN && value <= INT8_MAX) {
            return static_cast<int8_t>(value);
        } else {
            throw std::out_of_range("The value is out of range of int8_t .");
        }
    }

    static int16_t convert16(int value) {
        if (value >= INT16_MIN && value <= INT16_MAX) {
            return static_cast<int16_t>(value);
        } else {
            throw std::out_of_range("The value is out of range of int16_t .");
        }
    }

//    static void push8(std::vector<int8_t> &byteArray, int8_t value) {
//        byteArray.push_back(value);
//    }
//
//    static void push16(std::vector<int8_t> &byteArray, int16_t value) {
//        const vector<int8_t> &vector = convertShort(value);
//        for (const auto &item: vector) {
//            byteArray.push_back(item);
//        }
//    }
//
//    static void push32(std::vector<int8_t> &byteArray, int32_t value) {
//        const vector<int8_t> &vector = convertInt(value);
//        for (const auto &item: vector) {
//            byteArray.push_back(item);
//        }
//    }

    static void p4(std::vector<int8_t> &byteArray, int value) {
        byteArray.push_back(value & 0xff);
        byteArray.push_back((value >> 8) & 0xff);
        byteArray.push_back((value >> 16) & 0xff);
        byteArray.push_back((value >> 24) & 0xff);
    }

    static void p2(std::vector<int8_t> &byteArray, int value) {
        byteArray.push_back(value & 0xff);
        byteArray.push_back((value >> 8) & 0xff);
    }

    static void p1(std::vector<int8_t> &byteArray, int value) {
        byteArray.push_back(value & 0xff);
    }

    static void v() {

        std::vector<int8_t> byteArray;

        const cv::Mat &map = SegmentationCenter::instance().generateMat();
        int rows = map.rows;//387
        int cols = map.cols;//236
        cv::Point2d map_origin = MapAttributeSingleton::instance().getMapOrigin();
        cv::Point robotPosition = MapAttributeSingleton::instance().getRobotPositionPoint(map);


        int default_0 = 114;
        int default_1 = 114;
        int mapHeaderLength = sizeof(RRMap::default_0) + sizeof(RRMap::default_1) +
                              sizeof(RRMap::mapHeaderLength) + sizeof(RRMap::mapDataLength) +
                              sizeof(RRMap::majorVersion) + sizeof(RRMap::minorVersion) +
                              sizeof(RRMap::mapIndex) + sizeof(RRMap::mapSequence);
        int mapDataLength = 0;//total - 20
        int majorVersion = 1;
        int minorVersion = 0;
        int mapIndex = 132;
        int mapSequence = 2289;

        p1(byteArray, default_0);
        p1(byteArray, default_1);
        p2(byteArray, mapHeaderLength);
        p4(byteArray, mapDataLength);
        p2(byteArray, majorVersion);
        p2(byteArray, minorVersion);
        p4(byteArray, mapIndex);
        p4(byteArray, mapSequence);

        //RRMapSize
        int mapSizeType = 2;
        int mapSizeHeaderLength = sizeof(RRBlock::blockType) + sizeof(RRBlock::blockHeaderLength) +
                                  sizeof(RRBlock::blockDataLength) + sizeof(MMapResource::unknown) +
                                  sizeof(MMapResource::top) + sizeof(MMapResource::left) +
                                  sizeof(MMapResource::imgHeight) + sizeof(MMapResource::imgWidth);
        int mapSizeDataLength = rows * cols;
        int unknown = 0;
        int top = 337;
        int left = 320;
        int imgHeight = rows;
        int imgWidth = cols;

        p2(byteArray, mapSizeType);
        p2(byteArray, mapSizeHeaderLength);
        p4(byteArray, mapSizeDataLength);
        p4(byteArray, unknown);
        p4(byteArray, top);
        p4(byteArray, left);
        p4(byteArray, imgHeight);
        p4(byteArray, imgWidth);


        for (int y = 0; y < map.rows; y++) {
            for (int x = 0; x < map.cols; x++) {
                if (map.at<unsigned char>(y, x) == 255) {
                    p1(byteArray, 15);
                } else {
                    p1(byteArray, 0);
                }
            }
        }

        //RRMapCharger
        int chargerType = 1;
        int chargerHeaderLength = sizeof(RRBlock::blockType) + sizeof(RRBlock::blockHeaderLength) +
                                  sizeof(RRBlock::blockDataLength);
        int chargerDataLength = sizeof(MMapCharger::chargerX) + sizeof(MMapCharger::chargerY) +
                                sizeof(MMapCharger::chargerA);
        int chargerX = map_origin.x * 50;
        int chargerY = map_origin.y * 50;
        int chargerA = 0;

        p2(byteArray, chargerType);
        p2(byteArray, chargerHeaderLength);
        p4(byteArray, chargerDataLength);
        p4(byteArray, chargerX);
        p4(byteArray, chargerY);
        p4(byteArray, chargerA);

        //RRMapRobot
        int robotType = 8;
        int robotHeaderLength = sizeof(RRBlock::blockType) + sizeof(RRBlock::blockHeaderLength) +
                                sizeof(RRBlock::blockDataLength);
        int robotDataLength = sizeof(MMapRobot::robotX) + sizeof(MMapRobot::robotY) + sizeof(MMapRobot::robotA);
        int32_t robotX = robotPosition.x * 50;
        int32_t robotY = robotPosition.y * 50;
        int32_t robotA = -87;

        p2(byteArray, robotType);
        p2(byteArray, robotHeaderLength);
        p4(byteArray, robotDataLength);
        p4(byteArray, robotX);
        p4(byteArray, robotY);
        p4(byteArray, robotA);

        //RRMapPath
        int rrPointSize = sizeof(MPoint);
        std::vector<MPoint> points;
        int pathType = 3;
        int pathHeaderLength = sizeof(RRBlock::blockType) + sizeof(RRBlock::blockHeaderLength) +
                               sizeof(RRBlock::blockDataLength) + sizeof(MMapPath::pairs) +
                               sizeof(MMapPath::pointLength) + sizeof(MMapPath::pointSize) +
                               sizeof(MMapPath::angle);
        int pathDataLength = points.size() * rrPointSize;
        int32_t pairs = 0;
        int32_t pointLength = 0;
        int32_t pointSize = rrPointSize;
        int32_t angle = 0;

        p2(byteArray, pathType);
        p2(byteArray, pathHeaderLength);
        p4(byteArray, pathDataLength);
        p4(byteArray, pairs);
        p4(byteArray, pointLength);
        p4(byteArray, pointSize);
        p4(byteArray, angle);
        for (const auto &item: points) {
            p2(byteArray, item.x);
            p2(byteArray, item.y);
        }

        //RRMapArea
        int rrZoneSize = sizeof(MZone);
        std::vector<MZone> zones;
        int areaType = 9;
        int areaHeaderLength = sizeof(RRBlock::blockType) + sizeof(RRBlock::blockHeaderLength) +
                               sizeof(RRBlock::blockDataLength) + sizeof(MMapProhibition::number);
        int areaDataLength = zones.size() * rrZoneSize;
        int areaNumber = zones.size();

        p2(byteArray, areaType);
        p2(byteArray, areaHeaderLength);
        p4(byteArray, areaDataLength);
        p4(byteArray, areaNumber);
        for (const auto &item: zones) {
            p2(byteArray, item.p0.x);
            p2(byteArray, item.p0.y);
            p2(byteArray, item.p1.x);
            p2(byteArray, item.p1.y);
            p2(byteArray, item.p2.x);
            p2(byteArray, item.p2.y);
            p2(byteArray, item.p3.x);
            p2(byteArray, item.p3.y);
        }

        //RRMapWall
        int rrLineSize = sizeof(MLine);
        std::vector<MLine> walls;
        int wallType = 10;
        int wallHeaderLength = sizeof(RRBlock::blockType) + sizeof(RRBlock::blockHeaderLength) +
                               sizeof(RRBlock::blockDataLength) + sizeof(MMapVirtually::number);
        int wallDataLength = walls.size() * rrLineSize;
        int wallNumber = walls.size();

        p2(byteArray, wallType);
        p2(byteArray, wallHeaderLength);
        p4(byteArray, wallDataLength);
        p4(byteArray, wallNumber);
        for (const auto &item: walls) {
            p2(byteArray, item.pointStart.x);
            p2(byteArray, item.pointStart.y);
            p2(byteArray, item.pointEnd.x);
            p2(byteArray, item.pointEnd.y);
        }

        //RRMapZone
        int rrZoneSize2 = sizeof(MZone);
        std::vector<MZone> zone2s;
        int zoneType = 12;
        int zoneHeaderLength = sizeof(RRBlock::blockType) + sizeof(RRBlock::blockHeaderLength) +
                               sizeof(RRBlock::blockDataLength) + sizeof(MMapZone::number);
        int zoneDataLength = zone2s.size() * rrZoneSize2;
        int zoneNumber = zone2s.size();

        p2(byteArray, zoneType);
        p2(byteArray, zoneHeaderLength);
        p4(byteArray, zoneDataLength);
        p4(byteArray, zoneNumber);
        for (const auto &item: zone2s) {
            p2(byteArray, item.p0.x);
            p2(byteArray, item.p0.y);
            p2(byteArray, item.p1.x);
            p2(byteArray, item.p1.y);
            p2(byteArray, item.p2.x);
            p2(byteArray, item.p2.y);
            p2(byteArray, item.p3.x);
            p2(byteArray, item.p3.y);
        }

        //RRMap1024
        int validType = 1024;
        int validHeaderLength = sizeof(RRBlock::blockType) + sizeof(RRBlock::blockHeaderLength) +
                                sizeof(RRBlock::blockDataLength) + sizeof(MMapZone::number);
        int validDataLength = 20;

        p2(byteArray, validType);
        p2(byteArray, validHeaderLength);
        p4(byteArray, validDataLength);
        for (int i = 0; i < 20; i++) {
            p1(byteArray, i);
        }


        std::vector<int8_t> data;
        mapDataLength = byteArray.size() - mapHeaderLength;
        p4(data, mapDataLength);
        for (int i = 0; i < data.size(); i++) {
            byteArray[4 + i] = data[i];
        }

        std::stringstream input;
        for (int8_t b: byteArray) {
            input << b;
        }

        std::stringstream compressed;
        boost::iostreams::filtering_streambuf<boost::iostreams::input> outbuf;
        outbuf.push(boost::iostreams::gzip_compressor());
        outbuf.push(input);
        boost::iostreams::copy(outbuf, compressed);

        std::string compressedString = compressed.str();


        //save file
//        std::ofstream file("test.gz", std::ios_base::out | std::ios_base::binary);
//        boost::iostreams::filtering_streambuf<boost::iostreams::output> outbuf;
//
//        outbuf.push(boost::iostreams::gzip_compressor());
//        outbuf.push(file);
//
//        boost::iostreams::copy(input, outbuf);
//
//        boost::iostreams::close(outbuf);
//        file.close();

        std::cout << compressedString.size() << std::endl;
    };
};


#endif //APP_COMMUNICATION_RRMAP_H
