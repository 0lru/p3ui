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

    namespace skia
    {

        std::mutex mutex;
        std::unordered_map<RenderBackend const*, py::object> context_map;

        py::object make_context(RenderBackend const& render_backend)
        {
            std::lock_guard<std::mutex> l(mutex);
            if (auto it = context_map.find(&render_backend); it != context_map.end())
                return it->second;
            auto context = py::module::import("skia").attr("GrDirectContext").attr("MakeGL")();
            log_debug("created skia context");
            skia::context_map[&render_backend] = context;
            return context;
        }

        void cleanup()
        {
            for (auto& [k, v] : context_map)
                v.attr("abandonContext")();
            context_map.clear();
        }

    }

    class Surface
        : public p3::Node
    {
    public:
        using Viewport = std::array<double, 4>;
        using OnClick = std::function<void()>;
        using OnViewport = std::function<void(Viewport)>;

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

        Viewport const& viewport() const;
        void set_on_viewport(OnViewport);
        OnViewport on_viewport() const;

    protected:
        void dispose() override;

    private:
        void _draw_textured_rectangle();

        //
        // true whenever picture changed
        bool _is_dirty = false;

        //
        // remember backend where context was created
        std::shared_ptr<RenderBackend> _render_backend;

        RenderBackend::RenderTarget* _render_target = nullptr;
        std::optional<py::object> _skia_target;
        std::optional<py::object> _skia_picture;
        std::optional<py::object> _skia_surface;
        std::optional<py::object> _skia_context;

        //
        // this is just set in between enter & leave (context manager)
        std::optional<py::object> _recorder;

        OnClick _on_click;
        Viewport _viewport;
        OnViewport _on_viewport;
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
    }

    Surface::Viewport const& Surface::viewport() const
    {
        return _viewport;
    }

    void Surface::set_on_viewport(OnViewport on_viewport)
    {
        _on_viewport = std::move(on_viewport);
    }

    Surface::OnViewport Surface::on_viewport() const
    {
        return _on_viewport;
    }

    void Surface::dispose()
    {
        log_debug("dispose surface");
        //
        // dispose is always initiated with the gil held and (currently) with the mutex held
        // if the picture is deleted, we need to abandon the context instead
        _skia_picture.reset();
        _skia_surface.reset();
        _skia_target.reset();
        _skia_context.reset();
        _on_click = nullptr;
        _on_viewport = nullptr;
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
        auto viewport_width = std::uint32_t(content_max.x - content_min.x + 2 * frame_padding.x);
        auto viewport_height = std::uint32_t(content_max.y - content_min.y + 2 * frame_padding.y);

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
            if (_on_viewport) postpone([f = _on_viewport, rect = viewport]() {
                f(rect);
            });
        }

        //
        // only acquire gil if needed
        if (_is_dirty)
        {
            py::gil_scoped_acquire acquire;
            auto skia = py::module::import("skia");

            if (!_skia_context)
                _skia_context = python::skia::make_context(context.render_backend());

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
                log_debug("created render target {}x{}", viewport_width, viewport_height);
            }

            //
            // draw to fbo
            _render_target->bind();
            glClearColor(0.f, 0.f, 0.f, 0.f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            auto canvas = _skia_surface.value().attr("getCanvas")();
            canvas.attr("save")();
            canvas.attr("translate")(
                cursor.x - ImGui::GetScrollX(),
                cursor.y - ImGui::GetScrollY());
            auto clipRect = skia.attr("Rect").attr("MakeWH")(width, height);
            canvas.attr("clipRect")(clipRect, false);
            canvas.attr("drawPicture")(_skia_picture);
            canvas.attr("restore")();
            _skia_context.value().attr("flushAndSubmit")();
            _is_dirty = false;
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
            assign(kwargs, "on_viewport", *surface, &Surface::set_on_viewport);
            return surface;
        }));

        def_property_readonly(surface, "viewport", &Surface::viewport/*, &Surface::set_on_viewport*/);
        def_property(surface, "on_click", &Surface::on_click, &Surface::set_on_click);
        def_property(surface, "on_viewport", &Surface::on_viewport, &Surface::set_on_viewport);
        surface.def("__enter__", &Surface::enter);
        surface.def("__exit__", &Surface::exit);
        def_property_readonly(surface, "picture", &Surface::picture);
    }

}
