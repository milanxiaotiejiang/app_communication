//
// Created by lijiang on 2021/12/20.
//

#ifndef APP_COMMUNICATION_OLDFUNCTION_H
#define APP_COMMUNICATION_OLDFUNCTION_H

#define  BASE_MOVE  161  //A1
#define  BACK_HOME  162  //A2
#define  FOCUS_CLEAN  163  //A3
#define  IS_PAUSE  164  //A4
#define  SAVE_MAP  177  //B1
#define  GET_MAP  178  //B2
#define  REVISE_MAP  179  //B3
#define  SET_HOME  180  //B4
#define  RESET_MAP  181  //B5
#define  CLEAN_MISSION  193  //C1
#define  SWEEP_MISSION  194  //C2
#define  CLEAN_MODE  195  //C3

#define  SET_WALL  209  //D1
#define  SET_PROHIBITION_AREA  210  //D2
#define  SET_OBSTACLE  211  //D3
#define  RESET_OBSTACLE  212  //D4

#define SET_CREAT_MAP_MODE 225 //E1
#define SET_WORK_MODE 226 //E2
#define SET_ORIGINAL_MODE 227 //E3
#define SET_MAP_EDIT_MODE 228//E4

#include "string"
#include "math.h"
#include "iosfwd"
#include "iostream"
#include "stdlib.h"
#include "sstream"

static int hex2int(char c) {
    if ((c >= 'A') && (c <= 'Z')) {
        return c - 'A' + 10;
    } else if ((c >= 'a') && (c <= 'z')) {
        return c - 'a' + 10;
    } else if ((c >= '0') && (c <= '9')) {
        return c - '0';
    }
}

static int hexstring2int(std::string const &hexStr) {
    // int ID_num = (hex2int(hexStr[0])*16+hex2int(hexStr[1]));//只针对ID的两位数转化
    // return ID_num;

    //十六进制字符串转换成整形
    int data[4] = {0};//四个字节表示一个数
    int count = 0;
    for (int i = 0; i < hexStr.length(); i += 2) {
        int high = hex2int(hexStr[i]);   //高四位
        int low = hex2int(hexStr[i + 1]); //低四位
        data[count++] = (high << 4) + low;
    }
    int position = 0;
    for (int i = 0; i < hexStr.length() / 2; i++) {
        position += data[i] * pow(256, hexStr.length() / 2 - 1 - i);
    }
    return position;


}

static int htoi_signed(std::string hexstr) {
    unsigned int x;
    std::stringstream ss;
    ss << std::hex << hexstr;
    ss >> x;
    // output it as a signed type
    return x;
}

#endif //APP_COMMUNICATION_OLDFUNCTION_H