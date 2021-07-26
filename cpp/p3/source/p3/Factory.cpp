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

#define NOMINMAX

#include "Factory.h"
#include "Node.h"

#include "log.h"

#include "Button.h"
#include "Text.h"

#include "CheckBox.h"
#include "ChildWindow.h"
#include "Collapsible.h"
#include "ColorEdit.h"
#include "ComboBox.h"
#include "Flexible.h"
#include "Image.h"
#include "InputScalar.h"
#include "InputText.h"
#include "Menu.h"
#include "MenuBar.h"
#include "MenuItem.h"
#include "Plot.h"
#include "Popup.h"
#include "ProgressBar.h"
#include "ScrollArea.h"
#include "Slider.h"
#include "Tab.h"
#include "Text.h"
#include "UserInterface.h"
#include "DataSuffix.h"

#include <pugixml.hpp>

#include <iostream>
#include <list>
#include <regex>


namespace p3
{
    namespace
    {
        template<typename T>
        struct add_slider
        {
            void operator()(Factory::NodeConstructorMap& map)
            {
                auto name = "Slider" + DataSuffix<T>;
                map[std::move(name)] = []() -> std::shared_ptr<Node> { return std::make_shared<Slider<T>>(); };
            }
        };

        template<typename T>
        struct add_input
        {
            void operator()(Factory::NodeConstructorMap& map)
            {
                auto name = "Input" + DataSuffix<T>;
                map[std::move(name)] = []() -> std::shared_ptr<Node> { return std::make_shared<InputScalar<T>>(); };
            }
        };

    }

    Factory::NodeConstructorMap Factory::_node_constructors = []() {
        Factory::NodeConstructorMap map;
        map["Button"] = []() -> std::shared_ptr<Node> { return std::make_shared<Button>(); };
        map["CheckBox"] = []() -> std::shared_ptr<Node> { return std::make_shared<CheckBox>(); };
        map["ChildWindow"] = []() -> std::shared_ptr<Node> { return std::make_shared<ChildWindow>(); };
        map["Collapsible"] = []() -> std::shared_ptr<Node> { return std::make_shared<Collapsible>(); };
        map["ColorEdit"] = []() -> std::shared_ptr<Node> { return std::make_shared<ColorEdit>(); };
        map["ComboBox"] = []() -> std::shared_ptr<Node> { return std::make_shared<ComboBox>(); };
        map["Flexible"] = []() -> std::shared_ptr<Node> { return std::make_shared<Flexible>(); };
        map["Image"] = []() -> std::shared_ptr<Node> { return std::make_shared<Image>(); };
        p3::invoke_for_all_data_types<add_input>(map);
        map["InputText"] = []() -> std::shared_ptr<Node> { return std::make_shared<InputText>(); };
        map["Menu"] = []() -> std::shared_ptr<Node> { return std::make_shared<Menu>(); };
        map["MenuBar"] = []() -> std::shared_ptr<Node> { return std::make_shared<MenuBar>(); };
        map["MenuItem"] = []() -> std::shared_ptr<Node> { return std::make_shared<MenuItem>(); };
        map["Plot"] = []() -> std::shared_ptr<Node> { return std::make_shared<Plot>(); };
        map["Popup"] = []() -> std::shared_ptr<Node> { return std::make_shared<Popup>(); };
        map["ProgressBar"] = []() -> std::shared_ptr<Node> { return std::make_shared<ProgressBar>(); };
        map["ScrollArea"] = []() -> std::shared_ptr<Node> { return std::make_shared<ScrollArea>(); };
        p3::invoke_for_all_data_types<add_slider>(map);
        map["Tab"] = []() -> std::shared_ptr<Node> { return std::make_shared<Tab>(); };
        map["TabItem"] = []() -> std::shared_ptr<Node> { return std::make_shared<Tab::Item>(); };
        map["Text"] = []() -> std::shared_ptr<Node> { return std::make_shared<Text>(""); };
        map["UserInterface"] = []() -> std::shared_ptr<Node> { return std::make_shared<UserInterface>(); };
        return map;
    }();

    std::shared_ptr<Node> Factory::load(std::istream& stream, Bind bind)
    {
        std::string buffer((std::istreambuf_iterator<char>(stream)),
            std::istreambuf_iterator<char>());
        return parse(buffer, bind);
    }

    std::shared_ptr<Node> Factory::parse_node(pugi::xml_node const& pugi_node, Bind bind)
    {
        if (pugi_node.type() == pugi::node_pcdata)
        {
            return std::make_shared<Text>(pugi_node.text().as_string());
        }
        else if (pugi_node.type() == pugi::node_element)
        {
            std::unordered_map<std::string, std::string> attributes;
            auto pair = Factory::node_constructor_map().find(pugi_node.name());
            if (pair == Factory::node_constructor_map().end())
                throw std::runtime_error(std::format("failed to load type {}", pugi_node.name()));
            auto node = pair->second();
            for (auto& attribute : pugi_node.attributes())
            {
                static std::regex e(R"($\{([\w\d-]+)\})");
                std::cmatch match;
                if (std::regex_match(attribute.value(), match, e))
                {
                    if (!bind)
                        continue;
                    std::string name(attribute.name());
                    std::string value(match[1].str());
                    bind(*node, name, value);
                }
                else
                    node->set_attribute(attribute.name(), attribute.value());
            }
            for (pugi::xml_node& child : pugi_node)
            {
                node->add(parse_node(child, bind));
            }
            return node;
        }
        return nullptr;
    }

    std::shared_ptr<Node> Factory::parse(std::string const& buffer, Bind bind)
    {
        pugi::xml_document doc;
        pugi::xml_parse_result result =
            doc.load_string(buffer.c_str(),
                pugi::parse_ws_pcdata);
        return parse_node(*doc.children().begin(), bind);
    }

    void Factory::add_node_constructor(std::string type, NodeConstructor constructor)
    {
        _node_constructors[type] = constructor;
    }

    Factory::NodeConstructorMap const& Factory::node_constructor_map()
    {
        return _node_constructors;
    }

}
