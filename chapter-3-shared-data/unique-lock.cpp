#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

int main() {
    std::vector<int> data;
    std::mutex       mutex;

    std::jthread producer([&]() {
        for (unsigned i = 0; i < 10; ++i) {
            std::this_thread::sleep_for(250ms);

            std::unique_lock lock(mutex); // Lock the mutex immediately
            data.push_back(i);
            std::cout << "Produced: " << i << std::endl;
        }
    });

    std::jthread consumer([&]() {
        int processed = 0;

        while (processed < 10) {
            // Attempting to lock a mutex without blocking the execution thread (essentially calling `try_lock()`)
            std::unique_lock lock(mutex, std::try_to_lock);

            // Locked successfully
            if (lock.owns_lock()) {
                while (!data.empty()) {
                    int value = data.back();
                    data.pop_back();
                    std::cout << "Consumed: " << value << std::endl;
                    ++processed;
                }

                // Unlock the mutex before reaching the end of the scope so that other code can access the `data` vector, since we
                // will no longer be working with the `data` vector
                lock.unlock();
                std::this_thread::sleep_for(100ms);
            } else {
                std::cout << "Consumer: mutex busy, doing other work..." << std::endl;
                std::this_thread::sleep_for(50ms);
            }
        }
    });
}