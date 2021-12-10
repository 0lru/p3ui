#pragma once

#include <string>
#include <functional>

#include <p3/Node.h>

namespace p3
{

    class CheckBox : public Node
    {
    public:
        using OnChange = std::function<void(bool)>;

        CheckBox(std::optional<std::string> label = std::nullopt);
        
        StyleStrategy& style_strategy() const override;        
        void render_impl(Context&, float width, float height) override;

        void set_on_change(OnChange);
        OnChange on_change() const;

        bool value() const;
        void set_value(bool);

        void set_radio(bool);
        bool radio() const;

        void update_content() override;
    protected:
        void dispose();

    private:
        bool _radio = false;
        OnChange _on_change;
        bool _value = false;
    };

}