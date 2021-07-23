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
#include <optional>

#include "Node.h"

namespace p3
{

    class ComboBox : public Node
    {
    public:
        using OnChange = std::function<void(int)>;

        ComboBox();

        StyleStrategy& style_strategy() const override;
        void render_impl(Context&, float width, float height) override;
        void update_content() override;

        void set_options(std::vector<std::string>);
        std::vector<std::string> const& options() const;

        void set_on_change(OnChange);
        OnChange on_change() const;

        int selected_index() const;
        void set_selected_index(int);

        std::string const& hint() const;
        void set_hint(std::string);

    private:
        int _selected_index = -1;
        std::string _hint;
        std::vector<std::string> _options;
        OnChange _on_change;
    };

}