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

    class Tab : public Node
    {
    public:
        class Item;
        using Callback = std::function<void()>;

        Tab();
        void render_impl(float width, float height) override;
        void update_content() override;

        void add(std::shared_ptr<Item>);
        void remove(std::shared_ptr<Item>);
        std::shared_ptr<Item> operator[](std::size_t) const;

    private:
        std::vector<std::shared_ptr<Item>> _items;
    };

    class Tab::Item
    {
    public:
        Item(std::string name, std::shared_ptr<Node> = nullptr);

        std::string const& name() const;
        void set_name(std::string);

        std::shared_ptr<Node> content() const;
        void set_content(std::shared_ptr<Node>);

        void set_tab(Tab*);
        Tab* tab() const;

    private:
        Tab* _tab = nullptr;
        std::string _name;
        std::shared_ptr<Node> _content;
    };


}
