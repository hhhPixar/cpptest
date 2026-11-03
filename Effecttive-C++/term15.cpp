// Effective C++（第3版）Item 15：在资源管理类中提供对原始资源的访问。
// RAII 对象应保留所有权，同时在必要时受控地暴露底层资源给旧式 API。
// get() 返回借用指针；调用者不得 delete，也不得保存到拥有者销毁之后。
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>

class File {
public:
    explicit File(const char* path)
        : handle_(std::fopen(path, "w"), &std::fclose) {
        if (!handle_) throw std::runtime_error("cannot open output file");
    }

    std::FILE* get() const noexcept { return handle_.get(); }
private:
    // 自定义删除器使 unique_ptr 在作用域结束时调用 fclose。
    std::unique_ptr<std::FILE, int (*)(std::FILE*)> handle_;
};

void legacy_write(std::FILE* raw, const char* text) {
    // 旧 API 只借用 FILE*，不承担关闭责任。
    std::fputs(text, raw);
    std::fputc('\n', raw);
}

int main() {
    File file("/tmp/effective-cpp-term15.txt");
    legacy_write(file.get(), "RAII still owns this FILE");
    std::cout << "legacy API received a borrowed handle\n";
    // 绝不可写 std::fclose(file.get())；那会让 unique_ptr 之后再次关闭同一资源。
}
