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
#include <p3/Window.h>
#include <p3/ChildWindow.h>
#include <p3/Flexible.h>
#include <p3/Button.h>
#include <p3/Collapsible.h>
#include <p3/ScrollArea.h>

using namespace p3;

std::shared_ptr<ChildWindow> make_collapsible()
{
    auto button1 = make<Button>("button1");
    auto button2 = make<Button>("button2");
    auto button3 = make<Button>("button3");

    auto layout = make<Flexible>();
    layout->style()->set_align_items(Alignment::Stretch);
    layout->style()->set_justify_content(Justification::SpaceBetween);
    layout->add(button1);
    layout->add(button2);
    layout->add(button3);

    auto collapsible = make<Collapsible>("Collapsible Header");
    collapsible->set_content(layout);

    auto window = make<ChildWindow>("Child Window");
    window->set_content(collapsible);
    return window;
}

std::shared_ptr<ChildWindow> make_widgets()
{
    auto window = make<ChildWindow>("Widgets");

    auto scroll_area = make<ScrollArea>();
    window->set_content(scroll_area);

    auto layout = make<Flexible>();
    /*layout->set_align_items(Alignment::Stretch)
        .set_justify_content(Justification::Start)
        .set_width({ 100 | percent, 0.f, 0.f });
    */
    scroll_area->set_content(layout);
    
    auto button = make<Button>("button1");
    /*button->set_style(Style{
        .width = { 100 | px, 1.f, 1.f } 
    });*/
    layout->add(button);
    layout->add(make<Button>("button1"));
    layout->add(make<Button>("button2"));
    layout->add(make<Button>("button3"));
    layout->add(make<Button>("button4"));
    layout->add(make<Button>("button5"));
    layout->add(make<Button>("button6"));
    layout->add(make<Button>("button7"));
    layout->add(make<Button>("button8"));

    return window;
}

int main(int argc, char *argv[])
{
    auto context = make<Context>();
    auto window = make<Window>(context);

    window->add(make_collapsible());
    window->add(make_widgets());

    window->loop([](auto window) {});
}