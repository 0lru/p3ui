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
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>

namespace py = pybind11;

//
// NOTE: use fwd header?
namespace p3
{
    class Button;
    class CheckBox;
    class ChildWindow;
    class Collapsible;
    class Color;
    class ColorEdit;
    class ComboBox;
    class Flexible;
    class Image;
    template<typename T> class InputScalar;
    class InputText;
    class Menu;
    class MenuItem;
    class MenuBar;
    class Node;
    class Plot;
    class Popup;
    class ProgressBar;
    class ScrollArea;
    template<typename T> class Slider;
    class StyleBlock;
    class Tab;
    class Table;
    class Text;
    class Texture;
    class UserInterface;
    struct Theme;
    class Window;
}

namespace p3::python
{

    template<typename T>
    class Definition
    {
    public:
        static void parse(py::kwargs const&, T&);
        static void apply(py::module&);
    };

}
