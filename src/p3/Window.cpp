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

#include "log.h"
#include "ChildWindow.h"
#include "Font.h"
#include "Menu.h"
#include "MenuBar.h"
#include "Popup.h"
#include "Window.h"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl2.h>
#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <imgui.h>
#include <imgui_internal.h>
#include <ranges>


namespace
{

    static void error_callback(int error, const char* description)
    {
        fprintf(stderr, "Error: %s\n", description);
    }

}

namespace p3
{

    Window::Window(std::shared_ptr<Context> context)
        : Node("Window")
        , _context(std::move(context))
        , _theme{ Theme::make_default() }
    {
        _theme->add_observer(this);
        _theme_observer = OnScopeExit([this, theme=_theme]() {
            theme->remove_observer(this);
        });
        if (!glfwInit())
            throw std::runtime_error("could not init glfw");
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

        glfwSetErrorCallback(error_callback);
        _glfw_window = std::shared_ptr<GLFWwindow>(
            glfwCreateWindow(_width, _height, _title.c_str(), nullptr/*glfwGetPrimaryMonitor()*/, nullptr),
            glfwDestroyWindow);
        if (!_glfw_window)
        {
            glfwTerminate();
            throw std::runtime_error("failed to create glfw window");
        }

        glfwMakeContextCurrent(_glfw_window.get());
        glfwSwapInterval(1); // Enable vsync
        gladLoadGL(glfwGetProcAddress);
        ImGui_ImplGlfw_InitForOpenGL(_glfw_window.get(), true);
        ImGui_ImplOpenGL2_Init();
    }

    Window::~Window()
    {
        glfwTerminate();
    }

    void Window::init_styling()
    {
    }

    void Window::set_title(std::string title)
    {
        _title = std::move(title);
    }

    const std::string& Window::title() const
    {
        return _title;
    }

    void Window::set_content(std::shared_ptr<Node> content)
    {
        if (_content)
            Node::remove(_content);
        _content = std::move(content);
        if (_content)
            Node::add(_content);
    }

    std::shared_ptr<Node> Window::content() const
    {
        return _content;
    }

    void Window::add(std::shared_ptr<ChildWindow> child_window)
    {
        _child_windows.push_back(child_window);
        Node::add(child_window);
    }

    void Window::add(std::shared_ptr<Popup> popup)
    {
        _popups.push_back(popup);
        Node::add(popup);
    }

    void Window::update_content()
    {
    }

    void Window::render_impl(float, float)
    {
        std::optional<OnScopeExit> theme_guard;
        if (_theme_apply_function)
            theme_guard = _theme_apply_function();
        ImGuiWindowFlags flags =
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoDecoration;
        if (_menu_bar)
        {
            flags = flags | ImGuiWindowFlags_MenuBar;
        }
        ImGui::Begin("Window", 0, flags);
        if (_menu_bar)
        {
            if (ImGui::BeginMenuBar())
            {
                for (auto& menu : _menu_bar->children())
                    menu->render(0, 0);
                ImGui::EndMenuBar();
            }
        }
        auto content_region = ImGui::GetContentRegionAvail();
        if (_content)
            _content->render(content_region.x, content_region.y);
        for (auto& child_window : _child_windows)
            child_window->render(
                child_window->width(content_region.x),
                child_window->height(content_region.y));
        _popups.erase(std::remove_if(_popups.begin(), _popups.end(), [&](auto& popup) {
            popup->render(popup->width(content_region.x), popup->height(content_region.y));
            return !popup->opened();
        }), _popups.end());
        ImGui::End();
        _context->execute_postponed();
        ImGui::Render();
    }

    void Window::loop(UpdateCallback update_callback)
    {
        while (!glfwWindowShouldClose(_glfw_window.get()))
        {
            ImGui_ImplOpenGL2_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            auto mouse_position = _context->mouse_position();
            if (_mouse_position[0] != mouse_position[0] ||
                _mouse_position[1] != mouse_position[1])
            {
                _context->set_mouse_delta(std::array<float, 2>{
                    mouse_position[0] - _mouse_position[0],
                        mouse_position[1] - _mouse_position[1]
                });
                std::swap(_mouse_position, mouse_position);
            }
            else
            {
                _context->set_mouse_delta(std::optional<std::array<float, 2>>());
            }

            update_restyle();

            ImGui::SetNextWindowPos(ImVec2(.0f, .0f), ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(static_cast<float>(_width), static_cast<float>(_height)), ImGuiCond_Always);

            if (update_callback)
                update_callback(std::static_pointer_cast<Window>(shared_from_this()));

            render(0, 0);

            glfwGetFramebufferSize(_glfw_window.get(), &_width, &_height);
            glViewport(0, 0, _width, _height);
            ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(_glfw_window.get());
            glfwPollEvents();
        }

        _glfw_window = nullptr;
        glfwTerminate();
        exit(EXIT_SUCCESS);
    }

    void Window::set_menu_bar(std::shared_ptr<MenuBar> menu_bar)
    {
        if (_menu_bar)
            Node::remove(_menu_bar);
        _menu_bar = std::move(menu_bar);
        if (_menu_bar)
            Node::add(_menu_bar);
    }

    std::shared_ptr<MenuBar> const& Window::menu_bar() const
    {
        return _menu_bar;
    }

    void Window::set_theme(std::shared_ptr<Theme> theme)
    {
        std::swap(theme, _theme);
        _theme_observer.reset();
        if (_theme)
        {
            _theme->add_observer(this);
            _theme_observer = OnScopeExit([this, theme=_theme]() {
                theme->remove_observer(this);
            });
        }
        set_needs_restyle();
    }

    std::shared_ptr<Theme> const& Window::theme() const
    {
        return _theme;
    }

    void Window::on_theme_changed()
    {
        log_verbose("-style- theme changed");
        set_needs_restyle();
    }

    void Window::update_restyle(Context& context, bool force_recompute)
    {
        if (needs_update() && (needs_restyle() || force_recompute))
        {
            log_debug("restyling window");
            if (_theme)
                _theme_apply_function = _theme->compile(context);
            else
                _theme_apply_function = nullptr;
        }
        std::optional<OnScopeExit> theme_guard;
        if (_theme_apply_function)
            theme_guard = _theme_apply_function();
        Node::update_restyle(context, force_recompute);
    }

    void Window::update_restyle()
    {
        _context->set_theme(_theme.get());
        update_restyle(*_context, false);
    }

}