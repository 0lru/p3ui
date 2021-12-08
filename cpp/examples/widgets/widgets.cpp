
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