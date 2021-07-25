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

#include "Loader.h"
#include "Node.h"

#include <pugixml.hpp>

#include <iostream>
#include <list>

#include "log.h"

#include "Button.h"

namespace p3
{

    Loader::NodeConstructorMap Loader::_node_constructors = []() {
        std::unordered_map<std::string, Loader::NodeConstructor> map;
        map["Button"] = []() -> std::shared_ptr<Node> { return std::make_shared<Button>(); };
        return map;
    }();

    std::shared_ptr<Node> Loader::load(std::istream& stream)
    {
        std::string buffer((std::istreambuf_iterator<char>(stream)),
            std::istreambuf_iterator<char>());
        return load(buffer);
    }

    namespace
    {
        std::shared_ptr<Node> to_node(pugi::xml_node const& pugi_node)
        {
            if (pugi_node.type() == pugi::node_pcdata)
            {
                //                auto node = Node::create("#text");
                  //              node->set_text_content(pugi_node.text().as_string());
                  //              return node;
            }
            else if (pugi_node.type() == pugi::node_element)
            {
                std::unordered_map<std::string, std::string> attributes;
                auto pair = Loader::node_constructor_map().find(pugi_node.name());
                if (pair == Loader::node_constructor_map().end())
                    throw std::runtime_error(std::format("failed to load type {}", pugi_node.name()));
                auto node = pair->second();
                for (auto& attribute : pugi_node.attributes())
                    node->set_attribute(attribute.name(), attribute.value());
                for (pugi::xml_node& child : pugi_node)
                    ;//            node->add(to_node(child));
                return node;
            }
            return nullptr;
        }

    }

    std::shared_ptr<Node> Loader::load(std::string const& buffer)
    {
        pugi::xml_document doc;
        pugi::xml_parse_result result = 
            doc.load_string(buffer.c_str(),
            pugi::parse_ws_pcdata);
        return to_node(*doc.children().begin());
    }

    void Loader::add_node_constructor(std::string type, NodeConstructor constructor)
    {
        _node_constructors[type] = constructor;
    }

    Loader::NodeConstructorMap const& Loader::node_constructor_map()
    {
        return _node_constructors;
    }

}
