#include "ThreadPool.hpp"

ThreadPool::ThreadPool(const std::size_t number_of_workers)
{
    workers_.reserve(number_of_workers);
    for (std::size_t worker_id = 0; worker_id < number_of_workers; ++worker_id)
    {
        workers_.emplace_back(tasks_, is_stop_requested_);
        workers_[worker_id].start_working();
    }
}

void ThreadPool::shutdown()
{
    is_stop_requested_ = true;
    tasks_.unblock_waiters_and_invalidate_queue();
    workers_.clear();
}
