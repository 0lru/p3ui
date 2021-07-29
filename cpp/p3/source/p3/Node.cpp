/***************************************************************************//*/
  Copyright (c) 2021 Martin Rudoff

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
/******************************************************************************/

#include "Node.h"
#include "Context.h"
#include "Theme.h"
#include "log.h"
#include "convert.h"
#include "StyleDerivation.h"

#include <mutex>
#include <unordered_map>
#include <imgui.h>
#include <imgui_internal.h>

namespace p3
{

    // using default values..
    StyleStrategy Node::DefaultStyleStrategy;

    namespace id_pool
    {

        namespace
        {
            // Note: use threadlocal instead?
            std::mutex mutex;
            std::vector<std::uint64_t> pool;
            std::uint64_t max_id = 0;
            std::unordered_map<std::uint64_t, Node*> nodes;
        }

        std::uint64_t make_id(Node* node)
        {
            std::lock_guard<std::mutex> l(mutex);
            std::uint64_t id;
            if (pool.empty())
            {
                id = max_id++;
            }
            else
            {
                auto id = pool.back();
                pool.pop_back();
            }
            nodes[id] = node;
            return id;
        }

        void free(std::uint64_t id)
        {
            std::lock_guard<std::mutex> l(mutex);
            pool.push_back(id);
            nodes.erase(id);
        }

    }

    Node::Node(std::string element_name)
        : _element_name(std::move(element_name))
        , _imgui_id(id_pool::make_id(this))
        , _imgui_label("##" + std::to_string(_imgui_id))
        , _status_flag(ImGuiItemStatusFlags_None)
        , _style(std::make_shared<StyleBlock>())
    {
        _style->add_observer(this);
        _style_guard = OnScopeExit([this, style = _style]() {
            style->remove_observer(this);
        });
    }

    void Node::set_attribute(std::string const& name, std::string const& value)
    {
        static auto setter = std::unordered_map<std::string, std::function<void(Node&, std::string const&)>>{
            { "label", [](Node& node, std::string const& value) { node.set_label(value); }},
            { "width", [](Node& node, std::string const& value) { node.style()->set_width(parser::parse<FlexibleLength>(value.c_str())); }},
            { "height", [](Node& node, std::string const& value) { node.style()->set_height(parser::parse<FlexibleLength>(value.c_str())); }},
            { "direction", [](Node& node, std::string const& value) { node.style()->set_direction(parser::parse<Direction>(value.c_str())); }},
            { "align-items", [](Node& node, std::string const& value) { node.style()->set_align_items(parser::parse<Alignment>(value.c_str())); }},
            { "justify-content", [](Node& node, std::string const& value) { node.style()->set_justify_content(parser::parse<Justification>(value.c_str())); }},
        };
        auto it = setter.find(name);
        if (it == setter.end())
            throw parser::ParseError(std::format("attribute {} not found", name));
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
        ImGuiWindow* window = GImGui->CurrentWindow;
        auto const status_flag = window->DC.LastItemStatusFlags;
        if (status_flag == _status_flag)
        {
            if (_mouse.hovered && _mouse.move && Context::current().mouse_move())
                _mouse.move(MouseEvent(this));
            return;
        }
        if (status_flag & ImGuiItemStatusFlags_HoveredRect)
        {
            if (!(_status_flag & ImGuiItemStatusFlags_HoveredRect))
            {
                _mouse.hovered = true;
                if (_mouse.enter)
                    _mouse.enter(MouseEvent(this));
            }
        }
        else
        {
            if (_status_flag & ImGuiItemStatusFlags_HoveredRect)
            {
                _mouse.hovered = false;
                if (_mouse.leave)
                    _mouse.leave(MouseEvent(this));
            }
        }
        _status_flag = status_flag;
    }

    void Node::postpone(std::function<void()> f)
    {
        Context::current().postpone(std::move(f));
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
            log_verbose("-style- <{}>\"{}\" changes color to {}", _element_name, label() ? label().value() : "", to_string(_style_computation.color));
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
            log_verbose("-style- <{}>\"{}\" changes spacing to ({}, {})", _element_name, label() ? label().value() : "", spacing.x, spacing.y);
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
            log_verbose("-style- <{}>\"{}\" changes padding to ({}, {})", _element_name, label() ? label().value() : "", padding.x, padding.y);
            _style_compiled.push_back([padding{ std::move(padding) }]() mutable {
                std::swap(padding, GImGui->Style.FramePadding);
            });
        }
    }

    Node* Node::parent() const
    {
        return _parent;
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
        // clear flags
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

    void Node::set_needs_update()
    {
        _needs_update = true;
        auto it = _parent;
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

    void Node::add(std::shared_ptr<Node> node)
    {
        node->set_parent(this);
        _children.push_back(std::move(node));
        _children.back()->set_needs_restyle();
    }

    void Node::insert(std::size_t index, std::shared_ptr<Node> node)
    {
        node->set_parent(this);
        auto it = _children.begin();
        std::advance(it, index);
        (*_children.insert(it, std::move(node)))->set_needs_restyle();
    }

    void Node::remove(std::shared_ptr<Node> const& node)
    {
        node->set_parent(nullptr);
        _children.erase(std::remove_if(_children.begin(), _children.end(), [&](auto& item) {
            return item == node;
        }), _children.end());
        set_needs_update();
    }

    Node::Children const& Node::children() const
    {
        return _children;
    }

    Node::~Node()
    {
        id_pool::free(_imgui_id);
    }

    void Node::render(Context& context, float width, float height)
    {
        ImGui::GetStyle().Alpha = _disabled ? 0.2f : 1.0f;
        auto compiled_guard = _apply_style_compiled();
        auto &work_rect = GImGui->CurrentWindow->WorkRect;
        ImVec2 work_rect_max = work_rect.Min;
        work_rect_max.x += width + ImGui::GetCurrentContext()->Style.FramePadding.x;
        work_rect_max.y += height + ImGui::GetCurrentContext()->Style.FramePadding.y;
        std::swap(work_rect.Max, work_rect_max);
        render_impl(context, width, height);
        std::swap(work_rect.Max, work_rect_max);
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

    Node& Node::set_visible(bool visible)
    {
        _visible = visible;
        set_needs_restyle();
        return *this;
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
        auto& last_rect = context.CurrentWindow->DC.LastItemRect;
        _x = _global_x - last_rect.Min.x;
        _y = _global_y - last_rect.Min.y;
    }

    Node* Node::MouseEvent::source() const
    {
        return _source;
    }

    std::shared_ptr<StyleBlock> const& Node::style() const
    {
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

}
