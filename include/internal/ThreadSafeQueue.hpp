#pragma once
#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>
#include <utility>

template<typename T>
class ThreadSafeQueue final
{
public:
    void push(T&& item)
    {
        {
            const std::lock_guard lock(mutex_);
            queue_.push(std::move(item));
        }
        condition_variable_.notify_one();
    }

    [[nodiscard]] std::optional<T> try_pop_with_wait()
    {
        std::unique_lock lock(mutex_);
        condition_variable_.wait(lock, [this]() { return !queue_.empty() || is_unblock_requested_; });
        if (is_unblock_requested_)
        {
            return std::nullopt;
        }
        T item = std::move(queue_.front());
        queue_.pop();
        return item;
    }

    void unblock_waiters_and_invalidate_queue()
    {
        {
            const std::lock_guard lock(mutex_);
            is_unblock_requested_ = true;
        }
        condition_variable_.notify_all();
    }

private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable condition_variable_;
    bool is_unblock_requested_ = false;
};
