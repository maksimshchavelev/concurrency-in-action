#include <mutex>
#include <thread>

// A basic implementation of `std::lock` (the standard library uses a different algorithm) with basic exception safety
template <typename Mutex1, typename Mutex2> void lock(Mutex1& m1, Mutex2& m2) {
    if (std::addressof(m1) == std::addressof(m2)) {
        m1.lock();
        return;
    }

    // Locking mutexes in a specific order ensures that a deadlock will not occur
    if (std::addressof(m1) < std::addressof(m2)) {
        m1.lock();

        try {
            m2.lock();
        } catch (...) {
            m1.unlock();
            throw;
        }
    } else {
        m2.lock();

        try {
            m1.lock();
        } catch (...) {
            m2.unlock();
            throw;
        }
    }
}

// Testing
class Account {
  public:
    Account(int balance) : m_balance{balance} {}

  private:
    int                m_balance{0};
    mutable std::mutex m_mutex;

    friend void transfer(Account& from, Account& to, int amount);
};

void transfer(Account& from, Account& to, int amount) {
    if (std::addressof(from) == std::addressof(to)) {
        return;
    }

    lock(from.m_mutex, to.m_mutex); // Use `std::scoped_lock` in modern C++

    std::lock_guard g1(from.m_mutex, std::adopt_lock); // std::adopt_lock - mutex is already locked; unlock it in the destructor
    std::lock_guard g2(to.m_mutex, std::adopt_lock);   // std::adopt_lock - mutex is already locked; unlock it in the destructor

    from.m_balance -= amount;
    to.m_balance += amount;
}

int main() {
    Account account1(1000);
    Account account2(500);

    std::jthread t1([&]() { transfer(account1, account2, 200); });
    std::jthread t2([&]() { transfer(account2, account1, 300); });

    return 0;
}