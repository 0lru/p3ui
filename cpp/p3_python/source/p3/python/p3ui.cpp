#include "p3ui.h"
#include <p3/Node.h>

PYBIND11_MODULE(native, module)
{
    using namespace ::p3;
    p3::NodeInitializer = [](Node& node) {
        auto gc_dict = std::make_shared<py::dict>();
        (*gc_dict)["children"] = py::list();
        node.set_user_data(gc_dict);
    };
    module.doc() =
        R"docstring(
        Object Oriented ImGUI (https://github.com/0lru/p3ui)
    )docstring";
    python::Definition<EventLoop>::apply(module);

    python::Definition<Node>::apply(module);
    python::Definition<Button>::apply(module);
    python::Definition<CheckBox>::apply(module);
    python::Definition<Layout>::apply(module);
    python::Definition<Texture>::apply(module);
    python::Definition<ToolTip>::apply(module);
    python::Definition<Image>::apply(module);
    python::Definition<Color>::apply(module);
    python::Definition<ColorEdit>::apply(module);
    python::Definition<ComboBox>::apply(module);
    python::Definition<Tab>::apply(module);
    python::Definition<Table>::apply(module);
    python::Definition<Text>::apply(module);
    python::Definition<Collapsible>::apply(module);
    python::Definition<InputText>::apply(module);
    python::Definition<InputScalar<std::int8_t>>::apply(module);
    python::Definition<InputScalar<std::uint8_t>>::apply(module);
    python::Definition<InputScalar<std::int16_t>>::apply(module);
    python::Definition<InputScalar<std::uint16_t>>::apply(module);
    python::Definition<InputScalar<std::int32_t>>::apply(module);
    python::Definition<InputScalar<std::uint32_t>>::apply(module);
    python::Definition<InputScalar<std::int64_t>>::apply(module);
    python::Definition<InputScalar<std::uint64_t>>::apply(module);
    python::Definition<InputScalar<float>>::apply(module);
    python::Definition<InputScalar<double>>::apply(module);
    python::Definition<Menu>::apply(module);
    python::Definition<MenuItem>::apply(module);
    python::Definition<MenuBar>::apply(module);
    python::Definition<Plot>::apply(module);
    python::Definition<Popup>::apply(module);
    python::Definition<ProgressBar>::apply(module);
    python::Definition<ChildWindow>::apply(module);
    python::Definition<ScrollArea>::apply(module);
    python::Definition<Slider<std::int8_t>>::apply(module);
    python::Definition<Slider<std::uint8_t>>::apply(module);
    python::Definition<Slider<std::int16_t>>::apply(module);
    python::Definition<Slider<std::uint16_t>>::apply(module);
    python::Definition<Slider<std::int32_t>>::apply(module);
    python::Definition<Slider<std::uint32_t>>::apply(module);
    python::Definition<Slider<std::int64_t>>::apply(module);
    python::Definition<Slider<std::uint64_t>>::apply(module);
    python::Definition<Slider<float>>::apply(module);
    python::Definition<Slider<double>>::apply(module);
    python::Definition<StyleBlock>::apply(module);
    python::Definition<Theme>::apply(module);
    python::Definition<UserInterface>::apply(module);
    python::Definition<Window>::apply(module);

    python::Definition<python::Builder>::apply(module);
    python::Definition<python::Surface>::apply(module);
}
