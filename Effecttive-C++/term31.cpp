#include <iostream>
#include <memory>
#include <string>

// Effective C++ 第三版 Item 31：将文件间的编译依存关系降至最低。
// 本单文件实验模拟 Pimpl：公开的 Document 只持有实现对象的指针，
// 因而在真实头文件中可只前置声明 Implementation，不暴露 string 等细节。
// 真实 Pimpl 还会把析构、复制/移动等需完整类型的定义放在实现文件中。
// 这里为保证“一文件一实验”可编译，将实现结构体保留在同一文件的 private 区。
class Document {
public:
    Document(std::string title, int pages)
        : impl_(new Implementation{std::move(title), pages}) {}

    std::string summary() const {
        return impl_->title + "，页数=" + std::to_string(impl_->pages);
    }

private:
    struct Implementation { // 实际工程中可移至 .cpp，头文件只写 struct Implementation;
        std::string title;
        int pages;
    };
    std::unique_ptr<Implementation> impl_;
};

int main() {
    Document guide("编译隔离指南", 12);
    std::cout << guide.summary() << '\n';

    // 修改 Implementation 的容器、缓存等细节不会迫使只依赖 Document 接口的客户重编译。
}
