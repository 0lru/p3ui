#pragma once

#include <p3/Node.h>

#include <functional>
#include <string>

namespace p3 {

class Collapsible : public Node {
public:
    Collapsible(std::string title = "");

    void render_impl(Context&, float width, float height) override;

    void set_content(std::shared_ptr<Node>);
    std::shared_ptr<Node> content() const;

    void set_collapsed(bool);
    bool collapsed() const;

    void update_content() override;

private:
    std::shared_ptr<Node> _content;
    bool _collapsed = false;
    float _child_offset;
    std::optional<bool> _force_open = std::nullopt;
};

}
