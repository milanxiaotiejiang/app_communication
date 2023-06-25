//
// Created by lijiang on 2022/5/25.
//

#include "model/Location.h"

int Location::getLocationType() const {
    return locationType;
}

void Location::setLocationType(int locationType) {
    Location::locationType = locationType;
}

double Location::getLatitude() const {
    return latitude;
}

void Location::setLatitude(double latitude) {
    Location::latitude = latitude;
}

double Location::getLongitude() const {
    return longitude;
}

void Location::setLongitude(double longitude) {
    Location::longitude = longitude;
}

float Location::getAccuracy() const {
    return accuracy;
}

void Location::setAccuracy(float accuracy) {
    Location::accuracy = accuracy;
}

double Location::getAltitude() const {
    return altitude;
}

void Location::setAltitude(double altitude) {
    Location::altitude = altitude;
}

const std::string &Location::getAddress() const {
    return address;
}

void Location::setAddress(const std::string &address) {
    Location::address = address;
}

const std::string &Location::getCountry() const {
    return country;
}

void Location::setCountry(const std::string &country) {
    Location::country = country;
}

const std::string &Location::getProvince() const {
    return province;
}

void Location::setProvince(const std::string &province) {
    Location::province = province;
}

const std::string &Location::getCity() const {
    return city;
}

void Location::setCity(const std::string &city) {
    Location::city = city;
}

const std::string &Location::getDistrict() const {
    return district;
}

void Location::setDistrict(const std::string &district) {
    Location::district = district;
}

const std::string &Location::getStreet() const {
    return street;
}

void Location::setStreet(const std::string &street) {
    Location::street = street;
}

const std::string &Location::getStreetNum() const {
    return streetNum;
}

void Location::setStreetNum(const std::string &streetNum) {
    Location::streetNum = streetNum;
}

const std::string &Location::getCityCode() const {
    return cityCode;
}

void Location::setCityCode(const std::string &cityCode) {
    Location::cityCode = cityCode;
}

const std::string &Location::getAdCode() const {
    return adCode;
}

void Location::setAdCode(const std::string &adCode) {
    Location::adCode = adCode;
}

const std::string &Location::getAoiName() const {
    return aoiName;
}

void Location::setAoiName(const std::string &aoiName) {
    Location::aoiName = aoiName;
}
