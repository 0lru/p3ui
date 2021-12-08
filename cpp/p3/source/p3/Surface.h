#pragma once

#include "Node.h"
#include "RenderBackend.h"

#include <include/core/SkPicture.h>

namespace p3
{
    
    ///
    /// each surface represents a render target. Thus it may has it's
    /// own frambuffer object (fbo). the behavior is the following:
    /// a surface is considered invisible, if it is not visible within
    /// the parent content region or if it is styled to be invisible.
    /// in consequence:
    /// * the fbo will be released/destructed if the surface is invisible
    /// * the fbo will (normally) be as big as the surface size 
    ///
    /// on hardware, we may experience limitations due to texture size
    /// limits. this is why we introduce viewport/window-aligned surfaces.
    /// * a surface will automatically be viewport-aligned, if it's size
    ///   exceeds the size of the content region of the parent, or
    /// * if it's declared to be viewport aligned
    /// 
    /// as an optimization: the maximum width/height of a non-viewport aligned
    /// surface, which was automatically declared to be viewport aligned has
    /// * width = min(width, content-width)
    /// * height = min(height, height)
    class Surface : public p3::Node
    {
    public:
        using Viewport = std::array<double, 4>;
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
        // draws the result of the previously rendered fbo-attached texture
        void _draw_textured_rectangle();
        // frees render target if surface is invisible or on destruction
        void _dispose_render_target();

        //
        // true whenever picture changed
        bool _is_dirty = false;

        //
        // remember backend where context was created
        std::shared_ptr<RenderBackend> _render_backend;
        RenderBackend::RenderTarget* _render_target = nullptr;
        sk_sp<SkPicture> _skia_picture = nullptr;

//        sk_sp<SkSurface> _skia_surface;

        OnClick _on_click;
        Viewport _viewport;
        OnViewportChange _on_viewport_change;
    };

}
