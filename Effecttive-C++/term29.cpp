#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>

// Effective C++ 第三版 Item 29：为“异常安全”而努力是值得的。
// 这里以“先构造临时副本、成功后交换”实现强保证：失败时原对象不改变。
class Playlist {
public:
    explicit Playlist(std::vector<int> songs) : songs_(std::move(songs)) {}

    void replaceWith(const std::vector<int>& candidate, bool fail) {
        std::vector<int> next = candidate; // 复制/分配若抛异常，songs_ 尚未被修改。
        if (fail) {
            throw std::runtime_error("模拟校验失败");
        }
        songs_.swap(next); // vector::swap 不抛异常，提交改变。
    }

    void print() const {
        for (int song : songs_) std::cout << song << ' ';
        std::cout << '\n';
    }

private:
    std::vector<int> songs_;
};

int main() {
    Playlist list({1, 2});
    try {
        list.replaceWith({7, 8, 9}, true);
    } catch (const std::exception& error) {
        std::cout << "捕获: " << error.what() << '\n';
    }
    std::cout << "失败后仍为: ";
    list.print();
    list.replaceWith({7, 8, 9}, false);
    std::cout << "成功后为: ";
    list.print();
}
