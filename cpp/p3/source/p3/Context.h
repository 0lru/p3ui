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
    class RenderLayer;
    class RenderTarget;

    class Context : public std::enable_shared_from_this<Context>
    {
    public:
        using MouseMove = std::optional<std::array<float, 2>>;
        using Postponed = std::function<void()>;

        Context(UserInterface&, RenderBackend&, MouseMove);
        ~Context();

        static Context& current();

        UserInterface& user_interface() const;
        RenderBackend& render_backend() const;

        float rem() const;
        Theme& theme() const;
        
        MouseMove const& mouse_move() const;
        float to_actual(Length const&) const;

        RenderLayer& render_layer() const;
        void push_render_layer(RenderLayer& layer) { _render_layer.push_back(&layer); }
        void pop_render_layer() { _render_layer.pop_back(); }
        void set_render_target(RenderTarget* render_target) { _render_target = render_target; }
        RenderTarget* render_target() const { return _render_target; }

        void set_show_render_layers(bool show_render_layers) { _show_render_layers = show_render_layers; }
        bool show_render_layers() const { return _show_render_layers; }

    private:
        bool _show_render_layers = true;
        UserInterface& _user_interface;
        RenderBackend& _render_backend;
        MouseMove _mouse_move;
        RenderTarget* _render_target;
        std::vector<RenderLayer*> _render_layer;
    };

}
