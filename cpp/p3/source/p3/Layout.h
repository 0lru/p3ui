
#pragma once

#include <string>

#include "Node.h"
#include <imgui.h>

namespace p3 {

class Layout : public Node {
public:
    Layout();

    void update_content() override;
    void render_impl(Context&, float width, float height) override;

    Direction const& direction() const;
    void set_direction(Direction);

    Justification const& justify_content() const;
    void set_justify_content(Justification);

    Alignment const& align_items() const;
    void set_align_items(Alignment);

    std::optional<Length> const& spacing() const;
    void set_spacing(std::optional<Length>);

    std::optional<Length2> const& padding() const;
    void set_padding(std::optional<Length2>);

    std::optional<Color> const& background_color() const;
    void set_background_color(std::optional<Color>);

private:
    Direction _direction = Direction::Vertical;
    Justification _justify_content = Justification::SpaceBetween;
    Alignment _align_items = Alignment::Stretch;
    std::optional<Length> _spacing = std::nullopt;
    std::optional<Length2> _padding = Length2 { .25 | em, .25 | em };
    std::optional<Color> _background_color = std::nullopt;
};

}
