//
// Created by lijiang on 2022/5/25.
//

#ifndef APP_COMMUNICATION_LOCATION_H
#define APP_COMMUNICATION_LOCATION_H

#include "string"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

class Location {
private:
    int locationType; //获取当前定位结果来源，如网络定位结果，详见定位类型表
    double latitude;//获取纬度
    double longitude;//获取经度
    float accuracy;//获取精度信息
    double altitude;//海拔高度

    std::string address;//地址，如果option中设置isNeedAddress为false，则没有此结果，网络定位结果中会有地址信息，GPS定位不返回地址信息。
    std::string country;//国家信息
    std::string province;//省信息
    std::string city;//城市信息
    std::string district;//城区信息
    std::string street;//街道信息
    std::string streetNum;//街道门牌号信息
    std::string cityCode;//城市编码
    std::string adCode;//地区编码
    std::string aoiName;//获取当前定位点的AOI信息
public:
    int getLocationType() const;

    void setLocationType(int locationType);

    double getLatitude() const;

    void setLatitude(double latitude);

    double getLongitude() const;

    void setLongitude(double longitude);

    float getAccuracy() const;

    void setAccuracy(float accuracy);

    double getAltitude() const;

    void setAltitude(double altitude);

    const std::string &getAddress() const;

    void setAddress(const std::string &address);

    const std::string &getCountry() const;

    void setCountry(const std::string &country);

    const std::string &getProvince() const;

    void setProvince(const std::string &province);

    const std::string &getCity() const;

    void setCity(const std::string &city);

    const std::string &getDistrict() const;

    void setDistrict(const std::string &district);

    const std::string &getStreet() const;

    void setStreet(const std::string &street);

    const std::string &getStreetNum() const;

    void setStreetNum(const std::string &streetNum);

    const std::string &getCityCode() const;

    void setCityCode(const std::string &cityCode);

    const std::string &getAdCode() const;

    void setAdCode(const std::string &adCode);

    const std::string &getAoiName() const;

    void setAoiName(const std::string &aoiName);

    friend void to_json(json &j, const Location &location) {
        j = json{
                {"locationType", location.locationType},
                {"latitude",     location.latitude},
                {"longitude",    location.longitude},
                {"accuracy",     location.accuracy},
                {"altitude",     location.altitude},

                {"address",      location.address},
                {"country",      location.country},
                {"province",     location.province},
                {"city",         location.city},
                {"district",     location.district},
                {"street",       location.street},
                {"streetNum",    location.streetNum},
                {"cityCode",     location.cityCode},
                {"adCode",       location.adCode},
                {"aoiName",      location.aoiName},
        };
    }

    friend void from_json(const json &j, Location &location) {
        j.at("locationType").get_to(location.locationType);
        j.at("latitude").get_to(location.latitude);
        j.at("longitude").get_to(location.longitude);
        j.at("accuracy").get_to(location.accuracy);
        j.at("altitude").get_to(location.altitude);

        j.at("address").get_to(location.address);
        j.at("country").get_to(location.country);
        j.at("province").get_to(location.province);
        j.at("city").get_to(location.city);
        j.at("district").get_to(location.district);
        j.at("street").get_to(location.street);
        j.at("streetNum").get_to(location.streetNum);
        j.at("cityCode").get_to(location.cityCode);
        j.at("adCode").get_to(location.adCode);
        j.at("aoiName").get_to(location.aoiName);
    }

private:

    static Location *m_instance_ptr;

public:

    ~Location() = default;

    static Location *get_instance() {
        if (m_instance_ptr == nullptr) {
            m_instance_ptr = new Location;
        }
        return m_instance_ptr;
    }
};


#endif //APP_COMMUNICATION_LOCATION_H
