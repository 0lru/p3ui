#pragma once

#include "Font.h"
#include "Node.h"
#include "Theme.h"
#include "on_scope_exit.h"

#include <memory>
#include <optional>
#include <imgui.h>

struct ImGuiContext;
struct ImPlotContext;

namespace p3 {

class Popup;
class MenuBar;
class RenderBackend;
class Window;

class UserInterface
    : public Node,
      public Theme::Observer {
public:
    using OnChanged = std::function<void()>;

    UserInterface(std::size_t width = 1024, std::size_t height = 768);
    ~UserInterface();

    bool is_layered() const final override { return true; }

    using MousePosition = std::array<float, 2>;
    MousePosition mouse_position() const;

    // root em
    float rem() const;

    //
    // theme / styling
    void set_theme(std::shared_ptr<Theme>);
    std::shared_ptr<Theme> theme() const;
    void on_theme_changed() final override;

    //
    // aggregation
    void set_content(std::shared_ptr<Node>);
    std::shared_ptr<Node> content() const;

    void set_menu_bar(std::shared_ptr<MenuBar>);
    std::shared_ptr<MenuBar> menu_bar() const;

    //
    // fonts
    FontAtlas font_atlas();
    Font load_font(std::string const&, float size);
    Font default_font();
    void merge_font(std::string const&, float size, std::optional<float> offset = std::nullopt);
    void set_default_font(Font);

    //
    // img gui/plot context & options
    ImGuiContext& im_gui_context() const;
    ImPlotContext& im_plot_context() const;

    void set_mouse_cursor_scale(float);
    float mouse_cursor_scale() const;

    void set_anti_aliased_lines(bool);
    bool anti_aliased_lines() const;

    void set_anti_aliased_fill(bool);
    bool anti_aliased_fill() const;

    void set_curve_tessellation_tolerance(float);
    float curve_tessellation_tolerance() const;

    void set_circle_tessellation_maximum_error(float);
    float circle_tessellation_maximum_error() const;

    void render(Context&, float width, float height, bool) override;

    void set_on_active_node_changed(OnChanged);
    OnChanged on_active_node_changed() const;
    void set_active_node(std::shared_ptr<Node>);
    std::shared_ptr<Node> active_node() const;

    void add_input_character(unsigned int);
    void add_key_event(ImGuiKey_ imgui_key, bool down, std::optional<int> scancode);

protected:
    void update_content() override;
    void update_restyle(Context&, bool whole_tree = false) override;

private:
    std::weak_ptr<Node> _active_node;
    OnChanged _on_active_node_changed = nullptr;

    std::optional<p3::on_scope_exit> _theme_guard;
    Window* _window = nullptr;
    std::size_t _width = 1024;
    std::size_t _height = 768;

    std::shared_ptr<Theme> _theme;
    Theme::ApplyFunction _theme_apply_function;
    std::optional<on_scope_exit> _theme_observer;

    std::shared_ptr<Node> _content;
    std::shared_ptr<MenuBar> _menu_bar;

    std::shared_ptr<ImGuiContext> _im_gui_context;
    std::shared_ptr<ImPlotContext> _im_plot_context;
};

}
