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

namespace pugi
{
    class xml_node;
    class xml_document;
    class xml_text;
    class xml_attribute;
}

namespace p3
{
    class Node;

    class Builder
    {
    public:
        Builder();

        typedef std::shared_ptr<Node>(*NodeConstructor)();
        using Definitions = std::unordered_map<std::string, NodeConstructor>;

        void define(std::string type, NodeConstructor);
        Definitions const& definitions();

        std::shared_ptr<Node> build(std::istream&);
        std::shared_ptr<Node> build(std::string const&);

        std::shared_ptr<Node> build(pugi::xml_document const&);
        std::shared_ptr<Node> build(pugi::xml_node const&);
        void build(Node& node, pugi::xml_node const& pugi_node);

    protected:
        virtual void assign_text(Node&, std::string);
        virtual void assign_attribute(Node&, std::string const& name, std::string const& value);

    private:
        Definitions _definitions;
    };
}
