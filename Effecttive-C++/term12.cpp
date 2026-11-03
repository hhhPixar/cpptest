// Effective C++（第3版）Item 12：复制对象时勿忘其每一个成分。
// 自定义复制构造或复制赋值时，基类子对象和全部成员都必须被正确复制。
// 本例让 Revision 的默认复制负责基类状态，避免手写时漏掉 version_。
#include <iostream>
#include <string>

class Revision {
public:
    explicit Revision(int version) : version_(version) {}
    int version() const { return version_; }
private:
    int version_;
};

class Article : public Revision {
public:
    Article(std::string title, int version, int words)
        : Revision(version), title_(std::move(title)), words_(words) {}

    Article(const Article& other)
        : Revision(other), title_(other.title_), words_(other.words_) {
        std::cout << "copy constructor copied base and members\n";
    }

    Article& operator=(const Article& other) {
        if (this != &other) {
            Revision::operator=(other); // 显式复制基类子对象。
            title_ = other.title_;
            words_ = other.words_;
        }
        return *this;
    }

    void print() const { std::cout << title_ << " v" << version() << ", " << words_ << " words\n"; }
private:
    std::string title_;
    int words_;
};

int main() {
    Article original("copy completely", 7, 320);
    Article copied(original);
    Article assigned("old", 1, 1);
    assigned = original;
    copied.print();
    assigned.print();
}
