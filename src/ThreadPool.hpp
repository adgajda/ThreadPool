#pragma once
#include "ThreadSafeQueue.hpp"
#include "Worker.hpp"
#include <atomic>
#include <cstddef>
#include <functional>
#include <future>
#include <memory>
#include <utility>
#include <vector>

class ThreadPool final
{
public:
    explicit ThreadPool(const std::size_t number_of_workers);

    template<typename Func, typename... Args>
    [[nodiscard]] auto add_task(Func&& func, Args&&... args) -> std::future<decltype(func(args...))>
    {
        const auto task_ptr = std::make_shared<std::packaged_task<decltype(func(args...))()>>(
          std::bind(std::forward<Func>(func), std::forward<Args>(args)...));

        tasks_.push([task_ptr]() { std::invoke(*task_ptr); });

        return task_ptr->get_future();
    }

    void shutdown();

private:
    using Task = std::function<void()>;
    std::vector<Worker> workers_;
    ThreadSafeQueue<Task> tasks_;
};
