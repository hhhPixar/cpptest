# Effective C++（第 3 版）逐条实验

本目录以 Scott Meyers《Effective C++》**第三版（2005，55 个 Items）**为参照。现在采用和原有 `term02.cpp`、`term03.cpp` 一致的结构：

```text
term01.cpp  <=> Item 1
term02.cpp  <=> Item 2
...
term55.cpp  <=> Item 55
```

每个 `termNN.cpp` 都是一个**独立、原创、可编译运行**的 C++14 小实验：顶部写出对应条款的中文主题和学习目标，代码内解释状态变化、资源所有权、类型约束或异常路径。它们不是原书代码或正文的转录，不能替代原书的完整论证。

- 第三版与《More Effective C++》《Effective STL》《Effective Modern C++》是不同书籍；本目录只以第三版的 55 个 Items 为编号框架。
- 版本与范围可在 [Scott Meyers 作者书目页](https://www.aristeia.com/books.html) 核验：该页面将 *Effective C++ Third Edition* 标为 2005 年出版，并说明其含资源管理、模板、异常等新增内容。
- 原书面向 C++98 语境；示例在必要时用 C++11/14 的 `unique_ptr`、`=delete`、`override` 等工具表达相同的设计约束，代码注释会说明现代化差异。

## 编译与运行

每个条款是一个单独程序，不能把它们一起链接：每个文件都有自己的 `main`。

```bash
for source in Effecttive-C++/term*.cpp; do
  name="/tmp/$(basename "${source%.cpp}")"
  c++ -std=c++14 -Wall -Wextra -pedantic "$source" -o "$name" && "$name"
done
```

某些示例故意将错误写法放入 `#if 0`，因此常规构建仍会成功；阅读时可打开它们观察编译器诊断，但不要提交取消屏蔽的版本。

## 条款索引与阅读主线

| 阶段 | Items | 主入口 | 核心问题 |
|---|---:|---|---|
| 习惯 C++ | 1–4 | [term01.cpp](<term01.cpp>) → [term04.cpp](<term04.cpp>) | 常量、`const`、初始化和多范式思维如何影响接口？ |
| 构造/析构/赋值 | 5–12 | [term05.cpp](<term05.cpp>) → [term12.cpp](<term12.cpp>) | 编译器会生成什么？复制、析构、多态边界如何保持正确？ |
| 资源管理 | 13–17 | [term13.cpp](<term13.cpp>) → [term17.cpp](<term17.cpp>) | 谁拥有资源？复制资源时该共享、深拷贝还是禁止复制？ |
| 接口设计 | 18–26 | [term18.cpp](<term18.cpp>) → [term26.cpp](<term26.cpp>) | 如何让接口难以误用、兼顾效率和封装？ |
| 实现与继承 | 27–37 | [term27.cpp](<term27.cpp>) → [term37.cpp](<term37.cpp>) | 转型、异常安全、编译依赖、继承契约和 virtual 的边界是什么？ |
| 组合 | 38–40 | [term38.cpp](<term38.cpp>) → [term40.cpp](<term40.cpp>) | 何时是 has-a、implemented-in-terms-of，何时才用继承？ |
| 模板 | 41–48 | [term41.cpp](<term41.cpp>) → [term48.cpp](<term48.cpp>) | 隐式接口、依赖名称、traits 与编译期计算如何工作？ |
| 定制与生态 | 49–55 | [term49.cpp](<term49.cpp>) → [term55.cpp](<term55.cpp>) | 内存分配定制的配对规则、编译器诊断、标准库和 Boost 如何使用？ |

## 逐条文件索引

| Item | 文件 | 中文主题概括 |
|---:|---|---|
| 1 | [term01.cpp](<term01.cpp>) | 将 C++ 视作多种编程范式的组合 |
| 2 | [term02.cpp](<term02.cpp>) | 用类型化常量与内联替代宏 |
| 3 | [term03.cpp](<term03.cpp>) | 尽可能使用 `const` |
| 4 | [term04.cpp](<term04.cpp>) | 确保对象使用前已初始化 |
| 5 | [term05.cpp](<term05.cpp>) | 理解编译器生成的特殊成员 |
| 6 | [term06.cpp](<term06.cpp>) | 明确拒绝不需要的自动生成函数 |
| 7 | [term07.cpp](<term07.cpp>) | 多态基类使用虚析构 |
| 8 | [term08.cpp](<term08.cpp>) | 异常不离开析构函数 |
| 9 | [term09.cpp](<term09.cpp>) | 构造和析构期间不依赖向下虚调用 |
| 10 | [term10.cpp](<term10.cpp>) | 赋值运算符返回引用以支持连锁赋值 |
| 11 | [term11.cpp](<term11.cpp>) | 在赋值中正确处理自赋值 |
| 12 | [term12.cpp](<term12.cpp>) | 复制时覆盖对象的全部成分 |
| 13 | [term13.cpp](<term13.cpp>) | 使用对象管理资源（RAII） |
| 14 | [term14.cpp](<term14.cpp>) | 为资源管理类选择正确复制语义 |
| 15 | [term15.cpp](<term15.cpp>) | 审慎提供底层资源访问 |
| 16 | [term16.cpp](<term16.cpp>) | 使用相同形式的 `new` 与 `delete` |
| 17 | [term17.cpp](<term17.cpp>) | 立即用对象接管 `new` 的结果 |
| 18 | [term18.cpp](<term18.cpp>) | 让接口易用且难误用 |
| 19 | [term19.cpp](<term19.cpp>) | 把类设计视作类型设计 |
| 20 | [term20.cpp](<term20.cpp>) | 优先按 `const` 引用传递 |
| 21 | [term21.cpp](<term21.cpp>) | 必须创建新对象时按值返回 |
| 22 | [term22.cpp](<term22.cpp>) | 数据成员保持私有 |
| 23 | [term23.cpp](<term23.cpp>) | 优先 non-member、non-friend 函数 |
| 24 | [term24.cpp](<term24.cpp>) | 两侧需要转换时使用 non-member 函数 |
| 25 | [term25.cpp](<term25.cpp>) | 考虑支持不抛异常的 `swap` |
| 26 | [term26.cpp](<term26.cpp>) | 尽量延后变量定义 |
| 27 | [term27.cpp](<term27.cpp>) | 尽量少做转型 |
| 28 | [term28.cpp](<term28.cpp>) | 避免返回指向内部对象的 handle |
| 29 | [term29.cpp](<term29.cpp>) | 争取异常安全保证 |
| 30 | [term30.cpp](<term30.cpp>) | 理解 `inline` 的真实边界 |
| 31 | [term31.cpp](<term31.cpp>) | 最小化编译依赖 |
| 32 | [term32.cpp](<term32.cpp>) | public 继承表达 is-a |
| 33 | [term33.cpp](<term33.cpp>) | 避免隐藏继承而来的名称 |
| 34 | [term34.cpp](<term34.cpp>) | 区分接口继承与实现继承 |
| 35 | [term35.cpp](<term35.cpp>) | 考虑 virtual 的替代方案 |
| 36 | [term36.cpp](<term36.cpp>) | 不重定义继承来的非 virtual 函数 |
| 37 | [term37.cpp](<term37.cpp>) | 不重定义继承来的 virtual 默认参数 |
| 38 | [term38.cpp](<term38.cpp>) | 用组合表达 has-a / implemented-in-terms-of |
| 39 | [term39.cpp](<term39.cpp>) | 谨慎使用 private 继承 |
| 40 | [term40.cpp](<term40.cpp>) | 谨慎使用多重继承 |
| 41 | [term41.cpp](<term41.cpp>) | 理解模板隐式接口与编译期多态 |
| 42 | [term42.cpp](<term42.cpp>) | 理解 `typename` |
| 43 | [term43.cpp](<term43.cpp>) | 访问依赖基类名称 |
| 44 | [term44.cpp](<term44.cpp>) | 抽取与模板参数无关的代码 |
| 45 | [term45.cpp](<term45.cpp>) | 用成员模板接纳兼容类型 |
| 46 | [term46.cpp](<term46.cpp>) | 所有参数需转换时用非成员接口 |
| 47 | [term47.cpp](<term47.cpp>) | 使用 traits 提供类型信息 |
| 48 | [term48.cpp](<term48.cpp>) | 认识模板元编程 |
| 49 | [term49.cpp](<term49.cpp>) | 了解 `new-handler` |
| 50 | [term50.cpp](<term50.cpp>) | 理解定制 `new/delete` 的正当用途 |
| 51 | [term51.cpp](<term51.cpp>) | 编写 `new/delete` 时遵守配对与尺寸约定 |
| 52 | [term52.cpp](<term52.cpp>) | placement `new` 配套 placement `delete` |
| 53 | [term53.cpp](<term53.cpp>) | 重视编译器警告 |
| 54 | [term54.cpp](<term54.cpp>) | 熟悉标准库与第三版时代的 TR1 主题 |
| 55 | [term55.cpp](<term55.cpp>) | 熟悉 Boost 生态（可选外部依赖） |

## 辅助文件与边界

- [条款实验说明.md](<条款实验说明.md>) 说明实验编写约定、原书与现代 C++ 的边界，以及如何读反例。
- 本目录只保留 `term01.cpp` 到 `term55.cpp` 作为条款代码入口，避免分组示例与逐条示例重复、混淆学习顺序。
- Item 55 不强制引入 Boost，避免仓库构建依赖第三方安装；[term55.cpp](<term55.cpp>) 提供标准库可运行入口和安装 Boost 后继续实验的说明。
- 55 个小实验只能展示条款的一个关键切面。需要完整的动机、例外和论证时，应回到原书阅读。
