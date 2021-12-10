
#pragma once

#include <string>
#include <functional>

#include "Node.h"

namespace p3
{

    class ChildWindow : public Node
    {
    public:
        using OnClose = std::function<void()>;

        ChildWindow();
        virtual bool is_layered() const final override { return true; }

        void render_impl(Context&, float width, float height) override;

        void set_content(std::shared_ptr<Node>);
        std::shared_ptr<Node> content() const;

        void update_content() override;

        void set_moveable(bool);
        bool moveable() const;

        void set_resizeable(bool);
        bool resizeable() const;

        void set_on_close(OnClose);
        OnClose on_close() const;

    protected:
        void dispose();

    private:
        std::shared_ptr<Node> _content;
        bool _collapsed;
        bool _moveable=true;
        bool _resizeable=false;
        OnClose _on_close = nullptr;
    };

}
