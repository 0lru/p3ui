#pragma once

#include <functional>
#include <string>

#include <p3/Node.h>

struct ImGuiInputTextCallbackData;

namespace p3 {

class InputText : public Node {
public:
    using OnChange = std::function<void()>;

    InputText(std::size_t size = 1024, std::optional<std::string> label = std::optional<std::string>());

    StyleStrategy& style_strategy() const override;
    void render_impl(Context&, float width, float height) override;

    void set_hint(std::optional<std::string>);
    std::optional<std::string> const& hint(std::string) const;

    void set_on_change(OnChange);
    OnChange on_change() const;

    void update_content() override;

    void set_value(std::string);
    std::string const& value() const;

    void set_multi_line(bool);
    bool multi_line() const;

protected:
    void dispose() override;

private:
    std::string _value;
    std::size_t _size;
    std::optional<std::string> _hint;
    OnChange _on_change;
    static int Callback(ImGuiInputTextCallbackData* data);
    bool _multi_line = false;
};

}
