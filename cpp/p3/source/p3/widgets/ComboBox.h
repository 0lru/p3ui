#pragma once

#include <functional>
#include <optional>
#include <string>

#include <p3/Node.h>

namespace p3 {

class ComboBox : public Node {
public:
    using OnChange = std::function<void(int)>;

    ComboBox();

    void render_impl(Context&, float width, float height) override;
    void update_content() override;

    void set_options(std::vector<std::string>);
    std::vector<std::string> const& options() const;

    void set_on_change(OnChange);
    OnChange on_change() const;

    int selected_index() const;
    void set_selected_index(int);

    std::string const& hint() const;
    void set_hint(std::string);

protected:
    void dispose() override;

private:
    int _selected_index = -1;
    std::string _hint;
    std::vector<std::string> _options;
    OnChange _on_change;
};

}
