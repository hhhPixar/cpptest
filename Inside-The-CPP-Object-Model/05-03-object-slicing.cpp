#include <iostream>
#include <string>

// 第 5 章实验：把派生对象按值传给基类参数会发生对象切片，派生状态被丢弃。
// 需要保持多态时，应传递 Base& 或 Base*，且基类析构函数通常应为 virtual。
class Document {
public:
    explicit Document(std::string title) : title_(std::move(title)) {}
    virtual ~Document() = default;

    virtual void print() const {
        std::cout << "Document: " << title_ << '\n';
    }

private:
    std::string title_;
};

class Report : public Document {
public:
    Report(std::string title, int pages) : Document(std::move(title)), pages_(pages) {}

    void print() const override {
        std::cout << "Report: " << pages_ << " pages\n";
    }

private:
    int pages_;
};

void printByValue(Document document) {
    // 形参只保存 Document 子对象，动态类型已经成为 Document。
    document.print();
}

void printByReference(const Document& document) {
    // 引用不复制对象，因此仍可根据完整对象的动态类型分派。
    document.print();
}

int main() {
    Report report("Object Model", 12);
    printByValue(report);
    printByReference(report);
}
