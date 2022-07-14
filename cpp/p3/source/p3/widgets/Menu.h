#pragma once

#include <functional>
#include <string>

#include <p3/Node.h>

namespace p3 {

class Menu : public Node {
public:
    Menu(std::string label = "");

    using OnOpen = std::function<void()>;
    using OnClose = std::function<void()>;

    void update_content() override;
    void render_impl(Context&, float width, float height) override;

    void set_on_open(OnOpen);
    OnOpen on_open() const;

    void set_on_close(OnClose);
    OnClose on_close() const;

protected:
    void dispose() override;

private:
    bool _opened = false;
    OnOpen _on_open;
    OnClose _on_close;
};

}
