//
// Created by io on 24-2-27.
//

#ifndef APP_COMMUNICATION_ELEVATORSTATUS_H
#define APP_COMMUNICATION_ELEVATORSTATUS_H

#include <ostream>
#include "string"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class ElevatorModel {
private:
    int floor;
    int doorState;
    int lastDirection;
    int availability;
    int nextDirection;
public:
    ElevatorModel() {}

    ElevatorModel(int floor, int doorState, int lastDirection, int availability, int nextDirection)
            : floor(floor),
              doorState(doorState),
              lastDirection(lastDirection),
              availability(availability),
              nextDirection(nextDirection) {}

    int getFloor() const {
        return floor;
    }

    void setFloor(int floor) {
        ElevatorModel::floor = floor;
    }

    int getDoorState() const {
        return doorState;
    }

    void setDoorState(int doorState) {
        ElevatorModel::doorState = doorState;
    }

    int getLastDirection() const {
        return lastDirection;
    }

    void setLastDirection(int lastDirection) {
        ElevatorModel::lastDirection = lastDirection;
    }

    int getAvailability() const {
        return availability;
    }

    void setAvailability(int availability) {
        ElevatorModel::availability = availability;
    }

    int getNextDirection() const {
        return nextDirection;
    }

    void setNextDirection(int nextDirection) {
        ElevatorModel::nextDirection = nextDirection;
    }


    friend void to_json(json &j, const ElevatorModel &b) {
        j = json{
                {"floor",         b.floor},
                {"doorState",     b.doorState},
                {"lastDirection", b.lastDirection},
                {"availability",  b.availability},
                {"nextDirection", b.nextDirection}
        };
    }

    friend void from_json(const json &j, ElevatorModel &b) {
        j.at("floor").get_to(b.floor);
        j.at("doorState").get_to(b.doorState);
        j.at("lastDirection").get_to(b.lastDirection);
        j.at("availability").get_to(b.availability);
        j.at("nextDirection").get_to(b.nextDirection);
    }
};

#endif //APP_COMMUNICATION_ELEVATORSTATUS_H
