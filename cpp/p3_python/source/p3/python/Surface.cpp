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

        Surface();

        StyleStrategy& style_strategy() const override;

        // sets automatic width/height values
        void update_content() override;

        void render_impl(Context& context, float width, float height) final override;

        std::optional<py::object> picture() const { return _skia_picture; }

        // python context for accessing the canvas of the surface
        py::object enter();
        void exit(py::args);

        void set_on_click(OnClick);
        OnClick on_click() const;

    protected:
        void dispose() override;

    private:
        void _render_image(std::uint32_t width, std::uint32_t height);

        bool _is_dirty = false;
        p3::RenderBackend::RenderTarget* _render_target;
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

    Surface::Surface()
        : Node("Surface")
    {
        _skia = py::module::import("skia");
        _skia_recorder = _skia.attr("PictureRecorder")();
    }

    void Surface::dispose()
    {
        _on_click = nullptr;
        Node::dispose();
    }

    void Surface::update_content()
    {
        _automatic_height = 1.f;
        _automatic_width = 1.f;
    }

    void Surface::_render_image(std::uint32_t width, std::uint32_t height)
    {
        ImVec2 size(static_cast<float>(width), static_cast<float>(height));
        ImGui::Image(_render_target->texture_id(), size);
        if (ImGui::IsItemClicked() && _on_click && !disabled())
            postpone([f = _on_click]() {
            f();
        });

        update_status();
    }

    void Surface::render_impl(Context& context, float fwidth, float fheight)
    {
        if (fwidth * fheight <= 0 || !_skia_picture)
            return;
        auto width = std::uint32_t(fwidth + 0.5f);
        auto height = std::uint32_t(fwidth + 0.5f);
        if (!_is_dirty && _render_target && width == _render_target->width() && height == _render_target->height())
        {
            _render_image(width, height);
            return;
        }
        {
            py::gil_scoped_acquire acquire;
            if (!_skia_context)
            {
                _skia_context = _skia.attr("GrDirectContext").attr("MakeGL")();
                log_debug("created skia context");
            }

            if (!_render_target ||
                width != _render_target->width() ||
                height != _render_target->height())
            {
                _render_target = context.render_backend().create_render_target(width, height);
                _render_target->bind();
                auto framebuffer_info = _skia.attr("GrGLFramebufferInfo")(
                    _render_target->framebuffer_id(), GL_RGBA8);
                _skia_target = _skia.attr("GrBackendRenderTarget")(
                    width, height, 0, 0, framebuffer_info);
                auto origin = _skia.attr("GrSurfaceOrigin").attr("kTopLeft_GrSurfaceOrigin");
                auto color_type = _skia.attr("ColorType").attr("kRGBA_8888_ColorType");
                auto color_space = _skia.attr("ColorSpace").attr("MakeSRGB")();
                auto make_surface = _skia.attr("Surface").attr("MakeFromBackendRenderTarget");
                _skia_surface = make_surface(_skia_context, _skia_target, origin, color_type, color_space, nullptr);
                // force render
                _is_dirty = true;
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
        _render_image(width, height);
    }

    py::object Surface::enter()
    {
        auto skia = py::module::import("skia");
        // auto inf = skia.attr("SK_ScalarInfinity");
        auto inf = std::numeric_limits<float>::max();
        return _skia_recorder.attr("beginRecording")(inf, inf);
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

        surface.def(py::init<>([](py::kwargs kwargs) {
            auto surface = std::make_shared<Surface>();
            ArgumentParser<p3::Node>()(kwargs, *surface);
            assign(kwargs, "on_click", *surface, &Surface::set_on_click);
            return surface;
        }));

        def_property(surface, "on_click", &Surface::on_click, &Surface::set_on_click);
        surface.def("__enter__", &Surface::enter);
        surface.def("__exit__", &Surface::exit);
        surface.def_property_readonly("picture", &Surface::picture);
    }

}
