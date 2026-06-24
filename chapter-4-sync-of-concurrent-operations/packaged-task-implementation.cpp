// A sample implementation of `packaged_task`. It is by no means perfect, and, of course, the STL implementation is significantly
// better.

#include <functional>
#include <future>
#include <iostream>
#include <thread>

// Primary template declaration: used only to define the shape of the type.
// A single template parameter 'Signature' (e.g., int(int)) is expected.
// However, this primary template is never defined — only the partial specialization below is.
template <typename Signature> class my_packaged_task;

// Partial specialization for function types.
// When the user writes my_packaged_task<int(int)>, the compiler matches this specialization.
// The type int(int) is "decomposed" into R = int and Args... = int.
// This allows us to store the callable with the exact signature and forward arguments perfectly.
template <typename R, typename... Args> class my_packaged_task<R(Args...)> {
  public:
    my_packaged_task(std::move_only_function<R(Args&&...)> function) : m_function(std::move(function)) {}

    my_packaged_task(const my_packaged_task& other) = delete;

    my_packaged_task(my_packaged_task&& other) :
        m_function(std::move(other.m_function)), m_promise(std::move(other.m_promise)), m_got_future(other.m_got_future),
        m_has_state(other.m_has_state), m_invoked(other.m_invoked) {
        other.m_has_state = false;
        other.m_got_future = false;
        other.m_invoked = false;
    }

    my_packaged_task& operator=(const my_packaged_task& other) = delete;

    my_packaged_task& operator=(my_packaged_task&& other) {
        if (std::addressof(other) == this) {
            return *this;
        }

        m_function = std::move(other.m_function);
        m_promise = std::move(other.m_promise);
        m_got_future = other.m_got_future;
        m_has_state = other.m_has_state;
        m_invoked = other.m_invoked;
        other.m_has_state = false;
        other.m_got_future = false;
        other.m_invoked = false;

        return *this;
    }

    bool valid() const noexcept {
        return m_has_state;
    }

    std::future<R> get_future() {
        if (m_got_future) {
            throw std::future_error(std::future_errc::future_already_retrieved);
        }

        if (!m_has_state) {
            throw std::future_error(std::future_errc::no_state);
        }

        m_got_future = true;
        return m_promise.get_future();
    }

    void operator()(Args&&... args) {
        if (!m_has_state) {
            throw std::future_error(std::future_errc::no_state);
        }

        if (m_invoked) {
            throw std::future_error(std::future_errc::promise_already_satisfied);
        }

        if constexpr (std::is_same_v<R, void>) {
            try {
                m_function(std::forward<Args>(args)...);
                m_promise.set_value();
            } catch (...) {
                m_promise.set_exception(std::current_exception());
            }
        } else {
            try {
                m_promise.set_value(m_function(std::forward<Args>(args)...));
            } catch (...) {
                m_promise.set_exception(std::current_exception());
            }
        }

        m_invoked = true;
    }

  private:
    std::move_only_function<R(Args&&...)>
                    m_function; // std::move_only_function is C++23 feature. It can only be moved, not copied
    std::promise<R> m_promise;
    bool            m_got_future{false};
    bool            m_has_state{true};
    bool            m_invoked{false};
};


int main() {
    my_packaged_task<int(int)> task([](int value) { return value * value; });

    auto future = task.get_future();

    std::jthread t(std::move(task), 4);

    std::cout << "Got value: " << future.get(); // 16
}
