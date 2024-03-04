#pragma once
#include "ThreadSafeQueue.hpp"
#include <atomic>
#include <functional>
#include <thread>

class Worker final
{
public:
    Worker(ThreadSafeQueue<std::function<void()>>& tasks) : tasks_{ tasks }
    {
    }

    void start_working()
    {
        thread_ = std::jthread(&Worker::run, this);
    }

private:
    void run()
    {
        while (const auto task = tasks_.try_pop_with_wait())
        {
            std::invoke(task.value());
        }
    }

    using Task = std::function<void()>;
    ThreadSafeQueue<Task>& tasks_;
    std::jthread thread_;
};
