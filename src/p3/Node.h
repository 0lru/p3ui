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

#pragma once

#include "FlexibleTypes.h"
#include "StyleBlock.h"
#include "StyleStrategy.h"
#include "StyleComputation.h"
#include "OnScopeExit.h"

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

    class Node 
        : public std::enable_shared_from_this<Node>
        , public StyleBlock::Observer
    {
    public:
        virtual ~Node();

        // ###### composition ##################################################
        
        using Children = std::vector<std::shared_ptr<Node>>;

        void set_parent(Node*);
        Node* parent() const;
        Children const& children() const;

        void add(std::shared_ptr<Node>);
        void insert(std::size_t, std::shared_ptr<Node>);
        void remove(std::shared_ptr<Node> const&);

        // #### style ##########################################################
        
        static StyleStrategy DefaultStyleStrategy;
        virtual StyleStrategy& style_strategy() const;

        std::shared_ptr<StyleBlock> const& style() const;
        void set_style(std::shared_ptr<StyleBlock>);

        /// inform that this node needs to update it's actual values
        void set_needs_update();

        /// inform that this node needs to update it's computed values
        void set_needs_restyle();

        /// wheter needs to update it's actual values
        bool needs_update() const;

        /// wheter needs to update it's computed values
        bool needs_restyle() const;

        /// do update/restyle pass for the whole tree
        virtual void update_restyle(Context& context, bool force);

        /// computed style (state after style cascade)
        StyleComputation const& style_computation() const;

        float width(float content) const;
        float height(float content) const;

        float automatic_width() const;
        float automatic_height() const;

        Node& set_visible(bool);
        bool visible() const;

        // ##### mouse #########################################################
        
        class MouseEvent;
        using MouseEventHandler = std::function<void(MouseEvent)>;
        void set_mouse_tracking_enabled(bool);
        bool mouse_tracking_enabled() const;

        void set_on_mouse_enter(MouseEventHandler handler);
        MouseEventHandler on_mouse_enter() const;

        void set_on_mouse_leave(MouseEventHandler handler);
        MouseEventHandler on_mouse_leave() const;

        void set_on_mouse_move(MouseEventHandler handler);
        MouseEventHandler on_mouse_move() const;

        bool hovered() const;

        // ##### render ########################################################

        virtual void render(float width, float height);
        virtual void update_content() {};

        void set_label(std::optional<std::string>);
        std::optional<std::string> const& label() const;

        std::uint64_t imgui_id() const;
        std::string const& imgui_label() const;

    protected:
        Node(std::string element_name);

        // TODO: make private
        float _automatic_width = 0.f;
        float _automatic_height = 0.f;
        void update_status();

        // node specific render implementation
        virtual void render_impl(float width, float height) {};

        // dom must not be modified during an update traversal of imgui, user callbacks need to be "postponed"
        void postpone(std::function<void()>);

        [[nodiscard]] OnScopeExit _apply_style_compiled();

    private:
        std::string _element_name;
        std::optional<std::string> _class_name;
        std::optional<std::string> _label;
        std::uint64_t _imgui_id;
        std::string _imgui_label;

        Node* _parent = nullptr;
        std::vector<std::shared_ptr<Node>> _children;

        bool _visible = true; // NOTE: style..

        // needed for det. of the node specific imgui state
        int _status_flag;
        struct
        {
            bool tracking_enabled = false;
            bool hovered = false;
            MouseEventHandler enter;
            MouseEventHandler leave;
            MouseEventHandler move;
        } _mouse;

        // 
        // styling... no KISS :-/
        bool _needs_update = true;
        bool _needs_restyle = true;
        void on_style_changed();
        std::optional<OnScopeExit> _style_guard;
        std::shared_ptr<StyleBlock> _style = nullptr;
        StyleComputation _style_computation;
        std::vector<std::function<void()>> _style_compiled;
        
        void _perform_style_cascade(Context&);
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
