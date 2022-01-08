#include "Node.h"
#include "convert.h"
#include "Context.h"
#include "Theme.h"
#include "log.h"
#include "StyleDerivation.h"
#include "TaskQueue.h"
#include "RenderLayer.h"

#include <p3/Parser.h>

#include <mutex>
#include <unordered_map>
#include <imgui.h>
#include <imgui_internal.h>

namespace p3
{

    // using default values..
    StyleStrategy Node::DefaultStyleStrategy;

    class NodeRegistry
    {
    public:
        std::size_t count()
        {
            std::lock_guard<std::mutex> l(_mutex);
            return _nodes.size();
        }

        std::uint64_t add(Node* node)
        {
            std::lock_guard<std::mutex> l(_mutex);
            std::uint64_t id;
            if (_pool.empty())
            {
                id = _max_id++;
            }
            else
            {
                id = _pool.back();
                _pool.pop_back();
            }
            _nodes[id] = node;
            return id;
        }

        void release(std::uint64_t id)
        {
            std::lock_guard<std::mutex> l(_mutex);
            _pool.push_back(id);
            _nodes.erase(id);
        }

        static NodeRegistry& instance()
        {
            static NodeRegistry instance;
            return instance;
        }

    private:
        NodeRegistry() = default;
        ~NodeRegistry() = default;
        NodeRegistry(const NodeRegistry&) = delete;
        NodeRegistry& operator=(const NodeRegistry&) = delete;
        static std::shared_ptr<NodeRegistry> _instance;

        std::mutex _mutex;
        std::vector<std::uint64_t> _pool;
        std::uint64_t _max_id = 0;
        std::unordered_map<std::uint64_t, Node*> _nodes;
    };


    Node::Node(std::string element_name)
        : _element_name(std::move(element_name))
        , _imgui_id(NodeRegistry::instance().add(this))
        , _imgui_label("##" + std::to_string(_imgui_id))
        , _status_flags(ImGuiItemStatusFlags_None)
        , _style(std::make_shared<StyleBlock>())
    {
        _style->add_observer(this);
        _style_guard = OnScopeExit([this, style = _style]() {
            style->remove_observer(this);
        });
    }

    Node::~Node()
    {
        for (auto& child : _children)
            child->_parent = nullptr;
        log_verbose("~Node: {} {}", this->imgui_label(), element_name());
        NodeRegistry::instance().release(_imgui_id);
    }

    void Node::set_attribute(std::string const& name, std::string const& value)
    {
        static auto setter = std::unordered_map<std::string, std::function<void(Node&, std::string const&)>>{
            { "label", [](Node& node, std::string const& value) { node.set_label(value); }},
            { "width", [](Node& node, std::string const& value) { node.style()->set_width(parser::parse<LayoutLength>(value.c_str())); }},
            { "height", [](Node& node, std::string const& value) { node.style()->set_height(parser::parse<LayoutLength>(value.c_str())); }},
            { "direction", [](Node& node, std::string const& value) { node.style()->set_direction(parser::parse<Direction>(value.c_str())); }},
            { "align-items", [](Node& node, std::string const& value) { node.style()->set_align_items(parser::parse<Alignment>(value.c_str())); }},
            { "justify-content", [](Node& node, std::string const& value) { node.style()->set_justify_content(parser::parse<Justification>(value.c_str())); }},
        };
        auto it = setter.find(name);
        if (it == setter.end())
            throw parser::ParserError(fmt::format("attribute {} not found", name));
        it->second(*this, value);
    }

    void Node::on_style_changed()
    {
        set_needs_restyle();
    }

    StyleStrategy& Node::style_strategy() const
    {
        return DefaultStyleStrategy;
    }

    void Node::update_status()
    {
        ImGuiWindow const& window = *GImGui->CurrentWindow;
        auto const status_flags = GImGui->LastItemData.StatusFlags;
        if (status_flags == _status_flags)
        {
            if (_mouse.hovered && _mouse.move && Context::current().mouse_move())
                postpone([f = _mouse.move, e = MouseEvent(this)]() mutable { f(std::move(e)); });
        }
        else if ((status_flags & ImGuiItemStatusFlags_HoveredRect) && !(_status_flags & ImGuiItemStatusFlags_HoveredRect))
        {
            _mouse.hovered = true;
            if (_mouse.enter)
                postpone([f = _mouse.enter, e = MouseEvent(this)]() mutable {f(std::move(e)); });
        }
        else if (_status_flags & ImGuiItemStatusFlags_HoveredRect)
        {
            _mouse.hovered = false;
            if (_mouse.leave)
                postpone([f = _mouse.leave, e = MouseEvent(this)]() mutable { f(std::move(e)); });
        }
        _status_flags = status_flags;
    }

    void Node::postpone(std::function<void()> f)
    {
        Context::current().task_queue().run(std::move(f));
    }

    namespace
    {
        template<typename T>
        T cascade(Node const& node, Cascadable<T> const& combined, T StyleComputation::* member, T const& initial)
        {
            if (std::holds_alternative<Cascade>(combined))
                if ((std::get<Cascade>(combined) == Cascade::Inherit) && node.parent())
                    return node.parent()->style_computation().*member;
                else
                    return initial;
            return std::get<T>(combined);
        }
    }

    std::string const& Node::element_name() const
    {
        return _element_name;
    }

    void Node::_cascade_styles_from_parent(Context& context)
    {
        StyleDerivation combined;
        StyleBlock style;
        if (_style)
            combined.merge(*_style);
        _style_computation.color = cascade(*this, combined.color, &StyleComputation::color, context.theme().text_color());
        static Length const bw = 1 | px;
        _style_computation.border_width = cascade(*this, combined.border_width, &StyleComputation::border_width, bw);
        _style_computation.border_radius = cascade(*this, combined.border_radius, &StyleComputation::border_radius, bw);
        _style_computation.padding = cascade(*this, combined.padding, &StyleComputation::padding, context.theme().frame_padding());
        _style_computation.spacing = cascade(*this, combined.spacing, &StyleComputation::spacing, context.theme().item_spacing());
        _style_computation.position = cascade(*this, combined.position, &StyleComputation::position, Position::Static);
        //
        // flex..
        static LengthPercentage initial_position = 0 | px;
        static Direction initial_direction = Direction::Vertical;
        static Alignment initial_alignment = Alignment::Stretch;
        static Justification initial_justification = Justification::Start;
        bool initial_visibility = true;
        _style_computation.visible = cascade(*this, combined.visible, &StyleComputation::visible, initial_visibility);
        _style_computation.x = cascade(*this, combined.x, &StyleComputation::x, initial_position);
        _style_computation.y = cascade(*this, combined.y, &StyleComputation::y, initial_position);
        _style_computation.width = cascade(*this, combined.width, &StyleComputation::width, style_strategy().initial_width());
        _style_computation.height = cascade(*this, combined.height, &StyleComputation::height, style_strategy().initial_height());
        _style_computation.direction = cascade(*this, combined.direction, &StyleComputation::direction, initial_direction);
        _style_computation.justify_content = cascade(*this, combined.justify_content, &StyleComputation::justify_content, initial_justification);
        _style_computation.align_items = cascade(*this, combined.align_items, &StyleComputation::align_items, initial_alignment);
    }

    void Node::_compile_style_computation(Context& context)
    {
        _style_compiled.clear();
        //
        // check if computed color changes the render state
        ImVec4 color;
        assign(color, _style_computation.color);
        if (color != GImGui->Style.Colors[ImGuiCol_Text])
        {
            log_verbose("-style- <{}>\"{}\" changes color to {}", _element_name, _label ? _label.value() : "", to_string(_style_computation.color));
            _style_compiled.push_back([color{ std::move(color) }]() mutable {
                std::swap(color, GImGui->Style.Colors[ImGuiCol_Text]);
            });
        }
        //
        // check if spacing 
        ImVec2 spacing(
            context.to_actual(_style_computation.spacing[0]),
            context.to_actual(_style_computation.spacing[1])
        );
        //
        // spacing.. between buttons (inside a flex etc.)
        if (spacing != GImGui->Style.ItemSpacing)
        {
            log_verbose("-style- <{}>\"{}\" changes spacing to ({}, {})", _element_name, _label ? _label.value() : "", spacing.x, spacing.y);
            _style_compiled.push_back([spacing{ std::move(spacing) }]() mutable {
                std::swap(spacing, GImGui->Style.ItemSpacing);
            });
        }
        //
        // padding (between border and content). note that
        // this doesn't work for "Windows"
        ImVec2 padding(
            context.to_actual(_style_computation.padding[0]),
            context.to_actual(_style_computation.padding[1])
        );
        if (padding != GImGui->Style.FramePadding)
        {
            log_verbose("-style- <{}>\"{}\" changes padding to ({}, {})", _element_name, _label ? _label.value() : "", padding.x, padding.y);
            _style_compiled.push_back([padding{ std::move(padding) }]() mutable {
                std::swap(padding, GImGui->Style.FramePadding);
            });
        }
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

        if (_needs_restyle)
        {
            _cascade_styles_from_parent(context);
            //
            // at this point the anchestor-style is still applied
            _compile_style_computation(context);
            //
            // force layer to redraw
            if (_render_layer)
                _render_layer->set_dirty();
        }

        {
            //
            // needs to be done before update_content()
            auto compiled_guard = _apply_style_compiled();
            for (auto& child : _children)
                if (child->visible())
                    child->update_restyle(context,
                        _needs_restyle /* force restyle of child if this was restyled*/);
            update_content();
        }
        //
        // change state
        _needs_update = _needs_restyle = false;
    }

    StyleComputation const& Node::style_computation() const
    {
        return _style_computation;
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
        return NodeRegistry::instance().count();
    }

    void Node::set_needs_update()
    {
        auto it = this;
        while (it)
        {
            if (it->_render_layer)
            {
                it->_render_layer->set_dirty();
                break;
            }
            it = it->_parent;
        }
        _needs_update = true;
        it = _parent;
        while (it)
        {
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
        if (node._disposed)
            throw std::invalid_argument("cannot reuse disposed node");
        if (node._parent)
            throw std::invalid_argument("node is already assigned");
    }

    void Node::add(std::shared_ptr<Node> node)
    {
        before_add(*node);
        node->synchronize_with(*this);
        node->set_parent(this);
        _children.push_back(std::move(node));
        _children.back()->set_needs_restyle();
    }

    void Node::insert(std::size_t index, std::shared_ptr<Node> node)
    {
        before_add(*node);
        node->synchronize_with(*this);
        node->set_parent(this);
        auto it = _children.begin();
        std::advance(it, index);
        (*_children.insert(it, std::move(node)))->set_needs_restyle();
    }

    void Node::remove(std::shared_ptr<Node> node)
    {
        node->dispose();
        node->set_parent(nullptr);
        node->release();
        _children.erase(std::remove_if(_children.begin(), _children.end(), [&](auto& item) {
            return item == node;
        }), _children.end());
        set_needs_update();
    }

    Node::Children const& Node::children() const
    {
        return _children;
    }

    void Node::synchronize_with(Synchronizable& synchronized)
    {
        Synchronizable::synchronize_with(synchronized);
        _style->synchronize_with(*this);
        for (auto& node : _children)
            node->synchronize_with(synchronized);
    }

    void Node::release()
    {
        Synchronizable::release();
        _style->synchronize_with(*this);
        for (auto& node : _children)
            node->synchronize_with(*this);
    }

    void Node::set_render_layer(std::shared_ptr<RenderLayer> render_layer)
    {
        _render_layer = std::move(render_layer);
    }

    std::shared_ptr<RenderLayer> const& Node::render_layer() const
    {
        return _render_layer;
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
        auto size = Size{ width, height };
        if (size != _size)
        {
            if (_on_resize)
                postpone([on_resize = _on_resize, size]() { on_resize(std::move(size)); });
            std::swap(size, _size);
        }

        //
        // hacky "disabled"
        float disabled_alpha = 0.2f;
        if (_disabled)
            std::swap(disabled_alpha, ImGui::GetStyle().Alpha);

        //
        // apply style
        auto compiled_guard = _apply_style_compiled();

        if (adjust_worksrect)
        {
            //
            // this needs to be done after style was applied and is only used by Layout.cpp
            auto& work_rect = GImGui->CurrentWindow->WorkRect;
            ImVec2 work_rect_max = work_rect.Min;
            work_rect_max.x += width + ImGui::GetCurrentContext()->Style.FramePadding.x /* 2*/;
            work_rect_max.y += height + ImGui::GetCurrentContext()->Style.FramePadding.y /* 2*/;
            std::swap(work_rect.Max, work_rect_max);
            render_impl(context, width, height);
            std::swap(work_rect.Max, work_rect_max);
        }
        else
        {
            render_impl(context, width, height);
        }

        //
        // hacky disabled
        if (_disabled)
            std::swap(disabled_alpha, ImGui::GetStyle().Alpha);
    }

    void Node::render_absolute(Context& context)
    {
        for (auto& child : _children)
            if (child->style_computation().position == Position::Absolute)
            {
                auto avail = ImGui::GetContentRegionAvail();
                child->render(context, child->width(avail.x), child->height(avail.y));
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
        _visible = visible;
        set_needs_restyle();
    }

    bool Node::visible() const
    {
        return _visible;
    }

    float Node::width(float content) const
    {
        if (!style_computation().width_basis())
            return _automatic_width;
        auto const& lp = style_computation().width_basis().value();
        if (std::holds_alternative<Percentage>(lp))
            return content * std::get<Percentage>(lp).value / 100.f;
        else
            return Context::current().to_actual(std::get<Length>(lp));
    }

    float Node::height(float content) const
    {
        if (!style_computation().height_basis())
            return _automatic_height;
        auto const& lp = style_computation().height_basis().value();
        if (std::holds_alternative<Percentage>(lp))
            return content * std::get<Percentage>(lp).value / 100.f;
        else
            return Context::current().to_actual(std::get<Length>(lp));
    }

    float Node::automatic_width() const
    {
        return _automatic_width;
    }

    float Node::automatic_height() const
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
    }

    Node* Node::MouseEvent::source() const
    {
        return _source;
    }

    std::shared_ptr<StyleBlock> const& Node::style() const
    {
        // no lock needed, _style never changes
        return _style;
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

    OnScopeExit Node::_apply_style_compiled()
    {
        auto apply = [this]() {
            for (auto& f : _style_compiled)
                f();
        };
        apply();
        return OnScopeExit(apply);
    }

    void Node::dispose()
    {
        _mouse.enter = nullptr;
        _mouse.leave = nullptr;
        _mouse.move = nullptr;
        _on_resize = nullptr;
        for (auto& child : _children)
            child->dispose();
        _disposed = true;
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

}
