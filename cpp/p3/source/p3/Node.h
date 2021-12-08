
#pragma once

#include "OnScopeExit.h"
#include "StyleTypes.h"
#include "StyleBlock.h"
#include "StyleStrategy.h"
#include "StyleComputation.h"
#include "Synchronizable.h"
#include "TaskQueue.h"


#include <optional>
#include <memory>
#include <vector>
#include <array>
#include <string>
#include <cstdint>
#include <functional>

namespace p3
{

    class Context;

    template<typename T>
    using ref = std::shared_ptr<T>;

    template<typename T, typename ... Args>
    std::shared_ptr<T> make(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    /**
    * for synchronization/multi-threading, a shared mutex is used. the
    * mutex is assigned to the one of the parent.
    * 
    * node are state-descriptions. concerning styling and sizing, the
    * node can be in the state of
    * - needs restyling (needs to perform a style cascade)
    * - needs update (some sizing changed)
    * - needs a redraw
    * or it's "clean", which means, that it is in sync with the visual
    * appearance.
    **/
    class Node
        : public std::enable_shared_from_this<Node>
        , public StyleBlock::Observer
        , public Synchronizable
    {
    public:
        virtual ~Node();

        virtual void synchronize_with(Synchronizable&) override;
        virtual void release() override;

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

        float width(float content) const;
        float height(float content) const;

        float automatic_width() const;
        float automatic_height() const;

        void set_visible(bool);
        bool visible() const;

        using Size = std::array<float, 2>;
        using OnResize = std::function<void(Size)>;

        Size size() const;
        void set_on_resize(OnResize);
        OnResize on_resize() const;
        // ##### mouse #########################################################

        class MouseEvent;
        using MouseEventHandler = std::function<void(MouseEvent)>;

        void set_on_mouse_enter(MouseEventHandler handler);
        MouseEventHandler on_mouse_enter() const;

        void set_on_mouse_leave(MouseEventHandler handler);
        MouseEventHandler on_mouse_leave() const;

        void set_on_mouse_move(MouseEventHandler handler);
        MouseEventHandler on_mouse_move() const;

        bool hovered() const;

        // ##### render ########################################################

        virtual void render(Context&, float width, float height, bool adjust_worksrect=false);
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

    protected:
        Node(std::string element_name);

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
        Size _size = Size{ 0, 0 };
        OnResize _on_resize;

        void update_status();
        // TODO: remove this
        void postpone(std::function<void()>);

        // node specific render implementation
        virtual void render_impl(Context&, float width, float height) {};

        [[nodiscard]] OnScopeExit _apply_style_compiled();

        virtual void dispose();

        void set_tooltip(std::shared_ptr<Node>);
        std::shared_ptr<Node>const &tooltip() const;

    private:
        std::string _element_name;
        std::optional<std::string> _class_name;
        std::optional<std::string> _label;
        std::uint64_t _imgui_id;
        std::string _imgui_label;

        std::shared_ptr<Node> _tooltip;
        Node* _parent = nullptr;
        std::vector<std::shared_ptr<Node>> _children;

        bool _visible = true; // NOTE: style..
        bool _disabled = false;

        // nodes can't be reused a.t.m. once node is removed, it's marked "disposed"
        bool _disposed = false;

        // needed for det. of the node specific imgui state
        int _status_flags;
        struct
        {
            bool tracking_enabled = false;
            bool hovered = false;
            MouseEventHandler enter;
            MouseEventHandler leave;
            MouseEventHandler move;
        } _mouse;

        bool _needs_update = true;
        bool _needs_restyle = true;
        void on_style_changed();
        std::optional<OnScopeExit> _style_guard;
        std::shared_ptr<StyleBlock> _style = nullptr;
        StyleComputation _style_computation;
        std::vector<std::function<void()>> _style_compiled;

        void _cascade_styles_from_parent(Context&);
        void _compile_style_computation(Context&);
    };

    class Node::MouseEvent
    {
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

}
