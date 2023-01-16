//
// Created by lijiang on 2022/5/26.
//

#ifndef APP_COMMUNICATION_PROJECT_H
#define APP_COMMUNICATION_PROJECT_H

#include "string"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

using namespace std;

class Project {
private:
    string name;
public:

    const string &getName() const;

    void setName(const string &name);

    friend void to_json(json &j, const Project &project) {
        j = json{
                {"name", project.name},
        };
    }

    friend void from_json(const json &j, Project &project) {
        j.at("name").get_to(project.name);
    }
};


#endif //APP_COMMUNICATION_PROJECT_H
