
#ifndef APP_COMMUNICATION_SENSORSELFMODEL_H
#define APP_COMMUNICATION_SENSORSELFMODEL_H

#include <vector>
#include <ostream>
#include "RosImu.h"
#include "RosLaserScan.h"
#include "RosPointCloud2.h"
#include "RosRange.h"

struct SensorSelf {
    int carpet;//地毯
    std::vector<int> collision;//碰撞
    RosImu imu;//imu
    int metal;//金属
    RosOdom odom;
    RosLaserScan scan;//雷达
    RosPointCloud2 silverUp;
    RosPointCloud2 silverDown;
    RosRange tofLeft;
    RosRange tofRight;
    RosRange ultrasonic1;
    RosRange ultrasonic2;
    RosRange ultrasonic3;
    RosRange ultrasonic4;

    SensorSelf() {}

    SensorSelf(int carpet, const std::vector<int> &collision, const RosImu &imu, int metal, const RosOdom &odom,
               const RosLaserScan &scan, const RosPointCloud2 &silverUp, const RosPointCloud2 &silverDown,
               const RosRange &tofLeft, const RosRange &tofRight, const RosRange &ultrasonic1,
               const RosRange &ultrasonic2, const RosRange &ultrasonic3, const RosRange &ultrasonic4) : carpet(carpet),
                                                                                                        collision(
                                                                                                                collision),
                                                                                                        imu(imu),
                                                                                                        metal(metal),
                                                                                                        odom(odom),
                                                                                                        scan(scan),
                                                                                                        silverUp(
                                                                                                                silverUp),
                                                                                                        silverDown(
                                                                                                                silverDown),
                                                                                                        tofLeft(tofLeft),
                                                                                                        tofRight(
                                                                                                                tofRight),
                                                                                                        ultrasonic1(
                                                                                                                ultrasonic1),
                                                                                                        ultrasonic2(
                                                                                                                ultrasonic2),
                                                                                                        ultrasonic3(
                                                                                                                ultrasonic3),
                                                                                                        ultrasonic4(
                                                                                                                ultrasonic4) {}

    friend void to_json(json &j, const SensorSelf &model) {
        j = json{
                {"carpet",      model.carpet},
                {"collision",   model.collision},
                {"imu",         model.imu},
                {"metal",       model.metal},
                {"odom",        model.odom},
                {"scan",        model.scan},
                {"silverUp",    model.silverUp},
                {"silverDown",  model.silverDown},
                {"tofLeft",     model.tofLeft},
                {"tofRight",    model.tofRight},
                {"ultrasonic1", model.ultrasonic1},
                {"ultrasonic2", model.ultrasonic2},
                {"ultrasonic3", model.ultrasonic3},
                {"ultrasonic4", model.ultrasonic4},
        };
    }

    friend void from_json(const json &j, SensorSelf &model) {
        j.at("carpet").get_to(model.carpet);
        j.at("collision").get_to(model.collision);
        j.at("imu").get_to(model.imu);
        j.at("metal").get_to(model.metal);
        j.at("odom").get_to(model.odom);
        j.at("scan").get_to(model.scan);
        j.at("silverUp").get_to(model.silverUp);
        j.at("silverDown").get_to(model.silverDown);
        j.at("tofLeft").get_to(model.tofLeft);
        j.at("tofRight").get_to(model.tofRight);
        j.at("ultrasonic1").get_to(model.ultrasonic1);
        j.at("ultrasonic2").get_to(model.ultrasonic2);
        j.at("ultrasonic3").get_to(model.ultrasonic3);
        j.at("ultrasonic4").get_to(model.ultrasonic4);
    }

    friend std::ostream &operator<<(std::ostream &os, const SensorSelf &self) {
        os << "carpet: " << self.carpet << " collision: " << self.collision.size() << " imu: " << self.imu << " metal: "
           << self.metal << " odom: " << self.odom << " scan: " << self.scan << " silverUp: " << self.silverUp
           << " silverDown: " << self.silverDown << " tofLeft: " << self.tofLeft << " tofRight: " << self.tofRight
           << " ultrasonic1: " << self.ultrasonic1 << " ultrasonic2: " << self.ultrasonic2 << " ultrasonic3: "
           << self.ultrasonic3 << " ultrasonic4: " << self.ultrasonic4;
        return os;
    }
};

#endif //APP_COMMUNICATION_SENSORSELFMODEL_H
