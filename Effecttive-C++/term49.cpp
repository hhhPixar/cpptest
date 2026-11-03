#include <cstdlib>
#include <iostream>
#include <new>

// Item 49：了解 new-handler 的行为。
// 普通 operator new 分配失败会反复调用当前 new_handler；handler 要么释放资源、要么抛异常/终止。
void releaseEmergencyReserve() {
    std::cout << "new_handler：释放紧急储备并改为抛 bad_alloc\n";
    std::set_new_handler(nullptr); // 下一次分配失败时由 operator new 抛 std::bad_alloc。
}

int main() {
    std::new_handler old = std::set_new_handler(releaseEmergencyReserve);
    std::cout << "已安装 new_handler\n";
    // 真实耗尽内存不适合作为可重复的小实验；直接调用展示 handler 的契约与副作用。
    std::get_new_handler()();
    try { throw std::bad_alloc(); }
    catch (const std::bad_alloc&) { std::cout << "调用方可捕获 bad_alloc\n"; }
    std::set_new_handler(old);
}
// set_new_handler 返回旧处理器，便于像本例这样在结束前恢复全局状态。
// handler 不应简单返回且不改变任何条件，否则下一次失败会再次调用它。
// 常见策略包括释放缓存、降低请求，或直接抛出 bad_alloc。
// 本例不故意申请荒谬大小，避免测试依赖机器内存和 overcommit 策略。
// 直接调用已注册函数只演示协议；真实失败路径仍由 operator new 触发。
// std::nothrow new 的空指针语义也仍会先涉及 new-handler 的处理流程。
// 类专属 new-handler 是更进阶的封装问题，需要避免影响其他类。
// 还原 old 让该独立程序不会向后续代码泄漏全局设置。
// bad_alloc 被捕获后，程序可选择降级处理或向上报告失败。
// 内存分配失败是异常安全设计中必须明确面对的边界条件。
