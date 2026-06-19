#include <mutex>
#include <thread>
#include <vector>

template <typename T, typename Container = std::vector<T>> class Stack {
  public:
    void push(T value) {
        std::lock_guard _(m_mutex);
        m_container.push_back(std::move(value));
    }

    T pop() {
        std::lock_guard _(m_mutex);

        T value = std::move_if_noexcept(m_container.back());
        m_container.pop_back();

        return value;
    }

    T top() const {
        std::lock_guard _(m_mutex);
        return m_container.back();
    }

    bool empty() const {
        std::lock_guard _(m_mutex);
        return m_container.empty();
    }

    size_t size() const {
        std::lock_guard _(m_mutex);
        return m_container.size();
    }

  private:
    Container          m_container;
    mutable std::mutex m_mutex;
};

int main() {
    Stack<int> stack;

    std::vector<std::jthread> threads;

    for (size_t i = 0; i < 16; ++i) {
        // A quarter of the threads push values, while the rest attempt to pop them
        if (i % 4 == 0) {
            threads.emplace_back([&stack]() {
                for (size_t i = 0; i < 1'000'000; ++i) {
                    stack.push(i);
                }
            });
        } else {
            threads.emplace_back([&stack]() {
                for (size_t i = 0; i < 1'000'000; ++i) {
                    // Race condition. After a successful call to `!stack.empty()`, but before `stack.pop()`, another thread may
                    // call `stack.pop()`, so that the stack may become empty for the current thread and `stack.pop()` causes a
                    // crash.
                    // Solution: Protect both operations—the empty check and the pop()
                    if (!stack.empty()) {
                        stack.pop();
                    }
                }
            });
        }
    }

    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    return 0;
}