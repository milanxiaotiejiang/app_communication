//
// Created by Looper on 2023/5/18.
//

#ifndef APP_COMMUNICATION_RRMAP2_H
#define APP_COMMUNICATION_RRMAP2_H

#include <cstdint>
#include "climits"
#include "vector"
#include "stdexcept"
#include "boost/iostreams/filtering_streambuf.hpp"
#include "boost/iostreams/copy.hpp"
#include "boost/iostreams/filter/gzip.hpp"
#include "segmentation/SegmentationCenter.h"
#include "segmentation/map_attribute.h"
#include "db/task_data_base.h"
#include "thread"
#include "db/segmentation_data_base.h"
#include "exploration/ExplorationCenter.h"
#include "cppfs/FileHandle.h"
#include <type_traits>

#include <opencv2/opencv.hpp>

#define SIZE_OF_HEAD_TYPE 2
#define SIZE_OF_HEAD_LENGTH 2
#define SIZE_OF_DATA_LENGTH 4
#define NUMERICAL_EXPANSION 50
#define VALID_DEFAULT_LENGTH 20

enum MMapType {
    M_MAP_CHARGER = 1,
    M_MAP_RESOURCE = 2,
    M_MAP_PATH = 3,
    M_MAP_TARGET = 7,
    M_MAP_ROBOT = 8,
    M_MAP_PROHIBITION = 9,
    M_MAP_VIRTUALLY = 10,
    M_MAP_ZONE = 12,
    M_MAP_COVER = 14,
    M_MAP_VALID = 1024,
    M_MAP_OUTER = 29298
};

class MMapObject;

class MMapExtend {
public:
    static void writeIntToByteArray(std::vector<int8_t> &byteArray, int value) {
        byteArray.push_back(value & 0xff);
        byteArray.push_back((value >> 8) & 0xff);
        byteArray.push_back((value >> 16) & 0xff);
        byteArray.push_back((value >> 24) & 0xff);
    }

    static void writeShortToByteArray(std::vector<int8_t> &byteArray, int value) {
        byteArray.push_back(value & 0xff);
        byteArray.push_back((value >> 8) & 0xff);
    }

    static void writeByteToByteArray(std::vector<int8_t> &byteArray, int value) {
        byteArray.push_back(value & 0xff);
    }

    static int readIntFromByteArray(const std::vector<int8_t> &byteArray, int startIndex) {
        return ((byteArray[startIndex + 3] & 0xff) << 24) |
               ((byteArray[startIndex + 2] & 0xff) << 16) |
               ((byteArray[startIndex + 1] & 0xff) << 8) |
               (byteArray[startIndex] & 0xff);
    }

    static int readShortFromByteArray(const std::vector<int8_t> &byteArray, int startIndex) {
        return ((byteArray[startIndex + 1] & 0xff) << 8) |
               (byteArray[startIndex] & 0xff);
    }

    static int readByteFromByteArray(const std::vector<int8_t> &byteArray, int startIndex) {
        return byteArray[startIndex] & 0xff;
    }

    static std::vector<int8_t> generateIntToByteArray(const std::vector<int> &values) {
        std::vector<int8_t> byteArray;
        for (const auto &value: values) {
            writeIntToByteArray(byteArray, value);
        }
        return byteArray;
    }

    static std::vector<int8_t> generateShortToByteArray(const std::vector<int> &values) {
        std::vector<int8_t> byteArray;
        for (const auto &value: values) {
            writeShortToByteArray(byteArray, value);
        }
        return byteArray;
    }

    static void appendToByteArray(std::vector<int8_t> &byteArray, const std::vector<int8_t> &data) {
        byteArray.insert(byteArray.end(), data.begin(), data.end());
    }

    static void updateByteArray(std::vector<int8_t> &byteArray, const std::vector<int8_t> &data, int start) {
        std::copy(data.begin(), data.end(), byteArray.begin() + start);
    }

};

class MMapObject {
public:
    virtual ~MMapObject() = default;

    virtual std::vector<int8_t> toByteArray() const = 0;

    template<typename T>
    std::vector<int8_t> objectsToByteArray(const std::vector<T> &mmapObjects) const {
        static_assert(std::is_base_of<MMapObject, T>::value, "T must inherit from MMapObject");
        std::vector<int8_t> result;
        for (const auto &mmapObject: mmapObjects) {
            MMapExtend::appendToByteArray(result, mmapObject.toByteArray());
        }
        return result;
    }
};

class MMapHead : public MMapObject {
private:
    MMapType type;
    int headerLength{};
    int dataLength{};
    std::vector<int8_t> additionalHeader;

protected:
    int getHeaderLength() const {
        return headerLength;
    }

    void setHeaderLength(int headerLength) {
        MMapHead::headerLength = headerLength;
    }

    void setDataLength(int dataLength) {
        MMapHead::dataLength = dataLength;
    }

    void setAdditionalHeader(const std::vector<int8_t> &additionalHeader) {
        MMapHead::additionalHeader = additionalHeader;
    }

    int buildHeaderLength() {
        return SIZE_OF_HEAD_TYPE + SIZE_OF_HEAD_LENGTH + SIZE_OF_DATA_LENGTH + additionalHeader.size();
    }

    void writeHeadToByteArray(std::vector<int8_t> &byteArray) const {
        MMapExtend::writeShortToByteArray(byteArray, static_cast<int>(type));
        MMapExtend::writeShortToByteArray(byteArray, headerLength);
        MMapExtend::writeIntToByteArray(byteArray, dataLength);
        byteArray.insert(byteArray.end(), additionalHeader.begin(), additionalHeader.end());
    }

    void readHeadToByteArray(const std::vector<int8_t> &byteArray) {
        type = static_cast<MMapType>(MMapExtend::readShortFromByteArray(byteArray, 0));
        headerLength = MMapExtend::readShortFromByteArray(byteArray, SIZE_OF_HEAD_TYPE);
        dataLength = MMapExtend::readIntFromByteArray(byteArray, SIZE_OF_HEAD_TYPE + SIZE_OF_HEAD_LENGTH);
        additionalHeader = std::vector<int8_t>(
                byteArray.begin() + SIZE_OF_HEAD_TYPE + SIZE_OF_HEAD_LENGTH + SIZE_OF_DATA_LENGTH, byteArray.end()
        );
    }

public:
    explicit MMapHead(MMapType type) : type(type) {}

};

class MPoint : public MMapObject {
private:
    int32_t x;
    int32_t y;
public:
    ~MPoint() override = default;

    MPoint(int32_t x, int32_t y) : x(x), y(y) {}

    static int toSize() {
        return (sizeof(x) + sizeof(y)) / 2;
    }

    std::vector<int8_t> toByteArray() const override {
        return MMapExtend::generateShortToByteArray(std::vector<int>{x * NUMERICAL_EXPANSION, y * NUMERICAL_EXPANSION});
    }

    int32_t getX() const {
        return x / NUMERICAL_EXPANSION;
    }

    int32_t getY() const {
        return y / NUMERICAL_EXPANSION;
    }

    cv::Point toCvPoint() const {
        return cv::Point(getX(), getY());
    }
};

class MLine : public MMapObject {
private:
    MPoint pointStart;
    MPoint pointEnd;
public:
    ~MLine() override = default;

    MLine(const MPoint &pointStart, const MPoint &pointEnd) : pointStart(pointStart), pointEnd(pointEnd) {}

    static int toSize() {
        return MPoint::toSize() * 2;
    }

    std::vector<int8_t> toByteArray() const override {
        return objectsToByteArray(std::vector<MPoint>{pointStart, pointEnd});
    }
};

class MZone : public MMapObject {
private:
    MPoint p0;
    MPoint p1;
    MPoint p2;
    MPoint p3;
public:
    ~MZone() override = default;

    MZone(const MPoint &p0, const MPoint &p1, const MPoint &p2, const MPoint &p3) : p0(p0), p1(p1), p2(p2), p3(p3) {}

    static int toSize() {
        return MPoint::toSize() * 4;
    }

    std::vector<int8_t> toByteArray() const override {
        return objectsToByteArray(std::vector<MPoint>{p0, p1, p2, p3});
    }

    const MPoint &getP0() const {
        return p0;
    }

    const MPoint &getP1() const {
        return p1;
    }

    const MPoint &getP2() const {
        return p2;
    }

    const MPoint &getP3() const {
        return p3;
    }

};

class MMapResource : public MMapHead {
private:
    int32_t unknown = 0;
    int32_t top = 0;
    int32_t left = 0;
    int32_t imgHeight;
    int32_t imgWidth;

    std::vector<int8_t> mapArray;
public:
    ~MMapResource() override = default;

    MMapResource(int32_t imgHeight, int32_t imgWidth) : MMapHead(MMapType::M_MAP_RESOURCE),
                                                        imgHeight(imgHeight),
                                                        imgWidth(imgWidth) {
        setAdditionalHeader(MMapExtend::generateIntToByteArray(
                std::vector<int>{unknown, top, left, imgHeight, imgWidth})
        );
        setHeaderLength(buildHeaderLength());
        setDataLength(imgHeight * imgWidth);
    }

    void setMapArray(const std::vector<int8_t> &mapArray) {
        MMapResource::mapArray = mapArray;
    }

    std::vector<int8_t> toByteArray() const override {
        std::vector<int8_t> byteArray;
        writeHeadToByteArray(byteArray);
        MMapExtend::appendToByteArray(byteArray, mapArray);
        return byteArray;
    }
};

class MMapCharger : public MMapHead {
private:
    int32_t chargerX{};
    int32_t chargerY{};
    int32_t chargerA{};
public:
    ~MMapCharger() override = default;

    MMapCharger() : MMapHead(MMapType::M_MAP_CHARGER) {
        setHeaderLength(buildHeaderLength());
        setDataLength(sizeof(chargerX) + sizeof(chargerY) + sizeof(chargerA));
    }

    void setCharger(int32_t chargerX, int32_t chargerY, int32_t chargerA) {
        MMapCharger::chargerX = chargerX;
        MMapCharger::chargerY = chargerY;
        MMapCharger::chargerA = chargerA;
    }

    std::vector<int8_t> toByteArray() const override {
        std::vector<int8_t> byteArray;
        writeHeadToByteArray(byteArray);
        MMapExtend::appendToByteArray(byteArray, MMapExtend::generateIntToByteArray(
                std::vector<int>{chargerX * NUMERICAL_EXPANSION, chargerY * NUMERICAL_EXPANSION, chargerA}
        ));
        return byteArray;
    }
};

class MMapRobot : public MMapHead {
private:
    int32_t robotX;
    int32_t robotY;
    int32_t robotA;
public:
    ~MMapRobot() override = default;

    MMapRobot() : MMapHead(MMapType::M_MAP_ROBOT) {
        setHeaderLength(buildHeaderLength());
        setDataLength(sizeof(robotX) + sizeof(robotY) + sizeof(robotA));
    }

    void setRobot(int32_t robotX, int32_t robotY, int32_t robotA) {
        MMapRobot::robotX = robotX;
        MMapRobot::robotY = robotY;
        MMapRobot::robotA = robotA;
    }

    std::vector<int8_t> toByteArray() const override {
        std::vector<int8_t> byteArray;
        writeHeadToByteArray(byteArray);
        MMapExtend::appendToByteArray(byteArray, MMapExtend::generateIntToByteArray(
                std::vector<int>{robotX * NUMERICAL_EXPANSION, robotY * NUMERICAL_EXPANSION, robotA}
        ));
        return byteArray;
    }
};

class MMapTarget : public MMapHead {
private:
    int32_t targetX;
    int32_t targetY;
public:
    ~MMapTarget() override = default;

    MMapTarget() : MMapHead(MMapType::M_MAP_TARGET) {
        setHeaderLength(buildHeaderLength());
        setDataLength((sizeof(targetX) + sizeof(targetY)) / 2);
    }

    void setTarget(int32_t targetX, int32_t targetY) {
        MMapTarget::targetX = targetX;
        MMapTarget::targetY = targetY;
    }

    std::vector<int8_t> toByteArray() const override {
        std::vector<int8_t> byteArray;
        writeHeadToByteArray(byteArray);
        MMapExtend::writeShortToByteArray(byteArray, targetX * NUMERICAL_EXPANSION);
        MMapExtend::writeShortToByteArray(byteArray, targetY * NUMERICAL_EXPANSION);
        return byteArray;
    }

};

class MMapPath : public MMapHead {
private:
    int32_t pointLength = 0;
    int32_t pointSize = 0;
    int32_t angle = 0;

    std::vector<MPoint> points;
public:
    ~MMapPath() override = default;

    MMapPath() : MMapHead(MMapType::M_MAP_PATH) {
        setPoints(points);
    }

    void setPoints(const std::vector<MPoint> &points) {
        MMapPath::points = points;

        pointLength = points.size();
        pointSize = MPoint::toSize();

        setAdditionalHeader(MMapExtend::generateIntToByteArray(std::vector<int>{pointLength, pointSize, angle}));
        setHeaderLength(buildHeaderLength());
        setDataLength(points.size() * MPoint::toSize());
    }

    std::vector<int8_t> toByteArray() const override {
        std::vector<int8_t> byteArray;
        writeHeadToByteArray(byteArray);
        MMapExtend::appendToByteArray(byteArray, objectsToByteArray(points));
        return byteArray;
    }
};

class MMapProhibition : public MMapHead {
private:
    std::vector<MZone> prohibitions;
public:
    ~MMapProhibition() override = default;

    MMapProhibition() : MMapHead(MMapType::M_MAP_PROHIBITION) {
        setProhibitions(prohibitions);
    }

    void setProhibitions(const std::vector<MZone> &prohibitions) {
        MMapProhibition::prohibitions = prohibitions;

        setAdditionalHeader(
                MMapExtend::generateIntToByteArray(std::vector<int>{static_cast<int>(prohibitions.size())})
        );
        setHeaderLength(buildHeaderLength());
        setDataLength(prohibitions.size() * MZone::toSize());
    }

    std::vector<int8_t> toByteArray() const override {
        std::vector<int8_t> byteArray;
        writeHeadToByteArray(byteArray);
        MMapExtend::appendToByteArray(byteArray, objectsToByteArray(prohibitions));
        return byteArray;
    }
};

class MMapVirtually : public MMapHead {
private:
    std::vector<MLine> virtuallys;
public:
    ~MMapVirtually() override = default;

    MMapVirtually() : MMapHead(MMapType::M_MAP_VIRTUALLY) {
        setVirtuallys(virtuallys);
    }

    void setVirtuallys(const std::vector<MLine> &virtuallys) {
        MMapVirtually::virtuallys = virtuallys;

        setAdditionalHeader(MMapExtend::generateIntToByteArray(std::vector<int>{static_cast<int>(virtuallys.size())}));
        setHeaderLength(buildHeaderLength());
        setDataLength(virtuallys.size() * MLine::toSize());
    }

    std::vector<int8_t> toByteArray() const override {
        std::vector<int8_t> byteArray;
        writeHeadToByteArray(byteArray);
        MMapExtend::appendToByteArray(byteArray, objectsToByteArray(virtuallys));
        return byteArray;
    }
};

class MMapZone : public MMapHead {
private:
    std::vector<MZone> zones;
public:
    ~MMapZone() override = default;

    MMapZone() : MMapHead(MMapType::M_MAP_ZONE) {
        setZones(zones);
    }

    void setZones(const std::vector<MZone> &zones) {
        MMapZone::zones = zones;

        setAdditionalHeader(MMapExtend::generateIntToByteArray(std::vector<int>{static_cast<int>(zones.size())}));
        setHeaderLength(buildHeaderLength());
        setDataLength(zones.size() * MZone::toSize());
    }

    std::vector<int8_t> toByteArray() const override {
        std::vector<int8_t> byteArray;
        writeHeadToByteArray(byteArray);
        MMapExtend::appendToByteArray(byteArray, objectsToByteArray(zones));
        return byteArray;
    }
};

class MMapCover : public MMapHead {
private:
    int32_t coverLength = 0;
    int32_t coverSize = 0;

    std::vector<MPoint> covers;
public:
    ~MMapCover() override = default;

    MMapCover() : MMapHead(MMapType::M_MAP_COVER) {
        setCovers(covers);
    }

    void setCovers(const std::vector<MPoint> &covers) {
        MMapCover::covers = covers;

        coverLength = covers.size();
        coverSize = MPoint::toSize();

        setAdditionalHeader(MMapExtend::generateIntToByteArray(std::vector<int>{coverLength, coverSize}));
        setHeaderLength(buildHeaderLength());
        setDataLength(covers.size() * MPoint::toSize());
    }

    std::vector<int8_t> toByteArray() const override {
        std::vector<int8_t> byteArray;
        writeHeadToByteArray(byteArray);
        MMapExtend::appendToByteArray(byteArray, objectsToByteArray(covers));
        return byteArray;
    }
};

class MMapValid : public MMapHead {
public:
    ~MMapValid() override = default;

    MMapValid() : MMapHead(MMapType::M_MAP_VALID) {
        setHeaderLength(buildHeaderLength());
        setDataLength(VALID_DEFAULT_LENGTH);
    }

    std::vector<int8_t> toByteArray() const override {
        std::vector<int8_t> byteArray;
        writeHeadToByteArray(byteArray);
        for (int i = 0; i < VALID_DEFAULT_LENGTH; i++) {
            MMapExtend::writeByteToByteArray(byteArray, i);
        }
        return byteArray;
    }
};

class RRMap : public MMapHead {
private:
    const std::array<int8_t, 2> defaults = {114, 114};

    int16_t majorVersion = 1;
    int16_t minorVersion = 0;
    int mapIndex = 1;
    int mapSequence = 1;

    std::vector<std::unique_ptr<MMapObject>> mapArrays;

public:
    ~RRMap() override = default;

    RRMap() : MMapHead(MMapType::M_MAP_OUTER) {
        std::vector<int8_t> headerArray;
        MMapExtend::writeShortToByteArray(headerArray, majorVersion);
        MMapExtend::writeShortToByteArray(headerArray, minorVersion);
        MMapExtend::writeIntToByteArray(headerArray, mapIndex);
        MMapExtend::writeIntToByteArray(headerArray, mapSequence);
        setAdditionalHeader(headerArray);
        setHeaderLength(buildHeaderLength());
    }

    void addArray(std::unique_ptr<MMapObject> array) {
        mapArrays.push_back(std::move(array));
    }

    std::vector<int8_t> toByteArray() const override {
        std::vector<int8_t> byteArray;
        writeHeadToByteArray(byteArray);

        for (const auto &array: mapArrays) {
            MMapExtend::appendToByteArray(byteArray, array->toByteArray());
        }

        int mapDataLength = byteArray.size() - getHeaderLength();
        std::vector<int8_t> dataArray;
        MMapExtend::writeIntToByteArray(dataArray, mapDataLength);
        MMapExtend::updateByteArray(byteArray, dataArray, 4);

        return byteArray;
    }
};

class RRMapTest {
public:
    static void v() {
//        std::thread t([]() {
//            sleep(10);
//            generate();
//        });
//        t.detach();

        parse();
    }

    static void parse() {

        std::string local_path = path::robot_slam_map_dir() + "local/";

        std::ifstream file(local_path + "4436eb6f-c6ea-4e71-9e7e-42b8bc69ee8a.rrmap",
                           std::ios_base::in | std::ios_base::binary);
        boost::iostreams::filtering_streambuf<boost::iostreams::input> inbuf;

        inbuf.push(boost::iostreams::gzip_decompressor());
        inbuf.push(file);

        std::istream instream(&inbuf);

        std::stringstream output;
        boost::iostreams::copy(instream, output);

        boost::iostreams::close(inbuf);
        file.close();

        std::string contents = output.str();
        std::vector<int8_t> byteArray(contents.begin(), contents.end());


        int mapHeaderLength = MMapExtend::readShortFromByteArray(byteArray, 2);
        int mapDataLength = MMapExtend::readIntFromByteArray(byteArray, 4);
        int16_t majorVersion = MMapExtend::readShortFromByteArray(byteArray, 8);//1
        int16_t minorVersion = MMapExtend::readShortFromByteArray(byteArray, 10);//0
        int mapIndex = MMapExtend::readIntFromByteArray(byteArray, 12);//1
        int mapSequence = MMapExtend::readIntFromByteArray(byteArray, 16);//1

        std::vector<int8_t> mapArray;

        int32_t unknown = 0;
        int32_t top = 0;
        int32_t left = 0;
        int32_t imgHeight;
        int32_t imgWidth;

        int32_t chargerX;
        int32_t chargerY;
        int32_t chargerA;

        int32_t robotX;
        int32_t robotY;
        int32_t robotA;

        int32_t targetX;
        int32_t targetY;

        std::vector<MPoint> paths;

        std::vector<MZone> prohibitions;
        std::vector<MLine> virtuallys;
        std::vector<MZone> zones;

        std::vector<MPoint> covers;

        int blockHeaderLength = 0;
        int blockDataLength = 0;
        for (int blockStartPos = mapHeaderLength; blockStartPos < byteArray.size();
             blockStartPos = blockStartPos + blockHeaderLength + blockDataLength) {

            blockHeaderLength = MMapExtend::readShortFromByteArray(byteArray, blockStartPos + 2);
            std::vector<int8_t> headerArray(byteArray.begin() + blockStartPos,
                                            byteArray.begin() + blockStartPos + blockHeaderLength);
            int blockType = MMapExtend::readShortFromByteArray(headerArray, 0);
            blockDataLength = MMapExtend::readIntFromByteArray(headerArray, 4);
            int blockDataStart = blockStartPos + blockHeaderLength;
            std::vector<int8_t> dataArray(byteArray.begin() + blockDataStart,
                                          byteArray.begin() + blockDataStart + blockDataLength);
            std::cout << dataArray.size() << std::endl;

            switch (blockType) {
                case M_MAP_CHARGER:
                    chargerX = MMapExtend::readIntFromByteArray(dataArray, 0);
                    chargerY = MMapExtend::readIntFromByteArray(dataArray, 4);
                    break;
                case M_MAP_RESOURCE:
                    top = MMapExtend::readIntFromByteArray(headerArray, blockHeaderLength - 16);
                    left = MMapExtend::readIntFromByteArray(headerArray, blockHeaderLength - 12);
                    imgHeight = MMapExtend::readIntFromByteArray(headerArray, blockHeaderLength - 8);
                    imgWidth = MMapExtend::readIntFromByteArray(headerArray, blockHeaderLength - 4);
                    for (const auto &value: dataArray) {
                        mapArray.push_back(value);
                    }
                    break;
                case M_MAP_PATH: {
                    int pairs = MMapExtend::readIntFromByteArray(headerArray, 4) / 4;
                    int32_t pointLength = MMapExtend::readIntFromByteArray(headerArray, 8);
                    int32_t pointSize = MMapExtend::readIntFromByteArray(headerArray, 12);
                    int32_t angle = MMapExtend::readIntFromByteArray(headerArray, 16);
                    for (int pathPair = 0; pathPair < pairs; ++pathPair) {
                        int x = MMapExtend::readShortFromByteArray(dataArray, pathPair * 4);
                        int y = MMapExtend::readShortFromByteArray(dataArray, pathPair * 4 + 2);
                        paths.emplace_back(x, y);
                    }
                    break;
                }
                case M_MAP_TARGET:
                    targetX = MMapExtend::readIntFromByteArray(dataArray, 0);
                    targetY = MMapExtend::readIntFromByteArray(dataArray, 2);
                    break;
                case M_MAP_ROBOT:
                    robotX = MMapExtend::readIntFromByteArray(dataArray, 0);
                    robotY = MMapExtend::readIntFromByteArray(dataArray, 4);
                    robotA = MMapExtend::readIntFromByteArray(dataArray, 8);
                    break;
                case M_MAP_PROHIBITION: {
                    int prohibitionPairs = MMapExtend::readIntFromByteArray(headerArray, 8);
                    for (int prohibitionPair = 0; prohibitionPair < prohibitionPairs; ++prohibitionPair) {
                        int x0 = MMapExtend::readShortFromByteArray(dataArray, prohibitionPair * 16);
                        int y0 = MMapExtend::readShortFromByteArray(dataArray, prohibitionPair * 16 + 2);
                        int x1 = MMapExtend::readShortFromByteArray(dataArray, prohibitionPair * 16 + 4);
                        int y1 = MMapExtend::readShortFromByteArray(dataArray, prohibitionPair * 16 + 6);
                        int x2 = MMapExtend::readShortFromByteArray(dataArray, prohibitionPair * 16 + 8);
                        int y2 = MMapExtend::readShortFromByteArray(dataArray, prohibitionPair * 16 + 10);
                        int x3 = MMapExtend::readShortFromByteArray(dataArray, prohibitionPair * 16 + 12);
                        int y3 = MMapExtend::readShortFromByteArray(dataArray, prohibitionPair * 16 + 14);
                        MPoint p0(x0, y0);
                        MPoint p1(x1, y1);
                        MPoint p2(x2, y2);
                        MPoint p3(x3, y3);
                        prohibitions.emplace_back(p0, p1, p2, p3);
                    }
                    break;
                }
                case M_MAP_VIRTUALLY: {
                    int virtuallyPairs = MMapExtend::readIntFromByteArray(headerArray, 8);
                    for (int virtuallyPair = 0; virtuallyPair < virtuallyPairs; ++virtuallyPair) {
                        int x0 = MMapExtend::readShortFromByteArray(dataArray, virtuallyPair * 16);
                        int y0 = MMapExtend::readShortFromByteArray(dataArray, virtuallyPair * 16 + 2);
                        int x1 = MMapExtend::readShortFromByteArray(dataArray, virtuallyPair * 16 + 4);
                        int y1 = MMapExtend::readShortFromByteArray(dataArray, virtuallyPair * 16 + 6);
                        MPoint p0(x0, y0);
                        MPoint p1(x1, y1);
                        virtuallys.emplace_back(p0, p1);
                    }
                    break;
                }
                case M_MAP_ZONE: {
                    int zonePairs = MMapExtend::readIntFromByteArray(headerArray, 8);
                    for (int zonePair = 0; zonePair < zonePairs; ++zonePair) {
                        int x0 = MMapExtend::readShortFromByteArray(dataArray, zonePair * 16);
                        int y0 = MMapExtend::readShortFromByteArray(dataArray, zonePair * 16 + 2);
                        int x1 = MMapExtend::readShortFromByteArray(dataArray, zonePair * 16 + 4);
                        int y1 = MMapExtend::readShortFromByteArray(dataArray, zonePair * 16 + 6);
                        int x2 = MMapExtend::readShortFromByteArray(dataArray, zonePair * 16 + 8);
                        int y2 = MMapExtend::readShortFromByteArray(dataArray, zonePair * 16 + 10);
                        int x3 = MMapExtend::readShortFromByteArray(dataArray, zonePair * 16 + 12);
                        int y3 = MMapExtend::readShortFromByteArray(dataArray, zonePair * 16 + 14);
                        MPoint p0(x0, y0);
                        MPoint p1(x1, y1);
                        MPoint p2(x2, y2);
                        MPoint p3(x3, y3);
                        zones.emplace_back(p0, p1, p2, p3);
                    }
                    break;
                }
                case M_MAP_COVER: {
                    int coverLength = MMapExtend::readIntFromByteArray(headerArray, 8);
                    int coverSize = MMapExtend::readIntFromByteArray(headerArray, 12);
                    for (int pathPair = 0; pathPair < coverLength; ++pathPair) {
                        int x = MMapExtend::readShortFromByteArray(dataArray, pathPair * 4);
                        int y = MMapExtend::readShortFromByteArray(dataArray, pathPair * 4 + 2);
                        covers.emplace_back(x, y);
                    }
                    break;
                }
                case M_MAP_VALID:

                    break;
                case M_MAP_OUTER:

                    break;
            }

            std::cout << "blockType : " << blockType << std::endl;
        }

        cv::Mat map = cv::Mat::zeros(imgHeight, imgWidth, CV_8UC1);
        std::cout << mapArray.size() << std::endl;
        for (int y = 0; y < map.rows; y++) {
            for (int x = 0; x < map.cols; x++) {
                //& 0xff
                auto value = mapArray[y * imgWidth + x] & 0xff;
                if (value == 15) {
                    map.at<unsigned char>(y, x) = 255;
                } else {
                    map.at<unsigned char>(y, x) = 0;
                }
            }
        }

        for (const auto &zone: zones) {
            MPoint p0 = zone.getP0();
            MPoint p1 = zone.getP1();
            MPoint p2 = zone.getP2();
            MPoint p3 = zone.getP3();
            std::vector<std::vector<cv::Point>> polygon_array;

            std::vector<cv::Point> cvPoints;
            cv::Point c0(p0.getX(), p0.getY());
            cv::Point c1(p1.getX(), p1.getY());
            cv::Point c2(p2.getX(), p2.getY());
            cv::Point c3(p3.getX(), p3.getY());

            polygon_array.push_back(cvPoints);

            cv::line(map, c0, c1, cv::Scalar(50), 1, cv::LINE_8);
            cv::line(map, c1, c2, cv::Scalar(50), 1, cv::LINE_8);
            cv::line(map, c2, c3, cv::Scalar(50), 1, cv::LINE_8);
            cv::line(map, c3, c0, cv::Scalar(50), 1, cv::LINE_8);
        }

        if (!covers.empty()) {
            std::vector<cv::Point> point_path;
            for (const auto &cover: covers) {
                point_path.push_back(cover.toCvPoint());
            }

            cv::circle(map, point_path[0], 2, cv::Scalar(200), CV_FILLED);

            for (size_t step = 1; step < point_path.size(); ++step) {
                cv::Point p1(point_path[step - 1].x, point_path[step - 1].y);
                cv::Point p2(point_path[step].x, point_path[step].y);
                cv::circle(map, p2, 1, cv::Scalar(200), CV_FILLED);
                cv::line(map, p1, p2, cv::Scalar(150), 1);
            }
        }

        if (!paths.empty()) {
            std::vector<cv::Point> point_path;
            for (const auto &path: paths) {
                point_path.push_back(path.toCvPoint());
            }

            cv::circle(map, point_path[0], 2, cv::Scalar(200), CV_FILLED);

            for (size_t step = 1; step < point_path.size(); ++step) {
                cv::Point p1(point_path[step - 1].x, point_path[step - 1].y);
                cv::Point p2(point_path[step].x, point_path[step].y);
                cv::circle(map, p2, 1, cv::Scalar(200), CV_FILLED);
                cv::line(map, p1, p2, cv::Scalar(150), 1);
            }
        }

        cv::Point cvCharger(chargerX / NUMERICAL_EXPANSION, chargerY / NUMERICAL_EXPANSION);
        cv::circle(map, cvCharger, 3, cv::Scalar(200), CV_FILLED);

        cv::Point cvRobot(robotX / NUMERICAL_EXPANSION, robotY / NUMERICAL_EXPANSION);
        cv::circle(map, cvRobot, 3, cv::Scalar(200), CV_FILLED);

        cv::resize(map, map, cv::Size(), 3, 3, cv::INTER_LINEAR);
        cv::imshow("1", map);
        cv::waitKey();
        std::cout << "finish" << std::endl;
    }

    static void generate() {

        const cv::Mat &map = SegmentationCenter::instance().generateMat();
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
        MMapResource rrMapSize(map.rows, map.cols);
        rrMapSize.setMapArray(mapArray);

        const RoomCoverage &coverage = ExplorationCenter::instance().obtainCoveragePath();
        auto penaltyZoneList = MapAttributeSingleton::instance().getPenaltyZoneList();
        auto virtualWallList = MapAttributeSingleton::instance().getVirtualWallList();

        cv::Point2d map_origin = MapAttributeSingleton::instance().getMapOrigin();
        const cv::Point &stationPoint = MapAttributeSingleton::instance().rosPoint2MapPoint(map, Point(0, 0));
        MMapCharger rrMapCharger;
        rrMapCharger.setCharger(stationPoint.x + 10, stationPoint.y + 20, 0);

        cv::Point robotPosition = MapAttributeSingleton::instance().getRobotPositionPoint(map);
        MMapRobot rrMapRobot;
        rrMapRobot.setRobot(robotPosition.x, robotPosition.y, -87);

        MMapTarget rrMapTarget;
        rrMapTarget.setTarget(map.cols / 3, map.rows / 3);

        std::vector<MPoint> paths;
        std::vector<PointVo> pointList = coverage.getPointList();
        for (const auto &point: pointList) {
            paths.emplace_back(point.getX(), point.getY());
        }
        MMapPath rrMapPath;
        rrMapPath.setPoints(paths);

        std::vector<MZone> areas;
        for (int i = 0; i < penaltyZoneList.size(); ++i) {
            auto vector = penaltyZoneList[i];

            const cv::Point &point0 = MapAttributeSingleton::instance().rosPoint2MapPoint(map, vector[0]);
            const cv::Point &point1 = MapAttributeSingleton::instance().rosPoint2MapPoint(map, vector[1]);
            const cv::Point &point2 = MapAttributeSingleton::instance().rosPoint2MapPoint(map, vector[2]);
            const cv::Point &point3 = MapAttributeSingleton::instance().rosPoint2MapPoint(map, vector[3]);

            MPoint p0(point0.x, point0.y);
            MPoint p1(point1.x, point1.y);
            MPoint p2(point2.x, point2.y);
            MPoint p3(point3.x, point3.y);
            MZone zone(p0, p1, p2, p3);
            areas.push_back(zone);
        }
        MMapProhibition rrMapArea;
        rrMapArea.setProhibitions(areas);

        std::vector<MLine> walls;
        for (const auto &vector: virtualWallList) {
            const cv::Point &pointStart = MapAttributeSingleton::instance().rosPoint2MapPoint(map, vector[0]);
            const cv::Point &pointEnd = MapAttributeSingleton::instance().rosPoint2MapPoint(map, vector[1]);
            MPoint p0(pointStart.x, pointStart.y);
            MPoint p1(pointEnd.x, pointEnd.y);
            MLine rrLine(p0, p1);
            walls.push_back(rrLine);
        }
        MMapVirtually rrMapWall;
        rrMapWall.setVirtuallys(walls);

        std::vector<MZone> zones;
        const std::vector<TaskVo> &tasks = TaskDataBase::instance().loadTaskFoMap(
                SegmentationDataBase::instance().getDbMap().id);
        for (const auto &task: tasks) {
            if (task.getMode() == static_cast<int>(TaskMode::Zoned)) {
                std::vector<ZoneVo> taskZones = task.getZones();
                for (const auto &tzp: taskZones) {
                    std::vector<PointVo> points = tzp.getPoints();
                    MPoint p0(points[0].getX(), points[0].getY());
                    MPoint p1(points[1].getX(), points[1].getY());
                    MPoint p2(points[2].getX(), points[2].getY());
                    MPoint p3(points[3].getX(), points[3].getY());
                    MZone zone(p0, p1, p2, p3);
                    zones.push_back(zone);
                }
            }
        }
        MMapZone rrMapZone;
        rrMapZone.setZones(zones);

        MMapValid rrMapValid;

        RRMap rrMap;
        rrMap.addArray(std::make_unique<MMapResource>(rrMapSize));
        rrMap.addArray(std::make_unique<MMapCharger>(rrMapCharger));
        rrMap.addArray(std::make_unique<MMapRobot>(rrMapRobot));
        rrMap.addArray(std::make_unique<MMapTarget>(rrMapTarget));
        rrMap.addArray(std::make_unique<MMapPath>(rrMapPath));
        rrMap.addArray(std::make_unique<MMapProhibition>(rrMapArea));
        rrMap.addArray(std::make_unique<MMapVirtually>(rrMapWall));
        rrMap.addArray(std::make_unique<MMapZone>(rrMapZone));
        rrMap.addArray(std::make_unique<MMapValid>(rrMapValid));

        const std::vector<int8_t> byteArray = rrMap.toByteArray();

        std::stringstream input;
        for (int8_t b: byteArray) {
            input << b;
        }

        //save file
        std::ofstream file("test.rrmap", std::ios_base::out | std::ios_base::binary);
        boost::iostreams::filtering_streambuf<boost::iostreams::output> outbuf;

        outbuf.push(boost::iostreams::gzip_compressor());
        outbuf.push(file);

        boost::iostreams::copy(input, outbuf);

        boost::iostreams::close(outbuf);
        file.close();

        std::cout << "finish" << std::endl;
    }
};

#endif //APP_COMMUNICATION_RRMAP2_H
