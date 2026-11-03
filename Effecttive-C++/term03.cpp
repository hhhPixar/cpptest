#include <cstring>
#include <iostream>
#include <stdexcept>

// Item 3：const 成员函数与非 const 成员函数应避免维护两份逻辑。
// 本例用动态字符数组演示；真实项目优先使用 std::string 管理资源。
class TextBlock {
public:
    explicit TextBlock(const char* text) {
        const char* source = text == nullptr ? "" : text;
        const std::size_t length = std::strlen(source);
        data_ = new char[length + 1];  // +1 为字符串末尾的 '\0' 留出空间。
        std::strcpy(data_, source);
    }

    // 深拷贝：使每个对象拥有独立的字符数组，避免析构时重复释放。
    TextBlock(const TextBlock& other) : TextBlock(other.data_) {}

    TextBlock& operator=(const TextBlock& other) {
        if (this != &other) {
            TextBlock copy(other);  // 先完成可能抛异常的分配，保证强异常安全。
            swap(copy);
        }
        return *this;
    }

    ~TextBlock() {
        delete[] data_;
    }

    const char& operator[](std::size_t index) const {
        // 下标检查属于示例额外保护；原书条款的重点是 const 重载复用。
        if (index >= std::strlen(data_)) {
            throw std::out_of_range("TextBlock index out of range");
        }
        return data_[index];
    }

    char& operator[](std::size_t index) {
        // 先把 *this 视为 const 对象并调用唯一的读取实现，再移除结果的 const。
        // 此转换安全的前提：当前对象本身不是 const；本重载正满足此前提。
        return const_cast<char&>(static_cast<const TextBlock&>(*this)[index]);
    }

private:
    void swap(TextBlock& other) noexcept {
        char* temporary = data_;
        data_ = other.data_;
        other.data_ = temporary;
    }

    char* data_;
};

int main() {
    TextBlock editable("text");
    const TextBlock readOnly("hello");

    editable[0] = 'T';  // 选择非 const operator[]，因此结果可写。
    std::cout << editable[1] << '\n';
    std::cout << readOnly[0] << '\n';  // 选择 const operator[]，因此结果只读。
}
