
#include "p3ui.h"
#include <p3/ToolTip.h>

namespace p3::python
{

    void Definition<ToolTip>::apply(py::module& module)
    {
        py::class_<ToolTip, Node, std::shared_ptr<ToolTip>> tooltip(module, "ToolTip");
        tooltip.def(py::init<>([](std::shared_ptr<Node> content, py::kwargs kwargs) {
            auto tooltip = std::make_shared<ToolTip>();
            ArgumentParser<Node>()(kwargs, *tooltip);
            tooltip->set_content(std::move(content));
            return tooltip;
        }), py::kw_only(), py::arg("content")=py::none());
        def_property(tooltip, "content", &ToolTip::content, &ToolTip::set_content);
    }

}