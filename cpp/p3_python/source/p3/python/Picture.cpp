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

    class Picture
        : public p3::Node
    {
    public:
        using OnClick = std::function<void()>;

        Picture();

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
        void _prepare_render_target(p3::RenderBackend&, std::uint32_t width, std::uint32_t height);
        void _draw_textured_rectangle();
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

    StyleStrategy& Picture::style_strategy() const
    {
        return _style_strategy;
    }

    Picture::Picture()
        : Node("Picture")
    {
        _skia = py::module::import("skia");
        _skia_recorder = _skia.attr("PictureRecorder")();
    }

    void Picture::dispose()
    {
        _on_click = nullptr;
        Node::dispose();
    }

    void Picture::update_content()
    {
        _automatic_height = 1.f;
        _automatic_width = 1.f;
    }

    void Picture::_render_image(std::uint32_t width, std::uint32_t height)
    {
        ImVec2 size(static_cast<float>(width), static_cast<float>(height));
        ImGui::Image(_render_target->texture_id(), size);
        if (ImGui::IsItemClicked() && _on_click && !disabled())
            postpone([f = _on_click]() {
            f();
        });

        update_status();
    }

    void Picture::_draw_textured_rectangle()
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

    void Picture::render_impl(Context& context, float fwidth, float fheight)
    {
        //
        // nothing todo.. leave
        if (fwidth * fheight <= 0 || !_skia_picture)
            return;
        auto width = std::uint32_t(fwidth + 0.5f);
        auto height = std::uint32_t(fheight + 0.5f);
        //
        // compute render target size & viewport rect
        auto content_min = ImGui::GetWindowContentRegionMin();
        auto content_max = ImGui::GetWindowContentRegionMax();
        auto scroll_x = ImGui::GetScrollX();
        auto scroll_y = ImGui::GetScrollY();
        auto const& imgui_content = *ImGui::GetCurrentContext();
        ImGuiWindow* window = GImGui->CurrentWindow;

        auto cursor = ImGui::GetCursorPos();
        auto p1 = ImGui::GetCursorPos();
        auto const& imgui_context = *ImGui::GetCurrentContext();
        auto const& item_spacing = imgui_context.Style.ItemSpacing;
        p1.x -= scroll_x;
        p1.x += window->WindowPadding.x - window->WindowBorderSize - item_spacing.x; // really?
        p1.y -= scroll_y;
        p1.y += window->WindowPadding.y - window->WindowBorderSize - item_spacing.y; // really?
        ImVec2 p2{ p1.x + float(width), p1.y + float(height) };
        cursor.x += float(width);
        cursor.y += float(height);
        // set virtual width/height
        ImGui::SetCursorPos(cursor);

        _is_dirty = true;
        {
            py::gil_scoped_acquire acquire;
            //
            // assure that render target has same size, recreate whenever needed
            auto viewport_width = std::uint32_t(content_max.x - content_min.x + 2 * item_spacing.x);
            auto viewport_height = std::uint32_t(content_max.y - content_min.y + 2 * item_spacing.y);
            _prepare_render_target(context.render_backend(), viewport_width, viewport_height);
            _render_target->bind();
            if (_is_dirty && _skia_picture)
            {
                glClearColor(0.f, 0.f, 0.f, 0.2f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                auto canvas = _skia_surface.value().attr("getCanvas")();
                canvas.attr("restore")();
                canvas.attr("save")();
                
                auto clip_rect = _skia.attr("Rect").attr("MakeXYWH")(p1.x, p1.y, 
                    width, height);
                canvas.attr("clipRect")(clip_rect, false);
                canvas.attr("translate")(p1.x, p1.y);
                canvas.attr("drawPicture")(_skia_picture);
                _skia_context.value().attr("flush")();
            }
        }
        //
        // bind default render target
        _render_target->release();
        _draw_textured_rectangle();
    }

    void Picture::_prepare_render_target(p3::RenderBackend& backend, std::uint32_t width, std::uint32_t height)
    {
        if (!_skia_context)
        {
            _skia_context = _skia.attr("GrDirectContext").attr("MakeGL")();
            log_debug("created skia context");
        }

        if (!_render_target || width != _render_target->width() || height != _render_target->height())
        {
            _skia_context = _skia.attr("GrDirectContext").attr("MakeGL")();
//            if (_render_target)
//                backend.delete_render_target(_render_target); // do we need that?
            _render_target = backend.create_render_target(width, height);
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
    }

    py::object Picture::enter()
    {
        auto skia = py::module::import("skia");
        // auto inf = skia.attr("SK_ScalarInfinity");
        auto inf = std::numeric_limits<float>::max();
        return _skia_recorder.attr("beginRecording")(inf, inf);
    }

    void Picture::exit(py::args)
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

    void Picture::set_on_click(OnClick on_click)
    {
        _on_click = on_click;
    }

    Picture::OnClick Picture::on_click() const
    {
        return _on_click;
    }

    void Definition<Picture>::apply(py::module& module)
    {
        py::class_<Picture, p3::Node, std::shared_ptr<Picture>> surface(module, "Picture");

        surface.def(py::init<>([](py::kwargs kwargs) {
            auto surface = std::make_shared<Picture>();
            ArgumentParser<p3::Node>()(kwargs, *surface);
            assign(kwargs, "on_click", *surface, &Picture::set_on_click);
            return surface;
        }));

        def_property(surface, "on_click", &Picture::on_click, &Picture::set_on_click);
        surface.def("__enter__", &Picture::enter);
        surface.def("__exit__", &Picture::exit);
        surface.def_property_readonly("picture", &Picture::picture);
    }

}
