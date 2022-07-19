#include "spacer.h"

namespace p3 {

Spacer::Spacer(std::optional<std::string> label)
    : Node("Spacer")
{
    set_width(LayoutLength { std::nullopt, 0.f, 0.f });
    set_height(LayoutLength { std::nullopt, 0.f, 0.f });
    set_label(std::move(label));
}

void Spacer::render_impl(Context& context, float width, float height)
{
    render_absolute(context);
}

void Spacer::update_content()
{
    _automatic_width = _automatic_height = 0.f;
}

}
