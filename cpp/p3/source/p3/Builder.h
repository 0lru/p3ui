
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
