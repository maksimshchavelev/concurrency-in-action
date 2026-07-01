#include <atomic>
#include <iostream>
#include <memory>
#include <thread>
#include <latch>

int main() {
    std::atomic<std::shared_ptr<int>> atomic_ptr;
    atomic_ptr = std::make_shared<int>(0);

    std::latch latch(1);

    std::jthread t1([&] {
        auto new_ptr = std::make_shared<int>(100);
        atomic_ptr.exchange(new_ptr);
        latch.count_down();
    });

    std::jthread t2([&] {
        latch.wait();

        std::shared_ptr<int> expected = atomic_ptr.load();

        if (expected && *expected == 100) {
            std::shared_ptr<int> desired = std::make_shared<int>(200);

            if (atomic_ptr.compare_exchange_strong(expected, desired)) {
                std::cout << "CAS succeeded, new value: " << *desired << std::endl;
            } else {
                std::cout << "CAS failed, current value: " << *expected << std::endl;
            }
        }
    });

    t1.join();
    t2.join();

    std::shared_ptr<int> final = atomic_ptr.load();

    if (final) {
        std::cout << "Final value: " << *final << std::endl;
    }

    return 0;
}