#include "p3ui.h"

#include <p3/widgets/spacer.h>

namespace p3::python {

void Definition<Spacer>::apply(py::module& module)
{
    py::class_<Spacer, Node, std::shared_ptr<Spacer>> spacer(module, "Spacer");
    spacer.def(py::init<>([](py::kwargs kwargs) {
        auto spacer = std::make_shared<Spacer>();
        ArgumentParser<Node>()(kwargs, *spacer);
        return spacer;
    }));
}

}
