#include <atomic>

/// A spinlock is used when:
/// - the critical section is very short
/// - there is low contention
/// - the system is real-time (frequent context switching must be avoided)
class Spinlock {
  public:
    void lock() {
        while (m_flag.test_and_set(std::memory_order::acquire)) {
        }
    }

    void unlock() {
        m_flag.clear(std::memory_order::release);
    }

  private:
    // In C++20, the flag is guaranteed to be false by default, so the ATOMIC_FLAG_INIT macro is not needed
    std::atomic_flag m_flag;
};

int main() {
    return 0;
}
