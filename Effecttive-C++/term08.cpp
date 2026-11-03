// Effective C++（第3版）Item 8：别让异常逃离析构函数。
// 栈展开期间若第二个异常从析构函数逃出，C++ 会调用 std::terminate。
// 因此析构函数应吞下/记录关闭失败，并提供可显式报告失败的 close()。
#include <iostream>
#include <stdexcept>

class Connection {
public:
    ~Connection() noexcept {
        if (!closed_) {
            try {
                close();
            } catch (const std::exception& error) {
                // 析构路径只能记录：此处不能再把异常传播给调用者。
                std::cerr << "destructor suppressed: " << error.what() << '\n';
            }
        }
    }

    void close() {
        if (closed_) return;
        closed_ = true;
        if (fail_on_close_) throw std::runtime_error("network close failed");
        std::cout << "connection closed\n";
    }
    void simulate_failure() { fail_on_close_ = true; }

private:
    bool closed_ = false;
    bool fail_on_close_ = false;
};

int main() {
    Connection good;
    good.close(); // 推荐：在正常控制流显式关闭，调用者可自行处理异常。

    {
        Connection damaged;
        damaged.simulate_failure();
        // 析构时发生失败，但 noexcept 析构函数捕获它，程序能继续运行。
    }
    std::cout << "program remains alive\n";
}
