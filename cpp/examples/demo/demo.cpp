
#include <p3/Context.h>
#include <p3/Window.h>
#include <p3/child_window.h>
#include <p3/layout.h>
#include <p3/button.h>
#include <p3/collapsible.h>
#include <p3/ScrollArea.h>

using namespace p3;

std::shared_ptr<ChildWindow> make_collapsible()
{
    auto button1 = make<Button>("button1");
    auto button2 = make<Button>("button2");
    auto button3 = make<Button>("button3");

    auto layout = make<Layout>();
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

    auto layout = make<Layout>();
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