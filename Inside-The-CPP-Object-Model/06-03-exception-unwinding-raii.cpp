#include <iostream>
#include <stdexcept>

// 第 6 章实验：异常离开作用域时会进行栈展开，已构造的自动对象
// 按构造的反序自动析构。RAII 把释放操作放入析构函数，因而异常路径
// 与正常路径共享同一份资源清理逻辑。
class ScopeGuard {
public:
    explicit ScopeGuard(const char* name) : name_(name) {
        std::cout << "获得资源: " << name_ << '\n';
    }

    ~ScopeGuard() {
        // 析构函数应当不抛异常，否则栈展开期间会导致 std::terminate。
        std::cout << "释放资源: " << name_ << '\n';
    }

private:
    const char* name_;
};

void readConfiguration(bool malformed) {
    ScopeGuard file("配置文件句柄");
    ScopeGuard lock("配置互斥锁");

    if (malformed) {
        throw std::runtime_error("配置格式错误");
    }
    std::cout << "配置读取成功\n";
} // 正常返回也按反序释放 lock、file。

int main() {
    try {
        readConfiguration(true);
        std::cout << "这一行不会执行\n";
    } catch (const std::exception& error) {
        // 到达 catch 前，readConfiguration 中两个守卫均已析构。
        std::cout << "捕获异常: " << error.what() << '\n';
    }

    std::cout << "程序可在清理完成后继续运行\n";
}
