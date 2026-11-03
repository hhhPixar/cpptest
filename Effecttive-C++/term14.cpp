// Effective C++（第3版）Item 14：在资源管理类中小心 copying 行为。
// 资源类复制时必须定义语义：禁止复制、深复制、转移所有权或共享所有权。
// 本例选择 shared_ptr：两张“租约”共同拥有同一个文件式资源。
#include <iostream>
#include <memory>
#include <string>

class Printer {
public:
    explicit Printer(std::string name) : name_(std::move(name)) {
        std::cout << "acquire " << name_ << '\n';
    }
    ~Printer() { std::cout << "release " << name_ << '\n'; }
    void print(const char* text) const { std::cout << name_ << ": " << text << '\n'; }
private:
    std::string name_;
};

class PrintLease {
public:
    explicit PrintLease(std::string name) : printer_(std::make_shared<Printer>(std::move(name))) {}
    void print(const char* text) const { printer_->print(text); }
    long owners() const { return printer_.use_count(); }
private:
    // shared_ptr 的复制增加引用计数；最后一个 PrintLease 消失时才释放 Printer。
    std::shared_ptr<Printer> printer_;
};

int main() {
    PrintLease morning("office-printer");
    {
        PrintLease afternoon = morning;
        afternoon.print("shared job");
        std::cout << "owners: " << morning.owners() << '\n';
    }
    std::cout << "owners after copy dies: " << morning.owners() << '\n';
}
