#include <thread>

struct func {
    func(int& counter) : m_counter(counter) {}

    void operator()() {
        for (size_t i = 0; i < 10'000'000; ++i) {
            m_counter += 1;
        }
    }

    int& m_counter;
};

int main() {
    int counter{0};

    std::thread t(func{counter});

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(10ms);

    t.detach();

    return 0;
}