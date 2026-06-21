#include <shared_mutex>
#include <string>
#include <thread>
#include <unordered_map>

class DNS {
  public:
    /// Returns the IP address for a domain
    std::optional<std::string> get_ip(const std::string& domain) const {
        std::shared_lock lock(m_mutex); // Multiple threads can read, but the writer has to wait (the writer uses a regular lock
                                        // instead of a shared_lock)
        auto res = m_domains.find(domain);

        if (res == m_domains.end()) {
            return std::nullopt;
        }

        return res->second;
    }

    /// Creates or updates the IP address for the specified domain
    void update_or_create_ip(const std::string& domain, std::string ip) {
        std::lock_guard lock(m_mutex); // A shared mutex is locked exclusively. Now, new readers will not be able to read (or
                                       // another writer will not be able to write) and will be forced to wait
        m_domains.insert_or_assign(domain, std::move(ip));
    }

  private:
    std::unordered_map<std::string /* domain */, std::string /* ip */> m_domains;
    mutable std::shared_mutex                                          m_mutex;
};

int main() {
    DNS dns;

    std::jthread reader([&]() {
        for (unsigned i = 0; i < 10'000'000; ++i) {
            auto res = dns.get_ip("google.com");
        }
    });

    std::jthread writer([&](){
        for (unsigned i = 0; i < 10'000; ++i) {
            dns.update_or_create_ip("google.com", "1.1.1.1");
        }
    });

    return 0;
}