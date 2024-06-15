
#ifndef APP_COMMUNICATION_ROSPOINTCLOUD2_H
#define APP_COMMUNICATION_ROSPOINTCLOUD2_H

#include <ostream>
#include "RosBasic.h"

class RosPointField {
private:
    std::string name;
    int offset;
    int datatype;
    int count;
public:
    RosPointField() {}

    RosPointField(const std::string &name, int offset, int datatype, int count) : name(name), offset(offset),
                                                                                  datatype(datatype), count(count) {}

    friend void to_json(json &j, const RosPointField &model) {
        j = json{
                {"name",     model.name},
                {"offset",   model.offset},
                {"datatype", model.datatype},
                {"count",    model.count},
        };
    }

    friend void from_json(const json &j, RosPointField &model) {
        j.at("name").get_to(model.name);
        j.at("offset").get_to(model.offset);
        j.at("datatype").get_to(model.datatype);
        j.at("count").get_to(model.count);
    }

    friend std::ostream &operator<<(std::ostream &os, const RosPointField &field) {
        os << "name: " << field.name << " offset: " << field.offset << " datatype: " << field.datatype << " count: "
           << field.count;
        return os;
    }
};

class RosPointCloud2 {
private:
    RosHeader header;
    int height;
    int width;
    std::vector<RosPointField> fields;
    int is_bigendian;
    int point_step;
    int row_step;
    std::vector<int> data;
    int is_dense;

public:
    RosPointCloud2() {}

    RosPointCloud2(const RosHeader &header, int height, int width, const std::vector<RosPointField> &fields,
                   int isBigendian, int pointStep, int rowStep, const std::vector<int> &data, int isDense) : header(
            header), height(height), width(width), fields(fields), is_bigendian(isBigendian), point_step(pointStep),
                                                                                                             row_step(
                                                                                                                     rowStep),
                                                                                                             data(data),
                                                                                                             is_dense(
                                                                                                                     isDense) {}


    friend void to_json(json &j, const RosPointCloud2 &model) {
        j = json{
                {"header",       model.header},
                {"height",       model.height},
                {"width",        model.width},
                {"fields",       model.fields},
                {"is_bigendian", model.is_bigendian},
                {"point_step",   model.point_step},
                {"row_step",     model.row_step},
//                {"data",         model.data},
                {"is_dense",     model.is_dense},
        };
    }

    friend void from_json(const json &j, RosPointCloud2 &model) {
        j.at("header").get_to(model.header);
        j.at("height").get_to(model.height);
        j.at("width").get_to(model.width);
        j.at("fields").get_to(model.fields);
        j.at("is_bigendian").get_to(model.is_bigendian);
        j.at("point_step").get_to(model.point_step);
        j.at("row_step").get_to(model.row_step);
//        j.at("data").get_to(model.data);
        j.at("is_dense").get_to(model.is_dense);
    }

    friend std::ostream &operator<<(std::ostream &os, const RosPointCloud2 &cloud2) {
        os << "header: " << cloud2.header << " height: " << cloud2.height << " width: " << cloud2.width << " fields: "
           << cloud2.fields.size() << " is_bigendian: " << cloud2.is_bigendian << " point_step: " << cloud2.point_step
           << " row_step: " << cloud2.row_step << " data: " << cloud2.data.size() << " is_dense: " << cloud2.is_dense;
        return os;
    }
};

#endif //APP_COMMUNICATION_ROSPOINTCLOUD2_H
