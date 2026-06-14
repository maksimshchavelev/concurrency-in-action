#include <concepts>
#include <iostream>
#include <iterator>
#include <numeric>
#include <thread>
#include <vector>

template <std::input_iterator Iter, typename T, std::invocable<T, T> BinaryOp>
T accumulate(Iter first, Iter last, T init, BinaryOp op) {
    const size_t items = std::distance(first, last);

    if (items <= 0) {
        return init;
    }

    const size_t max_items_per_thread = 1024;
    const size_t hardware_threads = std::max(1u, std::thread::hardware_concurrency());
    const size_t threads_count = std::min(hardware_threads, std::max(1ul, items / max_items_per_thread));

    if (threads_count == 1) {
        return std::accumulate(first, last, init, op);
    }

    const size_t block_size = items / threads_count;
    const size_t remainder = items % threads_count;

    std::vector<T> results(threads_count);

    std::vector<std::jthread> threads; // jthread for RAII (in exception case)
    threads.reserve(threads_count);

    auto iter = first;
    for (size_t i = 0; i < threads_count; ++i) {
        auto   start = iter;
        size_t block = block_size + (i < remainder ? 1 : 0);
        auto   end = std::next(start, block);

        iter = end;

        threads.emplace_back([start, end, &res = results[i], op] { res = std::accumulate(start, end, T{}, op); });
    }

    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    return std::accumulate(results.begin(), results.end(), init, op);
}


int main() {
    std::vector<size_t> nums;

    std::cout << "Filling vector...\n";

    for (size_t i = 0; i < 1'000'000; ++i) {
        nums.push_back(i);
    }

    std::cout << "Accumulating...\n\n";

    int sum = ::accumulate(nums.begin(), nums.end(), 0ul, std::plus<>{});

    std::cout << sum;
}