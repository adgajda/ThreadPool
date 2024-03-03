#include "Worker.hpp"
#include <optional>

Worker::Worker(ThreadSafeQueue<Task>& tasks) : tasks_{ tasks }
{
}

void Worker::start_working()
{
    thread_ = std::jthread(&Worker::run, this);
}

void Worker::run()
{
    std::optional<Task> task;
    while (task = tasks_.try_pop_with_wait())
    {
        std::invoke(task.value());
    }
}
