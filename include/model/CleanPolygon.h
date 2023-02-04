#ifndef C_PTHREAD_CLEANPOLYGONTTASK_H
#define C_PTHREAD_CLEANPOLYGONTTASK_H

#include "model/Polygon_info.h"
#include "nlohmann/json.hpp"

using namespace std;

using json = nlohmann::json;

class CleanPolygon {
private:
    std::string polygon_id;
    bool is_customized;
    std::vector<Polygon_info> polygon_list;

public:
    virtual ~CleanPolygon();

    CleanPolygon();

    CleanPolygon(std::string polygonid, bool isCustomized);

    const std::string &getPolygonId() const;

    void setPolygonId(const std::string &polygonId);


    bool isCustomized() const;

    void setIsCustomized(bool isCustomized);

    const vector<Polygon_info> &getPolygonList() const;

    void setPolygonList(const vector<Polygon_info> &polygonList);

    friend void to_json(json &j, const CleanPolygon &b) {
        j = json{
                {"polygon_id",    b.polygon_id},
                {"is_customized", b.is_customized},
                {"polygon_list",  b.polygon_list},
        };
    }

    friend void from_json(const json &j, CleanPolygon &b) {
        j.at("polygon_id").get_to(b.polygon_id);
        j.at("is_customized").get_to(b.is_customized);
        j.at("polygon_list").get_to(b.polygon_list);
    }

};

#endif //C_PTHREAD_POLYGONLISTTASK_H