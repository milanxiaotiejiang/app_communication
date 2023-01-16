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

const string &Location::getAddress() const {
    return address;
}

void Location::setAddress(const string &address) {
    Location::address = address;
}

const string &Location::getCountry() const {
    return country;
}

void Location::setCountry(const string &country) {
    Location::country = country;
}

const string &Location::getProvince() const {
    return province;
}

void Location::setProvince(const string &province) {
    Location::province = province;
}

const string &Location::getCity() const {
    return city;
}

void Location::setCity(const string &city) {
    Location::city = city;
}

const string &Location::getDistrict() const {
    return district;
}

void Location::setDistrict(const string &district) {
    Location::district = district;
}

const string &Location::getStreet() const {
    return street;
}

void Location::setStreet(const string &street) {
    Location::street = street;
}

const string &Location::getStreetNum() const {
    return streetNum;
}

void Location::setStreetNum(const string &streetNum) {
    Location::streetNum = streetNum;
}

const string &Location::getCityCode() const {
    return cityCode;
}

void Location::setCityCode(const string &cityCode) {
    Location::cityCode = cityCode;
}

const string &Location::getAdCode() const {
    return adCode;
}

void Location::setAdCode(const string &adCode) {
    Location::adCode = adCode;
}

const string &Location::getAoiName() const {
    return aoiName;
}

void Location::setAoiName(const string &aoiName) {
    Location::aoiName = aoiName;
}
