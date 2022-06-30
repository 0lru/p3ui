#include "Surface.h"
#include "Context.h"
#include "RenderBackend.h"
#include "log.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <include/core/SkCanvas.h>

namespace p3 {

namespace {
    //
    // declare the surface to a non-shrink nor growing rectangle per default.
    class LocalStyleStrategy : public StyleStrategy {
    public:
        LayoutLength const& initial_width() override
        {
            static auto initial = LayoutLength { std::nullopt, 0.f, 0.f };
            return initial;
        }
        LayoutLength const& initial_height() override
        {
            static auto initial = LayoutLength { std::nullopt, 0.f, 0.f };
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
    set_render_layer(nullptr);
//  _on_click = nullptr;
//  _on_viewport_change = nullptr;
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

    context.render_layer().register_object();

    //
    // make viewport in local coordinates
    auto cursor = ImGui::GetCursorPos();
    //
    // viewport is scollx/scrolly
    auto viewport = context.render_layer().viewport();
    auto& window = *ImGui::GetCurrentWindow();
    viewport[0] = window.ClipRect.Min.x - window.DC.CursorPos.x;
    viewport[1] = window.ClipRect.Min.y - window.DC.CursorPos.y;
    if (viewport != _viewport) {
        _viewport = viewport;
        if (_on_viewport_change)
            postpone([f = _on_viewport_change, rect = viewport]() {
                f(rect);
            });
    }

    //
    // TODO: move out of here, specialize
//    ImGuiWindow& window = *ImGui::GetCurrentWindow();
    auto const id = window.GetID(this->imgui_label().c_str());
    ImVec2 pos(window.DC.CursorPos.x, window.DC.CursorPos.y + window.DC.CurrLineTextBaseOffset);
    auto bb_bottom_right = ImVec2(window.DC.CursorPos.x + fwidth, window.DC.CursorPos.y + fheight);
    ImRect bb(pos, bb_bottom_right);
    ImGui::ItemSize(bb, 0.f /*baseline*/);
    ImGui::ItemAdd(bb, id);
    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, 0);
    if (pressed && _on_click && !disabled())
        postpone([f = _on_click]() {
            f();
        });
    update_status();
}

void Surface::render(RenderBackend::RenderTarget& render_target)
{
    auto& canvas = *render_target.skia_surface()->getCanvas();
    canvas.save();
    auto clip_rect = SkRect::MakeWH(
        std::uint32_t(_viewport[2] + 0.5f),
        std::uint32_t(_viewport[3] + 0.5f));
    canvas.clipRect(clip_rect, false);
    canvas.translate(-_viewport[0], -_viewport[1]);
    canvas.drawPicture(_skia_picture);
    canvas.restore();
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
