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

#pragma once

#include <string>
#include <functional>

#include "Node.h"

namespace p3
{
   
    class MenuItem : public Node
    {
    public:
        using OnClick = std::function<void()>;
        using OnChange = std::function<void(bool)>;

        MenuItem(std::string label="");
        void update_content() override;
        void render_impl(Context&, float width, float height) override;

        void set_enabled(bool);
        bool enabled() const;

        void set_shortcut(std::optional<std::string>);
        std::optional<std::string> const& shortcut() const;

        void set_checked(bool);
        bool checked() const;

        void set_checkable(bool);
        bool checkable() const;

        void set_on_click(OnClick);
        OnClick on_click() const;

        void set_on_change(OnChange);
        OnChange on_change() const;

    private:
        bool _enabled = true;
        bool _checkable = false;
        bool _checked = false;
        std::optional<std::string> _shortcut = std::string();
        OnClick _on_click;
        OnChange _on_change;
    };

}
