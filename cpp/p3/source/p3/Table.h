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

    class Table : public Node
    {
    public:
        Table();

        // 
        // column sepcification
        class Column : public Synchronizable
        {
        public:
            std::string const& title() const { return _title; }
            void set_title(std::string title){ _title = std::move(title); }

            std::optional<Length> const& width() const { return _width; }
            void set_width(std::optional<Length> width) { _width = std::move(width); }

            float weight() const { return _weight; }
            void set_weight(float weight) { _weight = weight; }

        private:
            std::string _title;
            std::optional<Length> _width;
            float _weight = 1.0f;
        };

        void set_columns(std::vector<std::shared_ptr<Column>>);
        std::vector<std::shared_ptr<Column>> columns() const;

        //
        // a row ..
        class Row : public Node
        {
        public:
            Row() : Node("Row") {}
            void render_impl(Context&, float width, float height) override;
        };

        void render_impl(Context&, float width, float height) override;
        void update_content() override;

    private:
        std::vector<std::shared_ptr<Column>> _columns;
    };

}
