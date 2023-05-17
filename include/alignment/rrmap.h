//
// Created by Looper on 2023/5/17.
//

#ifndef APP_COMMUNICATION_RRMAP_H
#define APP_COMMUNICATION_RRMAP_H

#include <cstdint>
#include "climits"

class RRMap {
private:
    // head 20
    int8_t default_0;           // 1 114
    int8_t default_1;           // 1 114
    int16_t mapHeaderLength;    // 2 20 (default_0 + default_1 + mapHeaderLength + mapDataLength + majorVersion + minorVersion + mapIndex + mapSequence = 20)
    int32_t mapDataLength;      // 4
    int16_t majorVersion;       // 2 1
    int16_t minorVersion;       // 2 0
    int32_t mapIndex;           // 4 132
    int32_t mapSequence;        // 4 2289

    // 28
    int16_t blockType;          // 2 2
    int16_t blockHeaderLength;  // 2 28 ()
    int32_t blockDataLength;    // 4 166934
    int32_t top;                // 4
    int32_t left;               // 4
    int32_t imgHeight;          // 4
    int32_t imgWidth;           // 4
};


#endif //APP_COMMUNICATION_RRMAP_H
