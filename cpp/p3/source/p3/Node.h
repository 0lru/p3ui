
#pragma once

#include "on_scope_exit.h"
#include "RenderBackend.h"
#include "StyleBlock.h"
#include "StyleComputation.h"
#include "StyleStrategy.h"
#include "StyleTypes.h"

#include <array>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace p3 {

class Context;
class RenderLayer;

template <typename T>
using ref = std::shared_ptr<T>;

template <typename T, typename... Args>
std::shared_ptr<T> make(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

class Node
    : public std::enable_shared_from_this<Node>,
      public StyleBlock::Observer {
public:
    virtual ~Node();

    std::string const& element_name() const;
    // this is used by the loader to apply xml attributes
    virtual void set_attribute(std::string const&, std::string const&);

    // ###### composition ##################################################

    using Children = std::vector<std::shared_ptr<Node>>;

    void set_parent(Node*);
    Node* parent() const;
    std::shared_ptr<Node> shared_parent() const;
    Children const& children() const;

    void add(std::shared_ptr<Node>);
    void insert(std::size_t, std::shared_ptr<Node>);
    void remove(std::shared_ptr<Node>);

    // #### style ##########################################################

    static StyleStrategy DefaultStyleStrategy;
    virtual StyleStrategy& style_strategy() const;

    std::shared_ptr<StyleBlock> const& style() const;

    /// do update/restyle pass for the whole tree
    virtual void update_restyle(Context& context, bool force);

    /// computed style (state after style cascade)
    StyleComputation const& style_computation() const;

    float contextual_width(float available_width) const;
    float contextual_height(float available_height) const;
    float contextual_minimum_content_width() const;
    float contextual_minimum_content_height() const;

    bool visible() const;
    void set_visible(bool);

    LayoutLength const& width() const;
    void set_width(LayoutLength);
    LayoutLength const& height() const;
    void set_height(LayoutLength);

    std::optional<Color> const& color() const;
    void set_color(std::optional<Color>);

    using Size = std::array<float, 2>;
    using OnResize = std::function<void(Size)>;

    Size size() const;
    void set_on_resize(OnResize);
    OnResize on_resize() const;

    // ##### mouse #########################################################

    class MouseEvent;
    using MouseEventHandler = std::function<void(MouseEvent)>;
    using MouseWheelHandler = std::function<void(float)>;

    void set_on_mouse_wheel(MouseWheelHandler);
    MouseWheelHandler on_mouse_wheel() const;

    void set_on_mouse_enter(MouseEventHandler);
    MouseEventHandler on_mouse_enter() const;

    void set_on_mouse_leave(MouseEventHandler);
    MouseEventHandler on_mouse_leave() const;

    void set_on_mouse_move(MouseEventHandler);
    MouseEventHandler on_mouse_move() const;

    bool hovered() const;

    // ##### render ########################################################

    virtual void render(Context&, float width, float height, bool adjust_worksrect = false);
    virtual void render(RenderBackend::RenderTarget&);
    virtual void update_content() {};

    void set_label(std::optional<std::string>);
    std::optional<std::string> const& label() const;

    std::uint64_t imgui_id() const;
    std::string const& imgui_label() const;

    void set_disabled(bool);
    bool disabled() const;

    /// force redraw of the tree
    virtual void redraw();

    static std::size_t node_count();

    void set_render_layer(std::shared_ptr<RenderLayer>);
    std::shared_ptr<RenderLayer> const& render_layer() const;

    std::shared_ptr<void> const& user_data() const;
    void set_user_data(std::shared_ptr<void>);

protected:
    std::shared_ptr<void> _user_data = nullptr;

    Node(std::string element_name);

    // validate if node valid for beeing added to this, throws..
    virtual void before_add(Node&) const;

    //
    // layers will only be created on demand for user interfaces,
    // scrolls areas, popups and child windows.
    virtual bool is_layered() const { return false; }

    virtual void render_absolute(Context&);

    /// inform that this node needs to update it's actual values
    virtual void set_needs_update();

    /// inform that this node needs to update it's computed values
    void set_needs_restyle();

    bool needs_restyle() const;
    bool needs_update() const;

    // TODO: make private
    float _automatic_width = 0.f;
    float _automatic_height = 0.f;
    Size _size = Size { 0, 0 };
    OnResize _on_resize;

    void update_status();
    // TODO: remove this
    void postpone(std::function<void()>);

    // node specific render implementation
    virtual void render_impl(Context&, float width, float height);

    [[nodiscard]] on_scope_exit _apply_style_compiled();

    virtual void dispose();

    void set_tooltip(std::shared_ptr<Node>);
    std::shared_ptr<Node> const& tooltip() const;

protected:
    virtual void push_style();
    virtual void pop_style();

private:
    std::string _element_name;
    std::optional<std::string> _class_name;
    std::optional<std::string> _label;
    std::uint64_t _imgui_id;
    std::string _imgui_label;

    std::shared_ptr<RenderLayer> _render_layer;

    std::shared_ptr<Node> _tooltip;
    Node* _parent = nullptr;
    std::vector<std::shared_ptr<Node>> _children;

    bool _visible = true; // NOTE: style..
    bool _disabled = false;
    LayoutLength _width = LayoutLength { std::nullopt, 1, 1 };
    LayoutLength _height = LayoutLength { std::nullopt, 1, 1 };
    std::optional<Color> _color = std::nullopt;

    // nodes can't be reused a.t.m. once node is removed, it's marked "disposed"
    bool _disposed = false;

    // needed for det. of the node specific imgui state
    int _status_flags;
    struct
    {
        bool tracking_enabled = false;
        bool hovered = false;
        float x, y;
        MouseEventHandler enter;
        MouseEventHandler leave;
        MouseEventHandler move;
        std::function<void(float)> wheel;
    } _mouse;

    bool _needs_update = true;
    bool _needs_restyle = true;
    void on_style_changed();
    std::optional<on_scope_exit> _style_guard;
    std::shared_ptr<StyleBlock> _style = nullptr;
    StyleComputation _style_computation;
    std::vector<std::function<void()>> _style_compiled;

    void _cascade_styles_from_parent(Context&);
    void _compile_style_computation(Context&);
};

class Node::MouseEvent {
public:
    MouseEvent(Node* source);
    Node* source() const;

    float global_x() const;
    float global_y() const;
    float x() const;
    float y() const;

private:
    Node* _source;
    float _global_x;
    float _global_y;
    float _x;
    float _y;
};

extern std::function<void(Node&)> NodeInitializer;

}
