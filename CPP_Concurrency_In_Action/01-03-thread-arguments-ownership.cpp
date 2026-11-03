#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <utility>

// 按值接收字符串：线程入口拥有自己的副本，不依赖调用者局部对象的寿命。
void print_copy(std::string text) {
    std::cout << "按值传入: " << text << '\n';
}

// unique_ptr 按值接收，明确表示资源的唯一所有权要转交给新线程。
void consume_number(std::unique_ptr<int> number) {
    std::cout << "线程取得唯一资源: " << *number << '\n';
}

int main() {
    std::string message = "线程参数在构造时复制";

    // 若直接传 message，std::thread 会保存其副本；std::ref 才会传引用。
    std::thread copied_thread(print_copy, message);
    message = "主线程后来修改的内容";

    std::unique_ptr<int> value(new int(42));
    // unique_ptr 不可复制，必须用 std::move 显式把所有权移入线程参数存储。
    std::thread owner_thread(consume_number, std::move(value));

    // move 之后主线程不再拥有资源，不能解引用 value。
    std::cout << "主线程仍拥有资源吗: " << (value ? "是" : "否") << '\n';

    // 两个线程均被回收，main 返回前不会留下可运行线程。
    copied_thread.join();
    owner_thread.join();
    return 0;
}
