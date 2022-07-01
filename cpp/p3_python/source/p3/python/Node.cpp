#include "p3ui.h"
#include <p3/Node.h>

namespace p3::python {

void ArgumentParser<Node>::operator()(py::kwargs const& kwargs, Node& node)
{
    ArgumentParser<StyleBlock>()(kwargs, *node.style());
    assign(kwargs, "label", node, &Node::set_label);
    assign(kwargs, "visible", node, &Node::set_visible);
    assign(kwargs, "disabled", node, &Node::set_disabled);
    assign(kwargs, "on_resize", node, &Node::set_on_resize);
    assign(kwargs, "on_mouse_enter", node, &Node::set_on_mouse_enter);
    assign(kwargs, "on_mouse_move", node, &Node::set_on_mouse_move);
    assign(kwargs, "on_mouse_leave", node, &Node::set_on_mouse_leave);
    //
    // TODO: add setter and use assign
    if (kwargs.contains("children")) {
        auto dict = std::static_pointer_cast<py::dict>(node.user_data());
        (*dict)["children"] = kwargs["children"];
        auto children = kwargs["children"].cast<std::vector<std::shared_ptr<Node>>>();
        for (auto& child : children)
            node.add(child);
    }
}

void Definition<Node>::apply(py::module& module)
{
    //
    // by value, no need for sync
    py::class_<Node::MouseEvent> mouse_event(module, "MouseEvent");
    mouse_event.def_property_readonly("source", [](Node::MouseEvent& e) {
        return e.source()->shared_from_this();
    });
    mouse_event.def_property_readonly("x", &Node::MouseEvent::x);
    mouse_event.def_property_readonly("y", &Node::MouseEvent::y);
    mouse_event.def_property_readonly("global_x", &Node::MouseEvent::global_x);
    mouse_event.def_property_readonly("global_y", &Node::MouseEvent::global_y);

    //
    // Node, synced
    py::class_<Node, std::shared_ptr<Node>> node(module, "Node", py::custom_type_setup([](PyHeapTypeObject* heap_type) {
        auto* type = &heap_type->ht_type;
        type->tp_flags |= Py_TPFLAGS_HAVE_GC;
        type->tp_traverse = [](PyObject* self_base, visitproc visit, void* arg) {
            py::handle handle(self_base);
            if (handle.is_none())
                return 0;

            try {
                auto& self = py::cast<std::shared_ptr<Node>>(handle);
                if (!self->user_data())
                    return 0;
                auto ptr = std::static_pointer_cast<py::dict>(self->user_data());
                Py_VISIT(ptr->ptr());
            } catch (std::exception&) {
                //
                // the cast can fail if the holder is not initialized yet
                // currently it throws "Unable to cast from non-held to held instance (T& to Holder<T>) (compile in debug mode for type information)"
                // TODO: check if holder is initialized to avoid the exception
            }
            return 0;
        };
        type->tp_clear = [](PyObject* self_base) {
            auto& self = py::cast<Node&>(py::handle(self_base));
            self.set_user_data(nullptr);
            return 0;
        };
    }));
    node.def_property_readonly("user_data", [](Node& node) {
        py::object result = py::none();
        if (node.user_data())
            result = *std::static_pointer_cast<py::dict>(node.user_data());
        return result;
    });
    node.def_property_readonly_static("node_count", [](py::object&) {
        return Node::node_count();
    });
    def_property_readonly(node, "parent", &Node::shared_parent);
    def_property_readonly(node, "children", &Node::children);
    def_property_readonly(node, "style", &Node::style);
    node.def_property_readonly("foo", [](std::shared_ptr<Node>& self) {
        auto s = py::cast(self);
        return s.attr("__foo");
    });
    def_property(node, "visible", &Node::visible, &Node::set_visible);
    def_property(node, "disabled", &Node::disabled, &Node::set_disabled);
    def_property(node, "label", &Node::label, &Node::set_label);
    def_signal_property(node, "on_resize", &Node::on_resize, &Node::set_on_resize);
    def_signal_property(node, "on_mouse_enter", &Node::on_mouse_enter, &Node::set_on_mouse_enter);
    def_signal_property(node, "on_mouse_move", &Node::on_mouse_move, &Node::set_on_mouse_move);
    def_signal_property(node, "on_mouse_leave", &Node::on_mouse_leave, &Node::set_on_mouse_leave);
    def_method(node, "redraw", &Node::redraw);
    def_method(node, "add", [](Node& node, py::object child) {
        if (child.is_none())
            return;
        py::list children = (*std::static_pointer_cast<py::dict>(node.user_data()))["children"];
        children.append(child);
        node.add(py::cast<std::shared_ptr<Node>>(child));
    });
    def_method(node, "remove", [](Node& node, py::object child) {
        if (child.is_none())
            return;
        py::list children = (*std::static_pointer_cast<py::dict>(node.user_data()))["children"];
        children.attr("remove")(child);
        node.remove(py::cast<std::shared_ptr<Node>>(child));
    });
    def_method(node, "insert", [](Node& node, std::size_t index, py::object child) {
        if (child.is_none())
            return;
        py::list children = (*std::static_pointer_cast<py::dict>(node.user_data()))["children"];
        children.attr("insert")(index, child);
        node.insert(index, py::cast<std::shared_ptr<Node>>(child));
    });
}

}
