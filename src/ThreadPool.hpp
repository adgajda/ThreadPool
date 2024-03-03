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
    explicit ThreadPool(const std::size_t number_of_workers)
    {
        workers_.reserve(number_of_workers);
        for (std::size_t worker_id = 0; worker_id < number_of_workers; ++worker_id)
        {
            workers_.emplace_back(tasks_, is_stop_requested_);
            workers_[worker_id].start_working();
        }
    }

    template<typename Func, typename... Args>
    [[nodiscard]] auto add_task(Func&& func, Args&&... args) -> std::future<decltype(func(args...))>
    {
        const auto task_ptr = std::make_shared<std::packaged_task<decltype(func(args...))()>>(
          std::bind(std::forward<Func>(func), std::forward<Args>(args)...));

        tasks_.push([task_ptr]() { std::invoke(*task_ptr); });

        return task_ptr->get_future();
    }

    void shutdown()
    {
        is_stop_requested_ = true;
        tasks_.unblock_waiters_and_invalidate_queue();
        workers_.clear();
    }

private:
    using Task = std::function<void()>;
    std::vector<Worker> workers_;
    ThreadSafeQueue<Task> tasks_;
    std::atomic_bool is_stop_requested_ = false;
};
