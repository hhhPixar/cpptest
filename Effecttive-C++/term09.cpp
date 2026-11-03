// Effective C++（第3版）Item 9：绝不在构造和析构过程中调用 virtual 函数。
// 基类构造期间派生部分尚不存在；基类析构期间派生部分已消失。
// 此时虚调用只按当前基类版本分派，容易造成误解或访问未就绪状态。
#include <iostream>
#include <string>

class AuditBase {
public:
    AuditBase() {
        std::cout << "base constructor calls: ";
        report();
    }
    virtual ~AuditBase() {
        std::cout << "base destructor calls: ";
        report();
    }
    virtual void report() const { std::cout << "base report\n"; }
};

class FileAudit : public AuditBase {
public:
    explicit FileAudit(std::string name) : name_(std::move(name)) {
        // 派生构造完成后，从这里调用 virtual 函数才会分派到 FileAudit。
        std::cout << "derived constructor calls: ";
        report();
    }
    ~FileAudit() override { std::cout << "derived destructor\n"; }
    void report() const override { std::cout << "file=" << name_ << '\n'; }
private:
    std::string name_;
};

int main() {
    FileAudit audit("build.log");
    std::cout << "normal call: ";
    audit.report();
    // 输出中的两次 base report 展示了构造/析构阶段的受限虚分派。
}
