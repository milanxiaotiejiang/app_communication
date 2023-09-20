//
// Created by io on 23-9-19.
//

#include "model/gate_info.h"

GateInfo::GateInfo() {}

GateInfo::GateInfo(long id, const std::string &oMapId, double startX, double startY, double endX, double endY,
                   double leftPositionX, double leftPositionY, double leftPositionZ, double leftOrientationX,
                   double leftOrientationY, double leftOrientationZ, double leftOrientationW, double rightPositionX,
                   double rightPositionY, double rightPositionZ, double rightOrientationX, double rightOrientationY,
                   double rightOrientationZ, double rightOrientationW, const std::string &leftGateId,
                   const std::string &rightGateId) : id(id), o_map_id(oMapId), start_x(startX), start_y(startY),
                                                     end_x(endX), end_y(endY), left_position_x(leftPositionX),
                                                     left_position_y(leftPositionY), left_position_z(leftPositionZ),
                                                     left_orientation_x(leftOrientationX),
                                                     left_orientation_y(leftOrientationY),
                                                     left_orientation_z(leftOrientationZ),
                                                     left_orientation_w(leftOrientationW),
                                                     right_position_x(rightPositionX), right_position_y(rightPositionY),
                                                     right_position_z(rightPositionZ),
                                                     right_orientation_x(rightOrientationX),
                                                     right_orientation_y(rightOrientationY),
                                                     right_orientation_z(rightOrientationZ),
                                                     right_orientation_w(rightOrientationW), left_gate_ID(leftGateId),
                                                     right_gate_ID(rightGateId) {}

long GateInfo::getId() const {
    return id;
}

void GateInfo::setId(long id) {
    GateInfo::id = id;
}

const std::string &GateInfo::getOMapId() const {
    return o_map_id;
}

void GateInfo::setOMapId(const std::string &oMapId) {
    o_map_id = oMapId;
}

double GateInfo::getStartX() const {
    return start_x;
}

void GateInfo::setStartX(double startX) {
    start_x = startX;
}

double GateInfo::getStartY() const {
    return start_y;
}

void GateInfo::setStartY(double startY) {
    start_y = startY;
}

double GateInfo::getEndX() const {
    return end_x;
}

void GateInfo::setEndX(double endX) {
    end_x = endX;
}

double GateInfo::getEndY() const {
    return end_y;
}

void GateInfo::setEndY(double endY) {
    end_y = endY;
}

double GateInfo::getLeftPositionX() const {
    return left_position_x;
}

void GateInfo::setLeftPositionX(double leftPositionX) {
    left_position_x = leftPositionX;
}

double GateInfo::getLeftPositionY() const {
    return left_position_y;
}

void GateInfo::setLeftPositionY(double leftPositionY) {
    left_position_y = leftPositionY;
}

double GateInfo::getLeftPositionZ() const {
    return left_position_z;
}

void GateInfo::setLeftPositionZ(double leftPositionZ) {
    left_position_z = leftPositionZ;
}

double GateInfo::getLeftOrientationX() const {
    return left_orientation_x;
}

void GateInfo::setLeftOrientationX(double leftOrientationX) {
    left_orientation_x = leftOrientationX;
}

double GateInfo::getLeftOrientationY() const {
    return left_orientation_y;
}

void GateInfo::setLeftOrientationY(double leftOrientationY) {
    left_orientation_y = leftOrientationY;
}

double GateInfo::getLeftOrientationZ() const {
    return left_orientation_z;
}

void GateInfo::setLeftOrientationZ(double leftOrientationZ) {
    left_orientation_z = leftOrientationZ;
}

double GateInfo::getLeftOrientationW() const {
    return left_orientation_w;
}

void GateInfo::setLeftOrientationW(double leftOrientationW) {
    left_orientation_w = leftOrientationW;
}

double GateInfo::getRightPositionX() const {
    return right_position_x;
}

void GateInfo::setRightPositionX(double rightPositionX) {
    right_position_x = rightPositionX;
}

double GateInfo::getRightPositionY() const {
    return right_position_y;
}

void GateInfo::setRightPositionY(double rightPositionY) {
    right_position_y = rightPositionY;
}

double GateInfo::getRightPositionZ() const {
    return right_position_z;
}

void GateInfo::setRightPositionZ(double rightPositionZ) {
    right_position_z = rightPositionZ;
}

double GateInfo::getRightOrientationX() const {
    return right_orientation_x;
}

void GateInfo::setRightOrientationX(double rightOrientationX) {
    right_orientation_x = rightOrientationX;
}

double GateInfo::getRightOrientationY() const {
    return right_orientation_y;
}

void GateInfo::setRightOrientationY(double rightOrientationY) {
    right_orientation_y = rightOrientationY;
}

double GateInfo::getRightOrientationZ() const {
    return right_orientation_z;
}

void GateInfo::setRightOrientationZ(double rightOrientationZ) {
    right_orientation_z = rightOrientationZ;
}

double GateInfo::getRightOrientationW() const {
    return right_orientation_w;
}

void GateInfo::setRightOrientationW(double rightOrientationW) {
    right_orientation_w = rightOrientationW;
}

const std::string &GateInfo::getLeftGateId() const {
    return left_gate_ID;
}

void GateInfo::setLeftGateId(const std::string &leftGateId) {
    left_gate_ID = leftGateId;
}

const std::string &GateInfo::getRightGateId() const {
    return right_gate_ID;
}

void GateInfo::setRightGateId(const std::string &rightGateId) {
    right_gate_ID = rightGateId;
}