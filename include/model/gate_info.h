//
// Created by io on 23-9-19.
//

#ifndef APP_COMMUNICATION_GATE_INFO_H
#define APP_COMMUNICATION_GATE_INFO_H

#include <string>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class GateInfo {
    long id{};
    std::string o_map_id;
    double start_x{};
    double start_y{};
    double end_x{};
    double end_y{};

    double left_position_x{};
    double left_position_y{};
    double left_position_z{};
    double left_orientation_x{};
    double left_orientation_y{};
    double left_orientation_z{};
    double left_orientation_w{};

    double right_position_x{};
    double right_position_y{};
    double right_position_z{};
    double right_orientation_x{};
    double right_orientation_y{};
    double right_orientation_z{};
    double right_orientation_w{};

    std::string left_gate_ID{};
    std::string right_gate_ID{};

    std::string factory_ID{};

public:
    GateInfo();

    GateInfo(long id, const std::string &oMapId, double startX, double startY, double endX, double endY,
             double leftPositionX, double leftPositionY, double leftPositionZ, double leftOrientationX,
             double leftOrientationY, double leftOrientationZ, double leftOrientationW, double rightPositionX,
             double rightPositionY, double rightPositionZ, double rightOrientationX, double rightOrientationY,
             double rightOrientationZ, double rightOrientationW, const std::string &leftGateId,
             const std::string &rightGateId, const std::string &factoryId);

    friend void to_json(json &j, const GateInfo &b) {
        j = json{
                {"id",                  b.id},
                {"o_map_id",            b.o_map_id},
                {"start_x",             b.start_x},
                {"start_y",             b.start_y},
                {"end_x",               b.end_x},
                {"end_y",               b.end_y},
                {"left_position_x",     b.left_position_x},
                {"left_position_y",     b.left_position_y},
                {"left_position_z",     b.left_position_z},
                {"left_orientation_x",  b.left_orientation_x},
                {"left_orientation_y",  b.left_orientation_y},
                {"left_orientation_z",  b.left_orientation_z},
                {"left_orientation_w",  b.left_orientation_w},
                {"right_position_x",    b.right_position_x},
                {"right_position_y",    b.right_position_y},
                {"right_position_z",    b.right_position_z},
                {"right_orientation_x", b.right_orientation_x},
                {"right_orientation_y", b.right_orientation_y},
                {"right_orientation_z", b.right_orientation_z},
                {"right_orientation_w", b.right_orientation_w},
                {"left_gate_ID",        b.left_gate_ID},
                {"right_gate_ID",       b.right_gate_ID},
                {"factory_ID",          b.factory_ID},
        };
    }

    friend void from_json(const json &j, GateInfo &b) {
        j.at("id").get_to(b.id);
        j.at("o_map_id").get_to(b.o_map_id);
        j.at("start_x").get_to(b.start_x);
        j.at("start_y").get_to(b.start_y);
        j.at("end_x").get_to(b.end_x);
        j.at("end_y").get_to(b.end_y);
        j.at("left_position_x").get_to(b.left_position_x);
        j.at("left_position_y").get_to(b.left_position_y);
        j.at("left_position_z").get_to(b.left_position_z);
        j.at("left_orientation_x").get_to(b.left_orientation_x);
        j.at("left_orientation_y").get_to(b.left_orientation_y);
        j.at("left_orientation_z").get_to(b.left_orientation_z);
        j.at("left_orientation_w").get_to(b.left_orientation_w);
        j.at("right_position_x").get_to(b.right_position_x);
        j.at("right_position_y").get_to(b.right_position_y);
        j.at("right_position_z").get_to(b.right_position_z);
        j.at("right_orientation_x").get_to(b.right_orientation_x);
        j.at("right_orientation_y").get_to(b.right_orientation_y);
        j.at("right_orientation_z").get_to(b.right_orientation_z);
        j.at("right_orientation_w").get_to(b.right_orientation_w);
        j.at("left_gate_ID").get_to(b.left_gate_ID);
        j.at("right_gate_ID").get_to(b.right_gate_ID);
        j.at("factory_ID").get_to(b.factory_ID);
    }

    long getId() const;

    void setId(long id);

    const std::string &getOMapId() const;

    void setOMapId(const std::string &oMapId);

    double getStartX() const;

    void setStartX(double startX);

    double getStartY() const;

    void setStartY(double startY);

    double getEndX() const;

    void setEndX(double endX);

    double getEndY() const;

    void setEndY(double endY);

    double getLeftPositionX() const;

    void setLeftPositionX(double leftPositionX);

    double getLeftPositionY() const;

    void setLeftPositionY(double leftPositionY);

    double getLeftPositionZ() const;

    void setLeftPositionZ(double leftPositionZ);

    double getLeftOrientationX() const;

    void setLeftOrientationX(double leftOrientationX);

    double getLeftOrientationY() const;

    void setLeftOrientationY(double leftOrientationY);

    double getLeftOrientationZ() const;

    void setLeftOrientationZ(double leftOrientationZ);

    double getLeftOrientationW() const;

    void setLeftOrientationW(double leftOrientationW);

    double getRightPositionX() const;

    void setRightPositionX(double rightPositionX);

    double getRightPositionY() const;

    void setRightPositionY(double rightPositionY);

    double getRightPositionZ() const;

    void setRightPositionZ(double rightPositionZ);

    double getRightOrientationX() const;

    void setRightOrientationX(double rightOrientationX);

    double getRightOrientationY() const;

    void setRightOrientationY(double rightOrientationY);

    double getRightOrientationZ() const;

    void setRightOrientationZ(double rightOrientationZ);

    double getRightOrientationW() const;

    void setRightOrientationW(double rightOrientationW);

    const std::string &getLeftGateId() const;

    void setLeftGateId(const std::string &leftGateId);

    const std::string &getRightGateId() const;

    void setRightGateId(const std::string &rightGateId);

    const std::string &getFactoryId() const;

    void setFactoryId(const std::string &factoryId);
};

class GateSimpleInfo {
    long id{};
    std::string o_map_id;
    double start_x{};
    double start_y{};
    double end_x{};
    double end_y{};

    std::string left_gate_ID{};
    std::string right_gate_ID{};

    std::string factory_ID{};

public:

    GateSimpleInfo();

    GateSimpleInfo(long id, const std::string &oMapId, double startX, double startY, double endX, double endY,
                   const std::string &leftGateId, const std::string &rightGateId, const std::string &factoryId);

    friend void to_json(json &j, const GateSimpleInfo &b) {
        j = json{
                {"id",            b.id},
                {"o_map_id",      b.o_map_id},
                {"start_x",       b.start_x},
                {"start_y",       b.start_y},
                {"end_x",         b.end_x},
                {"end_y",         b.end_y},
                {"left_gate_ID",  b.left_gate_ID},
                {"right_gate_ID", b.right_gate_ID},
                {"factory_ID",    b.factory_ID},
        };
    }

    friend void from_json(const json &j, GateSimpleInfo &b) {
        j.at("id").get_to(b.id);
        j.at("o_map_id").get_to(b.o_map_id);
        j.at("start_x").get_to(b.start_x);
        j.at("start_y").get_to(b.start_y);
        j.at("end_x").get_to(b.end_x);
        j.at("end_y").get_to(b.end_y);
        j.at("left_gate_ID").get_to(b.left_gate_ID);
        j.at("right_gate_ID").get_to(b.right_gate_ID);
        j.at("factory_ID").get_to(b.factory_ID);
    }

    long getId() const;

    void setId(long id);

    const std::string &getOMapId() const;

    void setOMapId(const std::string &oMapId);

    double getStartX() const;

    void setStartX(double startX);

    double getStartY() const;

    void setStartY(double startY);

    double getEndX() const;

    void setEndX(double endX);

    double getEndY() const;

    void setEndY(double endY);

    const std::string &getLeftGateId() const;

    void setLeftGateId(const std::string &leftGateId);

    const std::string &getRightGateId() const;

    void setRightGateId(const std::string &rightGateId);

    const std::string &getFactoryId() const;

    void setFactoryId(const std::string &factoryId);

};

#endif //APP_COMMUNICATION_GATE_INFO_H
