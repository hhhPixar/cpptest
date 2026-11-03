#ifndef __SINGLETON__
#define __SINGLETON__

#include <iostream>

// Meyers Singleton：函数内局部 static 对象在第一次调用 getInstance 时才创建。
// 从 C++11 起，此初始化在多线程环境中也保证只执行一次。
class Singleton {
public:
    static Singleton& getInstance() {
        static Singleton instance;
        return instance;
    }

    void setup() const {
        std::cout << "setting up" << std::endl;
    }

    // 单例对象不应被复制或赋值，否则就会出现第二个实例。
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

private:
    // 构造函数私有化，外部只能通过 getInstance() 获取对象。
    Singleton() { std::cout << "create" << std::endl; }
    ~Singleton() = default;
};

#endif
