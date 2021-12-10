#include "RenderLayer.h"
#include "Context.h"
#include "log.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <include/core/SkCanvas.h>

namespace p3
{

    namespace
    {
        auto const whitef = ImVec4(1, 1, 1, 1);
        auto const redf = ImVec4(1, 0, 0, 1);
        auto const greenf = ImVec4(0, 1, 0, 1);
    }

    void RenderLayer::init_frame(Context& context)
    {
        _object_count = 0;
        context.push_render_layer(*this);
        ImGuiWindow& window = *ImGui::GetCurrentWindow();

        auto const& imgui_context = *ImGui::GetCurrentContext();
        auto const& frame_padding = imgui_context.Style.FramePadding;

        auto content_min = ImGui::GetWindowContentRegionMin();
        auto content_max = ImGui::GetWindowContentRegionMax();

        //
        // the content can overlap the frame spacing when scrolled.
        // we thus have a viewport height and a content region
        auto content_width = content_max.x - content_min.x;
        auto content_height = content_max.y - content_min.y;

        _requested_width = std::uint32_t(content_width + 2 * frame_padding.x + 0.5f);
        _requested_height = std::uint32_t(content_height + 2 * frame_padding.y + 0.5f);

        if (!_render_target)
            _dirty = true;
        else if (_requested_width != _render_target->width() || _requested_height != _render_target->height())
            _dirty = true;

        Viewport viewport{
            ImGui::GetScrollX(),
            ImGui::GetScrollY(),
            content_width,
            content_height
        };

        //
        // check if it was scrolled
        if (viewport != _viewport)
        {
            _dirty = true;
            _viewport = viewport;
        }
    }

    SkCanvas& RenderLayer::use(RenderBackend& backend)
    {
        if (!_dirty)
            throw std::runtime_error("only invalidated render targets can be used");
        ++_object_count;
        if (!_render_target
            || _requested_width != _render_target->width()
            || _requested_height != _render_target->height())
        {
            if (_render_target)
                backend.delete_render_target(_render_target);
            _render_target = backend.create_render_target(_requested_width, _requested_height);
            log_info("created render target {}x{}", _render_target->width(), _render_target->height());
        }
        // .. dirty .. bind
        auto& canvas = *_render_target->skia_surface()->getCanvas();
        _render_target->bind();
        canvas.clear(0x0000000);
        return canvas;
    }

    void RenderLayer::_draw_debug()
    {
        static auto const whiteu = ImGui::GetColorU32(whitef);
        static auto const redu = ImGui::GetColorU32(redf);
        static auto const greenu = ImGui::GetColorU32(greenf);

        auto const& imgui_context = *ImGui::GetCurrentContext();
        auto const& frame_padding = imgui_context.Style.FramePadding;

        auto& window = *ImGui::GetCurrentWindow();
        ImVec2 p1(window.Pos.x + frame_padding.x, window.Pos.y + frame_padding.y);
        ImVec2 p2(p1.x + _viewport[2], p1.y + _viewport[3]);
        window.DrawList->AddRect(p1, p2, _render_target ? greenu : redu, 0, 0, 2);
    }

    void RenderLayer::finish_frame(Context& context)
    {
        context.pop_render_layer();
        if (!_render_target)
        {
            if (context.show_render_layers())
                _draw_debug();
            return;
        }

        if (_dirty)
        {
            _render_target->skia_surface()->flushAndSubmit();
            _render_target->release();
            if (_object_count == 0)
            {
                context.render_backend().delete_render_target(_render_target);
                if (context.show_render_layers())
                    _draw_debug();
                _render_target = nullptr;
                return;
            }
            _dirty = false;
        }

        //
        // always draw rect, if fbo is present
        static auto const zero2D = ImVec2(0.f, 0.f);
        static auto const ones2D = ImVec2(1.f, 1.f);

        auto& window = *ImGui::GetCurrentWindow();

        ImVec2 p1(
            window.Pos.x,
            window.Pos.y
        );
        ImVec2 p2(
            p1.x + float(_render_target->width()),
            p1.y + float(_render_target->height())
        );
        auto const whiteu = ImGui::GetColorU32(whitef);
        window.DrawList->AddImage(_render_target->texture_id(),
            p1, p2,
            zero2D, ones2D,
            whiteu);
        if (context.show_render_layers())
            _draw_debug();
    }

    void RenderLayer::set_dirty()
    {
        _dirty = true;
    }

}
