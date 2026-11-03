#include <cstring>
#include <iostream>
#include <utility>

// 第 2 章：含资源指针的类必须定义正确的复制语义。
// 若直接使用编译器默认复制构造，两个 Word 会指向同一块字符数组，
// 在析构时造成 double delete；这里以“深拷贝 + copy-and-swap”修正它。
class Word {
public:
    explicit Word(const char* text) : text_(duplicate(text == nullptr ? "" : text)) {
        std::cout << "constructed: " << text_ << '\n';
    }

    Word(const Word& other) : text_(duplicate(other.text_)) {
        std::cout << "copied: " << text_ << '\n';
    }

    Word(Word&& other) noexcept : text_(other.text_) {
        other.text_ = nullptr;
        std::cout << "moved\n";
    }

    Word& operator=(Word other) noexcept {
        // 参数按值传入：先完成拷贝（或移动），再交换，自动处理自赋值。
        swap(other);
        return *this;
    }

    ~Word() {
        delete[] text_;
    }

    const char* c_str() const {
        return text_;
    }

    void swap(Word& other) noexcept {
        std::swap(text_, other.text_);
    }

private:
    static char* duplicate(const char* source) {
        const std::size_t length = std::strlen(source);
        char* copy = new char[length + 1];
        std::strcpy(copy, source);
        return copy;
    }

    char* text_;
};

int main() {
    Word original("hello");
    Word copy = original;  // 调用深拷贝构造，不与 original 共享字符数组。

    Word assigned("temporary");
    assigned = original;  // 复制赋值同样获得独立资源。

    std::cout << original.c_str() << ", " << copy.c_str()
              << ", " << assigned.c_str() << '\n';
}
