//
// Created by jiang on 2022/6/9.
//

#include "model/Partion.h"

Name_info::Name_info(int id_, string name_) {
    id = id_;
    name = name_;
}

Name_info::Name_info() {}

int Name_info::getId() const { return id; }

void Name_info::setId(int id) { Name_info::id = id; }

const string &Name_info::getName() const { return name; }

void Name_info::setName(const string &name) { Name_info::name = name; }

Name_info::~Name_info() = default;

//****************

Partion_line::Partion_line(Point start_, Point end_) {
    Partion_line::start = start_;
    Partion_line::end = end_;
}

Partion_line::Partion_line() {}

const Point &Partion_line::getStart() const { return start; }

void Partion_line::setStart(const Point &start) { Partion_line::start = start; }

const Point &Partion_line::getAnEnd() const { return end; }

void Partion_line::setAnEnd(const Point &anEnd) { end = anEnd; }

Partion_line::~Partion_line() = default;

//**************
Partion_component::Partion_component() {}

int Partion_component::getId() const { return id; }

void Partion_component::setId(int id) { Partion_component::id = id; }

int Partion_component::getIndex() const { return index; }

void Partion_component::setIndex(int index) {
    Partion_component::index = index;
}

double Partion_component::getLeft() const { return left; }

void Partion_component::setLeft(double left) { Partion_component::left = left; }

double Partion_component::getTop() const { return top; }

void Partion_component::setTop(double top) { Partion_component::top = top; }

double Partion_component::getWidth() const { return width; }

void Partion_component::setWidth(double width) {
    Partion_component::width = width;
}

double Partion_component::getHeight() const { return height; }

void Partion_component::setHeight(double height) {
    Partion_component::height = height;
}

double Partion_component::getArea() const { return area; }

void Partion_component::setArea(double area) { Partion_component::area = area; }

int Partion_component::getCx() const { return cx; }

void Partion_component::setCx(int cx) { Partion_component::cx = cx; }

int Partion_component::getCy() const { return cy; }

void Partion_component::setCy(int cy) { Partion_component::cy = cy; }

const string &Partion_component::getName() const { return name; }

void Partion_component::setName(const string &name) {
    Partion_component::name = name;
}

const vector<Point> &Partion_component::getBoard() const { return board; }

void Partion_component::setBoard(const vector<Point> &board) {
    Partion_component::board = board;
}

Partion_component::~Partion_component() = default;

//**************
Partion::Partion() {}

Partion::~Partion() = default;

void Partion::setMapWidth(int mapWidth_) { mapWidth = mapWidth_; }

int Partion::getyMapWidth() { return mapWidth; }

void Partion::setMapHeight(int mapHeight_) { mapHeight = mapHeight_; }

int Partion::getMapHeight() { return mapHeight; }

void Partion::setSegmentationPoints(vector<Partion_line> segmentationPoints_) {
    segmentationPoints = segmentationPoints_;
}

vector<Partion_line> Partion::getSegmentationPoints() {
    return segmentationPoints;
}

void Partion::setComponent(vector<Partion_component> component_) {
    component = component_;
}

vector<Partion_component> Partion::getComponent() { return component; }