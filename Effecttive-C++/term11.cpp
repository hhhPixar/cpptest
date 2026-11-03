// Effective C++（第3版）Item 11：在 operator= 中处理“自我赋值”。
// 直接先 delete 旧资源再读取 rhs，遇到 x = x 会读取已释放的内存。
// copy-and-swap 先复制，再以不会抛异常的交换提交结果，天然处理自赋值。
#include <iostream>
#include <string>
#include <utility>

class Note {
public:
    explicit Note(std::string text) : text_(new std::string(std::move(text))) {}
    Note(const Note& other) : text_(new std::string(*other.text_)) {}
    ~Note() { delete text_; }

    Note& operator=(const Note& other) {
        Note copy(other);       // 若分配失败，当前对象仍保持原值。
        swap(copy);             // 交换指针不抛异常；copy 随后释放旧资源。
        return *this;
    }

    void swap(Note& other) noexcept { std::swap(text_, other.text_); }
    void print() const { std::cout << *text_ << '\n'; }

private:
    std::string* text_;
};

int main() {
    Note note("keep this text");
    Note& same_object = note;
    note = same_object; // 别名形式仍是自我赋值；copy-and-swap 安全处理它且不触发编译器自赋值警告。
    note.print();

    Note copy("temporary");
    copy = note;
    copy.print();
}
