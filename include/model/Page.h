//
// Created by Looper on 2022/12/8.
//

#ifndef APP_COMMUNICATION_PAGE_H
#define APP_COMMUNICATION_PAGE_H

#include <nlohmann/json.hpp>

using json = nlohmann::json;

template<class T>
class Page {
private:
    int page_no;
    int page_size;
    T param;
public:
    Page() {}

    Page(int pageNo, int pageSize, T param) : page_no(pageNo), page_size(pageSize), param(param) {}

    int getPageNo() const {
        return page_no;
    }

    void setPageNo(int pageNo) {
        page_no = pageNo;
    }

    int getPageSize() const {
        return page_size;
    }

    void setPageSize(int pageSize) {
        page_size = pageSize;
    }

    T getParam() const {
        return param;
    }

    void setParam(T param) {
        Page::param = param;
    }

    friend void to_json(json &j, const Page &b) {
        j = json{
                {"page_no",   b.page_no},
                {"page_size", b.page_size},
                {"param",     b.param},
        };
    }

    friend void from_json(const json &j, Page &b) {
        j.at("page_no").get_to(b.page_no);
        j.at("page_size").get_to(b.page_size);
        j.at("param").get_to(b.param);
    }
};

template<class T>
class PageResult {
private:
    int total;
    int page_num;
    int page_size;
    std::vector<T> data;
public:
    PageResult() {}

    PageResult(int total, int pageNum, int pageSize, const std::vector<T> &data) : total(total), page_num(pageNum),
                                                                                   page_size(pageSize), data(data) {}

    int getTotal() const {
        return total;
    }

    void setTotal(int total) {
        PageResult::total = total;
    }

    int getPageNum() const {
        return page_num;
    }

    void setPageNum(int pageNum) {
        page_num = pageNum;
    }

    int getPageSize() const {
        return page_size;
    }

    void setPageSize(int pageSize) {
        page_size = pageSize;
    }

    const std::vector<T> &getData() const {
        return data;
    }

    std::vector<T> &getData() {
        return data;
    }

    void setData(const std::vector<T> &data) {
        PageResult::data = data;
    }

    friend void to_json(json &j, const PageResult &b) {
        j = json{
                {"total",     b.total},
                {"page_num",  b.page_num},
                {"page_size", b.page_size},
                {"data",      b.data}
        };
    }

    friend void from_json(const json &j, PageResult &b) {
        j.at("total").get_to(b.total);
        j.at("page_num").get_to(b.page_num);
        j.at("page_size").get_to(b.page_size);
        j.at("data").get_to(b.data);
    }
};


#endif //APP_COMMUNICATION_PAGE_H
