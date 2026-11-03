#include <algorithm>
#include <future>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

// 将区间均分给有限任务；每个任务只读自己的子区间，因此无需锁。
template <typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init) {
    const unsigned long length = std::distance(first, last);
    if (length == 0) return init;

    const unsigned long min_per_task = 25;
    const unsigned long max_tasks = (length + min_per_task - 1) / min_per_task;
    const unsigned long hardware_threads = std::max(1u, std::thread::hardware_concurrency());
    const unsigned long task_count = std::min(hardware_threads, max_tasks);
    const unsigned long block_size = length / task_count;

    std::vector<std::future<T>> futures;
    Iterator block_start = first;
    for (unsigned long i = 0; i + 1 < task_count; ++i) {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);
        futures.push_back(std::async(std::launch::async, [block_start, block_end] {
            return std::accumulate(block_start, block_end, T{});
        }));
        block_start = block_end;
    }

    T result = std::accumulate(block_start, last, init);
    for (auto& future : futures) result += future.get(); // get 同时等待并传播异常。
    return result;
}

int main() {
    std::vector<long long> values(100);
    std::iota(values.begin(), values.end(), 1LL);
    const long long sum = parallel_accumulate(values.begin(), values.end(), 0LL);
    std::cout << "sum(1..100)=" << sum << '\n';
}
