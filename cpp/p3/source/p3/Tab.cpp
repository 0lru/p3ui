
#include "Tab.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <iostream>

namespace p3
{

    Tab::Tab()
        : Node("Tab")
    {
    }

    void Tab::render_impl(Context& context, float width, float height)
    { 
        ImVec2 size(width, height);
        ImGui::BeginChild(imgui_label().c_str(), size, false, ImGuiWindowFlags_NoScrollbar);
        ImGui::BeginTabBar((imgui_label() + "_tab").c_str());
        for (auto& child : children())
            child->render(context, width, height);
        ImGui::EndTabBar();
        ImGui::EndChild();
        update_status();
    }

    void Tab::update_content()
    {
        for (auto& child : children())
            child->update_content();
        _automatic_height = 1.f;
        _automatic_width = 1.f;
    }

    Tab::Item::Item(std::string name, std::shared_ptr<Node> node)
        : Node("TabItem")
        , _content(std::move(node))
    {
        set_label(name);
    }

    std::shared_ptr<Node> Tab::Item::content() const
    {
        return _content;
    }

    void Tab::Item::set_content(std::shared_ptr<Node> content)
    {
        if (_content)
            remove(_content);
        _content = std::move(content);
        if (_content)
            add(_content);
    }

    void Tab::Item::render(Context& context, float width, float height, bool)
    {
        if (!ImGui::BeginTabItem(label() ? label().value().c_str() : ""))
            return;
        auto compiled_guard = _apply_style_compiled();
        if (_content)
        {
            auto region = ImGui::GetContentRegionAvail();
            _content->render(context, region.x, region.y);
        }
        ImGui::EndTabItem();
    }

    void Tab::Item::update_content()
    {
        if (_content)
        {
            _content->update_content();
            _automatic_width = _content->automatic_width();
            _automatic_height = _content->automatic_height();
        }
        else
        {
            _automatic_width = _automatic_height = 0.f;
        }
    }

}
