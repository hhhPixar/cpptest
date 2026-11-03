# 深度探索 C++ 对象模型：章节实验与阅读导图

本目录围绕 Stanley B. Lippman 的 *Inside the C++ Object Model*（1996）建立**原创的、可运行的 C++14 实验**。书的重点是 C++ 对象机制的语义、常见实现模型及由此产生的成本与取舍；它不是 ISO C++ 对象内存布局规范。

本书的出版信息、定位和公开目录可在 [InformIT 图书页](https://www.informit.com/store/inside-the-c-plus-plus-object-model-9780201834543) 核验。该页列出的七章主线是：对象课程、构造函数语义、数据语义、函数语义、构造/析构/复制语义、运行时语义，以及对象模型边缘主题（模板、异常、RTTI）。

> **最重要的边界**：C++ 标准保证可观察的语言语义（例如虚调用结果、对象生命周期、`dynamic_cast` 成功与否），但不保证 vptr/vtable 的存在、偏移、成员指针编码、对象大小或虚基表布局。所有涉及 `sizeof`、地址、ABI 的实验都只能用于当前编译器环境的观察，绝不能把输出数字硬编码进业务逻辑。

## 总体知识图

```text
对象语义（标准保证）
├── 对象保存哪些状态：非静态成员 / 静态成员 / 基类子对象
├── 如何开始和结束生命周期：构造 / 析构 / 复制 / 移动 / 临时对象
├── 如何选择函数：普通调用 / 虚调用 / 成员指针调用
├── 如何组织层次：单继承 / 多继承 / 虚继承
└── 如何在运行时检查类型：异常展开 / RTTI / dynamic_cast

常见实现模型（ABI 相关）
├── 多态对象常见的 vptr 与类级 vtable
├── 多重继承下基类子对象地址调整
├── 虚继承下定位共享虚基类的辅助信息
└── 指向成员的指针可能不是机器地址
```

## 第 1 章：Object Lessons（对象课程）

| 文件 | 学习问题 |
|---|---|
| [01-01-basic-object-model.cpp](<01-01-basic-object-model.cpp>) | 非静态成员、静态成员、虚函数与对象大小的边界 |
| [01-02-object-lessons.cpp](<01-02-object-lessons.cpp>) | C 风格结构、普通类、继承、模板与对象切片 |
| [01-03-encapsulation-layout-cost.cpp](<01-03-encapsulation-layout-cost.cpp>) | 封装不会自动让每个对象保存成员函数；`sizeof` 只能观察当前 ABI |

阅读目标：区分“对象数据”“类共享数据”“可执行代码”和“语言提供的动态绑定语义”。

## 第 2 章：The Semantics of Constructors（构造函数语义）

| 文件 | 学习问题 |
|---|---|
| [02-01-resource-copy-semantics.cpp](<02-01-resource-copy-semantics.cpp>) | 资源成员的深拷贝、copy-and-swap 与析构 |
| [02-02-default-construction-initialization.cpp](<02-02-default-construction-initialization.cpp>) | 何时需要默认构造、成员初始化列表与构造顺序 |
| [02-03-copy-construction-trigger-points.cpp](<02-03-copy-construction-trigger-points.cpp>) | 按值传参、按值返回、显式复制如何触发复制/移动构造 |

阅读目标：区分“编译器是否会生成某个特殊成员函数”与“默认逐成员语义是否符合类型的资源所有权”。

## 第 3 章：The Semantics of Data（数据成员语义）

| 文件 | 学习问题 |
|---|---|
| [03-01-data-member-binding-inheritance.cpp](<03-01-data-member-binding-inheritance.cpp>) | 成员访问、隐藏、`this` 与基类子对象 |
| [03-02-pointer-to-data-members.cpp](<03-02-pointer-to-data-members.cpp>) | `T C::*`、`.*`、`->*`、空成员指针与“成员指针不是普通地址” |

阅读目标：不要把“成员的名字查找规则”“对象中实际保存的数据”“ABI 布局”混为一谈。

## 第 4 章：The Semantics of Function（成员函数语义）

| 文件 | 学习问题 |
|---|---|
| [04-01-member-function-this.cpp](<04-01-member-function-this.cpp>) | 非静态成员函数的隐式 `this`、const `this`、链式返回、静态函数无 `this` |
| [04-02-virtual-dispatch-lifetime.cpp](<04-02-virtual-dispatch-lifetime.cpp>) | 正常虚调用，以及构造/析构阶段不会向未构造/已析构的派生部分派发 |
| [04-03-pointer-to-member-functions.cpp](<04-03-pointer-to-member-functions.cpp>) | `R (C::*)(Args...)`、`.*`、`->*`、const 成员函数指针 |
| [04-04-inline-odr-semantics.cpp](<04-04-inline-odr-semantics.cpp>) | `inline` 的 ODR 含义、函数地址与“不保证机器码展开” |

阅读目标：先用语言语义解释“调用了哪个函数”，再把 vtable 当成一种常见但非标准强制的实现方案。

## 第 5 章：Semantics of Construction, Destruction, and Copy

| 文件 | 学习问题 |
|---|---|
| [05-01-inheritance-construction-copy.cpp](<05-01-inheritance-construction-copy.cpp>) | 继承下基类优先构造/复制、析构反序 |
| [05-03-object-slicing.cpp](<05-03-object-slicing.cpp>) | 按值传递导致切片，引用保持多态 |
| [05-02-plain-object-copy-lifetime.cpp](<05-02-plain-object-copy-lifetime.cpp>) | 无继承对象的成员声明顺序构造、反序析构、复制顺序 |
| [05-04-virtual-multiple-inheritance.cpp](<05-04-virtual-multiple-inheritance.cpp>) | 虚继承/多继承的地址调整、最派生类构造虚基、ABI 边界 |
| [05-05-diamond-virtual-inheritance.cpp](<05-05-diamond-virtual-inheritance.cpp>) | 菱形虚继承与最派生类构造虚基类 |

阅读目标：把“生命周期顺序”作为标准语义，把“地址调整、辅助表”作为实现策略。

## 第 6 章：Runtime Semantics（运行时语义）

| 文件 | 学习问题 |
|---|---|
| [06-01-new-delete-lifetime.cpp](<06-01-new-delete-lifetime.cpp>) | 分配函数、构造、析构、释放的先后关系与 `unique_ptr` |
| [06-02-temporary-return-value.cpp](<06-02-temporary-return-value.cpp>) | 临时对象、按值返回、RVO/NRVO；不依赖构造次数 |
| [06-03-exception-unwinding-raii.cpp](<06-03-exception-unwinding-raii.cpp>) | 异常传播时自动对象逆序析构与 RAII |

## 第 7 章：On the Cusp of the Object Model

| 文件 | 学习问题 |
|---|---|
| [07-01-rtti-dynamic-cast-typeid.cpp](<07-01-rtti-dynamic-cast-typeid.cpp>) | `typeid`、多态类型与运行时受检向下转换 |
| [07-02-template-static-polymorphism.cpp](<07-02-template-static-polymorphism.cpp>) | 模板实例化与静态多态，不需要运行时虚调用 |

## 推荐顺序

```text
第 1 章：对象中有什么
   ↓
第 2 章：对象何时产生/复制
   ↓
第 3 章：数据成员怎样绑定与访问
   ↓
第 4 章：函数调用怎样选择目标
   ↓
第 5 章：继承下生命周期如何叠加
   ↓
第 6 章：运行时分配、临时值、异常
   ↓
第 7 章：RTTI 与模板的边界
```

## 编译方式

每个 `.cpp` 有独立 `main`，应单独构建：

```bash
for source in Inside-The-CPP-Object-Model/*.cpp; do
  name="/tmp/$(basename "${source%.cpp}")"
  c++ -std=c++14 -Wall -Wextra -pedantic "$source" -o "$name" && "$name"
done
```

## 练习方法

1. 先写下你预期的构造、析构或虚调用输出顺序；
2. 运行代码核对；
3. 改成按值/按引用/按指针传递，再解释差异；
4. 只在实验时打印地址和 `sizeof`，并在笔记中记录编译器/平台；
5. 对任何“vptr 在对象第一个字”之类的结论都追问：这是标准规则，还是当前 ABI 观察？
