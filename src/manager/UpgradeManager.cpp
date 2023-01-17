//
// Created by mi on 2022/8/1.
//

#include "manager/UpgradeManager.h"

void UpgradeManager::updateCombinationPrincipal() {
    auto dataBasePath = ros::package::getPath("data_base");
    std::string lastFilePath;
    lastFilePath.append(dataBasePath);
    lastFilePath.append("/config/combination_list.txt");

    if (!sh::File::exists(lastFilePath)) {
        LOG(INFO) << lastFilePath << " not exists";
        return;
    }

    std::string content;
    std::unique_ptr<sh::File> lastFilePtr(new sh::File(lastFilePath));
    if (lastFilePtr->open(std::ios::in)) {
        content = lastFilePtr->readAll();
        lastFilePtr->close();
    } else {
        std::remove(lastFilePath.c_str());
        LOG(INFO) << lastFilePath << " open fail";
        return;
    }

    if (content.empty()) {
        std::remove(lastFilePath.c_str());
        LOG(INFO) << lastFilePath << " content is empty " << "and remove this file";
        return;
    }

    auto decode = base64_decode(content);
    auto jdecode = json::parse(decode);
    auto lastList = jdecode.get<CombinationBriefUpgradeList>();

    std::string currentFilePath;
    currentFilePath.append(dataBasePath);
    currentFilePath.append("/config/combination_list_principal.txt");

    if (sh::File::exists(currentFilePath)) {
        if (!sh::File::saveTextTo(currentFilePath, "")) {
            LOG(ERROR) << currentFilePath << " save empty text fail !";
            return;
        }
    } else {
        unique_ptr<sh::File> cFilePtr(new sh::File(currentFilePath));
        if (!cFilePtr->create(currentFilePath)) {
            LOG(ERROR) << currentFilePath << " create fail !";
            return;
        }
    }

    std::vector<CombinationBrief> vector;
    for (const auto &item: lastList.getMCombinationBriefList()) {
        auto combination = CombinationBrief();
        combination.setCombinationID(item.getCombinationId());
        combination.setName(item.getName());
        combination.setRate(item.getRate());
        combination.setPartIDList(item.getPartIdList());
        combination.setPrincipal(false);
        vector.push_back(combination);
    }
    CombinationBriefList combinationBriefList;
    combinationBriefList.setCombinationBriefList(vector);

    json currentJson = combinationBriefList;
    auto jencode = base64_encode(currentJson.dump());

    if (!sh::File::saveTextTo(currentFilePath, jencode)) {
        LOG(ERROR) << currentFilePath << " save new combination fail !";
    }

    std::remove(lastFilePath.c_str());

}

void UpgradeManager::updateViewPartPrincipal() {
    auto dataBasePath = ros::package::getPath("data_base");
    std::string lastFilePath;
    lastFilePath.append(dataBasePath);
    lastFilePath.append("/config/view_part.txt");

    if (!sh::File::exists(lastFilePath)) {
        LOG(INFO) << lastFilePath << " not exists";
        return;
    }

    std::string content;
    std::unique_ptr<sh::File> lastFilePtr(new sh::File(lastFilePath));
    if (lastFilePtr->open(std::ios::in)) {
        content = lastFilePtr->readAll();
        lastFilePtr->close();
    } else {
        std::remove(lastFilePath.c_str());
        LOG(INFO) << lastFilePath << " open fail";
        return;
    }

    if (content.empty()) {
        std::remove(lastFilePath.c_str());
        LOG(INFO) << lastFilePath << " content is empty " << "and remove this file";
        return;
    }

    auto decode = base64_decode(content);
    auto jdecode = json::parse(decode);
    auto lastList = jdecode.get<ViewPartListUpgrade>();

    std::string currentFilePath;
    currentFilePath.append(dataBasePath);
    currentFilePath.append("/config/view_part_principal.txt");

    if (sh::File::exists(currentFilePath)) {
        if (!sh::File::saveTextTo(currentFilePath, "")) {
            LOG(ERROR) << currentFilePath << " save empty text fail !";
            return;
        }
    } else {
        unique_ptr<sh::File> cFilePtr(new sh::File(currentFilePath));
        if (!cFilePtr->create(currentFilePath)) {
            LOG(ERROR) << currentFilePath << " create fail !";
            return;
        }
    }

    std::vector<ViewPart> vector;
    for (const auto &item: lastList.getMViewPartList()) {
        WorkStatus ws;
        ws.setSweepStatus(item.getWorkStatus().getSweepStatus());
        ws.setMopStatus(item.getWorkStatus().getDragStatus());
        ws.setVacuumStatus(item.getWorkStatus().getAbsorbStatus());
        ws.setPushStatus(item.getWorkStatus().getPushStatus());
        //  ws.setStationDryStatus(item.getWorkStatus().setStationDryStatus());
        auto viewPart = ViewPart();
        viewPart.setMode(item.getMode());
        viewPart.setName(item.getName());
        viewPart.setPartID(item.getPartId());
        viewPart.setPolygon(item.getPolygon());
        viewPart.setRate(item.getRate());
        viewPart.setTeach(item.getTeach());
        viewPart.setPathID(item.getPathId());
        viewPart.setWorkStatus(ws);
        viewPart.setZoned(item.getZoned());
        vector.push_back(viewPart);
    }
    ViewPartList viewPartList;
    viewPartList.SetViewPartList(vector);

    json currentJson = viewPartList;
    auto jencode = base64_encode(currentJson.dump());

    if (!sh::File::saveTextTo(currentFilePath, jencode)) {
        LOG(ERROR) << currentFilePath << " save new combination fail !";
    }

    std::remove(lastFilePath.c_str());
}

void UpgradeManager::updateCleanHistoryPrincipal() {
    auto dataBasePath = ros::package::getPath("data_base");
    std::string lastFilePath;
    lastFilePath.append(dataBasePath);
    lastFilePath.append("/config/clean_history.txt");

    if (!sh::File::exists(lastFilePath)) {
        LOG(INFO) << lastFilePath << " not exists";
        return;
    }

    std::string content;
    std::unique_ptr<sh::File> lastFilePtr(new sh::File(lastFilePath));
    if (lastFilePtr->open(std::ios::in)) {
        content = lastFilePtr->readAll();
        lastFilePtr->close();
    } else {
        std::remove(lastFilePath.c_str());
        LOG(INFO) << lastFilePath << " open fail";
        return;
    }

    if (content.empty()) {
        std::remove(lastFilePath.c_str());
        LOG(INFO) << lastFilePath << " content is empty " << "and remove this file";
        return;
    }

    auto decode = base64_decode(content);
    auto jdecode = json::parse(decode);
    auto lastList = jdecode.get<CleanHistoryListUpgrade>();

    std::string currentFilePath;
    currentFilePath.append(dataBasePath);
    currentFilePath.append("/config/clean_history_principal.txt");

    if (sh::File::exists(currentFilePath)) {
        if (!sh::File::saveTextTo(currentFilePath, "")) {
            LOG(ERROR) << currentFilePath << " save empty text fail !";
            return;
        }
    } else {
        unique_ptr<sh::File> cFilePtr(new sh::File(currentFilePath));
        if (!cFilePtr->create(currentFilePath)) {
            LOG(ERROR) << currentFilePath << " create fail !";
            return;
        }
    }

    std::vector<CleanHistory> vector;
    for (const auto &item: lastList.getMCleanHistoryList()) {
        WorkStatus ws;
        //    ws.setStationDryStatus(item.getWorkStatus().setStationDryStatus());
        CleanHistory historyUpgrade(item.isComplete(), true, item.getLaunchTime(), item.getExecuteTime(),
                                    item.getEndTime(),
                                    item.getTaskMode(),
                                    item.getTaskId(), ws, item.getCleanArea(), item.getCleanTime(),
                                    item.getErrorCode(), item.getErrorMessage(), 1);

        vector.push_back(historyUpgrade);
    }
    CleanHistoryList cleanHistoryList;
    cleanHistoryList.SetCleanHistoryList(vector);

    json currentJson = cleanHistoryList;
    auto jencode = base64_encode(currentJson.dump());

    if (!sh::File::saveTextTo(currentFilePath, jencode)) {
        LOG(ERROR) << currentFilePath << " save new combination fail !";
    }

    std::remove(lastFilePath.c_str());
}

void UpgradeManager::updateCombinationBase64() {
    auto dataBasePath = ros::package::getPath("data_base");
    std::string lastFilePath;
    lastFilePath.append(dataBasePath);
    lastFilePath.append("/config/combination_list_principal.txt");

    if (!sh::File::exists(lastFilePath)) {
        LOG(INFO) << lastFilePath << " not exists";
        return;
    }

    std::string content;
    std::unique_ptr<sh::File> lastFilePtr(new sh::File(lastFilePath));
    if (lastFilePtr->open(std::ios::in)) {
        content = lastFilePtr->readAll();
        lastFilePtr->close();
    } else {
        std::remove(lastFilePath.c_str());
        LOG(INFO) << lastFilePath << " open fail";
        return;
    }

    if (content.empty()) {
        std::remove(lastFilePath.c_str());
        LOG(INFO) << lastFilePath << " content is empty " << "and remove this file";
        return;
    }

    auto decode = base64_decode(content);
    auto jdecode = json::parse(decode);
    auto lastList = jdecode.get<CombinationBriefList>();

    std::string currentFilePath;
    currentFilePath.append(dataBasePath);
    currentFilePath.append("/config/combination_list_principal_json.txt");

    if (sh::File::exists(currentFilePath)) {
        if (!sh::File::saveTextTo(currentFilePath, "")) {
            LOG(ERROR) << currentFilePath << " save empty text fail !";
            return;
        }
    } else {
        unique_ptr<sh::File> cFilePtr(new sh::File(currentFilePath));
        if (!cFilePtr->create(currentFilePath)) {
            LOG(ERROR) << currentFilePath << " create fail !";
            return;
        }
    }


    json currentJson = lastList;

    if (!sh::File::saveTextTo(currentFilePath, currentJson.dump())) {
        LOG(ERROR) << currentFilePath << " save new combination fail !";
    }

    std::remove(lastFilePath.c_str());

}

void UpgradeManager::updateViewPartBase64() {
    auto dataBasePath = ros::package::getPath("data_base");
    std::string lastFilePath;
    lastFilePath.append(dataBasePath);
    lastFilePath.append("/config/view_part_principal.txt");

    if (!sh::File::exists(lastFilePath)) {
        LOG(INFO) << lastFilePath << " not exists";
        return;
    }

    std::string content;
    std::unique_ptr<sh::File> lastFilePtr(new sh::File(lastFilePath));
    if (lastFilePtr->open(std::ios::in)) {
        content = lastFilePtr->readAll();
        lastFilePtr->close();
    } else {
        std::remove(lastFilePath.c_str());
        LOG(INFO) << lastFilePath << " open fail";
        return;
    }

    if (content.empty()) {
        std::remove(lastFilePath.c_str());
        LOG(INFO) << lastFilePath << " content is empty " << "and remove this file";
        return;
    }

    auto decode = base64_decode(content);
    auto jdecode = json::parse(decode);
    auto lastList = jdecode.get<ViewPartList>();

    std::string currentFilePath;
    currentFilePath.append(dataBasePath);
    currentFilePath.append("/config/view_part_principal_json.txt");

    if (sh::File::exists(currentFilePath)) {
        if (!sh::File::saveTextTo(currentFilePath, "")) {
            LOG(ERROR) << currentFilePath << " save empty text fail !";
            return;
        }
    } else {
        unique_ptr<sh::File> cFilePtr(new sh::File(currentFilePath));
        if (!cFilePtr->create(currentFilePath)) {
            LOG(ERROR) << currentFilePath << " create fail !";
            return;
        }
    }

    json currentJson = lastList;

    if (!sh::File::saveTextTo(currentFilePath, currentJson.dump())) {
        LOG(ERROR) << currentFilePath << " save new combination fail !";
    }

    std::remove(lastFilePath.c_str());
}

void UpgradeManager::updateCleanHistoryBase64() {
    auto dataBasePath = ros::package::getPath("data_base");
    std::string lastFilePath;
    lastFilePath.append(dataBasePath);
    lastFilePath.append("/config/clean_history_principal.txt");

    if (!sh::File::exists(lastFilePath)) {
        LOG(INFO) << lastFilePath << " not exists";
        return;
    }

    std::string content;
    std::unique_ptr<sh::File> lastFilePtr(new sh::File(lastFilePath));
    if (lastFilePtr->open(std::ios::in)) {
        content = lastFilePtr->readAll();
        lastFilePtr->close();
    } else {
        std::remove(lastFilePath.c_str());
        LOG(INFO) << lastFilePath << " open fail";
        return;
    }

    if (content.empty()) {
        std::remove(lastFilePath.c_str());
        LOG(INFO) << lastFilePath << " content is empty " << "and remove this file";
        return;
    }

    auto decode = base64_decode(content);
    auto jdecode = json::parse(decode);
    auto lastList = jdecode.get<CleanHistoryList>();

    std::string currentFilePath;
    currentFilePath.append(dataBasePath);
    currentFilePath.append("/config/clean_history_principal_json.txt");

    if (sh::File::exists(currentFilePath)) {
        if (!sh::File::saveTextTo(currentFilePath, "")) {
            LOG(ERROR) << currentFilePath << " save empty text fail !";
            return;
        }
    } else {
        unique_ptr<sh::File> cFilePtr(new sh::File(currentFilePath));
        if (!cFilePtr->create(currentFilePath)) {
            LOG(ERROR) << currentFilePath << " create fail !";
            return;
        }
    }

    json currentJson = lastList;

    if (!sh::File::saveTextTo(currentFilePath, currentJson.dump())) {
        LOG(ERROR) << currentFilePath << " save new combination fail !";
    }

    std::remove(lastFilePath.c_str());
}

void UpgradeManager::updateTimeInfoBase64() {
    auto dataBasePath = ros::package::getPath("data_base");
    std::string lastFilePath;
    lastFilePath.append(dataBasePath);
    lastFilePath.append("/config/timer_info.txt");

    if (!sh::File::exists(lastFilePath)) {
        LOG(INFO) << lastFilePath << " not exists";
        return;
    }

    std::string content;
    std::unique_ptr<sh::File> lastFilePtr(new sh::File(lastFilePath));
    if (lastFilePtr->open(std::ios::in)) {
        content = lastFilePtr->readAll();
        lastFilePtr->close();
    } else {
        std::remove(lastFilePath.c_str());
        LOG(INFO) << lastFilePath << " open fail";
        return;
    }

    if (content.empty()) {
        std::remove(lastFilePath.c_str());
        LOG(INFO) << lastFilePath << " content is empty " << "and remove this file";
        return;
    }

    auto decode = base64_decode(content);
    auto jdecode = json::parse(decode);
    auto lastList = jdecode.get<std::vector<TimerInfo>>();

    std::string currentFilePath;
    currentFilePath.append(dataBasePath);
    currentFilePath.append("/config/timer_info_json.txt");

    if (sh::File::exists(currentFilePath)) {
        if (!sh::File::saveTextTo(currentFilePath, "")) {
            LOG(ERROR) << currentFilePath << " save empty text fail !";
            return;
        }
    } else {
        unique_ptr<sh::File> cFilePtr(new sh::File(currentFilePath));
        if (!cFilePtr->create(currentFilePath)) {
            LOG(ERROR) << currentFilePath << " create fail !";
            return;
        }
    }

    json currentJson = lastList;

    if (!sh::File::saveTextTo(currentFilePath, currentJson.dump())) {
        LOG(ERROR) << currentFilePath << " save new combination fail !";
    }

    std::remove(lastFilePath.c_str());
}

void UpgradeManager::updateTeachPoint() {
    auto dataBasePath = ros::package::getPath("data_base");
    std::string lastFilePath;
    lastFilePath.append(dataBasePath);
    lastFilePath.append("/config/teach_point.txt");

    if (!sh::File::exists(lastFilePath)) {
        LOG(INFO) << lastFilePath << " not exists";
        return;
    }

    std::string content;
    std::unique_ptr<sh::File> lastFilePtr(new sh::File(lastFilePath));
    if (lastFilePtr->open(std::ios::in)) {
        content = lastFilePtr->readAll();
        lastFilePtr->close();
    } else {
        std::remove(lastFilePath.c_str());
        LOG(INFO) << lastFilePath << " open fail";
        return;
    }

    if (content.empty()) {
        std::remove(lastFilePath.c_str());
        LOG(INFO) << lastFilePath << " content is empty " << "and remove this file";
        return;
    }

    auto decode = base64_decode(content);
    auto jdecode = json::parse(decode);
    auto lastList = jdecode.get<std::vector<WayPointTask>>();

    std::string currentFilePath;
    currentFilePath.append(dataBasePath);
    currentFilePath.append("/config/teach_point_json.txt");

    if (sh::File::exists(currentFilePath)) {
        if (!sh::File::saveTextTo(currentFilePath, "")) {
            LOG(ERROR) << currentFilePath << " save empty text fail !";
            return;
        }
    } else {
        unique_ptr<sh::File> cFilePtr(new sh::File(currentFilePath));
        if (!cFilePtr->create(currentFilePath)) {
            LOG(ERROR) << currentFilePath << " create fail !";
            return;
        }
    }

    json currentJson = lastList;

    if (!sh::File::saveTextTo(currentFilePath, currentJson.dump())) {
        LOG(ERROR) << currentFilePath << " save new combination fail !";
    }

    std::remove(lastFilePath.c_str());
}

void UpgradeManager::updateCombinationPrincipalWork() {
    auto dataBasePath = ros::package::getPath("data_base");
    std::string lastFilePath;
    lastFilePath.append(dataBasePath);
    lastFilePath.append("/config/combination_list_principal_json.txt");

    if (!sh::File::exists(lastFilePath)) {
        LOG(INFO) << lastFilePath << " not exists";
        return;
    }

    std::string content;
    std::unique_ptr<sh::File> lastFilePtr(new sh::File(lastFilePath));
    if (lastFilePtr->open(std::ios::in)) {
        content = lastFilePtr->readAll();
        lastFilePtr->close();
    } else {
        std::remove(lastFilePath.c_str());
        LOG(INFO) << lastFilePath << " open fail";
        return;
    }

    if (content.empty()) {
        std::remove(lastFilePath.c_str());
        LOG(INFO) << lastFilePath << " content is empty " << "and remove this file";
        return;
    }

    auto jdecode = json::parse(content);
    auto lastList = jdecode.get<CombinationBriefWorkList>();

    std::string currentFilePath;
    currentFilePath.append(dataBasePath);
    currentFilePath.append("/config/combination_list_principal_json_work.txt");

    if (sh::File::exists(currentFilePath)) {
        if (!sh::File::saveTextTo(currentFilePath, "")) {
            LOG(ERROR) << currentFilePath << " save empty text fail !";
            return;
        }
    } else {
        unique_ptr<sh::File> cFilePtr(new sh::File(currentFilePath));
        if (!cFilePtr->create(currentFilePath)) {
            LOG(ERROR) << currentFilePath << " create fail !";
            return;
        }
    }

    std::vector<CombinationBrief> vector;
    for (const auto &item: lastList.getMCombinationBriefList()) {
        auto combination = CombinationBrief();
        combination.setCombinationID(item.getCombinationId());
        combination.setName(item.getName());
        combination.setRate(item.getRate());
        combination.setPartIDList(item.getPartIdList());
        combination.setPrincipal(item.isPrincipal());
        vector.push_back(combination);
    }
    CombinationBriefList combinationBriefList;
    combinationBriefList.setCombinationBriefList(vector);

    json currentJson = combinationBriefList;

    if (!sh::File::saveTextTo(currentFilePath, currentJson.dump())) {
        LOG(ERROR) << currentFilePath << " save new combination fail !";
    }

    std::remove(lastFilePath.c_str());
}

void UpgradeManager::checkWhetherFileExists() {
    auto dataBasePath = ros::package::getPath("data_base");
    createFile(dataBasePath, "combination_list_principal_json_work.txt");
    createFile(dataBasePath, "view_part_principal_json.txt");
    createFile(dataBasePath, "clean_history_principal_json.txt");
    createFile(dataBasePath, "timer_info_json.txt");
    createFile(dataBasePath, "teach_point_json.txt");
}

void UpgradeManager::createFile(const string &dataBasePath, const string &fileName) const {
    std::string currentFilePath;
    currentFilePath.append(dataBasePath);
    currentFilePath.append("/config/");
    currentFilePath.append(fileName);
    if (!sh::File::exists(currentFilePath)) {
        unique_ptr<sh::File> cFilePtr(new sh::File(currentFilePath));
        if (!cFilePtr->create(currentFilePath)) {
            LOG(ERROR) << currentFilePath << " create fail !";
        }
    }
}

void UpgradeManager::removePolygonViewPart() {
    //combination_list_principal_json_work
    //view_part_principal_json

    auto dataBasePath = ros::package::getPath("data_base");
    std::string viewPartFilePath;
    viewPartFilePath.append(dataBasePath);
    viewPartFilePath.append("/config/view_part_principal_json.txt");

    if (!sh::File::exists(viewPartFilePath)) {
        return;
    }

    std::string content;
    std::unique_ptr<sh::File> lastFilePtr(new sh::File(viewPartFilePath));
    if (lastFilePtr->open(std::ios::in)) {
        content = lastFilePtr->readAll();
        lastFilePtr->close();
    } else {
        LOG(INFO) << viewPartFilePath << " open fail";
        return;
    }

    if (content.empty()) {
        LOG(INFO) << viewPartFilePath << " content is empty " << "and remove this file";
        return;
    }

    auto jdecode = json::parse(content);
    auto lastList = jdecode.get<ViewPartList>();
    auto lastPartList = lastList.GetViewPartList();

    vector<ViewPart> saveList;
    vector<string> removeList;
    for (const auto &item: lastPartList) {
        if (item.getMode() != 2) {
            saveList.push_back(item);
        } else {
            removeList.push_back(item.getPartID());
        }
    }

    lastList.SetViewPartList(saveList);

    if (sh::File::exists(viewPartFilePath)) {
        if (!sh::File::saveTextTo(viewPartFilePath, "")) {
            LOG(ERROR) << viewPartFilePath << " save empty text fail !";
            return;
        }
    } else {
        unique_ptr<sh::File> cFilePtr(new sh::File(viewPartFilePath));
        if (!cFilePtr->create(viewPartFilePath)) {
            LOG(ERROR) << viewPartFilePath << " create fail !";
            return;
        }
    }

    json currentJson = lastList;

    if (!sh::File::saveTextTo(viewPartFilePath, currentJson.dump())) {
        LOG(ERROR) << viewPartFilePath << " save new combination fail !";
    }

    if (!removeList.empty()) {
        removeCombination(removeList);
    }
}

void UpgradeManager::removeCombination(vector<string> keys) {

    auto dataBasePath = ros::package::getPath("data_base");
    std::string combinationFilePath;
    combinationFilePath.append(dataBasePath);
    combinationFilePath.append("/config/combination_list_principal_json_work.txt");

    if (!sh::File::exists(combinationFilePath)) {
        return;
    }

    std::string content;
    std::unique_ptr<sh::File> lastFilePtr(new sh::File(combinationFilePath));
    if (lastFilePtr->open(std::ios::in)) {
        content = lastFilePtr->readAll();
        lastFilePtr->close();
    } else {
        LOG(INFO) << combinationFilePath << " open fail";
        return;
    }

    if (content.empty()) {
        LOG(INFO) << combinationFilePath << " content is empty " << "and remove this file";
        return;
    }

    auto jdecode = json::parse(content);
    auto lastList = jdecode.get<CombinationBriefList>();
    auto lastPartList = lastList.getCombinationBriefList();

    vector<CombinationBrief> saveList;
    for (auto combinationBrief: lastPartList) {
        vector<string> nIds;
        auto partIds = combinationBrief.getPartIDList();
        for (const auto id: partIds) {
            int nCount = std::count(keys.begin(), keys.end(), id);
            if (nCount <= 0) {
                nIds.push_back(id);
            }
        }
        combinationBrief.setPartIDList(nIds);
        saveList.push_back(combinationBrief);
    }

    lastList.setCombinationBriefList(saveList);

    if (sh::File::exists(combinationFilePath)) {
        if (!sh::File::saveTextTo(combinationFilePath, "")) {
            LOG(ERROR) << combinationFilePath << " save empty text fail !";
            return;
        }
    } else {
        unique_ptr<sh::File> cFilePtr(new sh::File(combinationFilePath));
        if (!cFilePtr->create(combinationFilePath)) {
            LOG(ERROR) << combinationFilePath << " create fail !";
            return;
        }
    }

    json currentJson = lastList;

    if (!sh::File::saveTextTo(combinationFilePath, currentJson.dump())) {
        LOG(ERROR) << combinationFilePath << " save new combination fail !";
    }
}
