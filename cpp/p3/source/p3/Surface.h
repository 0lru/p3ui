#pragma once

#include "Node.h"
#include "RenderBackend.h"
#include "RenderLayer.h"

#include <include/core/SkPicture.h>

namespace p3
{
    
    class Surface : public p3::Node
    {
    public:
        using Viewport = RenderLayer::Viewport;
        using OnClick = std::function<void()>;
        using OnViewportChange = std::function<void(Viewport)>;

        Surface();
        ~Surface();

        StyleStrategy& style_strategy() const override;

        void update_content() override;
        void render_impl(Context& context, float width, float height) final override;

        sk_sp<SkPicture> const& picture() const { return _skia_picture; }
        void set_picture(sk_sp<SkPicture>);

        void set_on_click(OnClick);
        OnClick on_click() const;

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
