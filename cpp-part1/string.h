// 本文件故意用裸指针实现一个简化字符串，用来学习资源所有权。
// 实际工程应优先使用 std::string；它已经正确实现了这些资源管理细节。
#ifndef __MYSTRING__
#define __MYSTRING__

#include <cstring>
#include <iostream>

class String {
public:
    // 接受 C 风格字符串；nullptr 表示空字符串，而不是让 strlen(nullptr) 产生未定义行为。
    String(const char* cstr = nullptr);

    // 该类型独占 char[] 资源，因此需要遵循“规则三”：
    // 自定义析构、拷贝构造和拷贝赋值，才能避免默认浅拷贝导致的重复释放。
    String(const String& other);
    String& operator=(const String& other);
    ~String();

    // 返回只读指针，调用方不应借此修改内部缓冲区。
    const char* get_c_str() const { return m_data; }
    void print() const { std::cout << m_data << std::endl; }

private:
    // 指向以 '\0' 结尾、由本对象负责 delete[] 的字符数组。
    char* m_data;
};

inline String::String(const char* cstr) {
    if (cstr) {
        m_data = new char[std::strlen(cstr) + 1]; // +1 留给字符串结束符
        std::strcpy(m_data, cstr);
    } else {
        m_data = new char[1];
        m_data[0] = '\0';
    }
}

inline String::String(const String& other) {
    // 深拷贝：分配新的数组并复制字符，而非仅复制地址。
    m_data = new char[std::strlen(other.m_data) + 1];
    std::strcpy(m_data, other.m_data);
}

inline String& String::operator=(const String& other) {
    // 自赋值时若先 delete[]，other.m_data 也会失效，后续 strcpy 就是未定义行为。
    if (this == &other) {
        return *this;
    }

    // 本例按“释放旧资源 → 申请新资源 → 复制”的顺序展示基本机制。
    // 若 new 抛异常，对象会保留已释放的指针；生产代码可使用 copy-and-swap 获得更强异常安全。
    delete[] m_data;
    m_data = new char[std::strlen(other.m_data) + 1];
    std::strcpy(m_data, other.m_data);
    return *this; // 允许 a = b = c
}

inline String::~String() {
    // new[] 必须对应 delete[]，这样运行库才会按数组形式回收内存。
    delete[] m_data;
}

inline std::ostream& operator<<(std::ostream& os, const String& value) {
    // 返回流引用，使 std::cout << a << b 可以连续调用。
    return os << value.get_c_str();
}

#endif
