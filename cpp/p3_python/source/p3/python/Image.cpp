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

    void Definition<Image>::parse(py::kwargs const& kwargs, Image& image)
    {
        Definition<Node>::parse(kwargs, image);
        if (kwargs.contains("texture"))
            image.set_texture(kwargs["texture"].cast<std::shared_ptr<Texture>>());
    }

    void Definition<Image>::apply(py::module& module)
    {
        py::class_<Image, Node, std::shared_ptr<Image>> image(module, "Image");

        image.def(py::init<>([](py::kwargs kwargs) {
            auto image = std::make_shared<Image>();
            parse(kwargs, *image);
            return image;
        }));

        image.def_property("texture", &Image::texture, &Image::set_texture);
        image.def("set_dirty", &Image::set_needs_update);
        image.def_property("scale", &Image::scale, &Image::set_scale);
    }

}
