//
// Created by mi on 2022/8/1.
//

#ifndef APP_COMMUNICATION_VERSIONSUBSCRIBE_H
#define APP_COMMUNICATION_VERSIONSUBSCRIBE_H

#include "nlohmann/json.hpp"

using json = nlohmann::json;

template<class T>
class VersionSubscribe {
private:
    int version;
    T content;
public:
    VersionSubscribe() = default;

    VersionSubscribe(int version, T content) : version(version), content(content) {}

    int getVersion() const {
        return version;
    }

    void setVersion(int version) {
        VersionSubscribe::version = version;
    }

    T getContent() const {
        return content;
    }

    void setContent(T content) {
        VersionSubscribe::content = content;
    }

    friend void to_json(json &j, const VersionSubscribe &versionSubscribe) {
        j = json{
                {"version", versionSubscribe.version},
                {"content", versionSubscribe.content}
        };
    }

    friend void from_json(const json &j, VersionSubscribe &versionSubscribe) {
        j.at("version").get_to(versionSubscribe.version);
        j.at("content").get_to(versionSubscribe.content);
    }
};


#endif //APP_COMMUNICATION_VERSIONSUBSCRIBE_H
