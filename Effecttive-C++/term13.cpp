// Effective C++（第3版）Item 13：以对象管理资源。
// RAII 把资源取得放进构造函数，把释放放进析构函数，作用域成为所有权边界。
// std::unique_ptr 表达独占所有权，并在异常或提前 return 时自动释放对象。
#include <iostream>
#include <memory>
#include <stdexcept>

class Socket {
public:
    explicit Socket(int id) : id_(id) { std::cout << "open socket " << id_ << '\n'; }
    ~Socket() { std::cout << "close socket " << id_ << '\n'; }
    void send(const char* message) { std::cout << "send: " << message << '\n'; }
private:
    int id_;
};

void transmit(bool fail) {
    // 资源取得后立刻交给 unique_ptr；不用手写任何 delete。
    std::unique_ptr<Socket> socket(new Socket(42));
    socket->send("hello");
    if (fail) throw std::runtime_error("link lost");
    socket->send("goodbye");
} // 无论普通离开还是异常离开，unique_ptr 都会调用 Socket 析构函数。

int main() {
    try {
        transmit(true);
    } catch (const std::exception& error) {
        std::cout << "caught: " << error.what() << '\n';
    }
    std::cout << "resource was still released\n";
}
