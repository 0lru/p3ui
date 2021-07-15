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
#include <p3/Popup.h>
#include <p3/Window.h>
#include <p3/Theme.h>
#include <p3/MenuBar.h>


namespace p3::python
{

    void Definition<Window>::parse(py::kwargs const& kwargs, Window& window)
    {
    }

    void Definition<Window>::apply(py::module& module)
    {
        py::class_<Window, Node, std::shared_ptr<Window>>(module, "Window")
            .def(py::init<>([](std::shared_ptr<Context> context, py::kwargs kwargs) {
                auto window = std::make_shared<Window>(std::move(context));
                parse(kwargs, *window);
                return window;
            }))
            .def_property("content", &Window::content, &Window::set_content)
            .def("add", [](Window& window, std::shared_ptr<Popup> popup) {
                window.add(std::move(popup));
            })
            .def_property("menu_bar", &Window::menu_bar, &Window::set_menu_bar)
            .def_property("theme", &Window::theme, &Window::set_theme)
            .def("loop", [](std::shared_ptr<Window> window, py::kwargs kwargs) {
                py::gil_scoped_release release;
                Window::UpdateCallback on_frame;
                if (kwargs.contains("on_frame"))
                {
                    auto f = kwargs["on_frame"].cast<py::function>();
                    on_frame = [f{std::move(f)}](auto window) {
                        py::gil_scoped_acquire acquire;
                            f(std::move(window));
                    };
                }
                window->loop(on_frame);
            });
    }

}
