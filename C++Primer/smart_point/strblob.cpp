#include "strblob.h"

#include <stdexcept>

// make_shared 同时完成对象分配和 shared_ptr 建立；相比 shared_ptr(new T)，
// 通常分配次数更少，异常安全性也更好。
StrBlob::StrBlob()
    : data(std::make_shared<std::vector<std::string>>()) {}

StrBlob::StrBlob(std::initializer_list<std::string> initial_values)
    : data(std::make_shared<std::vector<std::string>>(initial_values)) {}

void StrBlob::check(size_type i, const std::string& message) const {
    if (i >= data->size()) {
        throw std::out_of_range(message);
    }
}

std::string& StrBlob::front() {
    check(0, "front on empty StrBlob");
    return data->front();
}

std::string& StrBlob::back() {
    check(0, "back on empty StrBlob");
    return data->back();
}

void StrBlob::pop_back() {
    check(0, "pop_back on empty StrBlob");
    data->pop_back();
}
