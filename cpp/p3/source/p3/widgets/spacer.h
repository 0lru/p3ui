#pragma once

#include <string>

#include <p3/Node.h>

namespace p3 {

class Spacer : public Node {
public:
    using OnClick = std::function<void()>;

    Spacer(std::optional<std::string> label = std::nullopt);

    void render_impl(Context&, float width, float height) override;
    void update_content() override;
};

}
