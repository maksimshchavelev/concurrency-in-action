#include <future>
#include <iostream>
#include <thread>

int main() {
    // 1. Create a packaged_task: a wrapper around a callable that
    //    automatically manages a promise/future pair for its result.
    //    It takes an int and returns an int (square of the input).
    std::packaged_task<int(int)> task([](int value) { return value * value; });

    // 2. Obtain the future associated with this task.
    //    The future is a "receipt" for the result that will be produced later.
    auto future = task.get_future();

    // 3. Move the task into a new std::jthread (which automatically joins on destruction).
    //    - packaged_task is move-only, so we must use std::move.
    //    - The argument 50 is passed to the task's operator() when the thread starts.
    //    - The thread starts immediately and begins executing the task asynchronously.
    std::jthread t(std::move(task), 50);

    // 4. future.get() blocks the current thread (main) until the result is ready.
    //    - If the task has already finished, get() returns immediately.
    //    - If the task threw an exception, that exception is re-thrown here.
    //    - After get() is called, the shared state is released; subsequent get() would throw.
    std::cout << "Got value: " << future.get();
}