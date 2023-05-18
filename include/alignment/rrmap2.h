//
// Created by Looper on 2023/5/18.
//

#ifndef APP_COMMUNICATION_RRMAP2_H
#define APP_COMMUNICATION_RRMAP2_H

#include <cstdint>
#include "climits"
#include "vector"
#include "stdexcept"
#include "boost/iostreams/filtering_streambuf.hpp"
#include "boost/iostreams/copy.hpp"
#include "boost/iostreams/filter/gzip.hpp"
#include "segmentation/SegmentationCenter.h"
#include "segmentation/map_attribute.h"

class RRMapBlock {
public:
    virtual ~RRMapBlock() = default;

    virtual int type() const = 0;

    virtual int headerLength() const = 0;

    virtual int dataLength() const = 0;

    virtual std::vector<int8_t> toByteArray() const = 0;
};

class RRMapCharger : public RRMapBlock {
private:
    int32_t chargerX;
    int32_t chargerY;
    int32_t chargerA;
public:
    ~RRMapCharger() override = default;

    int type() const override {
        return 1;
    }

    int headerLength() const override {
        return 8;
    }

    int dataLength() const override {
        return sizeof(chargerX) + sizeof(chargerY) + sizeof(chargerA);
    }

    std::vector<int8_t> toByteArray() const override {
        return std::vector<int8_t>();
    }
};

#endif //APP_COMMUNICATION_RRMAP2_H
