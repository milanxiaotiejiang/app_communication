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

#define SIZE_OF_HEAD_TYPE 2
#define SIZE_OF_HEAD_LENGTH 2
#define SIZE_OF_DATA_LENGTH 4
#define NUMERICAL_EXPANSION 50

enum MMapType {
    M_MAP_CHARGER = 1,
    M_MAP_SIZE = 2,
    M_MAP_PATH = 3,
    M_MAP_GO_TO = 7,
    M_MAP_ROBOT = 8,
    M_MAP_AREA = 9,
    M_MAP_WALL = 10,
    M_MAP_ZONE = 12,
    M_MAP_VALID = 1024
};

class MMapObject {
public:
    virtual ~MMapObject() = default;

    virtual std::vector<int8_t> toByteArray() = 0;

    static void readInt(std::vector<int8_t> &byteArray, int value) {
        byteArray.push_back(value & 0xff);
        byteArray.push_back((value >> 8) & 0xff);
        byteArray.push_back((value >> 16) & 0xff);
        byteArray.push_back((value >> 24) & 0xff);
    }

    static void readShortToInt(std::vector<int8_t> &byteArray, int value) {
        byteArray.push_back(value & 0xff);
        byteArray.push_back((value >> 8) & 0xff);
    }

    static void readByteToInt(std::vector<int8_t> &byteArray, int value) {
        byteArray.push_back(value & 0xff);
    }
};

class MMapHead : public MMapObject {
public:
    ~MMapHead() override = default;

    virtual MMapType type() const = 0;

    virtual int headerLength() const = 0;

    virtual int dataLength() const = 0;

    virtual std::vector<int8_t> baseArray() const {
        std::vector<int8_t> byteArray;
        readShortToInt(byteArray, static_cast<int>(type()));
        readShortToInt(byteArray, headerLength());
        readInt(byteArray, dataLength());
        return byteArray;
    }
};

class RRPoint : public MMapObject {
private:
    int x;
    int y;
public:
    ~RRPoint() override = default;

    RRPoint(int x, int y) : x(x), y(y) {}

    static int toSize() {
        return (sizeof(x) + sizeof(y)) / 2;
    }

    std::vector<int8_t> toByteArray() override {
        std::vector<int8_t> byteArray;
        readShortToInt(byteArray, x * NUMERICAL_EXPANSION);
        readShortToInt(byteArray, y * NUMERICAL_EXPANSION);
        return byteArray;
    }
};

class RRLine : public MMapObject {
private:
    RRPoint pointStart;
    RRPoint pointEnd;
public:
    ~RRLine() override = default;

    RRLine(const RRPoint &pointStart, const RRPoint &pointEnd) : pointStart(pointStart), pointEnd(pointEnd) {}

    static int toSize() {
        return RRPoint::toSize() + RRPoint::toSize();
    }

    std::vector<int8_t> toByteArray() override {
        std::vector<int8_t> byteArray;
        for (const auto &item: pointStart.toByteArray()) {
            byteArray.push_back(item);
        }
        for (const auto &item: pointEnd.toByteArray()) {
            byteArray.push_back(item);
        }
        return byteArray;
    }
};

class RRZone : public MMapObject {
private:
    RRPoint p0;
    RRPoint p1;
    RRPoint p2;
    RRPoint p3;
public:
    ~RRZone() override = default;

    RRZone(const RRPoint &p0, const RRPoint &p1, const RRPoint &p2, const RRPoint &p3) : p0(p0), p1(p1), p2(p2),
                                                                                         p3(p3) {}

    static int toSize() {
        return RRPoint::toSize() + RRPoint::toSize() + RRPoint::toSize() + RRPoint::toSize();
    }

    std::vector<int8_t> toByteArray() override {
        std::vector<int8_t> byteArray;
        for (const auto &item: p0.toByteArray()) {
            byteArray.push_back(item);
        }
        for (const auto &item: p1.toByteArray()) {
            byteArray.push_back(item);
        }
        for (const auto &item: p2.toByteArray()) {
            byteArray.push_back(item);
        }
        for (const auto &item: p3.toByteArray()) {
            byteArray.push_back(item);
        }
        return byteArray;
    }
};

class RRMapSize : public MMapHead {
private:
    int unknown = 0;
    int top;
    int left;
    int imgHeight;
    int imgWidth;

    std::vector<int8_t> mapArray;
public:
    ~RRMapSize() override = default;

    RRMapSize(int top, int left, int imgHeight, int imgWidth,
              const std::vector<int8_t> &mapArray) : top(top), left(left), imgHeight(imgHeight),
                                                     imgWidth(imgWidth), mapArray(mapArray) {}

    MMapType type() const override {
        return MMapType::M_MAP_SIZE;
    }

    int headerLength() const override {
        return SIZE_OF_HEAD_TYPE + SIZE_OF_HEAD_LENGTH + SIZE_OF_DATA_LENGTH +
               sizeof(unknown) + sizeof(top) + sizeof(left) + sizeof(imgHeight) + sizeof(imgWidth);
    }

    int dataLength() const override {
        return imgHeight * imgWidth;
    }

    std::vector<int8_t> toByteArray() override {
        std::vector<int8_t> byteArray = baseArray();

        readInt(byteArray, unknown);
        readInt(byteArray, top);
        readInt(byteArray, left);
        readInt(byteArray, imgHeight);
        readInt(byteArray, imgWidth);

        for (const auto &item: mapArray) {
            byteArray.push_back(item);
        }
        return byteArray;
    }
};

class RRMapCharger : public MMapHead {
private:
    int chargerX;
    int chargerY;
    int chargerA;
public:
    ~RRMapCharger() override = default;

    RRMapCharger(int chargerX, int chargerY, int chargerA) : chargerX(chargerX), chargerY(chargerY),
                                                             chargerA(chargerA) {}

    MMapType type() const override {
        return MMapType::M_MAP_CHARGER;
    }

    int headerLength() const override {
        return SIZE_OF_HEAD_TYPE + SIZE_OF_HEAD_LENGTH + SIZE_OF_DATA_LENGTH;
    }

    int dataLength() const override {
        return sizeof(chargerX) + sizeof(chargerY) + sizeof(chargerA);
    }

    std::vector<int8_t> toByteArray() override {
        std::vector<int8_t> byteArray = baseArray();
        readInt(byteArray, chargerX * NUMERICAL_EXPANSION);
        readInt(byteArray, chargerY * NUMERICAL_EXPANSION);
        readInt(byteArray, chargerA);
        return byteArray;
    }
};

class RRMapRobot : public MMapHead {
private:
    int robotX;
    int robotY;
    int robotA;
public:
    ~RRMapRobot() override = default;

    RRMapRobot(int robotX, int robotY, int robotA) : robotX(robotX), robotY(robotY), robotA(robotA) {}

    MMapType type() const override {
        return MMapType::M_MAP_ROBOT;
    }

    int headerLength() const override {
        return SIZE_OF_HEAD_TYPE + SIZE_OF_HEAD_LENGTH + SIZE_OF_DATA_LENGTH;
    }

    int dataLength() const override {
        return sizeof(robotX) + sizeof(robotY) + sizeof(robotA);
    }

    std::vector<int8_t> toByteArray() override {
        std::vector<int8_t> byteArray = baseArray();
        readInt(byteArray, robotX * NUMERICAL_EXPANSION);
        readInt(byteArray, robotY * NUMERICAL_EXPANSION);
        readInt(byteArray, robotA);
        return byteArray;
    }
};

class RRMapGoTo : public MMapHead {
private:
    int gotoX;
    int gotoY;
public:
    ~RRMapGoTo() override = default;

    RRMapGoTo(int gotoX, int gotoY) : gotoX(gotoX), gotoY(gotoY) {}

    MMapType type() const override {
        return MMapType::M_MAP_GO_TO;
    }

    int headerLength() const override {
        return SIZE_OF_HEAD_TYPE + SIZE_OF_HEAD_LENGTH + SIZE_OF_DATA_LENGTH;
    }

    int dataLength() const override {
        return (sizeof(gotoX) + sizeof(gotoY)) / 2;
    }

    std::vector<int8_t> toByteArray() override {
        std::vector<int8_t> byteArray = baseArray();
        readShortToInt(byteArray, gotoX * NUMERICAL_EXPANSION);
        readShortToInt(byteArray, gotoY * NUMERICAL_EXPANSION);
        return byteArray;
    }

};

class RRMapPath : public MMapHead {
private:
    int pairs = 0;
    int pointLength = 0;
    int pointSize = 0;
    int angle = 0;

    std::vector<RRPoint> points;
public:
    ~RRMapPath() override = default;

    RRMapPath(const std::vector<RRPoint> &points) : points(points) {
        pairs = points.size() * RRPoint::toSize();
        pointLength = points.size();
        pointSize = RRPoint::toSize();
        angle = 0;
    }

    MMapType type() const override {
        return MMapType::M_MAP_PATH;
    }

    int headerLength() const override {
        return SIZE_OF_HEAD_TYPE + SIZE_OF_HEAD_LENGTH +
               sizeof(pairs) + sizeof(pointLength) + sizeof(pointSize) + sizeof(angle);
    }

    int dataLength() const override {
        return points.size() * RRPoint::toSize();
    }

    std::vector<int8_t> toByteArray() override {
        std::vector<int8_t> byteArray = baseArray();
        readInt(byteArray, pointLength);
        readInt(byteArray, pointSize);
        readInt(byteArray, angle);
        for (auto &point: points) {
            for (const auto &byte: point.toByteArray()) {
                byteArray.push_back(byte);
            }
        }
        return byteArray;
    }
};

class RRMapArea : public MMapHead {
private:
    int number;

    std::vector<RRZone> areas;
public:
    ~RRMapArea() override = default;

    RRMapArea(const std::vector<RRZone> &areas) : areas(areas) {
        number = areas.size();
    }

    MMapType type() const override {
        return MMapType::M_MAP_AREA;
    }

    int headerLength() const override {
        return SIZE_OF_HEAD_TYPE + SIZE_OF_HEAD_LENGTH + SIZE_OF_DATA_LENGTH + sizeof(number);
    }

    int dataLength() const override {
        return areas.size() * RRZone::toSize();
    }

    std::vector<int8_t> toByteArray() override {
        std::vector<int8_t> byteArray = baseArray();
        readInt(byteArray, number);
        for (auto &area: areas) {
            for (const auto &byte: area.toByteArray()) {
                byteArray.push_back(byte);
            }
        }
        return byteArray;
    }
};

class RRMapWall : public MMapHead {
private:
    int number;

    std::vector<RRLine> walls;
public:
    ~RRMapWall() override = default;

    RRMapWall(const std::vector<RRLine> &walls) : walls(walls) {
        number = walls.size();
    }

    MMapType type() const override {
        return MMapType::M_MAP_WALL;
    }

    int headerLength() const override {
        return SIZE_OF_HEAD_TYPE + SIZE_OF_HEAD_LENGTH + SIZE_OF_DATA_LENGTH + sizeof(number);
    }

    int dataLength() const override {
        return walls.size() * RRLine::toSize();
    }

    std::vector<int8_t> toByteArray() override {
        std::vector<int8_t> byteArray = baseArray();
        readInt(byteArray, number);
        for (auto &wall: walls) {
            for (const auto &byte: wall.toByteArray()) {
                byteArray.push_back(byte);
            }
        }
        return byteArray;
    }
};

class RRMapZone : public MMapHead {
private:
    int number;

    std::vector<RRZone> zones;
public:
    ~RRMapZone() override = default;

    RRMapZone(const std::vector<RRZone> &zones) : zones(zones) {
        number = zones.size();
    }

    MMapType type() const override {
        return MMapType::M_MAP_ZONE;
    }

    int headerLength() const override {
        return SIZE_OF_HEAD_TYPE + SIZE_OF_HEAD_LENGTH + SIZE_OF_DATA_LENGTH + sizeof(number);
    }

    int dataLength() const override {
        return zones.size() * RRZone::toSize();
    }

    std::vector<int8_t> toByteArray() override {
        std::vector<int8_t> byteArray = baseArray();
        readInt(byteArray, number);
        for (auto &zone: zones) {
            for (const auto &byte: zone.toByteArray()) {
                byteArray.push_back(byte);
            }
        }
        return byteArray;
    }
};

class RRMapValid : public MMapHead {
public:
    ~RRMapValid() override = default;

    MMapType type() const override {
        return MMapType::M_MAP_VALID;
    }

    int headerLength() const override {
        return SIZE_OF_HEAD_TYPE + SIZE_OF_HEAD_LENGTH + SIZE_OF_DATA_LENGTH;
    }

    int dataLength() const override {
        return 20;
    }

    std::vector<int8_t> toByteArray() override {
        std::vector<int8_t> byteArray = baseArray();
        for (int i = 0; i < 20; i++) {
            readByteToInt(byteArray, i);
        }
        return byteArray;
    }
};

class RRMap : public MMapObject {
private:
    int8_t default_0 = 114;
    int8_t default_1 = 114;
    int16_t mapHeaderLength;
    int mapDataLength;
    int16_t majorVersion = 1;
    int16_t minorVersion = 0;
    int mapIndex = 1;
    int mapSequence = 1;

    std::vector<std::unique_ptr<MMapObject>> mapArrays;

public:
    RRMap() {
        mapHeaderLength = sizeof(RRMap::default_0) + sizeof(RRMap::default_1) +
                          sizeof(RRMap::mapHeaderLength) + sizeof(RRMap::mapDataLength) +
                          sizeof(RRMap::majorVersion) + sizeof(RRMap::minorVersion) +
                          sizeof(RRMap::mapIndex) + sizeof(RRMap::mapSequence);
    }

    ~RRMap() override = default;

    void addArray(std::unique_ptr<MMapObject> array) {
        mapArrays.push_back(std::move(array));
    }

    std::vector<int8_t> toByteArray() override {

        std::vector<int8_t> byteArray;

        readByteToInt(byteArray, default_0);
        readByteToInt(byteArray, default_1);
        readShortToInt(byteArray, mapHeaderLength);
        readInt(byteArray, mapDataLength);
        readShortToInt(byteArray, majorVersion);
        readShortToInt(byteArray, minorVersion);
        readInt(byteArray, mapIndex);
        readInt(byteArray, mapSequence);

        for (const auto &array: mapArrays) {
            const auto objBytes = array->toByteArray();
            byteArray.insert(byteArray.end(), objBytes.begin(), objBytes.end());
        }

        mapDataLength = byteArray.size() - mapHeaderLength;
        std::vector<int8_t> data;
        readInt(data, mapDataLength);
        for (int i = 0; i < data.size(); i++) {
            byteArray[4 + i] = data[i];
        }

        return byteArray;
    }
};

class RRMapTest {
public:
    static void v() {
        std::thread t([]() {
            sleep(10);
            generate();
        });
        t.detach();
    }

    static void generate() {
        const cv::Mat &map = SegmentationCenter::instance().generateMat();
        std::vector<int8_t> mapArray;
        for (int y = 0; y < map.rows; y++) {
            for (int x = 0; x < map.cols; x++) {
                if (map.at<unsigned char>(y, x) == 255) {
                    MMapObject::readByteToInt(mapArray, 15);
                } else {
                    MMapObject::readByteToInt(mapArray, 0);
                }
            }
        }
        RRMapSize rrMapSize(0, 0, map.rows, map.cols, mapArray);
        const RoomCoverage &coverage = ExplorationCenter::instance().obtainCoveragePath();
        auto penaltyZoneList = MapAttribute::instance().getPenaltyZoneList();
        auto virtualWallList = MapAttribute::instance().getVirtualWallList();

        cv::Point2d map_origin = MapAttribute::instance().getMapOrigin();
        const cv::Point &stationPoint = MapAttribute::instance().rosPoint2MapPoint(map, Point(0, 0));
        RRMapCharger rrMapCharger(stationPoint.x + 10, stationPoint.y + 20, 0);

        cv::Point robotPosition = MapAttribute::instance().getRobotPositionPoint(map);
        RRMapRobot rrMapRobot(robotPosition.x, robotPosition.y, -87);

        RRMapGoTo rrMapGoTo(map.cols / 3, map.rows / 3);

        std::vector<RRPoint> paths;
        std::vector<PointVo> pointList = coverage.getPointList();
        for (const auto &point: pointList) {
            paths.emplace_back(point.getX(), point.getY());
        }
        RRMapPath rrMapPath(paths);

        std::vector<RRZone> areas;
        for (int i = 0; i < penaltyZoneList.size(); ++i) {
            auto vector = penaltyZoneList[i];

            const cv::Point &point0 = MapAttribute::instance().rosPoint2MapPoint(map, vector[0]);
            const cv::Point &point1 = MapAttribute::instance().rosPoint2MapPoint(map, vector[1]);
            const cv::Point &point2 = MapAttribute::instance().rosPoint2MapPoint(map, vector[2]);
            const cv::Point &point3 = MapAttribute::instance().rosPoint2MapPoint(map, vector[3]);

            RRPoint p0(point0.x, point0.y);
            RRPoint p1(point1.x, point1.y);
            RRPoint p2(point2.x, point2.y);
            RRPoint p3(point3.x, point3.y);
            RRZone zone(p0, p1, p2, p3);
            areas.push_back(zone);
        }
        RRMapArea rrMapArea(areas);

        std::vector<RRLine> walls;
        for (const auto &vector: virtualWallList) {
            const cv::Point &pointStart = MapAttribute::instance().rosPoint2MapPoint(map, vector[0]);
            const cv::Point &pointEnd = MapAttribute::instance().rosPoint2MapPoint(map, vector[1]);
            RRPoint p0(pointStart.x, pointStart.y);
            RRPoint p1(pointEnd.x, pointEnd.y);
            RRLine rrLine(p0, p1);
            walls.push_back(rrLine);
        }
        RRMapWall rrMapWall(walls);

        std::vector<RRZone> zones;
        const vector <TaskVo> &tasks = TaskDataBase::instance().loadTaskFoMap(
                SegmentationDataBase::instance().getDbMap().id);
        for (const auto &task: tasks) {
            if (task.getMode() == static_cast<int>(TaskMode::Zoned)) {
                std::vector<ZoneVo> taskZones = task.getZones();
                for (const auto &tzp: taskZones) {
                    std::vector<PointVo> points = tzp.getPoints();
                    RRPoint p0(points[0].getX(), points[0].getY());
                    RRPoint p1(points[1].getX(), points[1].getY());
                    RRPoint p2(points[2].getX(), points[2].getY());
                    RRPoint p3(points[3].getX(), points[3].getY());
                    RRZone zone(p0, p1, p2, p3);
                    zones.push_back(zone);
                }
            }
        }
        RRMapZone rrMapZone(zones);

        RRMapValid rrMapValid;

        RRMap rrMap;
        rrMap.addArray(make_unique<RRMapSize>(rrMapSize));
        rrMap.addArray(make_unique<RRMapCharger>(rrMapCharger));
        rrMap.addArray(make_unique<RRMapRobot>(rrMapRobot));
        rrMap.addArray(make_unique<RRMapGoTo>(rrMapGoTo));
        rrMap.addArray(make_unique<RRMapPath>(rrMapPath));
        rrMap.addArray(make_unique<RRMapArea>(rrMapArea));
        rrMap.addArray(make_unique<RRMapWall>(rrMapWall));
        rrMap.addArray(make_unique<RRMapZone>(rrMapZone));
        rrMap.addArray(make_unique<RRMapValid>(rrMapValid));

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
