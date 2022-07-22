#include "Node.h"
#include "Context.h"
#include "RenderLayer.h"
#include "Theme.h"
#include "convert.h"
#include "log.h"
#include "platform/event_loop.h"

#include <p3/Parser.h>

#include <imgui.h>
#include <imgui_internal.h>
#include <mutex>
#include <unordered_map>

namespace p3 {

std::function<void(Node&)> NodeInitializer = nullptr;

namespace registry {

    thread_local struct
    {
        std::vector<std::uint64_t> pool;
        std::uint64_t max = 0;
        std::unordered_map<std::uint64_t, Node*> nodes;
    } state;

    std::size_t count()
    {
        return state.nodes.size();
    }

    std::uint64_t add(Node* node)
    {
        std::uint64_t id;
        if (state.pool.empty()) {
            id = state.max++;
        } else {
            id = state.pool.back();
            state.pool.pop_back();
        }
        state.nodes[id] = node;
        return id;
    }

    void release(std::uint64_t id)
    {
        state.pool.push_back(id);
        state.nodes.erase(id);
    }

}

Node::Node(std::string element_name)
    : _element_name(std::move(element_name))
    , _imgui_id(registry::add(this))
    , _imgui_label("##" + std::to_string(_imgui_id))
    , _status_flags(ImGuiItemStatusFlags_None)
{
    if (NodeInitializer)
        NodeInitializer(*this);
}

Node::~Node()
{
    for (auto& child : _children)
        child->_parent = nullptr;
    // log_warn("~Node: {} {}", this->imgui_label(), element_name());
    registry::release(_imgui_id);
}

void Node::set_attribute(std::string const& name, std::string const& value)
{
    static auto setter = std::unordered_map<std::string, std::function<void(Node&, std::string const&)>> {
        { "label", [](Node& node, std::string const& value) { node.set_label(value); } },
        { "width", [](Node& node, std::string const& value) { node.set_width(parser::parse<LayoutLength>(value.c_str())); } },
        { "height", [](Node& node, std::string const& value) { node.set_height(parser::parse<LayoutLength>(value.c_str())); } },
    };
    auto it = setter.find(name);
    if (it == setter.end())
        throw parser::ParserError(fmt::format("attribute {} not found", name));
    it->second(*this, value);
}

void Node::update_status()
{
    ImGuiWindow const& window = *GImGui->CurrentWindow;
    auto const status_flags = GImGui->LastItemData.StatusFlags;
    if (status_flags == _status_flags) {
        if (_mouse.hovered && _mouse.move && Context::current().mouse_move()) {
            MouseEvent e(this);
            postpone([f = _mouse.move, e = std::move(e)]() mutable { f(std::move(e)); });
        }
    } else if ((status_flags & ImGuiItemStatusFlags_HoveredRect) && !(_status_flags & ImGuiItemStatusFlags_HoveredRect)) {
        _mouse.hovered = true;
        if (_mouse.enter || _mouse.hovered) {
            MouseEvent e(this);
            _mouse.x = e.global_x();
            _mouse.y = e.global_y();
            if (_mouse.enter)
                postpone([f = _mouse.enter, e = std::move(e)]() mutable { f(std::move(e)); });
        }
    } else if (_status_flags & ImGuiItemStatusFlags_HoveredRect) {
        _mouse.hovered = false;
        if (_mouse.leave)
            postpone([f = _mouse.leave, e = MouseEvent(this)]() mutable { f(std::move(e)); });
    }
    if (_mouse.hovered && _mouse.wheel) {
        auto wheel = ImGui::GetIO().MouseWheel;
        if (wheel != 0.f) {
            postpone([f = _mouse.wheel, wheel]() mutable { f(wheel); });
        }
    }
    _status_flags = status_flags;
}

void Node::postpone(std::function<void()> f)
{
    EventLoop::current()->call_at(EventLoop::Clock::now(), Event::create(f));
}

std::string const& Node::element_name() const
{
    return _element_name;
}

Node* Node::parent() const
{
    return _parent;
}

std::shared_ptr<Node> Node::shared_parent() const
{
    return _parent ? _parent->shared_from_this() : nullptr;
}

void Node::update_restyle(Context& context, bool force)
{
    //
    // set both to true if subtree is forced to restyle
    _needs_restyle = _needs_restyle || force;
    _needs_update = _needs_update || force;

    //
    // no change in this subtree, abort
    if (!_needs_update)
        return;

    if (_needs_restyle) {
        //
        // force layer to redraw
        if (_render_layer)
            _render_layer->set_dirty();
    }

    {
        //
        // needs to be done before update_content()
        push_style();
        for (auto& child : _children)
            if (child->visible())
                child->update_restyle(context,
                    _needs_restyle /* force restyle of child if this was restyled*/);
        pop_style();
        update_content();
    }
    //
    // change state
    _needs_update = _needs_restyle = false;
}

void Node::set_needs_restyle()
{
    _needs_restyle = true;
    set_needs_update();
}

void Node::redraw()
{
    if (_parent)
        _parent->redraw();
}

std::size_t Node::node_count()
{
    return registry::count();
}

void Node::set_needs_update()
{
    auto it = this;
    while (it) {
        if (it->_render_layer) {
            it->_render_layer->set_dirty();
            break;
        }
        it = it->_parent;
    }
    _needs_update = true;
    it = _parent;
    while (it) {
        if (it->_needs_update)
            break;
        it->_needs_update = true;
        it = it->_parent;
    }
}

bool Node::needs_restyle() const
{
    return _needs_restyle;
}

bool Node::needs_update() const
{
    return _needs_update;
}

void Node::set_parent(Node* parent)
{
    _parent = parent;
}

void Node::before_add(Node& node) const
{
    //    if (node._disposed)
    //        throw std::invalid_argument("cannot reuse disposed node");
    if (node._parent)
        throw std::invalid_argument("node is already assigned");
}

void Node::add(std::shared_ptr<Node> node)
{
    before_add(*node);
    node->set_parent(this);
    _children.push_back(std::move(node));
    _children.back()->set_needs_restyle();
}

void Node::insert(std::size_t index, std::shared_ptr<Node> node)
{
    before_add(*node);
    node->set_parent(this);
    auto it = _children.begin();
    std::advance(it, index);
    (*_children.insert(it, std::move(node)))->set_needs_restyle();
}

void Node::remove(std::shared_ptr<Node> node)
{
    node->dispose();
    node->set_parent(nullptr);
    _children.erase(std::remove_if(_children.begin(), _children.end(), [&](auto& item) {
        return item == node;
    }),
        _children.end());
    set_needs_update();
}

Node::Children const& Node::children() const
{
    return _children;
}

void Node::set_render_layer(std::shared_ptr<RenderLayer> render_layer)
{
    _render_layer = std::move(render_layer);
}

std::shared_ptr<RenderLayer> const& Node::render_layer() const
{
    return _render_layer;
}

std::shared_ptr<void> const& Node::user_data() const
{
    return _user_data;
}

void Node::set_user_data(std::shared_ptr<void> user_data)
{
    _user_data = std::move(user_data);
}

void Node::render(RenderBackend::RenderTarget& render_target)
{
    for (auto& child : _children)
        if (!child->is_layered())
            child->render(render_target);
}

void Node::render(Context& context, float width, float height, bool adjust_worksrect)
{
    if (!_visible)
        return;
    //
    // do not traverse this tree if rect is invalid
    if (width * height < 0)
        return;

    //
    // emit on_resize
    auto size = Size { width, height };
    if (size != _size) {
        if (_on_resize)
            postpone([on_resize = _on_resize, size]() { on_resize(std::move(size)); });
        std::swap(size, _size);
    }

    float disabled_alpha = 0.2f;
    if (_disabled)
        std::swap(disabled_alpha, ImGui::GetStyle().Alpha);

    push_style();
    on_scope_exit guard([&]() {
        pop_style();
    });
    if (adjust_worksrect) {
        //
        // this needs to be done after style was applied and is only used by Layout.cpp
        auto& work_rect = GImGui->CurrentWindow->WorkRect;
        ImVec2 work_rect_max = work_rect.Min;
        work_rect_max.x += width + ImGui::GetCurrentContext()->Style.FramePadding.x /* 2*/;
        work_rect_max.y += height + ImGui::GetCurrentContext()->Style.FramePadding.y /* 2*/;
        std::swap(work_rect.Max, work_rect_max);
        render_impl(context, width, height);
        std::swap(work_rect.Max, work_rect_max);
    } else {
        render_impl(context, width, height);
    }

    if (_disabled)
        std::swap(disabled_alpha, ImGui::GetStyle().Alpha);
}

void Node::render_absolute(Context& context)
{
    if (!parent())
        return;
    auto& parent = *this->parent();

    for (auto& child : _children)
        if (child->position() == Position::Absolute) {
            auto avail = ImGui::GetContentRegionAvail();
            auto cursor = ImGui::GetCursorPos();
            child->render(context, child->contextual_width(avail.x), child->contextual_height(avail.y));
        }
}

void Node::set_label(std::optional<std::string> label)
{
    _label = std::move(label);
    _imgui_label = (_label ? _label.value() : "") + "##n" + std::to_string(_imgui_id);
    set_needs_update();
}

std::optional<std::string> const& Node::label() const
{
    return _label;
}

std::uint64_t Node::imgui_id() const
{
    return _imgui_id;
}

std::string const& Node::imgui_label() const
{
    return _imgui_label;
}

void Node::set_disabled(bool disabled)
{
    _disabled = disabled;
}

bool Node::disabled() const
{
    return _disabled;
}
void Node::set_on_mouse_wheel(MouseWheelHandler handler)
{
    _mouse.wheel = std::move(handler);
}

Node::MouseWheelHandler Node::on_mouse_wheel() const
{
    return _mouse.wheel;
}

void Node::set_on_mouse_enter(MouseEventHandler handler)
{
    _mouse.enter = handler;
}

Node::MouseEventHandler Node::on_mouse_enter() const
{
    return _mouse.enter;
}

void Node::set_on_mouse_leave(MouseEventHandler handler)
{
    _mouse.leave = handler;
}

Node::MouseEventHandler Node::on_mouse_leave() const
{
    return _mouse.leave;
}

void Node::set_on_mouse_move(MouseEventHandler handler)
{
    _mouse.move = handler;
}

Node::MouseEventHandler Node::on_mouse_move() const
{
    return _mouse.move;
}

bool Node::hovered() const
{
    return _mouse.hovered;
}

void Node::set_visible(bool visible)
{
    if (_visible == visible)
        return;
    _visible = visible;
    set_needs_restyle();
}

bool Node::visible() const
{
    return _visible;
}

OptionalLengthPercentage const& Node::width_basis() const
{
    return std::get<0>(_width);
}

float Node::width_grow() const
{
    return std::get<1>(_width);
}

float Node::width_shrink() const
{
    return std::get<2>(_width);
}

OptionalLengthPercentage const& Node::height_basis() const
{
    return std::get<0>(_height);
}

float Node::height_grow() const
{
    return std::get<1>(_height);
}

float Node::height_shrink() const
{
    return std::get<2>(_height);
}

Position Node::position() const
{
    return _position;
}

void Node::set_position(Position position)
{
    _position = position;
}

LengthPercentage Node::left() const
{
    return _left;
}

void Node::set_left(LengthPercentage left)
{
    _left = left;
}

LengthPercentage Node::top() const
{
    return _top;
}

void Node::set_top(LengthPercentage top)
{
    _top = top;
}

float Node::contextual_width(float content) const
{
    if (!width_basis())
        return _automatic_width;
    auto const& basis = width_basis().value();
    if (std::holds_alternative<Percentage>(basis))
        return content * std::get<Percentage>(basis).value / 100.f;
    else
        return Context::current().to_actual(std::get<Length>(basis));
}

float Node::contextual_height(float content) const
{
    if (!height_basis())
        return _automatic_height;
    auto const& basis = height_basis().value();
    if (std::holds_alternative<Percentage>(basis))
        return content * std::get<Percentage>(basis).value / 100.f;
    else
        return Context::current().to_actual(std::get<Length>(basis));
}

float Node::contextual_minimum_content_width() const
{
    return _automatic_width;
}

float Node::contextual_minimum_content_height() const
{
    return _automatic_height;
}

Node::MouseEvent::MouseEvent(Node* source)
    : _source(source)
{
    ImGuiContext& context = *GImGui;
    _global_x = context.IO.MousePos.x;
    _global_y = context.IO.MousePos.y;
    auto& last_rect = context.LastItemData.Rect;
    _x = _global_x - last_rect.Min.x;
    _y = _global_y - last_rect.Min.y;
    _left_button_down = context.IO.MouseDown[0];
    _right_button_down = context.IO.MouseDown[1];
    _middle_button_down = context.IO.MouseDown[2];
}

Node* Node::MouseEvent::source() const
{
    return _source;
}

float Node::MouseEvent::global_x() const
{
    return _global_x;
}

float Node::MouseEvent::global_y() const
{
    return _global_y;
}

float Node::MouseEvent::x() const
{
    return _x;
}

float Node::MouseEvent::y() const
{
    return _y;
}

bool Node::MouseEvent::left_button_down() const
{
    return _left_button_down;
}

bool Node::MouseEvent::right_button_down() const
{
    return _right_button_down;
}

bool Node::MouseEvent::middle_button_down() const
{
    return _middle_button_down;
}

void Node::render_impl(Context&, float width, float height)
{
}

void Node::dispose()
{
    if (_render_layer)
        _render_layer->reset();
    for (auto& child : _children)
        child->dispose();
}

void Node::set_tooltip(std::shared_ptr<Node> tooltip)
{
    _tooltip = std::move(_tooltip);
}

std::shared_ptr<Node> const& Node::tooltip() const
{
    return _tooltip;
}

Node::Size Node::size() const
{
    return _size;
}

void Node::set_on_resize(OnResize on_resize)
{
    _on_resize = on_resize;
}

Node::OnResize Node::on_resize() const
{
    return _on_resize;
}

LayoutLength const& Node::width() const
{
    return _width;
}

void Node::set_width(LayoutLength width)
{
    if (_width == width)
        return;
    _width = std::move(width);

    set_needs_restyle();
}

LayoutLength const& Node::height() const
{
    return _height;
}

void Node::set_height(LayoutLength height)
{
    if (_height == height)
        return;
    _height = std::move(height);
    set_needs_restyle();
}

std::optional<Color> const& Node::color() const
{
    return _color;
}

void Node::set_color(std::optional<Color> color)
{
    _color = std::move(color);
    redraw();
}

void Node::push_style()
{
    if (_color) {
        ImVec4 imgui_color;
        assign(imgui_color, _color.value());
        ImGui::PushStyleColor(ImGuiCol_Text, imgui_color);
    }
}

void Node::pop_style()
{
    if (_color) {
        ImGui::PopStyleColor();
    }
}

}
