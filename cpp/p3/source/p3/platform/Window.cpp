#define NOMINMAX
#include "Window.h"

#define GLFW_INCLUDE_NONE
#pragma warning(push)
#pragma warning(disable : 4005)
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#pragma warning(pop)

#include "event_loop.h"

#include <backends/imgui_impl_glfw.h>
#include <p3/UserInterface.h>
#include <p3/backend/OpenGL3RenderBackend.h>
#include <p3/log.h>

#include <algorithm>
#include <future>
#include <imgui_internal.h>
#include <implot.h>
#include <thread>

// window -> eventloop : obligaroty
// eventloop -> window : weak

namespace p3 {

Window::Window(std::string title, std::size_t width, std::size_t height)
    : Node("MainWindow")
    , _render_backend(std::make_shared<OpenGL3RenderBackend>())
{
    _user_interface = std::make_shared<UserInterface>();
    Node::add(_user_interface);
    _user_interface->set_parent(this);
    ImGui::SetCurrentContext(&_user_interface->im_gui_context());
    ImPlot::SetCurrentContext(&_user_interface->im_plot_context());
    log_debug("window created");

    _event_loop = EventLoop::current();
    _event_loop->add_observer(this);

    if (!_event_loop)
        log_fatal("failed to create window, missing active event loop");

    _glfw_window = std::shared_ptr<GLFWwindow>(
        glfwCreateWindow(int(width), int(height), title.c_str(), nullptr, nullptr),
        glfwDestroyWindow);

    if (!_glfw_window)
        throw std::runtime_error("failed to create glfw window");

    glfwSetWindowUserPointer(_glfw_window.get(), this);
    glfwMakeContextCurrent(_glfw_window.get());
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(_vsync ? 1 : 0);

    glfwSetMouseButtonCallback(_glfw_window.get(), GlfwMouseButtonCallback);
    glfwSetScrollCallback(_glfw_window.get(), GlfwScrollCallback);
    glfwSetKeyCallback(_glfw_window.get(), GlfwKeyCallback);
    glfwSetCharCallback(_glfw_window.get(), GlfwCharCallback);
    glfwSetFramebufferSizeCallback(_glfw_window.get(), GlfwFramebufferSizeCallback);
    glfwSetWindowCloseCallback(_glfw_window.get(), GlfwWindowCloseCallback);

    glfwGetCursorPos(_glfw_window.get(), &_window_state.mouse[0], &_window_state.mouse[1]);
    glfwGetFramebufferSize(_glfw_window.get(), &_window_state.framebuffer_size.width, &_window_state.framebuffer_size.height);

    _render_backend->init();
    log_debug("maximum texture size: {}", _render_backend->max_texture_size());
    ImGui_ImplGlfw_InitForOpenGL(_glfw_window.get(), false);
    //
    // TODO: hook and route!
    glfwSetWindowFocusCallback(_glfw_window.get(), ImGui_ImplGlfw_WindowFocusCallback);
    glfwSetCursorEnterCallback(_glfw_window.get(), ImGui_ImplGlfw_CursorEnterCallback);
    glfwSetCursorPosCallback(_glfw_window.get(), ImGui_ImplGlfw_CursorPosCallback);
    glfwSetMonitorCallback(ImGui_ImplGlfw_MonitorCallback);
}

Window::~Window()
{
    log_debug("shutdown render backend");
    _render_backend->shutdown();
    log_debug("destroying window");
    _event_loop->remove_observer(this);
    _glfw_window.reset();
}

void Window::on_work_processed(EventLoop&)
{
    //
    // removing this can crash the application
    glfwMakeContextCurrent(_glfw_window.get());

    if (!_user_interface)
        return;
    ImGui::SetCurrentContext(&_user_interface->im_gui_context());
    ImPlot::SetCurrentContext(&_user_interface->im_plot_context());

    MousePosition mouse_position;
    glfwGetCursorPos(_glfw_window.get(), &(mouse_position[0]), &mouse_position[1]);
    Context::MouseMove mouse_move = std::nullopt;
    if (_window_state.mouse[0] != mouse_position[0] || _window_state.mouse[1] != mouse_position[1]) {
        mouse_move = std::array<float, 2> {
            float(mouse_position[0] - _window_state.mouse[0]),
            float(mouse_position[1] - _window_state.mouse[1])
        };
        std::swap(_window_state.mouse, mouse_position);
    }
    if (mouse_move)
        _idle_timer.reset();

    if (_idle_timeout) {
        if (_idle_timer.time() > _idle_timeout.value() && _frame_timer.time() < _idle_frame_time) {
            return;
        }
    }
    _frame_timer.reset();
    if (_user_interface) {
        _render_backend->new_frame();
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplGlfw_NewFrame();
        {
            _render_backend->gc(); // needs to be locked/synchonized
            Context context(*_user_interface, *_render_backend, mouse_move);
            _user_interface->render(context, float(_window_state.framebuffer_size.width), float(_window_state.framebuffer_size.height), false);
        }
        glViewport(0, 0, _window_state.framebuffer_size.width, _window_state.framebuffer_size.height);
        if (_user_interface)
            _render_backend->render(*_user_interface);
        glFlush();
        glfwSwapBuffers(_glfw_window.get());
    }
    if (!_key_release_events.empty()) {
        for (auto& e : _key_release_events)
            ImGui_ImplGlfw_KeyCallback(_glfw_window.get(), e.key, e.scancode, GLFW_RELEASE, e.modifications);
        _key_release_events.clear();
        redraw();
    }
}

void Window::set_title(std::string title)
{
    glfwSetWindowTitle(_glfw_window.get(), title.c_str());
    _title = std::move(title);
}

std::string Window::title() const
{
    return _title;
}

void Window::set_user_interface(std::shared_ptr<UserInterface> user_interface)
{
    _user_interface = std::move(user_interface);
    Node::add(_user_interface);
    _user_interface->set_parent(this);
    if (_user_interface) {
        log_debug("init imgui for opengl");
        ImGui::SetCurrentContext(&_user_interface->im_gui_context());
        ImPlot::SetCurrentContext(&_user_interface->im_plot_context());
        // _render_backend = std::make_shared < OpenGL3RenderBackend>();
        ImGui_ImplGlfw_InitForOpenGL(_glfw_window.get(), false);
        _render_backend->init();
        log_debug("done");
    }
}

std::shared_ptr<UserInterface> Window::user_interface() const
{
    return _user_interface;
}

bool Window::closed() const
{
    return glfwWindowShouldClose(_glfw_window.get());
}

Window::Size Window::framebuffer_size() const
{
    Size size;
    glfwGetFramebufferSize(_glfw_window.get(), &size.width, &size.height);
    return size;
}

double Window::frames_per_second() const
{
    return ImGui::GetIO().Framerate;
}

double Window::time_till_enter_idle_mode() const
{
    return _idle_timeout
        ? std::max(0., std::chrono::duration<double>(_idle_timeout.value() - _idle_timer.time()).count())
        : 0.;
}

std::optional<VideoMode> Window::video_mode() const
{
    auto monitor = glfwGetWindowMonitor(_glfw_window.get());
    if (monitor) {
        auto mode = glfwGetVideoMode(monitor);
        return VideoMode(monitor, mode->width, mode->height, mode->refreshRate);
    }
    return std::nullopt;
}

void Window::set_video_mode(std::optional<VideoMode> mode)
{
    auto monitor = glfwGetWindowMonitor(_glfw_window.get());
    if (mode) {
        if (!monitor) {
            glfwGetWindowPos(_glfw_window.get(), &_position.x, &_position.y);
            glfwGetWindowSize(_glfw_window.get(), &_size.width, &_size.height);
        }
        glfwSetWindowMonitor(
            _glfw_window.get(),
            mode.value().glfw_monitor(),
            0, 0,
            mode.value().width(), mode.value().height(),
            mode.value().hz());
    } else if (monitor) {
        glfwSetWindowMonitor(
            _glfw_window.get(),
            nullptr,
            _position.x, _position.y, _size.width, _size.height, 0);
    }
    glfwSwapInterval(_vsync ? 1 : 0);
}

Window::Position Window::position() const
{
    if (!glfwGetWindowMonitor(_glfw_window.get()))
        glfwGetWindowPos(_glfw_window.get(), &_position.x, &_position.y);
    return _position;
}

void Window::set_position(Position position)
{
    if (glfwGetWindowMonitor(_glfw_window.get()))
        _position = std::move(position);
    else
        glfwSetWindowPos(_glfw_window.get(), position.x, position.y);
}

Window::Size Window::size() const
{
    if (!glfwGetWindowMonitor(_glfw_window.get()))
        glfwGetWindowSize(_glfw_window.get(), &_size.width, &_size.height);
    return _size;
}

void Window::set_size(Size size)
{
    if (glfwGetWindowMonitor(_glfw_window.get()))
        _size = std::move(size);
    else
        glfwSetWindowSize(_glfw_window.get(), size.width, size.height);
}

Monitor Window::primary_monitor() const
{
    std::shared_ptr<Node const> x = shared_from_this();
    return Monitor(std::const_pointer_cast<Window>(shared_ptr()), glfwGetPrimaryMonitor());
}

std::vector<Monitor> Window::monitors() const
{
    int monitor_count;
    GLFWmonitor** glfw_monitors = glfwGetMonitors(&monitor_count);
    std::vector<Monitor> monitors(monitor_count);
    for (int i = 0; i < monitor_count; ++i)
        monitors[i] = Monitor(std::const_pointer_cast<Window>(shared_ptr()), glfw_monitors[i]);
    return monitors;
}

void Window::set_vsync(bool vsync)
{
    _vsync = vsync;
    glfwSwapInterval(_vsync ? 1 : 0);
}

bool Window::vsync() const
{
    return _vsync;
}

void Window::GlfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    self->_idle_timer.reset();
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
}

void Window::GlfwWindowCloseCallback(GLFWwindow* window)
{
    auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    log_debug("closing window");
    if (self->_close_callback) {
        self->_event_loop->call_at(EventLoop::Clock::now(), Event::create(self->_close_callback));
    }
}

void Window::GlfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    static_cast<Window*>(glfwGetWindowUserPointer(window))->_idle_timer.reset();
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
}

void Window::GlfwKeyCallback(GLFWwindow* glfw_window, int key, int scancode, int action, int mods)
{
    auto window = static_cast<Window*>(glfwGetWindowUserPointer(glfw_window));
    if (action == GLFW_RELEASE) {
        window->_key_release_events.push_back(KeyReleaseEvent { key, scancode, mods });
    } else {
        auto& events = window->_key_release_events;
        events.erase(std::remove_if(
                         events.begin(),
                         events.end(), [&](auto const& e) {
                             return e.key == key && e.scancode == scancode;
                         }),
            events.end());
        ImGui_ImplGlfw_KeyCallback(glfw_window, key, scancode, action, mods);
        window->_idle_timer.reset();
    }
}

void Window::GlfwCharCallback(GLFWwindow* window, unsigned int c)
{
    static_cast<Window*>(glfwGetWindowUserPointer(window))->_idle_timer.reset();
    ImGui_ImplGlfw_CharCallback(window, c);
}

void Window::GlfwFramebufferSizeCallback(GLFWwindow* window, int w, int h)
{
    auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    self->_window_state.framebuffer_size.width = w;
    self->_window_state.framebuffer_size.height = h;
    self->_idle_timer.reset();
}

void Window::GlfwCursorPosCallback(GLFWwindow* window, double x, double y)
{
    static_cast<Window*>(glfwGetWindowUserPointer(window))->_idle_timer.reset();
}

void Window::set_idle_timeout(std::optional<Seconds> idle_timeout)
{
    _idle_timeout = idle_timeout;
}

std::optional<Window::Seconds> Window::idle_timeout() const
{
    return _idle_timeout;
}

void Window::set_idle_frame_time(Seconds idle_frame_time)
{
    _idle_frame_time = idle_frame_time;
}

Window::Seconds Window::idle_frame_time() const
{
    return _idle_frame_time;
}

Monitor Window::monitor() const
{
    auto handle = glfwGetWindowMonitor(_glfw_window.get());
    if (handle == nullptr)
        handle = glfwGetPrimaryMonitor();
    return Monitor(std::const_pointer_cast<Window>(shared_ptr()), handle);
}

std::shared_ptr<Window const> Window::shared_ptr() const
{
    return std::static_pointer_cast<Window const>(shared_from_this());
}

bool Monitor::operator==(Monitor const& other) const
{
    auto w1 = _window.lock();
    auto w2 = other._window.lock();
    return w1 == w2 && _handle == other._handle;
}

double Monitor::dpi() const
{
    auto window = _window.lock();
    if (!window)
        return 96.0;
    int width, height;
    const GLFWvidmode* mode = glfwGetVideoMode(_handle);
    glfwGetMonitorPhysicalSize(_handle, &width, &height);
    // 25,4 = one inch in [mm]
    return width == 0
        ? 96.0
        : mode->width / (width / 25.4);
}

void Window::redraw()
{
    //
    // wake up the loop if sleeping
    glfwPostEmptyEvent();
}

void Window::set_needs_update()
{
    redraw();
}

void Window::set_close_callback(CloseCallback close_callback)
{
    if (_close_callback)
        log_fatal("can only set window close callback once for now");
    _close_callback = close_callback;
}

}
