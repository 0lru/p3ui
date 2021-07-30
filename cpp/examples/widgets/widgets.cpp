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

#include <p3/Context.h>
#include <p3/CheckBox.h>
#include <p3/Layout.h>
#include <p3/Button.h>
#include <p3/Window.h>

using namespace p3;

int main(int argc, char *argv[])
{
    auto context = make<Context>();
    auto window = make<Window>(context);

    auto main_layout = make<Layout>();
    main_layout->style()->set_direction(Direction::Horizontal);
    main_layout->style()->set_align_items(Alignment::Stretch);
    main_layout->add(make<CheckBox>("CheckBox"));
    window->set_content(main_layout);

    window->loop([](auto window) {
    });
}