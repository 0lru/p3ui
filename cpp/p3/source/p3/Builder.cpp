#define NOMINMAX

#include "Builder.h"
#include "Node.h"

#include "log.h"

#include "Text.h"
#include "Layout.h"
#include "UserInterface.h"
#include "DataSuffix.h"

#include <p3/widgets/button.h>
#include <p3/widgets/check_box.h>
#include <p3/widgets/ChildWindow.h>
#include <p3/widgets/Collapsible.h>
#include <p3/widgets/ColorEdit.h>
#include <p3/widgets/ComboBox.h>
#include <p3/widgets/Image.h>
#include <p3/widgets/InputScalar.h>
#include <p3/widgets/InputText.h>
#include <p3/widgets/Menu.h>
#include <p3/widgets/MenuBar.h>
#include <p3/widgets/MenuItem.h>
#include <p3/widgets/Plot.h>
#include <p3/widgets/Popup.h>
#include <p3/widgets/ProgressBar.h>
#include <p3/widgets/ScrollArea.h>
#include <p3/widgets/Slider.h>
#include <p3/widgets/Tab.h>

#include <pugixml.hpp>

#include <iostream>
#include <list>
#include <regex>

#include <fmt/format.h>

namespace p3
{

    namespace
    {

        template<typename T>
        struct add_slider
        {
            void operator()(Builder::Definitions& map)
            {
                auto name = "Slider" + DataSuffix<T>;
                map[std::move(name)] = []() -> std::shared_ptr<Node> { return std::make_shared<Slider<T>>(); };
            }
        };

        template<typename T>
        struct add_input
        {
            void operator()(Builder::Definitions& map)
            {
                auto name = "Input" + DataSuffix<T>;
                map[std::move(name)] = []() -> std::shared_ptr<Node> { return std::make_shared<InputScalar<T>>(); };
            }
        };

        Builder::Definitions p3_definitions = []() {
            Builder::Definitions map;
            map["Button"] = []() -> std::shared_ptr<Node> { return std::make_shared<Button>(); };
            map["CheckBox"] = []() -> std::shared_ptr<Node> { return std::make_shared<CheckBox>(); };
            map["ChildWindow"] = []() -> std::shared_ptr<Node> { return std::make_shared<ChildWindow>(); };
            map["Collapsible"] = []() -> std::shared_ptr<Node> { return std::make_shared<Collapsible>(); };
            map["ColorEdit"] = []() -> std::shared_ptr<Node> { return std::make_shared<ColorEdit>(); };
            map["ComboBox"] = []() -> std::shared_ptr<Node> { return std::make_shared<ComboBox>(); };
            map["Layout"] = []() -> std::shared_ptr<Node> { return std::make_shared<Layout>(); };
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

    }

    Builder::Builder()
        : _definitions(p3_definitions)
    {
    }

    std::shared_ptr<Node> Builder::build(std::istream& stream)
    {
        std::string buffer((std::istreambuf_iterator<char>(stream)),
            std::istreambuf_iterator<char>());
        return build(buffer);
    }

    std::shared_ptr<Node> Builder::build(std::string const& buffer)
    {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_string(buffer.c_str()/*, pugi::parse_ws_pcdata*/);
        return build(doc);
    }

    std::shared_ptr<Node> Builder::build(pugi::xml_document const& doc)
    {
        return build(*doc.children().begin());
    }

    std::shared_ptr<Node> Builder::build(pugi::xml_node const& pugi_node)
    {
        if (pugi_node.type() != pugi::node_element)
            return nullptr;

        //
        // find definition ("how to build the node") by (tag-)name
        auto pair = definitions().find(pugi_node.name());
        if (pair == definitions().end())
            throw std::runtime_error(fmt::format(R"(no definition found for node type "{}")", pugi_node.name()));

        //
        // call node constructor
        auto node = pair->second();
        build(*node, pugi_node);
        return node;
    }

    void Builder::build(Node& node, pugi::xml_node const& pugi_node)
    {
        for (auto& attribute : pugi_node.attributes())
            assign_attribute(node, attribute.name(), attribute.value());

        for (pugi::xml_node& child : pugi_node)
            if (child.type() == pugi::node_pcdata)
                assign_text(node, child.text().as_string());
            else if (child.type() == pugi::node_element)
                node.add(build(child));
    }

    void Builder::assign_text(Node& node, std::string text)
    {
        node.add(std::make_shared<Text>(std::move(text)));
    }

    void Builder::assign_attribute(Node& node, std::string const& name, std::string const& value)
    {
        node.set_attribute(name, value);
    }

    void Builder::define(std::string type, NodeConstructor constructor)
    {
        _definitions[type] = constructor;
    }

    Builder::Definitions const& Builder::definitions()
    {
        return _definitions;
    }

}
