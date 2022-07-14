#pragma once

#include <functional>
#include <string>

#include <p3/Node.h>

namespace p3 {

class Button : public Node {
public:
    using OnClick = std::function<void()>;

    Button(std::optional<std::string> label = std::nullopt);

    StyleStrategy& style_strategy() const override;
    void render_impl(Context&, float width, float height) override;

    void set_on_click(OnClick);
    OnClick on_click() const;

    void update_content() override;

protected:
    void dispose() override;

private:
    OnClick _on_click;
};

}
