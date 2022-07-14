#pragma once

#include <functional>
#include <string>

#include <p3/Node.h>
#include <p3/RenderLayer.h>

namespace p3 {

class ScrollArea : public Node {
public:
    using ContentRegion = std::array<double, 5>;
    using OnContentRegionChanged = std::function<void(ContentRegion)>;
    using Callback = std::function<void()>;

    ScrollArea();

    void render_impl(Context&, float width, float height) override;

    void set_content(std::shared_ptr<Node>);
    std::shared_ptr<Node> content() const;

    void update_content() override;

    ContentRegion const& content_region() const;
    OnContentRegionChanged on_content_region_changed() const;
    void set_on_content_region_changed(OnContentRegionChanged);

    void set_horizontal_scroll_enabled(bool);
    bool horizontal_scroll_enabled() const;

    void set_horizontal_scroll_autohide(bool);
    bool horizontal_scroll_autohide() const;

    void set_vertical_scroll_autohide(bool);
    bool vertical_scroll_autohide() const;

    bool is_layered() const final override { return true; }

    float scroll_x() const;
    float scroll_x_max() const;
    void set_scroll_x(float);

    float scroll_y() const;
    float scroll_y_max() const;
    void set_scroll_y(float);

    void set_mouse_scroll_enabled(bool);
    bool mouse_scroll_enabled() const;

    std::optional<std::array<float, 2>> const& content_size() const;
    void set_content_size(std::optional<std::array<float, 2>>);

    std::optional<Length2> const& padding() const;
    void set_padding(std::optional<Length2> padding);

protected:
    virtual void push_style() override;
    virtual void pop_style() override;

private:
    std::optional<std::array<float, 2>> _content_size;
    std::optional<float> _set_scroll_x;
    float _scroll_x_max = 0.;

    std::optional<float> _set_scroll_y;
    float _scroll_y_max = 0.;

    std::shared_ptr<Node> _content;
    ContentRegion _content_region;
    OnContentRegionChanged _on_content_region_changed;
    bool _horizontal_scroll_enabled = true;
    bool _horizontal_scroll_autohide = true;
    bool _vertical_scroll_autohide = true;
    bool _mouse_scroll_enabled = true;
    std::optional<Length2> _padding = std::nullopt;
};

}