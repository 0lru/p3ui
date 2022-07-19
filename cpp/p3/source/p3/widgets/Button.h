#pragma once

#include <functional>
#include <string>

#include <p3/Node.h>

namespace p3 {

class Button : public Node {
public:
    using OnClick = std::function<void()>;

    Button(std::optional<std::string> label = std::nullopt);

    void render_impl(Context&, float width, float height) override;

    void set_on_click(OnClick);
    OnClick on_click() const;

    std::optional<Color> const& background_color() const;
    void set_background_color(std::optional<Color>);

    void update_content() override;

private:
    OnClick _on_click;
    std::optional<Color> _background_color = std::nullopt;
};

}
