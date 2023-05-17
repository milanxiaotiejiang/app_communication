//
// Created by Looper on 2023/5/17.
//

#ifndef APP_COMMUNICATION_RRMAP_H
#define APP_COMMUNICATION_RRMAP_H

#include <cstdint>
#include "climits"
#include "vector"

struct RRPoint {
    int16_t x;
    int16_t y;
};

struct RRLine {
    RRPoint pointStart;
    RRPoint pointEnd;
};

struct RRZone {
    RRPoint p0;
    RRPoint p1;
    RRPoint p2;
    RRPoint p3;
};

struct RRBlock {
    // 8
    int16_t blockType;
    int16_t blockHeaderLength;
    int32_t blockDataLength;
};

struct RRMapSize {
    // 28 + 166934 = 116962
    RRBlock rrBlock;            //  blockType = 2  blockHeaderLength = 28  blockDataLength = 116934
    int32_t unknown;            //
    int32_t top;                // 337
    int32_t left;               // 320
    int32_t imgHeight;          // 437
    int32_t imgWidth;           // 382
};

struct RRMapCharger {
    // 8 + 12 = 20
    RRBlock rrBlock;            // blockType = 1  blockHeaderLength = 8  blockDataLength = 12
    int32_t chargerX;           // 25616
    int32_t chargerY;           // 25201
};

struct RRMapRobot {
    //  8 + 12 = 20
    RRBlock rrBlock;            // blockType = 8  blockHeaderLength = 8  blockDataLength = 12
    int32_t robotX;             // 25607
    int32_t robotY;             // 25379
    int32_t robotA;             // -87
};

struct RRMapPath {
    // 20 + 616 = 636
    RRBlock rrBlock;            // blockType = 3  blockHeaderLength = 20  blockDataLength = 616
    // head
    int32_t pairs;              // 616
    int32_t pointLength;        // 154
    int32_t pointSize;          // 4
    int32_t angle;              // 0
    // data
    std::vector<RRPoint> points;// 共 154 个，即 pairs/4
};

struct RRMapArea {
    // 12 + 0 = 12
    RRBlock rrBlock;            // blockType = 9  blockHeaderLength = 12  blockDataLength = 0
    int32_t number;             // 0
    // data
    RRZone zones;
};

struct RRMapWall {
    // 12 + 72 = 84
    RRBlock rrBlock;            // blockType = 10  blockHeaderLength = 12  blockDataLength = 72
    int32_t number;             // 9
    // data
    std::vector<RRLine> walls;
};

struct RRMapZone {
    // 12 + 0 = 12
    RRBlock rrBlock;            // blockType = 12  blockHeaderLength = 12  blockDataLength = 0
    int32_t number;             // 0
    // data
    std::vector<RRZone> zones;
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
    RRMapSize rrMapSize;

    // 167002 （166982 + 20）
    RRMapCharger rrMapCharger;

    // 167022 （167002 + 20）
    RRMapRobot rrMapRobot;

    // 167658 （167022 + 636）
    RRMapPath rrMapPath;

    // 167670 (167658 + 12)
    RRMapArea rrMapArea;

    // 167754 (167670 + 84)
    RRMapWall rrMapWall;

    // 167766 (167754 + 12)
    RRMapZone rrMapZone;

    // 167806 (167766 + 40)
    RRMapUnknown1 rrMapUnknown1;

    // 167842 (167806 + 36)
    RRMapUnknown2 rrMapUnknown2;

    // 167870 (167842 + 28)
    RRMap1024 rrMap1024;
};


#endif //APP_COMMUNICATION_RRMAP_H
