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
#include "numpy.h"
#include <p3/Texture.h>
#include <format>

namespace p3::python
{

    namespace
    {
        void copy(p3::Texture& texture, py::array_t<std::uint8_t> const& data)
        {
            auto in = data.request();
            if (in.shape.size() != 3)
                throw std::invalid_argument(std::format("array has wrong shape dimension of {}", in.shape.size()));
            auto depth = in.shape[2];
            if (depth != 4)
                throw std::invalid_argument(std::format("only rgba is supported, but image has {} channels", depth));
            auto height = in.shape[0];
            auto width = in.shape[1];
            if (width * height == 0)
                return;
            if (width != texture.width() || height != texture.height())
                texture.resize(width, height);
            auto src = reinterpret_cast<std::uint8_t*>(in.ptr);
            std::memcpy(texture.data(), in.ptr, width * height * depth);
        }
    }

    void Definition<Texture>::apply(py::module& module)
    {
        auto texture = py::class_<Texture, std::shared_ptr<Texture>>(module, "Texture");

        texture.def(py::init<>([](std::size_t width, std::size_t height) {
            return std::make_shared<Texture>(width, height);
        })).def(py::init<>([]() {
            return std::make_shared<Texture>(0, 0);
        })).def(py::init<>([](py::array_t<std::uint8_t> data) {
            auto texture = std::make_shared<Texture>(0, 0);
            copy(*texture, data);
            texture->update();
            return texture;
        }));

        texture.def_property("data", [](std::shared_ptr<Texture> texture) {
            decltype(texture->lock()) guard;
            {
                py::gil_scoped_release release;
                guard = texture->lock();
            }
            return wrap<std::uint8_t>(texture->data(), texture->height(), texture->width(), 4).attr("copy")();
        }, [](std::shared_ptr<Texture> texture, py::array_t<std::uint8_t> data) {
            decltype(texture->lock()) guard;
            {
                py::gil_scoped_release release;
                guard = texture->lock();
            }
            copy(*texture, data);
            texture->update();
        });
    }

}