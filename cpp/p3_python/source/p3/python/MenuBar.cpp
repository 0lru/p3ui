#include "p3ui.h"

#include <p3/widgets/Menu.h>
#include <p3/widgets/MenuBar.h>


namespace p3::python
{

    void Definition<MenuBar>::apply(py::module& module)
    {
        py::class_<MenuBar, Node, std::shared_ptr<MenuBar>> menu_bar(module, "MenuBar");
        menu_bar.def(py::init<>([](py::kwargs kwargs) {
            auto menu_bar = std::make_shared<MenuBar>();
            ArgumentParser<Node>()(kwargs, *menu_bar);
            return menu_bar;
        }));
    }

}
