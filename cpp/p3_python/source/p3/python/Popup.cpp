#include "p3ui.h"

#include <p3/widgets/Popup.h>


namespace p3::python
{

    void Definition<Popup>::apply(py::module& module)
    {
        py::class_<Popup, Node, std::shared_ptr<Popup>> popup(module, "Popup");
        popup.def(py::init<>([](py::kwargs kwargs) {
            auto popup = std::make_shared<Popup>();
            ArgumentParser<Node>()(kwargs, *popup);
            assign(kwargs, "content", *popup, &Popup::set_content);
            return popup;
        }));
        def_property(popup, "content", &Popup::content, &Popup::set_content);
    }

}