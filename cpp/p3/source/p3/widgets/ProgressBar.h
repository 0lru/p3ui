#pragma once

#include <functional>
#include <string>

#include <p3/Node.h>

namespace p3 {

class ProgressBar : public Node {
public:
    ProgressBar();

    void render_impl(Context&, float width, float height) override;

    void update_content() override;

    void set_value(float);
    float value() const;

private:
    float _value = 0.f;
};

}
