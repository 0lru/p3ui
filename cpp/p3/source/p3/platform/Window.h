#include "Monitor.h"
#include "Timer.h"
#include "event_loop.h"

#include <p3/Context.h>
#include <p3/Node.h>
#include <p3/Theme.h>

#include <imgui.h>

#include <atomic>
#include <chrono>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <thread>

struct GLFWwindow;

namespace p3 {

class EventLoop;
class ChildWindow;
class MenuBar;
class Popup;
class RenderBackend;

class Window
    : public Node,
      public EventLoop::Observer {
public:
    class TaskQueue;

    using MousePosition = std::array<double, 2>;
    using UpdateCallback = std::function<void(std::shared_ptr<Window>)>;
    using Seconds = std::chrono::duration<double>;

    struct Position {
        int x;
        int y;
    };
    struct Size {
        int width;
        int height;
    };

    Window(std::string title, std::size_t width, std::size_t height);
    ~Window();

    //
    // EventLoop::Observer implementation
    void on_work_processed(EventLoop&) override;

    void set_title(std::string);
    std::string title() const;

    void set_user_interface(std::shared_ptr<UserInterface>);
    std::shared_ptr<UserInterface> user_interface() const;

    void frame();
    bool closed() const;

    std::optional<VideoMode> video_mode() const;
    void set_video_mode(std::optional<VideoMode>);

    Monitor monitor() const;
    Monitor primary_monitor() const;
    std::vector<Monitor> monitors() const;

    Position position() const;
    void set_position(Position);

    Size size() const;
    void set_size(Size);

    Size framebuffer_size() const;

    void set_vsync(bool);
    bool vsync() const;

    void set_idle_timeout(std::optional<Seconds>);
    std::optional<Seconds> idle_timeout() const;

    void set_idle_frame_time(Seconds);
    Seconds idle_frame_time() const;

    double frames_per_second() const;
    double time_till_enter_idle_mode() const;

    void redraw() override;
    void set_needs_update() override final;

    std::shared_ptr<Window const> shared_ptr() const;

    using CloseCallback = std::function<void()>;
    void set_close_callback(CloseCallback);

    using RenderScope = std::function<void(std::function<void()>)>;
    void set_render_scope(RenderScope);

private:
    RenderScope _render_scope = [](std::function<void()> f) { f(); };
    bool _vsync = true;
    std::string _title;

    std::shared_ptr<RenderBackend> _render_backend;

    struct
    {
        MousePosition mouse { 0.f, 0.f };
        Size framebuffer_size;
    } _window_state;

    mutable Position _position { 10, 10 };
    mutable Size _size { 1024, 768 };

    std::shared_ptr<GLFWwindow> _glfw_window = nullptr;
    std::shared_ptr<UserInterface> _user_interface = nullptr;

    Timer _frame_timer;
    Timer _idle_timer;
    std::optional<Seconds> _idle_timeout = std::nullopt;
    Seconds _idle_frame_time = Seconds(1);

private:
    static void GlfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void GlfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void GlfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void GlfwCharCallback(GLFWwindow* window, unsigned int c);
    static void GlfwFramebufferSizeCallback(GLFWwindow* window, int, int);
    static void GlfwCursorPosCallback(GLFWwindow* window, double, double);
    static void GlfwWindowCloseCallback(GLFWwindow* window);

    struct KeyReleaseEvent {
        int key;
        int scancode;
        int modifications;
    };
    std::vector<KeyReleaseEvent> _key_release_events;
    std::shared_ptr<EventLoop> _event_loop;
    CloseCallback _close_callback;
};

}
