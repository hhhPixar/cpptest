#include <iostream>
#include <string>

// Effective C++ 第三版 Item 23：宁可使用 non-member、non-friend 函数替换 member 函数。
// 与对象私有状态无关的操作放在类外，可减小类接口并增加封装弹性。
// non-member 并不意味着能触碰私有成员；本例刻意只通过公开的查询函数工作。
// 后续可按客户需求把格式化工具分到不同命名空间，而无需改变 Address 的定义。
class Address {
public:
    Address(std::string city, std::string street)
        : city_(std::move(city)), street_(std::move(street)) {}

    const std::string& city() const { return city_; }
    const std::string& street() const { return street_; }

private:
    std::string city_;
    std::string street_;
};

// 此函数只使用公开查询接口，因此既不是成员也不是 friend。
std::string formatAddress(const Address& address) {
    return address.city() + " - " + address.street();
}

int main() {
    Address office("上海", "世纪大道");
    std::cout << formatAddress(office) << '\n';

    // formatAddress 可与其它格式化函数放进独立头文件/命名空间，Address 不必膨胀。
}
