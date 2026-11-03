# C++ Concurrency in Action：并发学习实验

本目录以 Anthony Williams 的 *C++ Concurrency in Action* 为主线，使用**原创、独立、可运行的 C++14 实验**学习标准库并发工具。它不是原书代码转录，也不会用不可靠的“偶尔复现”的数据竞争或死锁作为正常示例。

本模块对应的核心问题是：多个执行线程如何安全结束、共享状态如何保护、线程如何等待和通信、异步结果如何传递、原子操作的内存顺序意味着什么，以及如何逐步构造并发数据结构。

> **先记住**：并发程序的正确性不能只凭“这次输出看起来对”。没有同步的并发读写同一对象会形成 data race，属于未定义行为；示例只展示安全写法，并在注释中解释错误写法为何危险。

## 编译方式

每个文件都有单独的 `main`，需分别链接线程库：

```bash
for source in CPP_Concurrency_In_Action/*.cpp; do
  name="/tmp/$(basename "${source%.cpp}")"
  c++ -std=c++14 -Wall -Wextra -pedantic -pthread "$source" -o "$name" && "$name"
done
```

在 macOS 上 `-pthread` 通常可省略；仍建议保留，以使命令在 Linux 等平台更可移植。

## 总体学习图

```text
线程生命周期
  thread -> join / detach / RAII join guard
       ↓
共享数据安全
  mutex -> lock_guard / unique_lock -> scoped locking
       ↓
等待与结果传递
  condition_variable -> queue
  async / future / promise / shared_future
       ↓
无锁基础
  atomic -> memory order -> 原子标志/计数
       ↓
并发设计
  线程安全栈/队列 -> call_once -> 锁层级 -> 线程池/并行任务
       ↓
内存模型与无锁边界
  release/acquire -> CAS -> ABA -> 安全回收
       ↓
性能与架构
  任务粒度 -> work stealing 模型 -> 消息传递 -> 协作式取消
```

## 01：线程管理

| 顺序 | 文件 | 核心观察 |
|---:|---|---|
| 01 | [01-01-hello-thread.cpp](<01-01-hello-thread.cpp>) | 创建线程与 `join()`；`thread` 析构前必须 `join` 或 `detach` |
| 02 | [01-02-thread-join-guard.cpp](<01-02-thread-join-guard.cpp>) | 用 RAII 保证异常路径也会 `join()` |
| 03 | [01-03-thread-arguments-ownership.cpp](<01-03-thread-arguments-ownership.cpp>) | 线程函数参数复制、引用包装与对象生命周期 |
| 04 | [01-04-thread-transfer-detach.cpp](<01-04-thread-transfer-detach.cpp>) | `std::move` 转移线程所有权；`detach` 的生命周期约束 |

阅读目标：`std::thread` 对象只表示“线程的管理权”，不等于线程函数中的数据所有权。优先 `join`；只有当后台任务不再访问会消失的状态时才考虑 `detach`。

## 02：在线程间共享数据

| 顺序 | 文件 | 核心观察 |
|---:|---|---|
| 01 | [02-01-mutex-data-race-safe-counter.cpp](<02-01-mutex-data-race-safe-counter.cpp>) | mutex 建立互斥，避免读改写丢失更新 |
| 02 | [02-02-deadlock-free-locking.cpp](<02-02-deadlock-free-locking.cpp>) | 用 `std::lock` 和 `adopt_lock` 同时获取多个锁 |
| 03 | [02-03-unique-lock-deferred-locking.cpp](<02-03-unique-lock-deferred-locking.cpp>) | 延迟加锁、可移动的 `unique_lock` 与条件变量的基础 |

阅读目标：锁保护的是**不变量**而非单个字段。先界定“哪些状态必须同时一致”，再决定临界区。

## 03：线程间同步与结果通信

| 顺序 | 文件 | 核心观察 |
|---:|---|---|
| 01 | [03-01-condition-variable-queue.cpp](<03-01-condition-variable-queue.cpp>) | 条件变量、谓词等待、虚假唤醒与生产者/消费者 |
| 02 | [03-02-future-async-exception.cpp](<03-02-future-async-exception.cpp>) | `async`、`future::get()`、结果与异常传播 |
| 03 | [03-03-promise-future-handoff.cpp](<03-03-promise-future-handoff.cpp>) | `promise` 将值或异常交给对应 future |
| 04 | [03-04-shared-future-broadcast.cpp](<03-04-shared-future-broadcast.cpp>) | 一个就绪结果如何被多个消费者读取 |

阅读目标：条件变量只负责“等待条件可能变化”的通知；**谓词**才说明何时可以继续。`future::get()` 只能调用一次，除非使用 `shared_future`。

## 04：原子操作与内存模型入口

| 顺序 | 文件 | 核心观察 |
|---:|---|---|
| 01 | [04-01-atomic-counter-orders.cpp](<04-01-atomic-counter-orders.cpp>) | 原子读改写、`relaxed` 计数与顺序含义 |
| 02 | [04-02-atomic-flag-spinlock.cpp](<04-02-atomic-flag-spinlock.cpp>) | `atomic_flag` 自旋锁及其适用范围 |

阅读目标：原子性不自动等于完整同步。`memory_order_relaxed` 只保证该原子对象自身的修改次序；发布其他普通数据通常需要 acquire/release 或 mutex。自旋锁不适合长临界区或单核/高竞争环境。

## 05：基于锁的并发数据结构

| 顺序 | 文件 | 核心观察 |
|---:|---|---|
| 01 | [05-01-threadsafe-stack.cpp](<05-01-threadsafe-stack.cpp>) | 栈操作、互斥与空栈错误处理 |
| 02 | [05-02-threadsafe-queue.cpp](<05-02-threadsafe-queue.cpp>) | 队列、条件变量与关闭协议 |

阅读目标：单个 `empty()` 和后续 `pop()` 分开调用不是一个原子事务；好的接口要把“检查并取出”合并，或在同一锁保护下完成。

## 06：并发设计准则

| 顺序 | 文件 | 核心观察 |
|---:|---|---|
| 01 | [06-01-call-once-lazy-init.cpp](<06-01-call-once-lazy-init.cpp>) | `std::call_once` 的一次性初始化与异常重试语义 |
| 02 | [06-02-hierarchical-mutex.cpp](<06-02-hierarchical-mutex.cpp>) | 用锁层级主动拒绝违反既定顺序的加锁 |

阅读目标：减少共享状态优先于增加锁。多个锁不可避免时，建立且记录全局锁顺序；层级锁只能辅助检查，不能自动设计正确的不变量。

## 07：并行算法与任务执行

| 顺序 | 文件 | 核心观察 |
|---:|---|---|
| 01 | [07-01-parallel-accumulate.cpp](<07-01-parallel-accumulate.cpp>) | 将区间分块、每块局部累加、主线程汇总 |
| 02 | [07-02-thread-pool-task-queue.cpp](<07-02-thread-pool-task-queue.cpp>) | 简化线程池的任务队列、关闭和 worker 回收 |

阅读目标：并行不必然更快：任务太小、线程创建、同步、缓存和负载不均都会抵消收益。线程池示例是教学模型，不等同于生产级执行器。

## 08：内存模型深入

| 顺序 | 文件 | 核心观察 |
|---:|---|---|
| 01 | [08-01-release-acquire-publication.cpp](<08-01-release-acquire-publication.cpp>) | release 发布普通数据、acquire 读取标志，从而建立 happens-before |
| 02 | [08-02-relaxed-vs-synchronization.cpp](<08-02-relaxed-vs-synchronization.cpp>) | relaxed 计数只保证原子变量本身；跨变量发布需要同步关系 |
| 03 | [08-03-compare-exchange-loop.cpp](<08-03-compare-exchange-loop.cpp>) | weak CAS 的重试循环、expected 参数更新和伪失败 |
| 04 | [08-04-sequential-consistency-order.cpp](<08-04-sequential-consistency-order.cpp>) | `seq_cst` 的单一全局顺序及其比 acquire/release 更强的约束 |

阅读目标：不要从一次机器上的输出“证明”内存模型。例子用同步建立确定结果；真正要掌握的是 load/store/RMW 之间的 happens-before 与 memory order 约束。

## 09：无锁算法与安全回收边界

| 顺序 | 文件 | 核心观察 |
|---:|---|---|
| 01 | [09-01-lock-free-stack-shared-ptr.cpp](<09-01-lock-free-stack-shared-ptr.cpp>) | 用 `shared_ptr` 原子自由函数安全发布不可变快照；不等于完整 lock-free 栈 |
| 02 | [09-02-aba-problem-model.cpp](<09-02-aba-problem-model.cpp>) | 地址 A→B→A 会欺骗裸地址 CAS；版本标签模型如何检测变化 |
| 03 | [09-03-memory-reclamation-guide.cpp](<09-03-memory-reclamation-guide.cpp>) | 读者公告与延迟回收的概念模型：读者仍可能访问时不能释放节点 |
| 04 | [09-04-atomic-shared-ptr-limitations.cpp](<09-04-atomic-shared-ptr-limitations.cpp>) | `shared_ptr` 原子装载/存储的保护范围、可能内部加锁与非事务性限制 |

阅读目标：**CAS 成功不等于算法完成**。无锁结构还必须解决 ABA、对象生命周期、内存回收、异常安全和进度保证。教学代码刻意避免把有悬空指针风险的伪无锁实现包装成正确答案。

## 10：高级并行任务与性能模型

| 顺序 | 文件 | 核心观察 |
|---:|---|---|
| 01 | [10-01-parallel-quicksort-futures.cpp](<10-01-parallel-quicksort-futures.cpp>) | 递归任务拆分、future 等待与阈值控制 |
| 02 | [10-02-work-stealing-model.cpp](<10-02-work-stealing-model.cpp>) | 工作窃取的本地优先、空闲 worker 窃取任务的教学模型 |
| 03 | [10-03-false-sharing-padding.cpp](<10-03-false-sharing-padding.cpp>) | 伪共享与 cache line padding；不能把一次计时当作结论 |
| 04 | [10-04-granularity-cost-model.cpp](<10-04-granularity-cost-model.cpp>) | 任务粒度、调度开销与并行收益的定性成本模型 |

阅读目标：并行算法必须同时考虑正确性、任务数、递归深度、负载均衡和硬件缓存。线程越多不必然越快。

## 11：并发架构与协作式控制

| 顺序 | 文件 | 核心观察 |
|---:|---|---|
| 01 | [11-01-message-passing-actor.cpp](<11-01-message-passing-actor.cpp>) | actor 式消息队列：将可变状态限制在单一消费者线程 |
| 02 | [11-02-cancellation-design.cpp](<11-02-cancellation-design.cpp>) | C++14 中用原子标志实现协作式取消；取消点和资源收尾责任 |

阅读目标：尽可能通过消息传递、不可变快照和所有权转移减少共享可变状态；取消不能强行终止线程，应由任务在安全取消点合作退出。

## 阅读顺序与安全清单

按 01 → 02 → 03 → 04 → 05 → 06 → 07 → 08 → 09 → 10 → 11 阅读。每读一个例子，都回答：

1. 哪些对象被多个线程访问？谁拥有它们？
2. 哪个 mutex/atomic/条件变量保护哪个不变量？
3. 等待的谓词是什么？通知丢失或虚假唤醒会怎样？
4. 所有线程在离开作用域前如何结束、`join`？
5. 如果线程函数抛异常，异常到哪里被观察和处理？
6. 涉及原子操作时：哪个操作发布数据，哪个操作获取数据？是否真的建立了 happens-before？
7. 涉及无锁结构时：节点何时可回收？CAS 遇到 ABA、失败或并发更新时如何处理？

## 书籍与版本边界

- 本项目使用 C++14，以便专注书中奠定的 C++11 并发库：`thread`、mutex、condition variable、future/promise、atomic。
- 书的第二版还讨论了之后标准逐渐加入的工具；本目录以线程、共享数据、同步、内存模型、无锁边界和并行设计为核心，不复刻原书代码或声称覆盖每个案例。
- C++20 的 `jthread`、stop token、barrier、latch、semaphore 等不属于本模块的 C++14 范围。
- 定时与输出顺序受调度影响；代码只断言同步关系和最终结果，不要求线程日志有固定交错顺序。
