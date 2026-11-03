#include <iostream>
#include <memory>
#include <vector>

// Item 55：让自己熟悉 Boost。
// 本项目不强制依赖 Boost：本例使用 C++14 标准库演示其“先借鉴并验证通用组件”的学习路线。
// 多个 Boost 成员后来进入标准库（如 shared_ptr）；但 Boost 的具体版本和接口仍应查官方文档验证。
struct Task {
    explicit Task(int id) : id(id) {}
    int id;
};

int main() {
    std::vector<std::shared_ptr<Task>> queue;
    queue.push_back(std::make_shared<Task>(101));
    queue.push_back(std::make_shared<Task>(102));
    for (const auto& task : queue) std::cout << "任务 " << task->id << '\n';
    // 若项目需要 Boost，应按构建系统显式引入，并为所选 Boost 版本编译测试。
}
// Boost 是社区维护的高质量 C++ 库集合，很多设施影响或进入了标准。
// “熟悉”不等于无条件引入：应根据功能需求、许可证、版本和构建成本评估。
// 本例不伪装为 Boost API，也不要求机器安装 Boost，故可诚实地直接编译。
// shared_ptr 展示的是现代标准库对应能力，而非宣称与某个 Boost 版本完全等价。
// make_shared 将控制块与对象的分配策略封装起来，通常比分别 new 更合适。
// 队列存储共享所有权对象只是教学选择；生产代码应先确认所有权确实共享。
// 若引入 Boost，应让 CMake 或其他构建系统显式发现并链接所需组件。
// 某些 Boost 库仅头文件，另一些需要链接二进制库，不能一概而论。
// 升级 Boost 前应阅读迁移说明并在支持的编译器矩阵上重新测试。
// 官方文档、发行说明和项目锁定版本才是接口细节的可靠依据。
// 输出两个任务号证明标准库替代式导读程序可独立运行。
