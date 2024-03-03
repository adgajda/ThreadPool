#pragma once
#include "ThreadSafeQueue.hpp"
#include <atomic>
#include <functional>
#include <thread>

class Worker final
{
public:
    Worker(ThreadSafeQueue<std::function<void()>>& tasks, std::atomic_bool& is_stop_requested);
    void start_working();

private:
    void run();

    using Task = std::function<void()>;
    ThreadSafeQueue<Task>& tasks_;
    std::atomic_bool& is_stop_requested_;
    std::jthread thread_;
};
