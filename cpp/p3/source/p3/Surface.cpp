#include "Surface.h"

#include "log.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <p3/Context.h>
#include <p3/RenderBackend.h>

#include <include/core/SkCanvas.h>


namespace p3
{

    namespace
    {
        //
        // declare the surface to a non-shrink nor growing rectangle per default.
        class LocalStyleStrategy : public StyleStrategy
        {
        public:
            LayoutLength const& initial_width() override
            {
                static auto initial = LayoutLength{ std::nullopt, 0.f, 0.f };
                return initial;
            }
            LayoutLength const& initial_height() override
            {
                static auto initial = LayoutLength{ std::nullopt, 0.f, 0.f };
                return initial;
            }
        };
        LocalStyleStrategy _style_strategy;
    }

    StyleStrategy& Surface::style_strategy() const
    {
        return _style_strategy;
    }

    Surface::Surface()
        : Node("Surface")
    {
    }

    Surface::~Surface()
    {
    }

    Surface::Viewport const& Surface::viewport() const
    {
        return _viewport;
    }

    void Surface::set_on_viewport_change(OnViewportChange on_viewport_change)
    {
        _on_viewport_change = std::move(on_viewport_change);
    }

    Surface::OnViewportChange Surface::on_viewport_change() const
    {
        return _on_viewport_change;
    }

    void Surface::dispose()
    {
        log_debug("dispose surface");
        _skia_picture.reset();
//        _skia_surface.reset();
//        _skia_target.reset();
        _on_click = nullptr;
        _on_viewport_change = nullptr;
        Node::dispose();
    }

    void Surface::update_content()
    {
        _automatic_width = 1.f;
        _automatic_height = 1.f;
    }

    void Surface::_draw_textured_rectangle()
    {
        static auto const zero2D = ImVec2(0.f, 0.f);
        static auto const ones2D = ImVec2(1.f, 1.f);
        static auto const whitef = ImVec4(1, 1, 1, 1);
        static auto const whiteu = ImGui::GetColorU32(whitef);

        if (!_render_target)
            return;
        auto& window = *ImGui::GetCurrentWindow();
        auto const& padding = window.WindowPadding;

        ImVec2 p1(
            window.Pos.x,
            window.Pos.y
        );
        ImVec2 p2(
            p1.x + float(_render_target->width()),
            p1.y + float(_render_target->height())
        );
        window.DrawList->AddImage(_render_target->texture_id(),
            p1, p2,
            zero2D, ones2D,
            whiteu);
    }

    void Surface::_dispose_render_target()
    {
    }

    void Surface::render_impl(Context& context, float fwidth, float fheight)
    {
        ImGuiWindow& window = *ImGui::GetCurrentWindow();
        //
        // nothing todo.. leave
        if (fwidth * fheight <= 0 || !_skia_picture || window.SkipItems)
            return;

        //
        // use rounded width/height for fbo
        auto width = std::uint32_t(fwidth + 0.5f);
        auto height = std::uint32_t(fheight + 0.5f);
        //
        // get pos in local coordinates
        auto cursor = ImGui::GetCursorPos();

        //
        //
        auto const& imgui_context = *ImGui::GetCurrentContext();
        auto const& frame_padding = imgui_context.Style.FramePadding;

        auto content_min = ImGui::GetWindowContentRegionMin();
        auto content_max = ImGui::GetWindowContentRegionMax();

        //
        // the content can overlap the frame spacing if parent is scrolled
        auto vp_width = content_max.x - content_min.x;
        auto vp_height = content_max.y - content_min.y;
        auto viewport_width = std::uint32_t(content_max.x - content_min.x + 2 * frame_padding.x + 0.5f);
        auto viewport_height = std::uint32_t(content_max.y - content_min.y + 2 * frame_padding.y + 0.5f);

        //
        // 1) render target does not fit
        bool render_target_dirty = !_render_target
            || viewport_width != _render_target->width()
            || viewport_height != _render_target->height();

        _is_dirty = _is_dirty || render_target_dirty;

        //
        // define viewport rect (the viewport is the containing window)
        // of this surface in the coordinate-spface of the surface
        Viewport viewport{
            -cursor.x + ImGui::GetScrollX(),
            -cursor.y + ImGui::GetScrollY(),
            double(vp_width),
            double(vp_height)
        };
        if (viewport != _viewport)
        {
            _is_dirty = true;
            _viewport = viewport;
            if (_on_viewport_change) postpone([f = _on_viewport_change, rect = viewport]() {
                f(rect);
            });
        }

        //
        // only acquire gil if needed
        if (_is_dirty)
        {
            if (render_target_dirty)
            {
                //
                // free existing resources
                if (_render_target)
                {
                    context.render_backend().delete_render_target(_render_target);
                }
                //
                // create..
                _render_target = context.render_backend().create_render_target(viewport_width, viewport_height);
                _render_target->bind();
                log_debug("created render target {}x{}", viewport_width, viewport_height);
            }

            //
            // draw to fbo
            _render_target->bind();
            auto& canvas = *_render_target->skia_surface()->getCanvas();
            canvas.save();
            canvas.clear(0x0000000);
            canvas.translate(
                cursor.x - ImGui::GetScrollX(),
                cursor.y - ImGui::GetScrollY()
            );
            auto clip_rect = SkRect::MakeWH(width, height);
            canvas.clipRect(clip_rect, false);
            canvas.drawPicture(_skia_picture);
            canvas.restore();
            _is_dirty = false;
            _render_target->skia_surface()->flushAndSubmit();
            _render_target->release();
        }

        _draw_textured_rectangle();

        //
        // advance cursor
        auto const id = window.GetID(this->imgui_label().c_str());
        ImVec2 pos(window.DC.CursorPos.x, window.DC.CursorPos.y + window.DC.CurrLineTextBaseOffset);
        auto bb_bottom_right = ImVec2(window.DC.CursorPos.x + fwidth, window.DC.CursorPos.y + fheight);
        ImRect bb(pos, bb_bottom_right);
        ImGui::ItemSize(bb, 0.f/*baseline*/);
        ImGui::ItemAdd(bb, id);
        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, 0);
        if (pressed && _on_click && !disabled())
            postpone([f = _on_click]() {
            f();
        });
        update_status();
    }

    void Surface::set_picture(sk_sp<SkPicture> picture)
    {
        _skia_picture = std::move(picture);
        _is_dirty = true;
        redraw();
    }

    void Surface::set_on_click(OnClick on_click)
    {
        _on_click = on_click;
    }

    Surface::OnClick Surface::on_click() const
    {
        return _on_click;
    }

}
