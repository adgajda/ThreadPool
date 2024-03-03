#include "include/ThreadPool.hpp"
#include <chrono>
#include <future>
#include <iostream>
#include <mutex>
#include <string_view>
#include <thread>
#include <vector>

void print(std::string_view str, int val)
{
    static std::mutex mtx;
    std::lock_guard lock(mtx);
    std::cout << str << val << '\n';
}

int main()
{
    ThreadPool thread_pool(2);
    std::vector<std::future<int>> results;

    for (int task_id = 0; task_id < 8; ++task_id)
    {
        results.push_back(thread_pool.add_task([task_id] {
            print("in function: ", task_id);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            return task_id;
        }));
    }

    for (auto& result : results)
    {
        print("returned value: ", result.get());
    }

    thread_pool.shutdown();
    return 0;
}