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

#include "Popup.h"

#include <iostream>

#include <imgui.h>
#include <imgui_internal.h>

namespace p3
{

    namespace
    {
        class LocalStyleStrategy : public StyleStrategy
        {
        public:
            LayoutLength const& initial_width() override
            {
                static auto initial = LayoutLength{std::nullopt, 0.f, 0.f};
                return initial;
            }
            LayoutLength const& initial_height() override
            {
                static auto initial = LayoutLength{std::nullopt, 0.f, 0.f};
                return initial;
            }
        };
        LocalStyleStrategy _style_strategy;
    }

    StyleStrategy& Popup::style_strategy() const
    {
        return _style_strategy;
    }

    Popup::Popup()
        : Node("Popup")
    {
    }

    void Popup::render_impl(Context& context, float width, float height)
    {
        if (!_opened)
        {
            ImGui::OpenPopup(imgui_label().c_str());
            _opened = true;
        }
        if (ImGui::BeginPopup(imgui_label().c_str()))
        {
            _content->render(context, this->width(0), this->height(0));
            ImGui::EndPopup();
        }
        else
        {
            _opened = false;
            if (_on_close)
                _on_close();
        }
        update_status();
    }

    void Popup::update_content()
    {
        _content->update_content();
        _automatic_width = _content->automatic_width();
        _automatic_height = _content->automatic_height();
    }

    void Popup::set_content(std::shared_ptr<Node> content)
    {
        _content = content;
    }

    std::shared_ptr<Node> Popup::content()
    {
        return _content;
    }

    void Popup::set_on_close(OnClose on_close)
    {
        _on_close = on_close;
    }

    Popup::OnClose Popup::on_close() const
    {
        return _on_close;
    }

    bool Popup::opened() const
    {
        return _opened;
    }

}
