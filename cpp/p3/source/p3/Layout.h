
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

    Length2 const& spacing() const;
    void set_spacing(Length2);

    Length2 const& padding() const;
    void set_padding(Length2);

private:
    Direction _direction = Direction::Horizontal;
    Justification _justify_content = Justification::SpaceBetween;
    Alignment _align_items = Alignment::Stretch;
    Length2 _spacing = Length2 { 0 | px, 0 | px };
    Length2 _padding = Length2 { .25 | em, .25 | em };
};

}
