
#ifndef APP_COMMUNICATION_ROSRANGE_H
#define APP_COMMUNICATION_ROSRANGE_H

#include <ostream>
#include "RosBasic.h"

class RosRange {
private:
    RosHeader header;
    int radiation_type;
    float field_of_view;
    float min_range;
    float max_range;
    float range;
public:
    RosRange() {}

    RosRange(const RosHeader &header, int radiationType, float fieldOfView, float minRange, float maxRange, float range)
            : header(header), radiation_type(radiationType), field_of_view(fieldOfView), min_range(minRange),
              max_range(maxRange), range(range) {}

    friend void to_json(json &j, const RosRange &model) {
        j = json{
                {"header",         model.header},
                {"radiation_type", model.radiation_type},
                {"field_of_view",  model.field_of_view},
                {"min_range",      model.min_range},
                {"max_range",      model.max_range},
                {"range",          model.range}
        };
    }

    friend void from_json(const json &j, RosRange &model) {
        j.at("header").get_to(model.header);
        j.at("radiation_type").get_to(model.radiation_type);
        j.at("field_of_view").get_to(model.field_of_view);
        j.at("min_range").get_to(model.min_range);
        j.at("max_range").get_to(model.max_range);
        j.at("range").get_to(model.range);
    }

    friend std::ostream &operator<<(std::ostream &os, const RosRange &range) {
        os << "header: " << range.header << " radiation_type: " << range.radiation_type << " field_of_view: "
           << range.field_of_view << " min_range: " << range.min_range << " max_range: " << range.max_range
           << " range: " << range.range;
        return os;
    }
};

#endif //APP_COMMUNICATION_ROSRANGE_H
