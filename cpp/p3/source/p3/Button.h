
#pragma once

#include <string>
#include <functional>

#include "Node.h"

namespace p3
{

    class Button : public Node
    {
    public:
        using OnClick = std::function<void()>;

        Button(std::optional<std::string> label = std::nullopt);

        StyleStrategy& style_strategy() const override;
        void render_impl(Context&, float width, float height) override;

        void set_on_click(OnClick);
        OnClick on_click() const;

        void update_content() override;

        // this is used by the loader to apply xml attributes
        virtual void set_attribute(std::string const&, std::string const&) override;
    protected:
        void dispose() override;

    private:
        OnClick _on_click;
    };

}
