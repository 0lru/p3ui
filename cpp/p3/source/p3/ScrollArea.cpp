#include "constant.h"
#include "log.h"
#include "ScrollArea.h"
#include "Context.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <iostream>

namespace p3
{

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

        ImVec2 size(width, height);
        ImGui::BeginChild(imgui_label().c_str(), size, true, flags);
        render_layer()->init_frame(context);
        if (_content)
        {
            auto available = ImGui::GetContentRegionAvail();
            _content->render(context, _content->width(available.x), _content->height(available.y));
        }
        render_layer()->finish_frame(*this, context);
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

}