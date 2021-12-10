
#pragma once

#include <string>
#include <functional>

#include <p3/Node.h>

namespace p3
{
   
    class MenuItem : public Node
    {
    public:
        using OnClick = std::function<void()>;
        using OnChange = std::function<void(bool)>;

        MenuItem(std::string label="");
        void update_content() override;
        void render_impl(Context&, float width, float height) override;

        void set_enabled(bool);
        bool enabled() const;

        void set_shortcut(std::optional<std::string>);
        std::optional<std::string> const& shortcut() const;

        void set_value(bool);
        bool value() const;

        void set_checkable(bool);
        bool checkable() const;

        void set_on_click(OnClick);
        OnClick on_click() const;

        void set_on_change(OnChange);
        OnChange on_change() const;
    protected:
        void dispose() override;

    private:
        bool _enabled = true;
        bool _checkable = false;
        bool _value = false;
        std::optional<std::string> _shortcut = std::string();
        OnClick _on_click;
        OnChange _on_change;
    };

}
