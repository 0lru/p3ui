#include "ScrollArea.h"

#include <p3/Context.h>
#include <p3/constant.h>
#include <p3/log.h>

#include <imgui.h>
#include <imgui_internal.h>

#include <iostream>

namespace p3 {

ScrollArea::ScrollArea()
    : Node("ScrollArea")
{
    set_render_layer(std::make_shared<RenderLayer>());
}

void ScrollArea::render_impl(Context& context, float width, float height)
{
    ImGuiWindowFlags flags = 0;
    if (_horizontal_scroll_enabled)
        flags |= ImGuiWindowFlags_HorizontalScrollbar;
    if (!_horizontal_scroll_autohide)
        flags |= ImGuiWindowFlags_AlwaysHorizontalScrollbar;
    if (!_vertical_scroll_autohide)
        flags |= ImGuiWindowFlags_AlwaysVerticalScrollbar;

    auto& style = ImGui::GetStyle();
    style.WindowPadding = style.FramePadding;

    ImVec2 size(width, height);
    ImGui::BeginChild(imgui_label().c_str(), size, true, flags);
    // auto content_min = ImGui::GetWindowContentRegionMin();
    // auto content_max = ImGui::GetWindowContentRegionMax();
    auto clip_rect = ImGui::GetCurrentWindow()->ClipRect;
    auto& window = *ImGui::GetCurrentWindow();
    auto content_width = width - window.WindowBorderSize * 2.f - window.WindowPadding.x * 2.f;
    auto content_height = height - window.WindowBorderSize * 2.f - window.WindowPadding.y * 2.f;
    if (_vertical_scroll_autohide == false)
        content_height -= style.ScrollbarSize;
    if (_horizontal_scroll_autohide == false)
        content_width -= style.ScrollbarSize;
    auto content_region = ContentRegion {
        ImGui::GetScrollX(),
        ImGui::GetScrollY(),
        content_width,
        content_height,
        style.ScrollbarSize
    };
    if (content_region != _content_region) {
        _content_region = std::move(content_region);
        if (_on_content_region_changed)
            postpone([f = _on_content_region_changed, rect = _content_region]() {
                f(rect);
            });
    }
    render_layer()->push_to(context);
    if (_content) {
        auto available = ImGui::GetContentRegionAvail();
        _content->render(context, _content->width(available.x), _content->height(available.y));
    }
    render_layer()->pop_from_context_and_render(context, *this);
    ImGui::EndChild();
    update_status();
}

void ScrollArea::set_content(std::shared_ptr<Node> content)
{
    if (_content)
        Node::remove(_content);
    _content = std::move(content);
    if (_content)
        Node::add(_content);
}

std::shared_ptr<Node> const& ScrollArea::content() const
{
    return _content;
}

void ScrollArea::update_content()
{
    auto const context_ptr = ImGui::GetCurrentContext();
    auto const font_size = context_ptr->FontSize;
    _automatic_width = _automatic_height = DefaultItemWidthEm * font_size;
}

ScrollArea::ContentRegion const& ScrollArea::content_region() const
{
    return _content_region;
}

ScrollArea::OnContentRegionChanged ScrollArea::on_content_region_changed() const
{
    return _on_content_region_changed;
}

void ScrollArea::set_on_content_region_changed(OnContentRegionChanged on_content_region_changed)
{
    _on_content_region_changed = on_content_region_changed;
}

void ScrollArea::set_horizontal_scroll_enabled(bool value)
{
    _horizontal_scroll_enabled = value;
}

bool ScrollArea::horizontal_scroll_enabled() const
{
    return _horizontal_scroll_enabled;
}

void ScrollArea::set_horizontal_scroll_autohide(bool value)
{
    _horizontal_scroll_autohide = value;
}

bool ScrollArea::horizontal_scroll_autohide() const
{
    return _horizontal_scroll_autohide;
}

void ScrollArea::set_vertical_scroll_autohide(bool value)
{
    _vertical_scroll_autohide = value;
}

bool ScrollArea::vertical_scroll_autohide() const
{
    return _vertical_scroll_autohide;
}

float ScrollArea::scroll_x() const
{
    return _scroll_x;
}

float ScrollArea::scroll_x_max() const
{
    return _scroll_x_max;
}

void ScrollArea::set_scroll_x(float scroll_x)
{
    _scroll_x = scroll_x;
}

float ScrollArea::scroll_y() const
{
    return _scroll_y;
}

float ScrollArea::scroll_y_max() const
{
    return _scroll_y_max;
}

void ScrollArea::set_scroll_y(float scroll_y)
{
    _scroll_y = _scroll_y;
}

}