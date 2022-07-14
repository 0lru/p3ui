#pragma once

#include "Node.h"
#include "RenderLayer.h"

#include <include/core/SkPicture.h>

namespace p3 {

//
// the surface is drawn onto a layer. the layer may be shared by
// multiple surfaces.
class Surface : public p3::Node {
public:
    using Viewport = RenderLayer::Viewport;
    using OnClick = std::function<void()>;
    using OnViewportChange = std::function<void(Viewport)>;

    Surface();

    void update_content() override;
    void render_impl(Context&, float width, float height) final override;
    void render(RenderBackend::RenderTarget&) override;

    sk_sp<SkPicture> const& picture() const { return _skia_picture; }
    void set_picture(sk_sp<SkPicture>);

    void set_on_click(OnClick);
    OnClick on_click() const;

    //
    // viewport coordinates, relative to the render target ("local coordinates")
    Viewport const& viewport() const;
    void set_on_viewport_change(OnViewportChange);
    OnViewportChange on_viewport_change() const;

protected:
    void dispose() override;

private:
    sk_sp<SkPicture> _skia_picture = nullptr;
    OnClick _on_click;
    Viewport _viewport;
    OnViewportChange _on_viewport_change;
};

}
