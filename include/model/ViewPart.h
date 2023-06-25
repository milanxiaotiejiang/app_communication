/**
 * @file ViewPart.h
 * @author Quan Li (1204018828@qq.com)
 * @brief 单个part,路径组合用
 * @version 0.1
 * @date 2022-03-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef APP_COMMUNICATION_VIEW_PART_H
#define APP_COMMUNICATION_VIEW_PART_H

#include "WorkStatus.h"
#include "model/Point.h"
#include "vector"
#include <std_msgs/String.h>

using json = nlohmann::json;

class ViewPart {
private:
    int m_mode;
    std::string m_name;
    std::string m_part_id;
    std::vector<Point> m_polygon;
    int m_rate;
    std::vector<Point> m_teach;
    std::string m_path_id;
    WorkStatus m_work_status;
    std::vector<Point> m_zoned;


public:
    friend void to_json(json &j, const ViewPart &b) {
        j = json{
                {"mode",        b.m_mode},
                {"name",        b.m_name},
                {"part_id",     b.m_part_id},
                {"polygon",     b.m_polygon},
                {"rate",        b.m_rate},
                {"teach",       b.m_teach},
                {"path_id",     b.m_path_id},
                {"work_status", b.m_work_status},
                {"zoned",       b.m_zoned}};
    }

    friend void from_json(const json &j, ViewPart &b) {
        j.at("mode").get_to(b.m_mode);
        j.at("name").get_to(b.m_name);
        j.at("part_id").get_to(b.m_part_id);
        j.at("polygon").get_to(b.m_polygon);
        j.at("rate").get_to(b.m_rate);
        j.at("teach").get_to(b.m_teach);
        j.at("path_id").get_to(b.m_path_id);
        j.at("work_status").get_to(b.m_work_status);
        j.at("zoned").get_to(b.m_zoned);
    }

    ViewPart(int mode,
             std::string name,
             std::string part_id,
             std::vector<Point> polygon,
             int rate,
             std::vector<Point> teach,
             std::string path_id,
             WorkStatus work_status,
             std::vector<Point> zoned) : m_mode(mode),
                                         m_name(name),
                                         m_part_id(part_id),
                                         m_polygon(polygon),
                                         m_rate(rate),
                                         m_teach(teach),
                                         m_path_id(path_id),
                                         m_work_status(work_status) {}


    ViewPart() {}

    ~ViewPart() {}

    const int &getMode() const;

    void setMode(const int &mode);

    const std::string &getName() const;

    void setName(const std::string &name);

    const std::string &getPartID() const;

    void setPartID(const std::string &part_id);

    const std::vector<Point> &getPolygon() const;

    void setPolygon(const std::vector<Point> &polygon);

    const int &getRate() const;

    void setRate(const int &rate);

    const std::vector<Point> &getTeach() const;

    void setTeach(const std::vector<Point> &teach);

    const std::string &getPathID() const;

    void setPathID(const std::string &path_id);

    const WorkStatus &getWorkStatus() const;

    void setWorkStatus(const WorkStatus &work_status);

    const std::vector<Point> &getZoned() const;

    void setZoned(const std::vector<Point> &zoned);

    void showViewPart();

    void ShowPointList(std::vector<Point> point_list);
};

class ViewPartList {
private:
    std::vector<ViewPart> m_view_part_list;

public:
    friend void to_json(json &j, const ViewPartList &b) {
        j = json{
                {"m_view_part_list", b.m_view_part_list},
        };
    }

    friend void from_json(const json &j, ViewPartList &b) {
        j.at("m_view_part_list").get_to(b.m_view_part_list);
    }

    ViewPartList();

    ~ViewPartList();

    const std::vector<ViewPart> &GetViewPartList() const;

    void SetViewPartList(const std::vector<ViewPart> &view_part_list);

    bool AddViewPart(const ViewPart &view_part);

    bool CheckViewPart(const std::string &part_id);

    bool GetViewPart(ViewPart &view_part, const std::string &part_id);

    bool ResetViewPart(const ViewPart &view_part, const std::string &part_id);

    bool DelateViewPart(const std::string &part_id);

    bool SearchName(const std::string &part_name);

    void ShowAllViewPart();
};


#endif
