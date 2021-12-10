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


    void Surface::render_impl(Context& context, float fwidth, float fheight)
    {
        //
        // nothing todo.. leave
        if (fwidth * fheight <= 0 || !_skia_picture)
            return;

        //
        // set_needs_update dirties the active render_layer
        if (context.render_layer().dirty())
        {
            //
            // get pos in local coordinates
            auto cursor = ImGui::GetCursorPos();

            //
            // check if viewport was scrolled or resized
            auto viewport = context.render_layer().viewport();
            // move viewport to local coordinates
            viewport[0] -= cursor.x; viewport[1] -= cursor.y;
            if (viewport != _viewport)
            {
                _viewport = viewport;
                if (_on_viewport_change) postpone([f = _on_viewport_change, rect = viewport]() {
                    f(rect);
                });
            }

            auto& canvas = context.render_layer().use(context.render_backend());
            canvas.save();
            canvas.translate(cursor.x - ImGui::GetScrollX(), cursor.y - ImGui::GetScrollY());
            auto clip_rect = SkRect::MakeWH(
                std::uint32_t(fwidth + 0.5f), 
                std::uint32_t(fheight + 0.5f)
            );
            canvas.clipRect(clip_rect, false);
            canvas.drawPicture(_skia_picture);
            canvas.restore();
        }

        //
        // TODO: move out of here, specialize
        ImGuiWindow& window = *ImGui::GetCurrentWindow();
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
        set_needs_update();
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
