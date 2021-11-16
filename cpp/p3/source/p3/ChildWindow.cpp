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
#include "ChildWindow.h"
#include "Context.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <iostream>

namespace p3
{

    ChildWindow::ChildWindow()
        : Node("ChildWindow")
    {
    }

    void ChildWindow::render_impl(Context& context, float width, float height)
    {
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;
        ImGuiCond conditions = 0;
        if (!_moveable)
            flags |= ImGuiWindowFlags_NoMove;
        if (!_resizeable)
            flags |= ImGuiWindowFlags_NoResize;
        ImVec2 pos(
            std::holds_alternative<Percentage>(style_computation().x)
            ? width * std::get<Percentage>(style_computation().x).value / 100.f
            : context.to_actual(std::get<Length>(style_computation().x)),
            std::holds_alternative<Percentage>(style_computation().y)
            ? width * std::get<Percentage>(style_computation().y).value / 100.f
            : context.to_actual(std::get<Length>(style_computation().y)));
        ImGui::SetNextWindowPos(pos, _moveable ? ImGuiCond_Appearing : ImGuiCond_Always);
        ImVec2 size(width, height);
        ImGui::SetNextWindowSize(size, _resizeable ? ImGuiCond_Appearing : ImGuiCond_Always);
        bool open=true;
        ImGui::Begin(imgui_label().c_str(), _on_close ? &open : nullptr, flags);
        auto avail = ImGui::GetContentRegionAvail();
        if (_content)
            _content->render(context, avail.x, avail.y);
        ImGui::End();

        if (_on_close && !open)
            postpone([f=_on_close]() {
                f();
            });
        update_status();
    }

    void ChildWindow::set_content(std::shared_ptr<Node> content)
    {
        if (_content)
            Node::remove(_content);
        _content = content;
        if (_content)
            Node::add(_content);
    }

    std::shared_ptr<Node> ChildWindow::content() const
    {
        return _content;
    }

    void ChildWindow::update_content()
    {
        auto const context_ptr = ImGui::GetCurrentContext();
        auto const font_size = context_ptr->FontSize;
        auto const frame_padding = context_ptr->Style.FramePadding;
        _automatic_height = _automatic_height = DefaultItemWidthEm * font_size;
        if (_content)
        {
            _automatic_height =
                _content->automatic_height()
                + font_size
                + 2 * frame_padding.y
                + GImGui->Style.ItemInnerSpacing.y
                + GImGui->Style.WindowPadding.y * 2;
            _automatic_width = _content->automatic_width()
                + GImGui->Style.WindowPadding.x * 2;
        }
    }

    void ChildWindow::set_moveable(bool moveable)
    {
        _moveable = moveable;
    }

    bool ChildWindow::moveable() const
    {
        return _moveable;
    }

    void ChildWindow::set_resizeable(bool resizeable)
    {
        _resizeable = resizeable;
    }

    bool ChildWindow::resizeable() const
    {
        return _resizeable;
    }

    void ChildWindow::set_on_close(OnClose on_close)
    {
        _on_close = on_close;
    }

    ChildWindow::OnClose ChildWindow::on_close() const
    {
        return _on_close;
    }

}