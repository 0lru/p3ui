#pragma once

#include <string>
#include <functional>

#include <p3/Node.h>
#include <p3/RenderLayer.h>

namespace p3
{

    class ScrollArea : public Node
    {
    public:
        using Callback = std::function<void()>;

        ScrollArea();

        void render_impl(Context&, float width, float height) override;

        void set_content(std::shared_ptr<Node>);
        std::shared_ptr<Node> const& content() const;

        void update_content() override;

        void set_horizontal_scroll_enabled(bool);
        bool horizontal_scroll_enabled() const;

        void set_horizontal_scroll_autohide(bool);
        bool horizontal_scroll_autohide() const;

        void set_vertical_scroll_autohide(bool);
        bool vertical_scroll_autohide() const;

        bool is_layered() const final override { return true; }

    private:
        std::shared_ptr<Node> _content;
        bool _horizontal_scroll_enabled = true;
        bool _horizontal_scroll_autohide = true;
        bool _vertical_scroll_autohide = true;
    };

}