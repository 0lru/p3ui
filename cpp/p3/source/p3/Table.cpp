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

        
        ImGui::BeginTable(imgui_label().c_str(), 3, flags, size);
        if (_columns.size())
        {
            std::size_t suffix=0;
            for (auto& column : _columns)
            {
                auto infix = std::to_string(imgui_id());
                auto label = column->title() + "##" + infix + "_" + std::to_string(suffix++);
                ImGui::TableSetupColumn(label.c_str(), ImGuiTableColumnFlags_WidthFixed, 200.f);
            }
            ImGui::TableHeadersRow();
        }

        for (auto& child : children())
            child->render(context, width, height);

        ImGui::EndTable();
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
