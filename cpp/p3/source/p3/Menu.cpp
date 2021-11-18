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

#include "Menu.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace p3
{

    Menu::Menu(std::string label)
        : Node("Menu")
    {
        set_label(std::move(label));
    }

    void Menu::update_content()
    {
        _automatic_width = _automatic_height = 0.f;
    }

    void Menu::render_impl(Context& context, float width, float height)
    {
        
        if (ImGui::BeginMenu(imgui_label().c_str()))
        {
            if (!_opened)
            {
                _opened = true;
                if(_on_open)
                    postpone(_on_open);
            }
            for (auto& node : children())
                node->render(context, width, height);
            ImGui::EndMenu();
        }
        else if(_opened)
        {
            _opened = false;
            if(_on_close)
                postpone(_on_close);
        }
        update_status();
    }

    void Menu::set_on_open(OnOpen on_open)
    {
        _on_open = on_open;
    }

    Menu::OnOpen Menu::on_open() const
    {
        return _on_open;
    }

    void Menu::set_on_close(OnClose on_close)
    {
        _on_close = on_close;
    }

    Menu::OnClose Menu::on_close() const
    {
        return _on_close;
    }

    void Menu::dispose()
    {
        _on_open = nullptr;
        _on_close = nullptr;
        Node::dispose();
    }

}
