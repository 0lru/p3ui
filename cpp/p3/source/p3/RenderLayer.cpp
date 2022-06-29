#include "RenderLayer.h"
#include "Context.h"
#include "Node.h"
#include "log.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <include/core/SkCanvas.h>

namespace p3 {

namespace {
    auto const whitef = ImVec4(1, 1, 1, 1);
    auto const redf = ImVec4(1, 0, 0, 1);
    auto const greenf = ImVec4(0, 1, 0, 1);
}

RenderLayer::~RenderLayer()
{
    _reset();
}

void RenderLayer::push_to(Context& context)
{
    //
    // push this layer onto the stack.
    // children will/need to increase this->_object_count upon traversal
    _object_count = 0;
    context.push_render_layer(*this);

    //
    // the content can overlap the frame spacing when scrolled.
    // in consequence, we create the rt as big as the container
    // and provide a virtual viewport which is sligtly smaller than the rt.
    auto const& imgui_context = *ImGui::GetCurrentContext();
    auto content_min = ImGui::GetWindowContentRegionMin();
    auto content_max = ImGui::GetWindowContentRegionMax();
    //
    // e.g. same as GetContentRegionAvail, but does not depend on cursor pos
    auto& window = *ImGui::GetCurrentWindow();
    auto content_width = window.ClipRect.Max.x - window.ClipRect.Min.x;//content_max.x - content_min.x;
    auto content_height = window.ClipRect.Max.y - window.ClipRect.Min.y;//    content_max.y - content_min.y;

    //
    // the rt is 2 * padding larger than the content region
    _requested_width = std::uint32_t(std::max(1.f, content_width + 0.5f));
    _requested_height = std::uint32_t(std::max(1.f, content_height + 0.5f));

    //
    // no render target -> needs redraw, mark dirty
    if (!_render_target)
        _dirty = true;
    //
    // render target resized -> needs redraw, mark dirty
    else if (_requested_width != _render_target->width() || _requested_height != _render_target->height())
        _dirty = true;

    //
    // this is just virtual
    Viewport viewport {
        ImGui::GetScrollX(),
        ImGui::GetScrollY(),
        content_width,
        content_height
    };

    //
    // effectively we check scroll-x and scroll-y
    if (viewport != _viewport) {
        _dirty = true;
        _viewport = viewport;
    }
}

void RenderLayer::register_object()
{
    ++_object_count;
}

void RenderLayer::_draw_debug()
{
    ImU32 constexpr red = 0xFF0000FF;
    ImU32 constexpr green = 0xFF00FF00;
    auto& window = *ImGui::GetCurrentWindow();
    window.DrawList->AddRect(window.ClipRect.Min, window.ClipRect.Max, _render_target ? green : red, 0, 0, 2);
}

void RenderLayer::_reset()
{
    if (_render_target)
        _render_backend->delete_render_target(_render_target);
    _render_backend.reset();
    _render_target = nullptr;
}

void RenderLayer::pop_from_context_and_render(Context& context, Node& node)
{
    context.pop_render_layer();
    auto& backend = context.render_backend();
    //
    // return and possibly free object memory
    if (!_object_count) {
        if (context.show_render_layers())
            _draw_debug();
        _reset();
        return;
    }

    //
    // resize gpu memory lazily, on demand
    if (!_render_target
        || _requested_width != _render_target->width()
        || _requested_height != _render_target->height()) {
        if (_render_target) {
            _reset();
        }
        _render_target = backend.create_render_target(_requested_width, _requested_height);
        _render_backend = backend.shared_from_this();
        _dirty = true;
        log_debug("created render target {}x{}", _render_target->width(), _render_target->height());
    }

    //
    // need to redraw. bind rt and do the traversal
    if (_dirty) {
        // log_info("rendering {} objects", _object_count);
        auto& canvas = *_render_target->skia_surface()->getCanvas();
        _render_target->bind();
        canvas.clear(0x0000000);
        node.render(*_render_target);
        _render_target->skia_surface()->flushAndSubmit();
        _render_target->release();
        _dirty = false;
    }

    //
    // if fbo is present, add color buffer as texture
    ImU32 constexpr white = 0xFFFFFFFF;
    static auto const zero2D = ImVec2(0.f, 0.f);
    static auto const ones2D = ImVec2(1.f, 1.f);

    auto& window = *ImGui::GetCurrentWindow();
    ImVec2 p1(
        window.ClipRect.Min.x,
        window.ClipRect.Min.y);
    ImVec2 p2(
        p1.x + float(_render_target->width()),
        p1.y + float(_render_target->height()));
    window.DrawList->AddImage(_render_target->texture_id(),
        p1, p2, zero2D, ones2D, white);

    if (context.show_render_layers())
        _draw_debug();
}

void RenderLayer::set_dirty()
{
    _dirty = true;
}

}
