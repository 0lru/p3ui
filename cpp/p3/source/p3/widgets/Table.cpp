#include "Table.h"
#include <p3/constant.h>
#include <p3/Context.h>

#include <imgui.h>
#include <imgui_internal.h>

namespace p3
{

    Table::Table()
        : Node("Table")
    {
        for (auto& column : _columns)
            column->release();
    }

    void Table::set_columns(std::vector<std::shared_ptr<Column>> columns)
    {
        for (auto& column : _columns)
            column->release();
        _columns = std::move(columns);
        for (auto& column : _columns)
            column->synchronize_with(*this);
    }

    std::vector<std::shared_ptr<Table::Column>> Table::columns() const
    {
        return _columns;
    }

    int Table::freezed_columns() const
    {
        return _freezed_columns;
    }

    void Table::set_freezed_columns(int freezed_columns)
    {
        _freezed_columns = freezed_columns;
    }

    int Table::freezed_rows() const
    {
        return _freezed_rows;
    }

    void Table::set_freezed_rows(int freezed_rows)
    {
        _freezed_rows = freezed_rows;
    }

    bool Table::resizeable() const
    {
        return _resizeable;
    }

    void Table::set_resizeable(bool resizeable)
    {
        _resizeable = resizeable;
    }

    bool Table::reorderable() const
    {
        return _reorderable;
    }

    void Table::set_reorderable(bool reorderable)
    {
        _reorderable = reorderable;
    }

    void Table::render_impl(Context& context, float width, float height)
    {
        ImVec2 size(width, height);
        //
        // NOTE: todo: scrollbars.. borders.. 
        ImGuiTableFlags flags = ImGuiTableFlags_RowBg
            | ImGuiTableFlags_BordersOuterH
            | ImGuiTableFlags_BordersOuterV
            | ImGuiTableFlags_BordersInnerV
            | ImGuiTableFlags_ScrollX
            | ImGuiTableFlags_ScrollY;

        if (_resizeable)
            flags |= ImGuiTableFlags_Resizable;
        if (_reorderable)
            flags |= ImGuiTableFlags_Reorderable;

        if (ImGui::BeginTable(imgui_label().c_str(), int(_columns.size()), flags, size))
        {
            ImGui::TableSetupScrollFreeze(_freezed_columns, _freezed_rows);
            if (_columns.size())
            {
                std::size_t suffix = 0;
                for (auto& column : _columns)
                {
                    ImGuiTableColumnFlags flags = ImGuiTableColumnFlags_None;
                    float width = 0.f;
                    if (column->width())
                    {
                        flags |= ImGuiTableColumnFlags_WidthFixed;
                        width = context.to_actual(column->width().value());
                    }
                    ImGui::TableSetupColumn(column->title().c_str(), flags, width);
                }
                ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
                for (int column = 0; column < _columns.size(); column++)
                {
                    ImGui::TableSetColumnIndex(column);
                    ImGui::PushID(column);
                    ImGui::TableHeader(_columns[column]->title().c_str());
                    ImGui::PopID();
                }
//                ImGui::TableHeadersRow();
            }

            for (auto& child : children())
                child->render(context, width, height);

            ImGui::EndTable();
        }
    }

    void Table::update_content()
    {

        auto const em = ImGui::GetCurrentContext()->FontSize;
        _automatic_width = _automatic_height = DefaultItemWidthEm * em;
    }

    void Table::Row::render_impl(Context& context, float width, float height)
    {
        ImGui::TableNextRow();
        for (auto& child : children())
        {
            ImGui::TableNextColumn();
            // auto s = ImGui::GetContentRegionAvail() - pad;
            child->render(context, 0, 0);
        }
    }

}
