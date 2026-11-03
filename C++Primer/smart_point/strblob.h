#ifndef STRBLOB_H
#define STRBLOB_H

#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

// 值语义外观 + 共享底层 vector 的教学类型。
// StrBlob 的默认拷贝会复制 shared_ptr，从而让多个 StrBlob 共享同一份元素。
class StrBlob {
public:
    using size_type = std::vector<std::string>::size_type;

    StrBlob();
    StrBlob(std::initializer_list<std::string> initial_values);

    size_type size() const { return data->size(); }
    bool empty() const { return data->empty(); }
    void push_back(const std::string& value) { data->push_back(value); }

    void pop_back();
    std::string& front();
    std::string& back();

private:
    // shared_ptr 负责 vector 的生命周期：最后一个拥有者销毁时自动释放 vector。
    std::shared_ptr<std::vector<std::string>> data;

    // 统一完成边界检查；i 用 0 即可表达“容器不能为空”。
    void check(size_type i, const std::string& message) const;
};

#endif
