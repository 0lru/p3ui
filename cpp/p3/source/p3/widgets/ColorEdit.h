#pragma once

#include <functional>
#include <string>

#include <p3/color.h>
#include <p3/Node.h>

namespace p3 {

class ColorEdit : public Node {
public:
    using OnChange = std::function<void(Color)>;

    ColorEdit();

    void render_impl(Context&, float width, float height) override;
    void update_content() override;

    void set_on_change(OnChange);
    OnChange on_change() const;

    void set_value(Color);
    Color const& value() const;

protected:
    void dispose() override;

private:
    float _value[4];
    Color _color;
    OnChange _on_change;
};

}
