#include <barrier>
#include <format>
#include <iostream>
#include <thread>
#include <vector>

/// Thread-safe print
void print(const std::string& message) {
    static std::mutex mutex;
    std::lock_guard   lock(mutex);
    std::cout << message << std::endl;
}

int main() {
    const unsigned num_threads = 4;
    const int      iterations = 3;

    // std::barrier is a reusable synchronization point for a fixed number of threads.
    // It allows multiple threads to wait until all of them have reached the barrier
    // before proceeding to the next phase. Useful for iterative/parallel algorithms
    // where each phase must be completed by all threads. Unlike std::latch, it can be
    // reused across multiple phases.
    // The optional completion function (callable) is invoked by one of the threads
    // when the barrier is reached, and can be used to merge results or log phase changes.
    std::barrier barrier(num_threads, []() { print("All threads reached barrier, proceeding to next phase."); });

    std::vector<std::jthread> threads;

    for (unsigned i = 0; i < num_threads; ++i) {
        threads.emplace_back([&barrier, iterations, id = i] {
            for (unsigned phase = 0; phase < iterations; ++phase) {
                std::this_thread::sleep_for(std::chrono::milliseconds(400 * (id + 1)));

                print(std::format("Thread {} completed phase {}", id, phase));

                // We're waiting for the remaining threads at the barrier
                barrier.arrive_and_wait();
            }

            print(std::format("Thread {} finished all iterations", id));
        });
    }

    return 0;
}