//
// Created by jiang on 2022/6/9.
//

#ifndef C_PTHREAD_PARTION_H
#define C_PTHREAD_PARTION_H

#include "model/Point.h"
#include "string"
#include <nlohmann/json.hpp>
#include <ostream>

using json = nlohmann::json;

class Name_info {
private:
    int id;
    std::string name;
public:
    int getId() const;

    void setId(int id);

    const std::string &getName() const;

    void setName(const std::string &name);

public:
    Name_info();

    Name_info(int id, std::string name);

    virtual ~Name_info();


    friend void to_json(json &j, const Name_info &b) {
        j = json{
                {"id",   b.id},
                {"name", b.name}};
    }

    friend void from_json(const json &j, Name_info &b) {
        j.at("id").get_to(b.id);
        j.at("name").get_to(b.name);
    }
};

class Partion_line {
private:
    Point start;
    Point end;

public:
    Partion_line(Point start, Point end);

    Partion_line();

    ~Partion_line();


    const Point &getStart() const;

    void setStart(const Point &start);

    const Point &getAnEnd() const;

    void setAnEnd(const Point &anEnd);

    friend void to_json(json &j, const Partion_line &b) {
        j = json{
                {"start", b.start},
                {"end",   b.end}};
    }

    friend void from_json(const json &j, Partion_line &b) {
        j.at("start").get_to(b.start);
        j.at("end").get_to(b.end);
    }
};

class Partion_component {
private:
    int id;
    int index;
    double left;
    double top;
    double width;
    double height;
    double area;
    int cx;
    int cy;
    std::string name;
    std::vector<Point> board;

public:
    Partion_component();

    ~Partion_component();

    int getId() const;

    void setId(int id);

    int getIndex() const;

    void setIndex(int index);

    double getLeft() const;

    void setLeft(double left);

    double getTop() const;

    void setTop(double top);

    double getWidth() const;

    void setWidth(double width);

    double getHeight() const;

    void setHeight(double height);

    double getArea() const;

    void setArea(double area);

    int getCx() const;

    void setCx(int cx);

    int getCy() const;

    void setCy(int cy);

    const std::string &getName() const;

    void setName(const std::string &name);

    const std::vector<Point> &getBoard() const;

    void setBoard(const std::vector<Point> &board);

    friend void to_json(json &j, const Partion_component &b) {
        j = json{
                {"id",     b.id},
                {"index",  b.index},
                {"left",   b.left},
                {"top",    b.top},
                {"width",  b.width},
                {"height", b.height},
                {"area",   b.area},
                {"cx",     b.cx},
                {"cy",     b.cy},
                {"name",   b.name},
                {"board",  b.board}};
    }

    friend void from_json(const json &j, Partion_component &b) {
        j.at("id").get_to(b.id);
        j.at("index").get_to(b.index);
        j.at("left").get_to(b.left);
        j.at("top").get_to(b.top);
        j.at("width").get_to(b.width);
        j.at("height").get_to(b.height);
        j.at("area").get_to(b.area);
        j.at("cx").get_to(b.cx);
        j.at("cy").get_to(b.cy);
        j.at("name").get_to(b.name);
        j.at("board").get_to(b.board);
    }
};


class Partion {
private:
    int mapWidth;
    int mapHeight;
    std::vector<Partion_line> segmentationPoints;
    std::vector<Partion_component> component;


public:
    Partion();

    ~Partion();

    void setMapWidth(int mapWidth_);

    int getyMapWidth();

    void setMapHeight(int mapHeight_);

    int getMapHeight();

    void setSegmentationPoints(std::vector<Partion_line> segmentationPoints_);

    std::vector<Partion_line> getSegmentationPoints();

    void setComponent(std::vector<Partion_component> component);

    std::vector<Partion_component> getComponent();

    friend void to_json(json &j, const Partion &b) {
        j = json{
                {"mapWidth",           b.mapWidth},
                {"mapHeight",          b.mapHeight},
                {"segmentationPoints", b.segmentationPoints},
                {"component",          b.component}};
    }

    friend void
    from_json(const json &j, Partion &b) {
        j.at("mapWidth").get_to(b.mapWidth);
        j.at("mapHeight").get_to(b.mapHeight);
        j.at("segmentationPoints").get_to(b.segmentationPoints);
        j.at("component").get_to(b.component);
    }
};

#endif//C_PTHREAD_PARTION_H