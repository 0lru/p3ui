#pragma once

#include <functional>

namespace p3 {

class [[nodiscard]] on_scope_exit {
private:
    std::function<void()> f;

public:
    // Note: nodiscard for constructors requires C++17
    [[nodiscard]] on_scope_exit(std::function<void()> f)
        : f { std::move(f) }
    {
    }
    ~on_scope_exit()
    {
        if (f)
            f();
    }

    on_scope_exit() = delete;

    on_scope_exit(on_scope_exit&&) = default;
    on_scope_exit& operator=(on_scope_exit&&) = default;

    on_scope_exit(on_scope_exit const&) = delete;
    on_scope_exit& operator=(on_scope_exit const&) = delete;
};

}
