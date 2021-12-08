#pragma once

#include <functional>

namespace p3
{

    class [[nodiscard]] OnScopeExit
    {
    private:
        std::function<void()> f;

    public:
        // Note: nodiscard for constructors requires C++17
        [[nodiscard]] OnScopeExit(std::function<void()> f) : f{ std::move(f) } {}
        ~OnScopeExit() { if(f) f(); }

        OnScopeExit() = delete;

        OnScopeExit(OnScopeExit&&) = default;
        OnScopeExit& operator=(OnScopeExit&&) = default;

        OnScopeExit(OnScopeExit const&) = delete;
        OnScopeExit& operator=(OnScopeExit const&) = delete;
    };

}
