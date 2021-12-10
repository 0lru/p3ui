#pragma once

#include <string>
#include <optional>
#include <functional>

#include "Node.h"

namespace p3
{

    class Text : public Node
    {
    public:
        using Callback = std::function<void()>;

        Text(std::string value="", std::optional<std::string> label=std::optional<std::string>());

        StyleStrategy& style_strategy() const override;        
        void render_impl(Context&, float width, float height) override;

        void set_value(std::string);
        std::string const& value() const;

        void update_content() override;

    private:
        std::string _value;
        Callback _callback;
    };

}
