#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

template <typename T, typename Container = std::deque<T>> class queue {
  public:
    queue() = default;

    explicit queue(const queue& other) {
        std::lock_guard lock(other.m_mutex);
        m_container = other.m_container;
    }

    explicit queue(queue&& other) {
        std::lock_guard lock(other.m_mutex);
        m_container = std::move(other.m_container);
    }

    queue& operator=(const queue& other) {
        if (std::addressof(other) == this) {
            return *this;
        }

        std::scoped_lock lock(m_mutex, other.m_mutex);
        m_container = other.m_container;

        return *this;
    }

    queue& operator=(queue&& other) {
        if (std::addressof(other) == this) {
            return *this;
        }

        std::scoped_lock lock(m_mutex, other.m_mutex);
        m_container = std::move(other.m_container);

        return *this;
    }

    void push(T value) {
        std::lock_guard lock(m_mutex);
        m_container.push_back(std::move(value));
        m_cv.notify_one();
    }

    std::optional<T> try_pop() {
        std::lock_guard lock(m_mutex);

        if (m_container.empty()) {
            return std::nullopt;
        }

        T value = m_container.front();
        m_container.pop_front();

        return value;
    }

    T wait_and_pop() {
        std::unique_lock lock(m_mutex);
        m_cv.wait(lock, [this]() { return !m_container.empty(); });

        T value = m_container.front();
        m_container.pop_front();

        return value;
    }

    bool empty() const {
        std::lock_guard lock(m_mutex);
        return m_container.empty();
    }

  private:
    mutable std::mutex      m_mutex;
    std::condition_variable m_cv;
    Container               m_container;
};


int main() {
    queue<int> q;

    std::vector<std::jthread> producers;
    std::vector<std::jthread> consumers;

    // 6M values produced
    for (int i = 0; i < 6; ++i) {
        producers.emplace_back([&]() {
            for (int i = 0; i < 1'000'000; ++i) {
                q.push(i);
            }
        });
    }

    // 6M values consumed
    for (int i = 0; i < 6; ++i) {
        consumers.emplace_back([&]() {
            for (int i = 0; i < 1'000'000; ++i) {
                int value = q.wait_and_pop();
            }
        });
    }
}
