#pragma once

#include <mutex>
#include <memory>
#include <stdexcept>

namespace p3
{

    class Synchronizable
    {
    public:
        virtual ~Synchronizable() = default;

        virtual void synchronize_with(Synchronizable&);
        virtual void release();

        [[nodiscard]] std::unique_ptr<std::lock_guard<std::recursive_mutex>> lock();
        [[nodiscard]] std::unique_ptr<std::lock_guard<std::recursive_mutex>> lock() const;

        std::shared_ptr<std::recursive_mutex> const& recursive_mutex() const;

    private:
        std::shared_ptr<std::recursive_mutex> _mutex = std::make_unique<std::recursive_mutex>();
    };

}
