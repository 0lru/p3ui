#include "Synchronizable.h"

namespace p3
{

    void Synchronizable::synchronize_with(Synchronizable& synchronized)
    {
        _mutex = synchronized._mutex;
    }

    void Synchronizable::release()
    {
        _mutex = std::make_unique<std::recursive_mutex>();
    }

    std::unique_ptr<std::lock_guard<std::recursive_mutex>> Synchronizable::lock()
    {
        return std::make_unique<std::lock_guard<std::recursive_mutex>>(*_mutex);
    }

    std::unique_ptr<std::lock_guard<std::recursive_mutex>> Synchronizable::lock() const
    {
        return std::make_unique<std::lock_guard<std::recursive_mutex>>(*_mutex);
    }

    std::shared_ptr<std::recursive_mutex> const& Synchronizable::recursive_mutex() const
    {
        return _mutex;
    }

}
