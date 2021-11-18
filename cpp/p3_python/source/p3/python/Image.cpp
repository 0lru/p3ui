/***************************************************************************//*/
  Copyright (c) 2021 Martin Rudoff

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
/******************************************************************************/

#include "p3ui.h"
#include <p3/Image.h>


namespace p3::python
{

    void Definition<Image>::apply(py::module& module)
    {
        py::class_<Image, Node, std::shared_ptr<Image>> image(module, "Image");

        image.def(py::init<>([](std::optional<std::shared_ptr<Texture>> texture, std::optional<double> scale, py::kwargs kwargs) {
            auto image = std::make_shared<Image>();
            ArgumentParser<Node>()(kwargs, *image);
            assign(texture, *image, &Image::set_texture);
            assign(scale, *image, &Image::set_scale);
            assign(kwargs, "on_click", *image, &Image::set_on_click);
            return image;
        }), py::arg("texture")=py::none(), py::arg("scale")=py::none());

        def_property(image, "texture", &Image::texture, &Image::set_texture);
        def_property(image, "scale", &Image::scale, &Image::set_scale);
        def_property(image, "on_click", &Image::on_click, &Image::set_on_click);
    }

}
