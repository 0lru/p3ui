#include "p3ui.h"

#include <p3/widgets/Image.h>

namespace p3::python {

void Definition<Image>::apply(py::module& module)
{
    py::class_<Image, Node, std::shared_ptr<Image>> image(module, "Image");

    image.def(py::init<>([](std::optional<std::shared_ptr<Texture>> texture, std::optional<double> scale, py::kwargs kwargs) {
        auto image = std::make_shared<Image>();
        ArgumentParser<Node>()(kwargs, *image);
        auto dict = std::static_pointer_cast<py::dict>(image->user_data());
        
        (*dict)["texture"] = py::cast(texture);
        assign(texture, *image, &Image::set_texture);
        
        assign(scale, *image, &Image::set_scale);
        assign(kwargs, "on_click", *image, &Image::set_on_click);
        return image;
    }),
        py::arg("texture") = py::none(), py::arg("scale") = py::none());

    def_content_property(image, "texture", &Image::texture, &Image::set_texture);
    def_property(image, "scale", &Image::scale, &Image::set_scale);
    def_signal_property(image, "on_click", &Image::on_click, &Image::set_on_click);
}

}
