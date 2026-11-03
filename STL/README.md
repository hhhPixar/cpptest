# STL：从使用到源码设计

本目录不尝试复制某个实现（libstdc++、libc++、MSVC STL）的全部源码；它们包含大量 ABI 兼容、调试器支持、平台适配和边界优化。这里选择每个方向最能解释设计取舍的**精简源码模型**：保留不变量、调用关系和异常安全边界，删去与核心思想无关的工程细节。

> 阅读原则：先运行“使用层”示例确认行为，再阅读对应“源码模型”。模型使用的类名 `Mini*` 不是标准库实现，且只实现了为说明设计所需的最小接口。

## 1. 一张图理解 STL 的分层

```text
用户代码
  │
  ├── 容器：保存对象与维护数据结构不变量
  │     vector / list / map / unordered_map / stack ...
  │
  ├── 迭代器：把“如何遍历”抽象成能力等级
  │     input → forward → bidirectional → random access
  │
  ├── 算法：只操作 [first, last) 与可调用对象，不拥有容器
  │     find / sort / lower_bound / transform / ...
  │
  ├── 函数对象、比较器、hash：把策略作为类型/对象注入
  │
  └── allocator：把“原始内存”和“对象构造/销毁”分离
```

这种拆分的关键收益是**正交组合**：同一个算法可以服务多个容器；同一个容器可以接受不同的元素、比较器、哈希器和分配器；编译器在模板实例化时将这些组合具体化，而非依赖运行时虚调用。

## 2. 源码级主线：六个精华模型

| 源码模型 | 解释的核心 | 对应标准库主题 | 读完应回答的问题 |
|---|---|---|---|
| [implementation-vector.cpp](<implementation-vector.cpp>) | 三指针、扩容、对象构造与异常安全 | `vector`、allocator、移动语义 | 为什么尾插均摊 O(1)，扩容为何使迭代器失效？ |
| [implementation-iterator.cpp](<implementation-iterator.cpp>) | 标签分发、能力分层 | `iterator_traits`、`advance` | 为什么 `vector` 能跳跃、`list` 只能逐节点走？ |
| [implementation-algorithm.cpp](<implementation-algorithm.cpp>) | 半开区间、比较器、二分不变量 | `lower_bound`、`binary_search` | 为什么二分前必须按同一比较器排序？ |
| [implementation-ordered-index.cpp](<implementation-ordered-index.cpp>) | 比较器、BST 查找路径、平衡的必要性 | `set`、`map` | `map` 为什么能保持键有序且通常 O(log N)？ |
| [implementation-hashtable.cpp](<implementation-hashtable.cpp>) | 桶、冲突链、rehash、负载因子 | `unordered_map/set` | 哈希表平均 O(1) 的前提是什么？ |
| [implementation-adapter.cpp](<implementation-adapter.cpp>) | 组合、受限接口、底层容器可替换 | `stack`、`queue` | 为什么适配器不暴露全部底层容器操作？ |

### 建议源码阅读顺序

```text
implementation-vector
        ↓
implementation-iterator → implementation-algorithm
        ↓                        ↓
implementation-ordered-index   implementation-hashtable
        ↓                        ↓
              implementation-adapter
```

先理解连续存储与生命周期，再理解“容器只交出迭代器、算法只要能力”的解耦；最后比较树索引、哈希索引和受限接口三种不同设计。

---

## 3. 容器设计：不变量决定接口

### 3.1 `vector`：连续内存换随机访问与缓存局部性

阅读 [implementation-vector.cpp](<implementation-vector.cpp>)，重点跟踪：

```text
[begin_, end_)          已构造的元素
[end_, capacity_end_)   已分配但尚未构造的原始内存
```

`allocator::allocate` 只获得原始内存；`construct` 才在指定位置开始对象生命周期；析构时必须先 `destroy` 已构造对象，再 `deallocate` 原始内存。这解释了为什么容器内部不能把“申请内存”和“构造对象”混为一步。

扩容采取几何增长（模型中为 2 倍）：第 1 次搬 1 个、第 2 次搬 2 个、第 3 次搬 4 个……总搬迁量小于最终元素数的常数倍，因此尾插的**均摊**复杂度为 O(1)，但发生扩容的单次操作仍是 O(N)。新空间完全构造成功后才销毁旧空间，体现提交式的异常安全设计。

配合使用层：[vector.cpp](<vector.cpp>)、[sequence-containers.cpp](<sequence-containers.cpp>)。

### 3.2 链表与 deque：用不同布局换不同操作代价

[sequence-containers.cpp](<sequence-containers.cpp>) 展示的 `list`/`forward_list` 不需要复制完整实现即可理解其节点模型：每个元素附带链接指针，因此已知位置插删可重连链接；代价是额外指针空间、差的缓存局部性、以及不支持随机访问。

`deque` 通常采用“分段连续块 + 索引表”而非单块连续数组，从而兼顾两端增长和随机访问。**分段大小、map 布局和失效细节均是实现细节**，使用者只应依赖标准承诺。

### 3.3 有序关联容器：比较器定义“键等价”

[implementation-ordered-index.cpp](<implementation-ordered-index.cpp>) 故意只实现普通 BST：它让“向左/向右/相等”的查找路径直观可见，也暴露了有序输入退化为 O(N) 的问题。真实 `map/set` 实现通常以红黑树等平衡树维护高度，因此查找、插入和删除通常为 O(log N)。

`map` 的键是 `const Key`，因为就地修改键会破坏树的排序不变量。比较器中的“等价”定义为：

```cpp
!comp(a, b) && !comp(b, a)
```

它不必等于 `operator==`。这就是比较器必须满足严格弱序的原因。

配合使用层：[associative-containers.cpp](<associative-containers.cpp>)。

### 3.4 无序关联容器：哈希决定桶，等价函数确认键

[implementation-hashtable.cpp](<implementation-hashtable.cpp>) 展示分离链接法：`hash(key) % bucket_count` 定位桶，再沿冲突链以 `equal_to` 确认真正相等的键。哈希函数只负责**分布**，不能单独确认键相等。

平均 O(1) 建立在三项假设上：哈希分布均匀、负载因子受控、单桶链不过长。元素/桶数太高时需要 `rehash`，重新按新桶数链接节点；这也是标准库规定相关迭代器失效的根源。请勿依赖模型中“节点地址恰好未变”的实现细节。

配合使用层：[unordered-containers.cpp](<unordered-containers.cpp>)。

### 3.5 容器适配器：故意少给接口

[implementation-adapter.cpp](<implementation-adapter.cpp>) 的 `MiniStack` 用组合保存底层 `deque`，但不暴露 `begin`、`operator[]` 或 `push_front`。这不是功能不足，而是 API 设计：调用者只能从栈顶进出，LIFO 不变量不被破坏。

配合使用层：[adapters.cpp](<adapters.cpp>)。

---

## 4. 迭代器与算法：用“能力”而非“容器名称”编程

### 4.1 为什么 STL 算法通常不写成容器成员函数？

`std::find` 只需要可递增、可解引用的迭代器；`std::sort` 还需要随机访问；`std::lower_bound` 至少需要前向迭代器（随机访问迭代器可获得更好的步进效率）。若算法绑定在每个容器内，算法实现会大量重复，且用户自定义范围难以接入。

[implementation-iterator.cpp](<implementation-iterator.cpp>) 模拟 `std::advance`：通过 `iterator_traits<It>::iterator_category` 和标签重载，让编译器在实例化时选择 `it += n` 或循环 `++it`。这是**静态多态**：调用路径在编译期确定。

### 4.2 `lower_bound` 的循环不变量

[implementation-algorithm.cpp](<implementation-algorithm.cpp>) 保持如下不变量：答案始终在当前候选区间 `[first, first + count)` 内。

- 若 `comp(*middle, value)` 为真，`middle` 与它左边元素都不可能是答案，丢弃左半；
- 否则 `middle` 仍可能是第一个不小于 `value` 的位置，保留左半；
- `count == 0` 时，`first` 即答案。

注意“有序”必须相对于同一个 `comp` 成立，否则“丢弃半边”的逻辑没有依据。

配合使用层：[iterators.cpp](<iterators.cpp>)、[algorithms.cpp](<algorithms.cpp>)、[function-objects.cpp](<function-objects.cpp>)。

---

## 5. 分配器、对象生命周期与异常安全

标准容器通过 allocator traits 抽象内存策略。学习时应分清四个阶段：

1. `allocate(n)`：取得可容纳 `n` 个对象的**原始内存**；
2. `construct(pointer, args...)`：在原始内存指定位置构造对象；
3. `destroy(pointer)`：结束对象生命周期；
4. `deallocate(pointer, n)`：归还原始内存。

[implementation-vector.cpp](<implementation-vector.cpp>) 已包含这四步和扩容失败时的回滚路径。标准库常借助 RAII guard、类型萃取和针对平凡类型的优化完成同样目标；不要把模型的手写循环视为生产代码最佳写法。

---

## 6. 使用层示例索引

| 文件 | 主要知识点 | 源码模型对应 |
|---|---|---|
| [array.cpp](<array.cpp>) | 固定数组、排序、二分 | 算法 |
| [vector.cpp](<vector.cpp>) | 容量、线性/二分查找 | vector、算法 |
| [sequence-containers.cpp](<sequence-containers.cpp>) | 四种顺序容器 | vector/链表/deque |
| [iterators.cpp](<iterators.cpp>) | 范围、反向与 const 迭代器 | 迭代器 |
| [algorithms.cpp](<algorithms.cpp>) | `sort`、`transform`、erase-remove | 算法 |
| [function-objects.cpp](<function-objects.cpp>) | 仿函数、lambda、比较器 | 算法、策略 |
| [associative-containers.cpp](<associative-containers.cpp>) | `set/map/multimap` | 有序索引 |
| [unordered-containers.cpp](<unordered-containers.cpp>) | 哈希容器 | 哈希表 |
| [adapters.cpp](<adapters.cpp>) | 栈、队列、优先队列 | 适配器 |

## 7. 刻意没有完整实现的部分

- **红黑树旋转与删除修复：** 工程细节多，容易淹没“排序不变量 + 平衡保证”的主线；先掌握 BST 模型和 `map` 使用，再针对特定标准库源码追读。
- **完整 vector 复制/插入/erase API：** 这些操作都是“三指针 + 构造/销毁 + 异常安全”的不同组合。先透彻理解扩容，再查阅实现的区间插入路径。
- **完整 allocator traits、传播规则与 fancy pointer：** 是库实现兼容性主题，超过本阶段“容器生命周期”的必要深度。
- **`priority_queue` 堆调整：** 使用层先理解堆语义；后续可从 `<algorithm>` 的 `push_heap/pop_heap` 源码继续。

这不是遗漏：目标是每个方向选一个最小模型承载其关键不变量，避免在尚未建立框架时堆积实现细节。

## 8. 编译方式

每个 `implementation-*.cpp` 均可独立编译运行：

```bash
c++ -std=c++14 -Wall -Wextra -pedantic STL/implementation-vector.cpp -o mini-vector
./mini-vector
```

本目录的 [array.cpp](<array.cpp>) 与 [vector.cpp](<vector.cpp>) 是可被外部教学程序调用的函数集合，不含 `main`；请用 `-c` 将它们编译为目标文件，或另写调用程序。
