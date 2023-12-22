//
// Created by io on 23-12-19.
//

#ifndef APP_COMMUNICATION_ELE_PROTOCOL_H
#define APP_COMMUNICATION_ELE_PROTOCOL_H


#include <vector>
#include <cstdint>
#include <iostream>

struct ElevatorStatus {
    enum class DoorState {
        Unknown = 0, Open, Closed
    };
    enum class LastDirection {
        Unknown = 0, Up, Down
    };
    enum class Availability {
        Disabled = 0, Enabled
    };
    enum class NextDirection {
        Unavailable = 3, Unknown = 0, Up, Down
    };

    DoorState doorState;
    LastDirection lastDirection;
    Availability availability;
    NextDirection nextDirection;
};

struct EleStatus {
    uint8_t floor;//某个楼层”用数字表示，1 表示：1 层，2 表示：2 层……189，表示 189 层；190~200 表示部署中的虚拟楼层；201 表示地下一层，202 表示地下二层……210 表示地下十层。此楼层为按钮显示楼层，非物理楼层。
    /*
     Bit7-Bit6：电梯门状态（未知：00、打开：01、关闭：10）
     Bit5-Bit4：历史运行方向（未知：00、上行：01、下行：10）
     Bit3-Bit2：是否停用（停用：00、可用：01）
     Bit1-Bit0：未来运行方向（不可用：11、未知：00、上行：01、下行：10）
     */
    int elevatorStatus;
    int residenceTime;
    int openTime;//时间”占用 1 个字节，单位 S/秒，默认 5 秒，可用时间范围 1-9s，时间不累计，执行时间以最新通信成功附带时间为准。
    int historicalFloors;

    static uint8_t reverseFloorRule(int floor) {
        if (floor >= 1 && floor <= 189) {
            // 1~189 表示实际的楼层
            return static_cast<uint8_t>(floor);
        } else if (floor >= 190 && floor <= 200) {
            // 190~200 表示虚拟楼层
            return static_cast<uint8_t>(floor);
        } else if (floor >= -10 && floor <= -1) {
            // 地下 1~10 楼
            return static_cast<uint8_t>(210 + floor); // -1 对应 209, -10 对应 200
        } else {
            // 如果楼层号不在已定义的范围内，可以返回一个错误值或抛出异常
            return static_cast<uint8_t>(0); // 作为错误标志
        }
    }

    static int floorRule(uint8_t floorByte) {
        if (floorByte >= 1 && floorByte <= 189) {
            // 1~189 表示实际的楼层
            return floorByte;
        } else if (floorByte >= 190 && floorByte <= 200) {
            // 190~200 表示部署中的虚拟楼层
            return floorByte;
        } else if (floorByte >= 201 && floorByte <= 210) {
            // 201~210 表示地下楼层
            return -1 * (floorByte - 200);
        } else {
            // 如果楼层代码不在已定义的范围内，可以返回一个错误值或抛出异常
            return -999; // 作为错误标志
        }
    }

    static ElevatorStatus parseElevatorStatus(uint8_t statusByte) {
        ElevatorStatus status;
        status.doorState = static_cast<ElevatorStatus::DoorState>((statusByte >> 6) & 0x03);
        status.lastDirection = static_cast<ElevatorStatus::LastDirection>((statusByte >> 4) & 0x03);
        status.availability = static_cast<ElevatorStatus::Availability>((statusByte >> 2) & 0x01);
        status.nextDirection = static_cast<ElevatorStatus::NextDirection>(statusByte & 0x03);
        return status;
    }
};

class EleProtocol {
public:
    EleProtocol(uint8_t cmd) : cmd(cmd) {
        // 初始化默认值
        header = {0x7f, 0xf7};
        flag = 0x29;
        address = {0x10, 0x27};
        mac = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
        uniqueCode = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10, 0x11, 0x12};
    }

    EleProtocol(const std::vector<uint8_t> &header, uint8_t length, uint8_t flag, const std::vector<uint8_t> &address,
                const std::vector<uint8_t> &mac, uint8_t dataLength, uint8_t cmd, const std::vector<uint8_t> &data,
                uint8_t checksum) : header(header), length(length), flag(flag), address(address), mac(mac),
                                    dataLength(dataLength), cmd(cmd), data(data), checksum(checksum) {}

    void setData(const std::vector<uint8_t> &data) {
        this->data = data;
    }

    void setAddress(const std::vector<uint8_t> &address) {
        this->address = address;
    }

    std::vector<uint8_t> getProtocol() {
        calculateLength();
        calculateChecksum();

        std::vector<uint8_t> protocol;
        protocol.insert(protocol.end(), header.begin(), header.end());
        protocol.push_back(length);
        protocol.push_back(flag);
        protocol.insert(protocol.end(), address.begin(), address.end());
        protocol.insert(protocol.end(), mac.begin(), mac.end());
        protocol.push_back(dataLength);
        protocol.push_back(cmd);
        protocol.insert(protocol.end(), data.begin(), data.end());
        protocol.insert(protocol.end(), uniqueCode.begin(), uniqueCode.end());
        protocol.push_back(checksum);
        return protocol;
    }

    static bool checkCalculateChecksum(uint8_t length, uint8_t flag,
                                       const std::vector<uint8_t> &address,
                                       const std::vector<uint8_t> &mac,
                                       uint8_t dataLength, uint8_t cmd,
                                       const std::vector<uint8_t> &data,
                                       uint8_t sum
    ) {
        uint8_t checksum = flag ^ length;
        for (auto byte: address) checksum ^= byte;
        for (auto byte: mac) checksum ^= byte;
        checksum ^= dataLength;
        checksum ^= cmd;
        for (auto byte: data) checksum ^= byte;
//        for (auto byte: uniqueCode) checksum ^= byte;
        return checksum == sum;
    }

    static EleProtocol errorEleProtocol() {
        return EleProtocol();
    }

private:
    std::vector<uint8_t> header;//2
    uint8_t length;//1
    uint8_t flag;//1
    std::vector<uint8_t> address;//2
    std::vector<uint8_t> mac;//6
    uint8_t dataLength;//1
    uint8_t cmd;//1
    std::vector<uint8_t> data;
    std::vector<uint8_t> uniqueCode;
    uint8_t checksum;//1

    EleProtocol() = default;

    void calculateLength() {
        dataLength = data.size() + uniqueCode.size();
        //Flag + Addr + MAC + Len + Cmd/Resp + Data + UniqueCode
        length = 1 + address.size() + mac.size() + 1 + 1 + dataLength;
    }

    void calculateChecksum() {
        checksum = flag ^ length;
        for (auto byte: address) checksum ^= byte;
        for (auto byte: mac) checksum ^= byte;
        checksum ^= dataLength;
        checksum ^= cmd;
        for (auto byte: data) checksum ^= byte;
        for (auto byte: uniqueCode) checksum ^= byte;
    }

    friend std::ostream &operator<<(std::ostream &os, const EleProtocol &protocol) {
//        os << "header: ";
//        for (auto byte: protocol.header) os << std::hex << static_cast<int>(byte) << " ";
//        os << std::endl;
//        os << "length: " << std::hex << static_cast<int>(protocol.length) << std::endl;
//        os << "flag: " << std::hex << static_cast<int>(protocol.flag) << std::endl;
//        os << "address: ";
//        for (auto byte: protocol.address) os << std::hex << static_cast<int>(byte) << " ";
//        os << std::endl;
//        os << "mac: ";
//        for (auto byte: protocol.mac) os << std::hex << static_cast<int>(byte) << " ";
//        os << std::endl;
//        os << "dataLength: " << std::hex << static_cast<int>(protocol.dataLength) << std::endl;
//        os << "cmd: " << std::hex << static_cast<int>(protocol.cmd) << std::endl;
//        os << "data: ";
//        for (auto byte: protocol.data) os << std::hex << static_cast<int>(byte) << " ";
//        os << std::endl;
//        os << "uniqueCode: ";
//        for (auto byte: protocol.uniqueCode) os << std::hex << static_cast<int>(byte) << " ";
//        os << std::endl;
//        os << "checksum: " << std::hex << static_cast<int>(protocol.checksum) << std::endl;
        os << "cmd: " << std::hex << static_cast<int>(protocol.cmd);
        return os;
    }
};

#endif //APP_COMMUNICATION_ELE_PROTOCOL_H
