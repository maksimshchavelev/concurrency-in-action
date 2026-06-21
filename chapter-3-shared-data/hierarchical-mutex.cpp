#include <thread>

class hierarchical_mutex {
  public:
    explicit hierarchical_mutex(std::uint32_t hierarcy_level) : m_hierarcy_level{hierarcy_level} {}

    void lock() {
        if (m_hierarcy_level >= m_thread_hierarcy) {
            throw std::logic_error("Inappropriate hierarchy level");
        }

        m_mutex.lock();
        m_prev_thread_hierarcy_level = m_thread_hierarcy;
        m_thread_hierarcy = m_hierarcy_level;
    }

    bool try_lock() {
        if (m_hierarcy_level >= m_thread_hierarcy) {
            throw std::logic_error("Inappropriate hierarchy level");
        }

        if (bool res = m_mutex.try_lock(); res) {
            m_prev_thread_hierarcy_level = m_thread_hierarcy;
            m_thread_hierarcy = m_hierarcy_level;

            return true;
        }

        return false;
    }

    void unlock() {
        if (m_hierarcy_level != m_thread_hierarcy) {
            throw std::logic_error("Inappropriate hierarchy level");
        }

        m_mutex.unlock();
        m_thread_hierarcy = m_prev_thread_hierarcy_level;
    }

  private:
    std::mutex                        m_mutex;
    const std::uint32_t               m_hierarcy_level;
    std::uint32_t                     m_prev_thread_hierarcy_level{std::numeric_limits<std::uint32_t>::max()};
    static thread_local std::uint32_t m_thread_hierarcy;
};

thread_local std::uint32_t hierarchical_mutex::m_thread_hierarcy{std::numeric_limits<std::uint32_t>::max()};


class Account {
  public:
    Account(int balance) : m_balance{balance} {}

  private:
    int                        m_balance{0};
    mutable hierarchical_mutex m_mutex{200};

    friend void transfer(Account& from, Account& to, int amount);
};

void transfer(Account& from, Account& to, int amount) {
    std::lock_guard g1(from.m_mutex);
    std::lock_guard g2(to.m_mutex);

    from.m_balance -= amount;
    to.m_balance += amount;
}

int main() {
    Account account1(1000);
    Account account2(500);

    std::jthread t1([&]() { transfer(account1, account2, 200); });
    std::jthread t2([&]() { transfer(account2, account1, 300); });

    // Thread 1 locks the account1 mutex, causing a delay; during this time, Thread 2 will definitely start and lock the account2
    // mutex. Then, Thread 1 will attempt to lock the account2 mutex, but it is already locked by Thread 2. However, Thread 2
    // cannot continue because the account1 mutex is locked by Thread 1. Deadlock!

    return 0;
}
