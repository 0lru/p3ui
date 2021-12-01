/***************************************************************************//*/
  Copyright (c) 2021 Martin Rudoff

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
/******************************************************************************/

#pragma once

#include <string>
#include <memory>

#include <glad/gl.h>
#include <imgui.h>
#include <imgui_internal.h>

#include <p3/Node.h>
#include <p3/Context.h>
#include <p3/RenderBackend.h>

#include "p3ui.h"

namespace p3::python
{

    class Surface
        : public p3::Node
    {
    public:
        using OnClick = std::function<void()>;

        Surface();
        ~Surface();
        StyleStrategy& style_strategy() const override;
        void update_content() override;
        void render_impl(Context& context, float width, float height) final override;

        std::optional<py::object> picture() const { return _skia_picture; }

        // context manager: enter
        py::object enter();

        // context manager: exit
        void exit(py::args);

        void set_on_click(OnClick);
        OnClick on_click() const;

    protected:
        void dispose() override;

    private:
        void _draw_textured_rectangle();

        //
        // true whenever picture changed
        bool _is_dirty = false;

        float _last_scroll_x = 0.f;
        float _last_scroll_y = 0.f;

        //
        // remember backend where context was created
        std::shared_ptr<RenderBackend> _render_backend;
        std::optional<py::object> _skia_context;

        RenderBackend::RenderTarget* _render_target = nullptr;
        std::optional<py::object> _skia_target;
        std::optional<py::object> _skia_picture;
        std::optional<py::object> _skia_surface;

        //
        // this is just set in between enter & leave (context manager)
        std::optional<py::object> _recorder;

        OnClick _on_click;
    };

    namespace
    {
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
        if (_skia_context)
        {
            log_debug("abandon skia context");
            _skia_context.value().attr("abandonContext")();
            _skia_context.reset();
        }
    }

    void Surface::dispose()
    {
        //
        // dispose is always initiated with the gil held and (currently) with the mutex held
        // if the picture is deleted, we need to abandon the context instead
        _skia_picture.reset();
        _skia_surface.reset();
        _skia_target.reset();
        if (_skia_context)
        {
            _render_backend->exec([context = std::move(_skia_context)]() mutable {
                log_verbose("free picture");
                py::gil_scoped_acquire acquire;
                // -> releaseResourcesAndAbandonContext ??
                context.value().attr("freeGpuResources")();
                context.reset();
                log_verbose("picture freed");
            });
            _skia_context.reset();
        }
        _on_click = nullptr;
        Node::dispose();
    }

    void Surface::update_content()
    {
        _automatic_width = 1.f;
        _automatic_height = 1.f;
    }

    void Surface::_draw_textured_rectangle()
    {
        static auto zero2D = ImVec2(0.f, 0.f);
        static auto ones2D = ImVec2(1.f, 1.f);
        static auto whitef = ImVec4(1, 1, 1, 1);
        static auto whiteu = ImGui::GetColorU32(whitef);

        if (!_render_target)
            return;
        auto& window = *ImGui::GetCurrentWindow();
        auto const& padding = window.WindowPadding;
        auto const& spacing = ImGui::GetCurrentContext()->Style.ItemInnerSpacing;

        ImVec2 p1(padding.x, padding.y);

        ImVec2 p2(
            p1.x + float(_render_target->width()),
            p1.y + float(_render_target->height())
        );
        window.DrawList->AddImage(_render_target->texture_id(),
            p1, p2,
            zero2D, ones2D,
            whiteu);
    }

    void Surface::render_impl(Context& context, float fwidth, float fheight)
    {
        //
        // nothing todo.. leave
        if (fwidth * fheight <= 0 || !_skia_picture)
            return;

        //
        // use rounded width/height for fbo
        auto width = std::uint32_t(fwidth + 0.5f);
        auto height = std::uint32_t(fheight + 0.5f);

        //
        // set virtual node width/height
        auto cursor = ImGui::GetCursorPos();

        //
        //
        auto const& imgui_context = *ImGui::GetCurrentContext();
        auto const& item_spacing = imgui_context.Style.ItemSpacing;

        auto content_min = ImGui::GetWindowContentRegionMin();
        auto content_max = ImGui::GetWindowContentRegionMax();

        //
        // the content can overlap the item spacing if parent is scrolled..
        auto viewport_width = std::uint32_t(content_max.x - content_min.x + 2 * item_spacing.x);
        auto viewport_height = std::uint32_t(content_max.y - content_min.y + 2 * item_spacing.y);

        //
        // 1) no context
        _is_dirty = _is_dirty || !_skia_context;

        //
        // 2) render target does not fit
        bool render_target_dirty = !_render_target
            || viewport_width != _render_target->width()
            || viewport_height != _render_target->height();

        _is_dirty = _is_dirty || render_target_dirty;

        //
        // 3) was scrolled
        auto scroll_x = ImGui::GetScrollX();
        if (_last_scroll_x != scroll_x)
        {
            _is_dirty = true;
            _last_scroll_x = scroll_x;
        }

        auto scroll_y = ImGui::GetScrollY();
        if (_last_scroll_y != scroll_y)
        {
            _is_dirty = true;
            _last_scroll_y = scroll_y;
        }

        //
        // only acquire gil if needed
        if (_is_dirty)
        {
            py::gil_scoped_acquire acquire;
            auto skia = py::module::import("skia");

            if (!_skia_context)
            {
                _skia_context = skia.attr("GrDirectContext").attr("MakeGL")();
                _render_backend = context.render_backend().shared_from_this();
                log_verbose("created skia context");
            }

            if (render_target_dirty)
            {
                //
                // free existing resources
                if (_render_target)
                    context.render_backend().delete_render_target(_render_target);
                //
                // create..
                _render_target = nullptr;
                _render_target = context.render_backend().create_render_target(viewport_width, viewport_height);
                _render_target->bind();
                auto framebuffer_info = skia.attr("GrGLFramebufferInfo")(
                    _render_target->framebuffer_id(), GL_RGBA8);
                _skia_target = skia.attr("GrBackendRenderTarget")(
                    viewport_width, viewport_height, 0, 0, framebuffer_info);
                auto origin = skia.attr("GrSurfaceOrigin").attr("kTopLeft_GrSurfaceOrigin");
                auto color_type = skia.attr("ColorType").attr("kRGBA_8888_ColorType");
                auto color_space = skia.attr("ColorSpace").attr("MakeSRGB")();
                auto make_surface = skia.attr("Surface").attr("MakeFromBackendRenderTarget");
                _skia_surface = make_surface(_skia_context, _skia_target, origin, color_type, color_space, nullptr);
                log_verbose("created render target {}x{}", viewport_width, viewport_height);
            }

            //
            // draw to fbo
            // log_info("draw {}x{} - {}x{}", width, height, fwidth, fheight);
            _render_target->bind();
            ImGuiWindow* window = GImGui->CurrentWindow;
            auto p1 = cursor;
            p1.x -= scroll_x;
            p1.x += window->WindowPadding.x - window->WindowBorderSize - item_spacing.x; // really?
            p1.y -= scroll_y;
            p1.y += window->WindowPadding.y - window->WindowBorderSize - item_spacing.y; // really?
            ImVec2 p2{ p1.x + float(width), p1.y + float(height) };

            glClearColor(0.f, 0.f, 0.5f, 0.2f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            auto canvas = _skia_surface.value().attr("getCanvas")();
            canvas.attr("save")();
            auto clip_rect = skia.attr("Rect").attr("MakeXYWH")(p1.x, p1.y,
                width, height);
            canvas.attr("clipRect")(clip_rect, false);
            canvas.attr("translate")(p1.x, p1.y);
            canvas.attr("drawPicture")(_skia_picture);
            canvas.attr("restore")();
            _skia_context.value().attr("flushAndSubmit")();
            _is_dirty = false;
            _render_target->release();
        }

        _draw_textured_rectangle();

        //
        // advance cursor
        cursor.x += float(width);
        cursor.y += float(height);
        ImGui::SetCursorPos(cursor);
    }

    py::object Surface::enter()
    {
        auto skia = py::module::import("skia");
        // auto inf = skia.attr("SK_ScalarInfinity");
        auto inf = std::numeric_limits<float>::max();
        _recorder = skia.attr("PictureRecorder")();
        return _recorder.value().attr("beginRecording")(inf, inf);
    }

    void Surface::exit(py::args)
    {
        // need to finalize recording with gil held
        std::optional<py::object> recording = _recorder.value().attr("finishRecordingAsPicture")();
        _recorder.reset();
        {
            //
            // skia painting is done with acquired gil in the render thread
            // need to release the gil before the lock of the node..
            py::gil_scoped_release release;
            {
                auto guard = lock();
                std::swap(recording, _skia_picture);
                _is_dirty = true;
                redraw();
            }
        }
        // need to reset with gil
        recording.reset();
    }

    void Surface::set_on_click(OnClick on_click)
    {
        _on_click = on_click;
    }

    Surface::OnClick Surface::on_click() const
    {
        return _on_click;
    }

    void Definition<Surface>::apply(py::module& module)
    {
        py::class_<Surface, p3::Node, std::shared_ptr<Surface>> surface(module, "Surface");

        surface.def(py::init<>([](py::kwargs kwargs) {
            auto surface = std::make_shared<Surface>();
            ArgumentParser<p3::Node>()(kwargs, *surface);
            assign(kwargs, "on_click", *surface, &Surface::set_on_click);
            return surface;
        }));

        def_property(surface, "on_click", &Surface::on_click, &Surface::set_on_click);
        surface.def("__enter__", &Surface::enter);
        surface.def("__exit__", &Surface::exit);
        def_property_readonly(surface, "picture", &Surface::picture);
    }

}
