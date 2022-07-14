
#include <p3/Context.h>
#include <p3/Window.h>
#include <p3/Layout.h>
#include <p3/button.h>

using namespace p3;


ref<Layout> make_layout(Direction direction)
{
    auto layout = make<Layout>();
    layout->style()->set_direction(direction);
/*    layout->set_align_items(Alignment::Stretch)
        .set_justify_content(Justification::Start)
        .set_width({ 1|px, 1.f, 1.f })
        .set_height({ 1|px, 1.f, 1.f });
        */
    auto button = make<Button>("Start");
    button->set_on_click([layout = layout.get()]() {
        layout->style()->set_align_items(Alignment::Start);
    });
//    button->set_width({200|px, 1.f, 1.f }).set_height({200|px, 1.f, 1.f });
    layout->add(button);

    button = make<Button>("Center");
    button->set_on_click([layout = layout.get()]() {
        layout->style()->set_align_items(Alignment::Center);
    });
//    button->set_width({auto_, 1.f, 1.f }).set_height({auto_, 1.f, 1.f });
    layout->add(button);

    button = make<Button>("End");
//    button->set_width({100|px, 1.f, 1.f });
//    button->set_height({100|px, 1.f, 1.f });
    button->set_on_click([layout = layout.get()]() {
        layout->style()->set_align_items(Alignment::End);
    });
    layout->add(button);

    button = make<Button>("Stretch");
//    button->set_width({100|px, 1.f, 1.f });
//    button->set_height({100|px, 1.f, 1.f });
    button->set_on_click([layout = layout.get()]() {
        layout->style()->set_align_items(Alignment::Stretch);
    });
    layout->add(button);

    return layout;
}

int main(int argc, char *argv[])
{
    auto context = make<Context>();
    auto window = make<Window>(context);
    auto main_layout = make<Layout>();
    main_layout->style()->set_direction(Direction::Vertical);
    main_layout->style()->set_align_items(Alignment::Stretch);
    main_layout->add(make_layout(Direction::Vertical));
    main_layout->add(make_layout(Direction::Horizontal));
    window->set_content(main_layout);

    window->loop([](auto window) {
    });
}