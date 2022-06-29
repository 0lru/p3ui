
#pragma once

#include <functional>
#include <string>

#include <p3/Node.h>

namespace p3 {

class ToolTip : public Node {
public:
    ToolTip();

    void set_content(std::shared_ptr<Node>);
    std::shared_ptr<Node> content() const;

    void render(Context&, float width, float height, bool) override;
    void update_content();

private:
    std::shared_ptr<Node> _content;
};

}
