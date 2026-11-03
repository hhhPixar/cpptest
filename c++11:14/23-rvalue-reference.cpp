#include <cstring>
#include <iostream>
#include <string>
#include <utility>

// 这个类仅用于观察拷贝与移动的差异。真实项目优先使用 std::string，
// 即“规则零”（不直接管理资源，就无需自己编写特殊成员函数）。
class MyString {
public:
    static std::size_t default_ctor_count;
    static std::size_t ctor_count;
    static std::size_t copy_ctor_count;
    static std::size_t copy_assign_count;
    static std::size_t move_ctor_count;
    static std::size_t move_assign_count;
    static std::size_t dtor_count;

    MyString() noexcept : data_(nullptr), length_(0) { ++default_ctor_count; }

    explicit MyString(const char* text) : data_(nullptr), length_(std::strlen(text)) {
        ++ctor_count;
        allocate_and_copy(text);
    }

    // 深拷贝：新对象取得独立的字符数组。
    MyString(const MyString& other) : data_(nullptr), length_(other.length_) {
        ++copy_ctor_count;
        allocate_and_copy(other.data_ ? other.data_ : "");
    }

    MyString& operator=(const MyString& other) {
        ++copy_assign_count;
        if (this != &other) {
            // copy-and-swap：先构造临时副本，成功后再交换，避免分配失败破坏当前对象。
            MyString temporary(other);
            swap(temporary);
        }
        return *this;
    }

    // 移动不是“移动内存”，而是转移资源所有权。源对象仍须可析构、可赋值。
    MyString(MyString&& other) noexcept : data_(other.data_), length_(other.length_) {
        ++move_ctor_count;
        other.data_ = nullptr;
        other.length_ = 0;
    }

    MyString& operator=(MyString&& other) noexcept {
        ++move_assign_count;
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            length_ = other.length_;
            other.data_ = nullptr;
            other.length_ = 0;
        }
        return *this;
    }

    ~MyString() {
        ++dtor_count;
        delete[] data_; // 与 new char[] 严格配对。
    }

    void swap(MyString& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(length_, other.length_);
    }

    const char* get() const { return data_ ? data_ : ""; }

private:
    void allocate_and_copy(const char* text) {
        // 空字符串也分配一个字符，保证 data_ 非空时总是 C 字符串。
        data_ = new char[length_ + 1];
        std::memcpy(data_, text, length_);
        data_[length_] = '\0';
    }

    char* data_;
    std::size_t length_;
};

std::size_t MyString::default_ctor_count = 0;
std::size_t MyString::ctor_count = 0;
std::size_t MyString::copy_ctor_count = 0;
std::size_t MyString::copy_assign_count = 0;
std::size_t MyString::move_ctor_count = 0;
std::size_t MyString::move_assign_count = 0;
std::size_t MyString::dtor_count = 0;

int main() {
    MyString source("resource");

    // std::move 只把表达式转换为右值；是否发生资源转移由被调用的移动构造函数决定。
    MyString destination(std::move(source));

    std::cout << "moved-to: " << destination.get()
              << ", moved-from: " << source.get() << '\n';
}
