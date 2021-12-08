
#pragma once

#include "StyleTypes.h"

#include <memory>
#include <array>
#include <optional>
#include <string>
#include <functional>

namespace p3
{

    class Theme;
    class UserInterface;
    class RenderBackend;
    class TaskQueue;

    class Context : public std::enable_shared_from_this<Context>
    {
    public:
        using MouseMove = std::optional<std::array<float, 2>>;
        using Postponed = std::function<void()>;

        Context(UserInterface&, TaskQueue&, RenderBackend&, MouseMove);
        ~Context();
        static Context& current();

        UserInterface& user_interface() const;
        RenderBackend& render_backend() const;

        float rem() const;
        Theme& theme() const;
        
        MouseMove const& mouse_move() const;
        float to_actual(Length const&) const;

        // task queue related to the user interface
        TaskQueue& task_queue() const;

    private:
        UserInterface& _user_interface;
        TaskQueue& _task_queue;
        RenderBackend& _render_backend;
        MouseMove _mouse_move;
    };

}
