#include <iostream>
#include <string>

// Effective C++ 第三版 Item 20：宁以 pass-by-reference-to-const 替换 pass-by-value。
// 值传递会复制实参；const 引用既避免复制，也承诺函数不修改调用者对象。
// 这尤其适用于用户自定义对象和容器；它们的复制代价可能远高于一个引用。
// 函数若确实需要副本，再在函数体中按需创建，而不是强迫每个调用者预先复制。
class Report {
public:
    explicit Report(std::string title) : title_(std::move(title)) {}
    Report(const Report& other) : title_(other.title_) {
        std::cout << "发生一次复制\n";
    }
    const std::string& title() const { return title_; }
private:
    std::string title_;
};

// 对大型对象只观察时，const 引用不触发 Report 的复制构造。
void printTitle(const Report& report) {
    std::cout << "报告: " << report.title() << '\n';
}

int main() {
    Report weekly("周报");
    printTitle(weekly);

    // void bad(Report); // 若改为值传递，每次调用都会复制整个 Report。
    // 内建小类型（如 int）按值传递通常更简单且高效，不能机械套用本条。
}
