#include <iostream>
#include <string>

// 第 5 章实验：没有继承关系时，成员按声明顺序构造、按逆序析构。
// 编译器生成的复制构造会依成员顺序复制；本例显式写出它以记录观察结果。
class Label {
public:
    explicit Label(std::string text) : text_(std::move(text)) {
        std::cout << "Label ctor: " << text_ << '\n';
    }

    Label(const Label& other) : text_(other.text_) {
        std::cout << "Label copy: " << text_ << '\n';
    }

    ~Label() {
        std::cout << "Label dtor: " << text_ << '\n';
    }

private:
    std::string text_;
};

class Card {
public:
    Card(std::string front, std::string back)
        // 初始化列表应按声明顺序书写以避免 -Wreorder；无论书写顺序怎样，
        // 语言实际构造顺序始终由成员声明 first_、second_ 决定。
        : first_(std::move(front)), second_(std::move(back)) {
        std::cout << "Card ctor body\n";
    }

    Card(const Card& other) : first_(other.first_), second_(other.second_) {
        std::cout << "Card copy body\n";
    }

    ~Card() {
        std::cout << "Card dtor body\n";
    }

private:
    Label first_;
    Label second_;
};

int main() {
    std::cout << "--- construct ---\n";
    Card original("front", "back");
    std::cout << "--- copy ---\n";
    Card copy(original);
    std::cout << "--- scope exit ---\n";
}
