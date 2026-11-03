#ifndef CPP_PART1_OOP_DEMO_H
#define CPP_PART1_OOP_DEMO_H

#include <iostream>
#include <string>

// 抽象基类：draw 是接口，具体图形必须实现它。
class Shape {
public:
    virtual ~Shape() = default;  // 经由基类指针删除派生对象时必须使用虚析构函数。
    virtual void draw() const = 0;
    virtual void error(const std::string& message) const {
        std::cerr << "Shape error: " << message << '\n';
    }
    int object_id() const { return object_id_; }

protected:
    explicit Shape(int object_id = 0) : object_id_(object_id) {}

private:
    int object_id_;
};

// 模板方法示例：非虚函数固定流程，虚函数留给派生类定制步骤。
class CDocument {
public:
    virtual ~CDocument() = default;

    void on_open_file() {
        std::cout << "check file status\n";
        serialize();                     // 动态绑定到实际对象的实现。
        std::cout << "close file\n";
    }

private:
    virtual void serialize() { std::cout << "CDocument serializing...\n"; }
};

class MyDocument : public CDocument {
private:
    void serialize() override { std::cout << "MyDocument serializing...\n"; }
};

#endif
