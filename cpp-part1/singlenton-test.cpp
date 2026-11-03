#include <iostream>
#include "Singleton.h"

int main() {
    // 两次访问都得到同一个函数局部 static 对象；“create”只应输出一次。
    Singleton::getInstance().setup();
    Singleton::getInstance().setup();
}
