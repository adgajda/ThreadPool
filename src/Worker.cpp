#include "Worker.hpp"
#include <optional>

Worker::Worker(ThreadSafeQueue<Task>& tasks, std::atomic_bool& is_stop_requested)
  : tasks_{ tasks }, is_stop_requested_{ is_stop_requested }
{
}

void Worker::start_working()
{
    thread_ = std::jthread(&Worker::run, this);
}

void Worker::run()
{
    while (!is_stop_requested_)
    {
        const auto task = tasks_.try_pop_with_wait();
        if (is_stop_requested_)
        {
            break;
        }

        if (task.has_value())
        {
            std::invoke(task.value());
        }
    }
}
