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

#include <istream>
#include <memory>
#include <unordered_map>
#include <functional>

namespace pugi { class xml_node; }

namespace p3
{

    class Node;

    class Factory
    {
    public:
        using Bind = std::function<void(Node&, std::string& name, std::string& value)>;

        typedef std::shared_ptr<Node>(*NodeConstructor)();
        using NodeConstructorMap = std::unordered_map<std::string, NodeConstructor>;

        static void add_node_constructor(std::string type, NodeConstructor);
        static NodeConstructorMap const& node_constructor_map();
        
        static std::shared_ptr<Node> load(std::istream&, Bind=nullptr);
        static std::shared_ptr<Node> parse(std::string const&, Bind=nullptr);

    private:
        static std::shared_ptr<Node> parse_node(pugi::xml_node const&, Bind=nullptr);
        static NodeConstructorMap _node_constructors;
    };

}
