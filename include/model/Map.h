#ifndef Map_info
#define Map_info

#pragma once

#include <stdlib.h>
#include <vector>

#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace Map_info
{
    class Map_content {
    public:
        Map_content() {};

        Map_content(
                float resolution,
                int width,
                int height,
                int origin_position_x,
                int origin_position_y,
                std::vector<int8_t> data
        );

        ~Map_content() {};

    public:
        float resolution;
        int width;
        int height;
        int origin_position_x;
        int origin_position_y;
        std::vector<int8_t> data;

        friend void to_json(json &j, const Map_content &b) {
            j = json{
                    {"resolution",        b.resolution},
                    {"width",             b.width},
                    {"height",            b.height},
                    {"origin_position_x", b.origin_position_x},
                    {"origin_position_y", b.origin_position_y},
                    {"data",              b.data},
            };
        }

        friend void from_json(const json &j, Map_content &b) {
            j.at("resolution").get_to(b.resolution);
            j.at("width").get_to(b.width);
            j.at("height").get_to(b.height);
            j.at("origin_position_x").get_to(b.origin_position_x);
            j.at("origin_position_y").get_to(b.origin_position_y);
            j.at("data").get_to(b.data);
        }

    };

    Map_content::Map_content(
            float resolution,
            int width,
            int height,
            int origin_position_x,
            int origin_position_y,
            std::vector<int8_t> data
    ) {
        this->resolution = resolution;
        this->width = width;
        this->height = height;
        this->origin_position_x = origin_position_x;
        this->origin_position_y = origin_position_y;
        this->data = data;
    }

    class Map_data {
    public:
        Map_data() {};

        Map_data(
                std::vector<int8_t> data
        );

        ~Map_data() {};

    public:
        std::vector<int8_t> data;

        friend void to_json(json &j, const Map_data &b) {
            j = json{
                    {"data", b.data},
            };
        }

        friend void from_json(const json &j, Map_data &b) {
            j.at("data").get_to(b.data);
        }
    };

    Map_data::Map_data(
            std::vector<int8_t> data
    ) {
        this->data = data;
    }


    class Map_app {
    public:
        Map_app() {};

        Map_app(
                float resolution,
                int width,
                int height,
                int origin_position_x,
                int origin_position_y,
                std::vector<int8_t> data
        );

        ~Map_app() {};

    public:
        float resolution;
        int width;
        int height;
        int origin_position_x;
        int origin_position_y;
        std::vector<int8_t> data;

        friend void to_json(json &j, const Map_app &b) {
            j = json{
                    {"resolution",        b.resolution},
                    {"width",             b.width},
                    {"height",            b.height},
                    {"origin_position_x", b.origin_position_x},
                    {"origin_position_y", b.origin_position_y},
                    {"data",              b.data},
            };
        }

        friend void from_json(const json &j, Map_app &b) {
            j.at("resolution").get_to(b.resolution);
            j.at("width").get_to(b.width);
            j.at("height").get_to(b.height);
            j.at("origin_position_x").get_to(b.origin_position_x);
            j.at("origin_position_y").get_to(b.origin_position_y);
            j.at("data").get_to(b.data);
        }

    };

    Map_app::Map_app(
            float resolution,
            int width,
            int height,
            int origin_position_x,
            int origin_position_y,
            std::vector<int8_t> data
    ) {
        this->resolution = resolution;
        this->width = width;
        this->height = height;
        this->origin_position_x = origin_position_x;
        this->origin_position_y = origin_position_y;
        this->data = data;
    }
}


#endif /*Map_info*/