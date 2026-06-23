#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

using namespace std::chrono_literals;

class TaskQueue {
  public:
    using Task = std::packaged_task<void()>;

    void push(Task task) {
        {
            std::scoped_lock lock(m_mutex);
            m_tasks.push(std::move(task));
        }

        // We release the mutex lock before calling `notify_one()` so that the worker thread does not have to wait to lock the
        // mutex after waking up (since the mutex has not yet been released), but locks it immediately
        m_cv.notify_one();
    }

    Task wait_and_pop() {
        std::unique_lock lock(m_mutex);
        m_cv.wait(lock, [this]() { return !m_tasks.empty(); });
        Task task = std::move(m_tasks.front());
        m_tasks.pop();
        return task;
    }

  private:
    std::queue<Task>        m_tasks;
    std::mutex              m_mutex;
    std::condition_variable m_cv;
};


int main() {
    std::cout << "Main thread ID: " << std::this_thread::get_id() << std::endl;

    TaskQueue queue;

    // The work thread in which the tasks will be performed
    std::jthread worker([&]() {
        std::cout << "Worker thread ID: " << std::this_thread::get_id() << std::endl;

        for (unsigned i = 0; i < 5; ++i) {
            auto task = queue.wait_and_pop();
            task();
        }
    });

    // Push 5 tasks
    for (unsigned i = 0; i < 5; ++i) {
        std::packaged_task<void(void)> task([i]() {
            std::cout << "Task " << i << " running on thread with ID " << std::this_thread::get_id() << std::endl;
            std::this_thread::sleep_for(1s); // Simulate heavy work
        });

        // The task will be started in a worker thread, and their thread IDs will match
        // If the task didn't return `void`, it would be possible to wait for the result via a `future`, after first obtaining
        // that `future` by calling the `task.get_future()` method.
        queue.push(std::move(task));
    }

    return 0;
}