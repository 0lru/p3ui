/***************************************************************************//*/
  Copyright (c) 2021 Martin Rudoff

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
/******************************************************************************/

#include "constant.h"
#include "Table.h"
#include "Context.h"

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
                    auto infix = std::to_string(imgui_id());
                    auto label = column->title() + "##" + infix + "_" + std::to_string(suffix++);
                    ImGuiTableColumnFlags flags = ImGuiTableColumnFlags_None;
                    float width = 0.f;
                    if (column->width())
                    {
                        flags |= ImGuiTableColumnFlags_WidthFixed;
                        width = context.to_actual(column->width().value());
                    }
                    ImGui::TableSetupColumn(label.c_str(), flags, width);
                }
                ImGui::TableHeadersRow();
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
