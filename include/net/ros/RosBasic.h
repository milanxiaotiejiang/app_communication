//
// Created by mi on 2022/4/13.
//

#ifndef APP_COMMUNICATION_ROSBASIC_H
#define APP_COMMUNICATION_ROSBASIC_H

#include "iostream"
#include "string"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

typedef boost::array<double, 36> _covariance_type;

class RosMapLoadTime {
private:
    int nsecs;
    int secs;
public:
    RosMapLoadTime() = default;

    RosMapLoadTime(int nsecs, int secs) : nsecs(nsecs), secs(secs) {}

    virtual ~RosMapLoadTime() = default;

    int getNsecs() const {
        return nsecs;
    }

    void setNsecs(int nsecs) {
        RosMapLoadTime::nsecs = nsecs;
    }

    int getSecs() const {
        return secs;
    }

    void setSecs(int secs) {
        RosMapLoadTime::secs = secs;
    }

    friend std::ostream &operator<<(std::ostream &os, const RosMapLoadTime &oadTime) {
        os << "nsecs: " << oadTime.nsecs << " secs: " << oadTime.secs;
        return os;
    }

    friend void to_json(json &j, const RosMapLoadTime &model) {
        j = json{
                {"nsecs", model.nsecs},
                {"secs",  model.secs}
        };
    }

    friend void from_json(const json &j, RosMapLoadTime &model) {
        j.at("nsecs").get_to(model.nsecs);
        j.at("secs").get_to(model.secs);
    }
};

class RosStamp {
private:
    int nsecs;
    int secs;
public:
    RosStamp() = default;

    RosStamp(int nsecs, int secs) : nsecs(nsecs), secs(secs) {}

    virtual ~RosStamp() = default;

    int getNsecs() const {
        return nsecs;
    }

    void setNsecs(int nsecs) {
        RosStamp::nsecs = nsecs;
    }

    int getSecs() const {
        return secs;
    }

    void setSecs(int secs) {
        RosStamp::secs = secs;
    }

    friend std::ostream &operator<<(std::ostream &os, const RosStamp &stamp) {
        os << "nsecs: " << stamp.nsecs << " secs: " << stamp.secs;
        return os;
    }

    friend void to_json(json &j, const RosStamp &model) {
        j = json{
                {"nsecs", model.nsecs},
                {"secs",  model.secs}
        };
    }

    friend void from_json(const json &j, RosStamp &model) {
        j.at("nsecs").get_to(model.nsecs);
        j.at("secs").get_to(model.secs);
    }
};

class RosHeader {
private:
    std::string frame_id;
    int seq;
    RosStamp stamp;
public:
    RosHeader() = default;

    RosHeader(const std::string &frameId, int seq, const RosStamp &stamp) : frame_id(frameId), seq(seq), stamp(stamp) {}

    virtual ~RosHeader() = default;

    const std::string &getFrameId() const {
        return frame_id;
    }

    void setFrameId(const std::string &frameId) {
        frame_id = frameId;
    }

    int getSeq() const {
        return seq;
    }

    void setSeq(int seq) {
        RosHeader::seq = seq;
    }

    const RosStamp &getStamp() const {
        return stamp;
    }

    void setStamp(const RosStamp &stamp) {
        RosHeader::stamp = stamp;
    }

    friend std::ostream &operator<<(std::ostream &os, const RosHeader &header) {
        os << "frame_id: " << header.frame_id << " seq: " << header.seq << " stamp: " << header.stamp;
        return os;
    }

    friend void to_json(json &j, const RosHeader &model) {
        j = json{
                {"frame_id", model.frame_id},
                {"seq",      model.seq},
                {"stamp",    model.stamp}
        };
    }

    friend void from_json(const json &j, RosHeader &model) {
        j.at("frame_id").get_to(model.frame_id);
        j.at("seq").get_to(model.seq);
        j.at("stamp").get_to(model.stamp);
    }
};

class RosOrientation {
private:
    double w;
    double x;
    double y;
    double z;
public:
    RosOrientation() = default;

    RosOrientation(double w, double x, double y, double z) : w(w), x(x), y(y), z(z) {}

    virtual ~RosOrientation() = default;

    double getW() const {
        return w;
    }

    void setW(double w) {
        RosOrientation::w = w;
    }

    double getX() const {
        return x;
    }

    void setX(double x) {
        RosOrientation::x = x;
    }

    double getY() const {
        return y;
    }

    void setY(double y) {
        RosOrientation::y = y;
    }

    double getZ() const {
        return z;
    }

    void setZ(double z) {
        RosOrientation::z = z;
    }

    friend std::ostream &operator<<(std::ostream &os, const RosOrientation &orientation) {
        os << "w: " << orientation.w << " x: " << orientation.x << " y: " << orientation.y << " z: " << orientation.z;
        return os;
    }

    friend void to_json(json &j, const RosOrientation &model) {
        j = json{
                {"w", model.w},
                {"x", model.x},
                {"y", model.y},
                {"z", model.z}
        };
    }

    friend void from_json(const json &j, RosOrientation &model) {
        j.at("w").get_to(model.w);
        j.at("x").get_to(model.x);
        j.at("y").get_to(model.y);
        j.at("z").get_to(model.z);
    }
};

class RosPosition {
private:
    double x;
    double y;
    double z;
public:
    RosPosition() = default;

    RosPosition(double x, double y, double z) : x(x), y(y), z(z) {}

    virtual ~RosPosition() = default;

    double getX() const {
        return x;
    }

    void setX(double x) {
        RosPosition::x = x;
    }

    double getY() const {
        return y;
    }

    void setY(double y) {
        RosPosition::y = y;
    }

    double getZ() const {
        return z;
    }

    void setZ(double z) {
        RosPosition::z = z;
    }

    friend std::ostream &operator<<(std::ostream &os, const RosPosition &position) {
        os << "x: " << position.x << " y: " << position.y << " z: " << position.z;
        return os;
    }

    friend void to_json(json &j, const RosPosition &model) {
        j = json{
                {"x", model.x},
                {"y", model.y},
                {"z", model.z}
        };
    }

    friend void from_json(const json &j, RosPosition &model) {
        j.at("x").get_to(model.x);
        j.at("y").get_to(model.y);
        j.at("z").get_to(model.z);
    }
};

class RosOrigin {
private:
    RosOrientation orientation;
    RosPosition position;
public:
    RosOrigin() = default;

    RosOrigin(const RosOrientation &orientation, const RosPosition &position) : orientation(orientation),
                                                                                position(position) {}

    virtual ~RosOrigin() = default;

    const RosOrientation &getOrientation() const {
        return orientation;
    }

    void setOrientation(const RosOrientation &orientation) {
        RosOrigin::orientation = orientation;
    }

    const RosPosition &getPosition() const {
        return position;
    }

    void setPosition(const RosPosition &position) {
        RosOrigin::position = position;
    }

    friend std::ostream &operator<<(std::ostream &os, const RosOrigin &origin) {
        os << "orientation: " << origin.orientation << " position: " << origin.position;
        return os;
    }

    friend void to_json(json &j, const RosOrigin &model) {
        j = json{
                {"orientation", model.orientation},
                {"position",    model.position}
        };
    }

    friend void from_json(const json &j, RosOrigin &model) {
        j.at("orientation").get_to(model.orientation);
        j.at("position").get_to(model.position);
    }
};

class RosInfo {
private:
    int width;
    int height;
    double resolution;
    RosMapLoadTime map_load_time;
    RosOrigin origin;
public:
    RosInfo() = default;

    RosInfo(int width, int height, double resolution, const RosMapLoadTime &mapLoadTime, const RosOrigin &origin)
            : width(width), height(height), resolution(resolution), map_load_time(mapLoadTime), origin(origin) {}

    virtual ~RosInfo() = default;

    int getWidth() const {
        return width;
    }

    void setWidth(int width) {
        RosInfo::width = width;
    }

    int getHeight() const {
        return height;
    }

    void setHeight(int height) {
        RosInfo::height = height;
    }

    double getResolution() const {
        return resolution;
    }

    void setResolution(double resolution) {
        RosInfo::resolution = resolution;
    }

    const RosMapLoadTime &getMapLoadTime() const {
        return map_load_time;
    }

    void setMapLoadTime(const RosMapLoadTime &mapLoadTime) {
        map_load_time = mapLoadTime;
    }

    const RosOrigin &getOrigin() const {
        return origin;
    }

    void setOrigin(const RosOrigin &origin) {
        RosInfo::origin = origin;
    }

    friend std::ostream &operator<<(std::ostream &os, const RosInfo &info) {
        os << "width: " << info.width << " height: " << info.height << " resolution: " << info.resolution
           << " map_load_time: " << info.map_load_time << " origin: " << info.origin;
        return os;
    }

    friend void to_json(json &j, const RosInfo &model) {
        j = json{
                {"width",         model.width},
                {"height",        model.height},
                {"resolution",    model.resolution},
                {"map_load_time", model.map_load_time},
                {"origin",        model.origin}
        };
    }

    friend void from_json(const json &j, RosInfo &model) {
        j.at("width").get_to(model.width);
        j.at("height").get_to(model.height);
        j.at("resolution").get_to(model.resolution);
        j.at("map_load_time").get_to(model.map_load_time);
        j.at("origin").get_to(model.origin);
    }
};


class RosPose {
private:
    RosOrigin pose;
    std::vector<double> covariance;
public:

    RosPose() = default;

    RosPose(const RosOrigin &pose, const std::vector<double> &covariance) : pose(pose), covariance(covariance) {}

    virtual ~RosPose() = default;

    const RosOrigin &getPose() const {
        return pose;
    }

    void setPose(const RosOrigin &pose) {
        RosPose::pose = pose;
    }

    const std::vector<double> &getCovariance() const {
        return covariance;
    }

    void setCovariance(const std::vector<double> &covariance) {
        RosPose::covariance = covariance;
    }

    friend std::ostream &operator<<(std::ostream &os, const RosPose &rosPose) {
        os << "pose: " << rosPose.pose << " covariance: " << rosPose.covariance.size();
        return os;
    }

    friend void to_json(json &j, const RosPose &model) {
        j = json{
                {"pose",       model.pose},
                {"covariance", model.covariance}
        };
    }

    friend void from_json(const json &j, RosPose &model) {
        j.at("pose").get_to(model.pose);
        j.at("covariance").get_to(model.covariance);
    }

};

class RosAngular {
private:
    float x;
    float y;
    float z;
public:
    RosAngular() = default;

    RosAngular(float x, float y, float z) : x(x), y(y), z(z) {}

    virtual ~RosAngular() = default;

    float getX() const {
        return x;
    }

    void setX(float x) {
        RosAngular::x = x;
    }

    float getY() const {
        return y;
    }

    void setY(float y) {
        RosAngular::y = y;
    }

    float getZ() const {
        return z;
    }

    void setZ(float z) {
        RosAngular::z = z;
    }

    friend std::ostream &operator<<(std::ostream &os, const RosAngular &angular) {
        os << "x: " << angular.x << " y: " << angular.y << " z: " << angular.z;
        return os;
    }

    friend void to_json(json &j, const RosAngular &model) {
        j = json{
                {"x", model.x},
                {"y", model.y},
                {"z", model.z}
        };
    }

    friend void from_json(const json &j, RosAngular &model) {
        j.at("x").get_to(model.x);
        j.at("y").get_to(model.y);
        j.at("z").get_to(model.z);
    }
};

class RosLinear {
private:
    float x;
    float y;
    float z;
public:
    RosLinear() = default;

    RosLinear(float x, float y, float z) : x(x), y(y), z(z) {}

    virtual ~RosLinear() = default;

    float getX() const {
        return x;
    }

    void setX(float x) {
        RosLinear::x = x;
    }

    float getY() const {
        return y;
    }

    void setY(float y) {
        RosLinear::y = y;
    }

    float getZ() const {
        return z;
    }

    void setZ(float z) {
        RosLinear::z = z;
    }

    friend std::ostream &operator<<(std::ostream &os, const RosLinear &linear) {
        os << "x: " << linear.x << " y: " << linear.y << " z: " << linear.z;
        return os;
    }

    friend void to_json(json &j, const RosLinear &model) {
        j = json{
                {"x", model.x},
                {"y", model.y},
                {"z", model.z}
        };
    }

    friend void from_json(const json &j, RosLinear &model) {
        j.at("x").get_to(model.x);
        j.at("y").get_to(model.y);
        j.at("z").get_to(model.z);
    }
};

class RosTwistX {
private:
    RosAngular angular;
    RosLinear linear;
public:
    RosTwistX() = default;

    RosTwistX(const RosAngular &angular, const RosLinear &linear) : angular(angular), linear(linear) {}

    virtual ~RosTwistX() = default;

    const RosAngular &getAngular() const {
        return angular;
    }

    void setAngular(const RosAngular &angular) {
        RosTwistX::angular = angular;
    }

    const RosLinear &getLinear() const {
        return linear;
    }

    void setLinear(const RosLinear &linear) {
        RosTwistX::linear = linear;
    }

    friend std::ostream &operator<<(std::ostream &os, const RosTwistX &x) {
        os << "angular: " << x.angular << " linear: " << x.linear;
        return os;
    }

    friend void to_json(json &j, const RosTwistX &model) {
        j = json{
                {"angular", model.angular},
                {"linear",  model.linear}
        };
    }

    friend void from_json(const json &j, RosTwistX &model) {
        j.at("angular").get_to(model.angular);
        j.at("linear").get_to(model.linear);
    }

};

class RosTwist {
private:
    RosTwistX twist;
    std::vector<double> covariance;
public:
    RosTwist() = default;

    RosTwist(const RosTwistX &twist, const std::vector<double> &covariance) : twist(twist), covariance(covariance) {}

    virtual ~RosTwist() {

    }

    const RosTwistX &getTwist() const {
        return twist;
    }

    void setTwist(const RosTwistX &twist) {
        RosTwist::twist = twist;
    }

    const std::vector<double> &getCovariance() const {
        return covariance;
    }

    void setCovariance(const std::vector<double> &covariance) {
        RosTwist::covariance = covariance;
    }

    friend std::ostream &operator<<(std::ostream &os, const RosTwist &twist) {
        os << "twist: " << twist.twist << " covariance: " << twist.covariance.size();
        return os;
    }

    friend void to_json(json &j, const RosTwist &model) {
        j = json{
                {"twist",      model.twist},
                {"covariance", model.covariance}
        };
    }

    friend void from_json(const json &j, RosTwist &model) {
        j.at("twist").get_to(model.twist);
        j.at("covariance").get_to(model.covariance);
    }
};

class RosOdom {
private:
    std::string child_frame_id;
    RosHeader header;
    RosPose pose;
    RosTwist twist;
public:
    RosOdom() = default;

    RosOdom(const std::string &childFrameId, const RosHeader &header, const RosPose &pose, const RosTwist &twist)
            : child_frame_id(childFrameId), header(header), pose(pose), twist(twist) {}

    virtual ~RosOdom() = default;

    const std::string &getChildFrameId() const {
        return child_frame_id;
    }

    void setChildFrameId(const std::string &childFrameId) {
        child_frame_id = childFrameId;
    }

    const RosHeader &getHeader() const {
        return header;
    }

    void setHeader(const RosHeader &header) {
        RosOdom::header = header;
    }

    const RosPose &getPose() const {
        return pose;
    }

    void setPose(const RosPose &pose) {
        RosOdom::pose = pose;
    }

    const RosTwist &getTwist() const {
        return twist;
    }

    void setTwist(const RosTwist &twist) {
        RosOdom::twist = twist;
    }

    friend std::ostream &operator<<(std::ostream &os, const RosOdom &odom) {
        os << "child_frame_id: " << odom.child_frame_id << " header: " << odom.header << " pose: " << odom.pose
           << " twist: " << odom.twist;
        return os;
    }

    friend void to_json(json &j, const RosOdom &model) {
        j = json{
                {"child_frame_id", model.child_frame_id},
                {"header",         model.header},
                {"pose",           model.pose},
                {"twist",          model.twist}
        };
    }

    friend void from_json(const json &j, RosOdom &model) {
        j.at("child_frame_id").get_to(model.child_frame_id);
        j.at("header").get_to(model.header);
        j.at("pose").get_to(model.pose);
        j.at("twist").get_to(model.twist);
    }

};

class RosMap {
private:
    std::vector<int> data;
    RosHeader header;
    RosInfo info;
public:
    RosMap() = default;

    RosMap(const std::vector<int> &data, const RosHeader &header, const RosInfo &info) : data(data), header(header),
                                                                                    info(info) {}

    virtual ~RosMap() = default;

    const std::vector<int> &getData() const {
        return data;
    }

    void setData(const std::vector<int> &data) {
        RosMap::data = data;
    }

    const RosHeader &getHeader() const {
        return header;
    }

    void setHeader(const RosHeader &header) {
        RosMap::header = header;
    }

    const RosInfo &getInfo() const {
        return info;
    }

    void setInfo(const RosInfo &info) {
        RosMap::info = info;
    }

    friend std::ostream &operator<<(std::ostream &os, const RosMap &map) {
        os << "data: " << map.data.size() << " header: " << map.header << " info: " << map.info;
        return os;
    }

    friend void to_json(json &j, const RosMap &model) {
        j = json{
                {"data",   model.data},
                {"header", model.header},
                {"info",   model.info}
        };
    }

    friend void from_json(const json &j, RosMap &model) {
        j.at("data").get_to(model.data);
        j.at("header").get_to(model.header);
        j.at("info").get_to(model.info);
    }

};


class GRosMap {
private:
    std::string data;
    RosHeader header;
    RosInfo info;
public:
    GRosMap() = default;

    GRosMap(const std::string &data, const RosHeader &header, const RosInfo &info) : data(data), header(header),
                                                                                     info(info) {}

    virtual ~GRosMap() = default;

    const std::string &getData() const {
        return data;
    }

    void setData(const std::string &data) {
        GRosMap::data = data;
    }

    const RosHeader &getHeader() const {
        return header;
    }

    void setHeader(const RosHeader &header) {
        GRosMap::header = header;
    }

    const RosInfo &getInfo() const {
        return info;
    }

    void setInfo(const RosInfo &info) {
        GRosMap::info = info;
    }

    friend std::ostream &operator<<(std::ostream &os, const GRosMap &map) {
        os << "data: " << map.data.size() << " header: " << map.header << " info: " << map.info;
        return os;
    }

    friend void to_json(json &j, const GRosMap &model) {
        j = json{
                {"data",   model.data},
                {"header", model.header},
                {"info",   model.info}
        };
    }

    friend void from_json(const json &j, GRosMap &model) {
        j.at("data").get_to(model.data);
        j.at("header").get_to(model.header);
        j.at("info").get_to(model.info);
    }

};

#endif //APP_COMMUNICATION_ROSBASIC_H
