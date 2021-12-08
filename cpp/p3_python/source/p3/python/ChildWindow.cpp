
#include "p3ui.h"
#include <p3/ChildWindow.h>

namespace p3::python
{

    void Definition<ChildWindow>::apply(py::module& module)
    {
        py::class_<ChildWindow, Node, std::shared_ptr<ChildWindow>> window(module, "ChildWindow");

        window.def(py::init<>([](std::shared_ptr<Node> content, bool resizeable, bool moveable, py::kwargs kwargs) {
            auto window = std::make_shared<ChildWindow>();
            ArgumentParser<Node>()(kwargs, *window);
            assign(kwargs, "on_close", *window, &ChildWindow::set_on_close);
            window->set_content(std::move(content));
            window->set_resizeable(resizeable);
            window->set_moveable(moveable);
            return window;
        }), py::kw_only(), py::arg("content")=py::none(), py::arg("resizeable")=true, py::arg("moveable")=true);
        def_property(window, "content", &ChildWindow::content, &ChildWindow::set_content);
        def_property(window, "resizeable", &ChildWindow::resizeable, &ChildWindow::set_resizeable);
        def_property(window, "moveable", &ChildWindow::moveable, &ChildWindow::set_moveable);
        def_property(window, "on_close", &ChildWindow::on_close, &ChildWindow::set_on_close);
    }

}