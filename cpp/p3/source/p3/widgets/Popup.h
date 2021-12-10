#pragma once

#include <string>
#include <functional>

#include <p3/Node.h>

namespace p3
{

    class Popup : public Node
    {
    public:
        using OnClose = std::function<void()>;

        Popup();
        StyleStrategy& style_strategy() const override;
        void render_impl(Context&, float width, float height) override;
        void update_content() override;

        void set_content(std::shared_ptr<Node>);
        std::shared_ptr<Node> content();

        void set_on_close(OnClose);
        OnClose on_close() const;
        bool opened() const;

    private:
        std::shared_ptr<Node> _content;
        OnClose _on_close;
        bool _opened = false;
    };

}