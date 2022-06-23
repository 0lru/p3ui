
#include "Context.h"
#include "UserInterface.h"

#include <imgui.h>

namespace p3 {

namespace {
    thread_local Context* current_context = nullptr;
}

Context::Context(UserInterface& user_interface, RenderBackend& render_backend, MouseMove mouse_move)
    : _user_interface(user_interface)
    , _render_backend(render_backend)
    , _mouse_move(std::move(mouse_move))
{
    current_context = this;
}

Context::~Context()
{
}

RenderLayer& Context::render_layer() const
{
    if (_render_layer.empty())
        throw std::runtime_error("no render layer present");
    return *_render_layer.back();
}

Context& Context::current()
{
    if (!p3::current_context)
        throw std::runtime_error("no context active");
    return *p3::current_context;
}

UserInterface& Context::user_interface() const
{
    return _user_interface;
}

RenderBackend& Context::render_backend() const
{
    return _render_backend;
}

Context::MouseMove const& Context::mouse_move() const
{
    return _mouse_move;
}

float Context::to_actual(Length const& length) const
{
    if (std::holds_alternative<Px>(length))
        return std::get<Px>(length).value;
    if (std::holds_alternative<Em>(length))
        return ImGui::GetFontSize() * std::get<Em>(length).value;
    return rem() * std::get<Rem>(length).value;
}

float Context::rem() const
{
    return _user_interface.rem();
}

Theme& Context::theme() const
{
    return *_user_interface.theme();
}

}
