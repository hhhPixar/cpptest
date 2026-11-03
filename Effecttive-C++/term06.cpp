// Effective C++（第3版）Item 6：若不想使用编译器自动生成的函数，就该明确拒绝。
// 互斥锁这类对象不能有两个独立副本同时代表同一把锁。
// C++11 起可用 = delete，把“不允许复制”变成清晰的编译期诊断。
#include <iostream>

class NonCopyable {
protected:
    NonCopyable() = default;
    ~NonCopyable() = default;
public:
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};

class SessionLock : private NonCopyable {
public:
    SessionLock() { std::cout << "lock acquired\n"; }
    ~SessionLock() { std::cout << "lock released\n"; }

    void use() const { std::cout << "exclusive work\n"; }

    // 移动同样删除：本示例将锁的生命周期严格限制在原对象。
    SessionLock(SessionLock&&) = delete;
    SessionLock& operator=(SessionLock&&) = delete;
};

void inspect(const SessionLock& lock) {
    // 引用传参不复制，仍可安全使用不可复制对象。
    lock.use();
}

int main() {
    SessionLock lock;
    inspect(lock);

    // SessionLock copy(lock);  // 编译错误：复制构造已被明确删除。
    // SessionLock other; other = lock; // 编译错误：复制赋值已被明确删除。
    std::cout << "copying was rejected by the type interface\n";
}
