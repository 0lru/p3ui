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
        Surface(std::uint32_t width = 640, std::uint32_t height = 480);

        StyleStrategy& style_strategy() const override;

        void set_width(std::uint32_t);
        std::uint32_t width() const;

        void set_height(std::uint32_t);
        std::uint32_t height() const;

        // force update in ui-thread (initiate call to render_impl)
        void update();

        // sets automatic width/height values
        void update_content() override;

        void render_impl(Context& context, float width, float height) final override;

    private:
        std::uint32_t _width;
        std::uint32_t _height;
        bool _is_dirty = true;
        std::shared_ptr<p3::RenderTarget> _render_target;
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

    void Surface::update()
    {
        set_needs_update();
    }

    void Surface::render_impl(Context& context, float width, float height)
    {
        if (width * height < 0)
            return;
        if (!_render_target || _width != _render_target->width() || _height != _render_target->height())
            _render_target = context.render_backend().create_render_target(_width, _height);
//        ImVec2 size(width, height);
//        auto id = reinterpret_cast<ImTextureID>(_texture->use(context));
//        ImGui::Image(id, size);
//        update_status();
    }

    void Definition<Surface>::apply(py::module& module)
    {
        py::class_<Surface, p3::Node, std::shared_ptr<Surface>> surface(module, "Surface");

        surface.def(py::init<>([](std::uint32_t width, std::uint32_t height, py::kwargs kwargs) {
            auto surface = std::make_shared<Surface>(width, height);
            ArgumentParser<p3::Node>()(kwargs, *surface);
            return surface;
        }), py::arg("width") = 640, py::arg("height") = 480);

        def_property(surface, "width", &Surface::width, &Surface::set_width);
        def_property(surface, "height", &Surface::width, &Surface::set_height);
    }

}
