#include <thread>

class Account {
  public:
    Account(int balance) : m_balance{balance} {}

  private:
    int                m_balance{0};
    mutable std::mutex m_mutex;

    friend void transfer(Account& from, Account& to, int amount);
};

void transfer(Account& from, Account& to, int amount) {
    using namespace std::chrono_literals;

    std::lock_guard lock1(from.m_mutex);

    std::this_thread::sleep_for(100ms); // Artificial delay to trigger a deadlock

    std::lock_guard lock2(to.m_mutex);

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