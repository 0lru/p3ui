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

        Surface(std::uint32_t width = 640, std::uint32_t height = 480);

        StyleStrategy& style_strategy() const override;

        void set_width(std::uint32_t);
        std::uint32_t width() const;

        void set_height(std::uint32_t);
        std::uint32_t height() const;

        // sets automatic width/height values
        void update_content() override;

        void render_impl(Context& context, float width, float height) final override;

        std::optional<py::object> picture() const { return _skia_picture; }

        // python context for accessing the canvas of the surface
        py::object enter();
        void exit(py::args);

        void set_on_click(OnClick);
        OnClick on_click() const;

    private:
        std::uint32_t _width;
        std::uint32_t _height;
        bool _is_dirty = false;
        std::shared_ptr<p3::RenderTarget> _render_target;
        py::object _skia;
        py::object _skia_recorder;
        std::optional<py::object> _skia_context;
        std::optional<py::object> _skia_target;
        std::optional<py::object> _skia_picture;
        std::optional<py::object> _skia_surface;
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

    Surface::Surface(std::uint32_t width, std::uint32_t height)
        : Node("Surface")
        , _width(width)
        , _height(height)
    {
        _skia = py::module::import("skia");
        _skia_recorder = _skia.attr("PictureRecorder")();
    }

    void Surface::update_content()
    {
        _automatic_height = float(_height);
        _automatic_width = float(_width);
    }

    void Surface::set_width(std::uint32_t width)
    {
        _width = width;
        set_needs_update();
    }

    std::uint32_t Surface::width() const
    {
        return _width;
    }

    void Surface::set_height(std::uint32_t height)
    {
        _height = height;
        set_needs_update();
    }

    std::uint32_t Surface::height() const
    {
        return _height;
    }

    void Surface::render_impl(Context& context, float width, float height)
    {
        if (width * height < 0 || !_skia_picture)
            return;
        {
            py::gil_scoped_acquire acquire;
            if (!_skia_context)
            {
                _skia_context = _skia.attr("GrDirectContext").attr("MakeGL")();
                log_debug("created skia context");
            }

            if (!_render_target ||
                _width != _render_target->width() ||
                _height != _render_target->height())
            {
                _render_target = context.render_backend().create_render_target(_width, _height);
                _render_target->bind();
                auto framebuffer_info = _skia.attr("GrGLFramebufferInfo")(
                    _render_target->framebuffer_id(), GL_RGBA8);
                _skia_target = _skia.attr("GrBackendRenderTarget")(
                    _width, _height, 0, 0, framebuffer_info);
                auto origin = _skia.attr("GrSurfaceOrigin").attr("kTopLeft_GrSurfaceOrigin");
                auto color_type = _skia.attr("ColorType").attr("kRGBA_8888_ColorType");
                auto color_space = _skia.attr("ColorSpace").attr("MakeSRGB")();
                auto make_surface = _skia.attr("Surface").attr("MakeFromBackendRenderTarget");
                _skia_surface = make_surface(_skia_context, _skia_target, origin, color_type, color_space, nullptr);
            }

            //
            // draw recorded picture onto skia surface
            if (_is_dirty)
            {
                _render_target->bind();
                glClearColor(0.f, 0.f, 0.f, 0.f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                auto canvas = _skia_surface.value().attr("getCanvas")();
                // if nothing is saved, that'll reset the matrices
                canvas.attr("restore")();
                canvas.attr("drawPicture")(_skia_picture);
                _skia_context.value().attr("flush")();
                _render_target->release();
                _is_dirty = false;
            }
        }
        ImVec2 size(width, height);
        ImGui::Image(_render_target->texture_id(), size);
        if (ImGui::IsItemClicked() && _on_click && !disabled())
            postpone([f=_on_click]() {
                f();
            });

        update_status();
    }

    py::object Surface::enter()
    {
        auto skia = py::module::import("skia");
        return _skia_recorder.attr("beginRecording")(_width, _height);
    }

    void Surface::exit(py::args)
    {
        // need to finalize recording with gil held
        std::optional<py::object> recording = _skia_recorder.attr("finishRecordingAsPicture")();
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

        surface.def(py::init<>([](std::uint32_t width, std::uint32_t height, py::kwargs kwargs) {
            auto surface = std::make_shared<Surface>(width, height);
            ArgumentParser<p3::Node>()(kwargs, *surface);
            assign(kwargs, "on_click", *surface, &Surface::set_on_click);
            return surface;
        }), py::arg("width") = 640, py::arg("height") = 480);

        def_property(surface, "width", &Surface::width, &Surface::set_width);
        def_property(surface, "height", &Surface::width, &Surface::set_height);
        def_property(surface, "on_click", &Surface::on_click, &Surface::set_on_click);
        surface.def("__enter__", &Surface::enter);
        surface.def("__exit__", &Surface::exit);
        surface.def_property_readonly("picture", &Surface::picture);
    }

}
