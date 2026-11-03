# C++11/14 新特性：编号、示例与补充

本目录按**本 README 的学习顺序**编号。每个 `NN-*.cpp` 都是一个可独立编译运行的小实验；先读文件顶部的学习目标与中文注释，再观察输出。原有未编号文件保留为“补充”，放在主线之后，避免改名造成旧链接失效。

```bash
c++ -std=c++14 -Wall -Wextra -pedantic c++11:14/01-variadic-intro.cpp -o demo
./demo
```

> 说明：C++11/14 的语义细节很多。本目录用小例子建立正确直觉，不替代标准或权威参考。尤其是对象布局、lambda 闭包类型和移动后对象状态，不能用过度简化的口号代替规则。

## 主线：按 README 原有顺序编号

| 编号 | 主题 | 示例 | 阅读重点 |
|---:|---|---|---|
| 01 | Variadic Templates 基础 | [01-variadic-intro.cpp](<01-variadic-intro.cpp>) | 参数包、递归展开与终止条件 |
| 02 | 模板右尖括号 | [02-template-space.cpp](<02-template-space.cpp>) | C++11 起 `>>` 可用于嵌套模板闭合 |
| 03 | `nullptr` 与 `std::nullptr_t` | [03-nullptr.cpp](<03-nullptr.cpp>) | 空指针类型与重载决议 |
| 04 | `auto` | [04-auto.cpp](<04-auto.cpp>) | 类型推导、迭代器与 lambda |
| 05 | 统一初始化 | [05-uniform-initialization.cpp](<05-uniform-initialization.cpp>) | 花括号、窄化检查与构造函数选择 |
| 06 | 值初始化 | [06-value-initialization.cpp](<06-value-initialization.cpp>) | `{}` 初始化基本类型和指针 |
| 07 | `std::initializer_list` | [07-initializer-list.cpp](<07-initializer-list.cpp>) | 列表构造函数的优先匹配 |
| 08 | `std::array` | [08-array.cpp](<08-array.cpp>) | 固定长度、连续存储与 STL 接口 |
| 09 | 多参数构造函数的 `explicit` | [09-explicit.cpp](<09-explicit.cpp>) | 阻止不期望的隐式转换 |
| 10 | 范围 `for` | [10-range-for.cpp](<10-range-for.cpp>) | 值/引用遍历与修改元素 |
| 11 | `=default` / `=delete` | [11-default-delete.cpp](<11-default-delete.cpp>) | 特殊成员函数与拒绝复制 |
| 12 | No-Copy / Private-Copy | [12-no-copy.cpp](<12-no-copy.cpp>) | C++11 `=delete` 与旧式私有声明对比 |
| 13 | Alias Template | [13-alias-template.cpp](<13-alias-template.cpp>) | 带模板参数的类型别名 |
| 14 | Template Template Parameter | [14-template-template-parameter.cpp](<14-template-template-parameter.cpp>) | 接受“模板本身”作为参数 |
| 15 | Type Alias | [15-type-alias.cpp](<15-type-alias.cpp>) | `using` 与函数指针别名 |
| 16 | `using` 声明 | [16-using-declaration.cpp](<16-using-declaration.cpp>) | 引入名字和恢复基类重载 |
| 17 | `noexcept` | [17-noexcept.cpp](<17-noexcept.cpp>) | 不抛异常承诺与 `std::terminate` 边界 |
| 18 | `override` | [18-override.cpp](<18-override.cpp>) | 编译器验证重写关系 |
| 19 | `final` | [19-final.cpp](<19-final.cpp>) | 禁止继承或禁止继续重写 |
| 20 | `decltype` | [20-decltype.cpp](<20-decltype.cpp>) | 从表达式取得类型、尾置返回类型 |
| 21 | Lambda | [21-lambda.cpp](<21-lambda.cpp>) | 捕获、闭包和 erase-remove 惯用法 |
| 22 | Variadic Template 进阶 | [22-variadic-template.cpp](<22-variadic-template.cpp>) | 递归格式化与参数包处理 |
| 23 | 右值引用与移动语义 | [23-rvalue-reference.cpp](<23-rvalue-reference.cpp>) | 所有权转移、移动后有效状态 |
| 24 | Hashtable | [24-hashtable.cpp](<24-hashtable.cpp>) | `unordered_map`、桶、负载因子与 rehash |

## 原目录已有、README 未列出的补充示例

这些文件不是遗漏；它们作为主线后的扩展，保留原文件名以兼容已有链接。

| 补充主题 | 文件 | 与主线的关系 |
|---|---|---|
| 函数模板基础 | [template.cpp](<template.cpp>) | 为 01、14、22 的模板阅读补基础 |
| 固定异构记录 | [tuple.cpp](<tuple.cpp>) | C++11 标准库类型补充 |

至此，主线 01–24 的所有代码文件均已采用 `NN-主题.cpp` 的编号命名；未编号的 `template.cpp`、`tuple.cpp` 仅属于本节补充内容。

## 容易误解的规则（以代码注释为准）

- **花括号不总是“统一行为”**：它会参与重载决议；若存在匹配的 `initializer_list` 构造函数，通常优先选择它，同时会拒绝窄化转换。
- **`std::move` 不执行移动**：它只是把表达式转换为右值；移动构造/赋值函数是否转移资源由类型实现决定。被移动对象仍必须可析构、可赋值。
- **左值/右值不是“地址/真实值”**：命名变量表达式通常是左值，即使它的类型是右值引用。
- **`noexcept` 不是普通注释**：若异常离开 `noexcept` 函数，程序会调用 `std::terminate`。
- **lambda 并非“没有构造和赋值函数”**：闭包类型的特殊成员能力取决于捕获和语言版本；应关注捕获方式和生命周期。
- **`override` / `final` 应交给编译器检查**：不要只依赖人工确认函数签名是否匹配。

## 推荐阅读方式

先完成 01–10，建立模板、推导和初始化直觉；再读 11–20，理解类型系统与接口约束；最后进入 21–24，学习 lambda、变参模板、移动语义与无序关联容器。之后再读“补充示例”，并回到 [STL](<../STL/README.md>) 深入容器和算法实现。
