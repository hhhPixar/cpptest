#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

class connection {
public:
    void send(int request) const {
        // 连接对象只读使用；本例不在工作线程直接打印，输出因而稳定。
        (void)request;
    }
};

std::once_flag connection_flag;
std::unique_ptr<connection> global_connection;
int initialization_count = 0;

void open_connection() {
    // call_once 保证这段初始化恰好执行一次，并为其他调用者建立同步关系。
    global_connection.reset(new connection);
    ++initialization_count;
}

void process_request(int request) {
    std::call_once(connection_flag, open_connection);
    global_connection->send(request);
}

int main() {
    std::vector<std::thread> workers;
    for (int request = 0; request < 8; ++request) {
        workers.emplace_back(process_request, request);
    }
    for (auto& worker : workers) worker.join();

    // join 后读取普通变量安全；值固定为 1，证明延迟初始化未重复发生。
    std::cout << "requests=8, initialized=" << initialization_count << " time\n";
}
