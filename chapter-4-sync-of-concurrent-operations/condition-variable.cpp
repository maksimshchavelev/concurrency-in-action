#include <condition_variable>
#include <iostream>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

int main() {
    std::mutex              mutex;
    std::condition_variable cv;
    std::vector<unsigned>   data;

    std::jthread producer([&]() {
        for (unsigned i = 0; i < 10; ++i) {
            {
                std::lock_guard lock(mutex);
                data.push_back(i);
                std::cout << "Produced " << i << std::endl;
            }

            cv.notify_one();
            std::this_thread::sleep_for(750ms);
        }
    });

    std::jthread consumer([&]() {
        for (unsigned i = 0; i < 10; ++i) {
            std::unique_lock lock(mutex);

            cv.wait(lock, [&]() { return !data.empty(); });

            unsigned value = data.back();
            data.pop_back();
            std::cout << "Consumed: " << value << std::endl;
        }
    });

    return 0;
}
