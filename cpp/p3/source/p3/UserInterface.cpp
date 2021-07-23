#include "ChildWindow.h"
#include "Context.h"
#include "Font.h"
#include "Menu.h"
#include "MenuBar.h"
#include "Popup.h"
#include "UserInterface.h"
#include "log.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <implot.h>

namespace p3
{

    UserInterface::UserInterface(std::size_t width, std::size_t height)
        : Node("UserInterface")
        , _im_gui_context(ImGui::CreateContext(), &ImGui::DestroyContext)
        , _im_plot_context(ImPlot::CreateContext(), &ImPlot::DestroyContext)
    {
        set_theme(Theme::make_default());
        _im_gui_context->IO.IniFilename = nullptr;
    }

    UserInterface::~UserInterface()
    {
        _im_plot_context.reset();
        _im_gui_context.reset();
    }

    //
    // theme
    void UserInterface::set_theme(std::shared_ptr<Theme> theme)
    {
        std::swap(theme, _theme);
        _theme_observer.reset();
        if (_theme)
        {
            _theme->add_observer(this);
            // raii. theme is guarded by lambda capture
            _theme_observer = OnScopeExit([this, theme = _theme]() {
                theme->remove_observer(this);
            });
        }
        set_needs_restyle();
    }

    std::shared_ptr<Theme> const& UserInterface::theme() const
    {
        return _theme;
    }

    void UserInterface::on_theme_changed()
    {
        log_verbose("-style- theme changed");
        set_needs_restyle();
    }

    //
    // font

    FontAtlas UserInterface::font_atlas()
    {
        return FontAtlas(std::static_pointer_cast<UserInterface>(shared_from_this()), _im_gui_context->IO.Fonts);
    }

    Font UserInterface::load_font(std::string const& filename, float size)
    {
        auto im_gui_font = _im_gui_context->IO.Fonts->AddFontFromFileTTF(filename.c_str(), size);
        im_gui_font->FontSize = size;
        Font font(std::static_pointer_cast<UserInterface>(shared_from_this()), im_gui_font);
        if (_im_gui_context->IO.FontDefault == nullptr)
            set_default_font(font);
        return font;
    }

    void UserInterface::merge_font(std::string const& filename, float size)
    {
#define ICON_MIN_MDI 0xe000
#define ICON_MAX_MDI 0xeb4c
        ImFontConfig config;
        config.MergeMode = true;
        config.PixelSnapH = true;
        config.GlyphOffset.y = size / 4.0f;
        // config.OversampleH = 4;
        // config.OversampleV = 4;
        static const ImWchar icon_ranges[] = { ICON_MIN_MDI, ICON_MAX_MDI, 0 };
        _im_gui_context->IO.Fonts->AddFontFromFileTTF(filename.c_str(), size, &config, icon_ranges);
    }

    void UserInterface::set_default_font(Font font)
    {
        assert(&font.user_interface() == this);
        _im_gui_context->IO.FontDefault = &font.native();
    }

    Font UserInterface::default_font()
    {
        auto& io = _im_gui_context->IO;
        return Font(std::static_pointer_cast<UserInterface>(shared_from_this()), io.FontDefault ? io.FontDefault : io.Fonts[0].Fonts[0]);
    }

    //
    // aggregation

    void UserInterface::set_content(std::shared_ptr<Node> content)
    {
        if (_content)
            Node::remove(_content);
        _content = std::move(content);
        if (_content)
            Node::add(_content);
    }

    std::shared_ptr<Node> UserInterface::content() const
    {
        return _content;
    }

    void UserInterface::add(std::shared_ptr<ChildWindow> child_window)
    {
        _child_windows.push_back(child_window);
        Node::add(child_window);
    }

    void UserInterface::add(std::shared_ptr<Popup> popup)
    {
        _popups.push_back(popup);
        Node::add(popup);
    }

    void UserInterface::set_menu_bar(std::shared_ptr<MenuBar> menu_bar)
    {
        if (_menu_bar)
            Node::remove(_menu_bar);
        _menu_bar = std::move(menu_bar);
        if (_menu_bar)
            Node::add(_menu_bar);
    }

    std::shared_ptr<MenuBar> const& UserInterface::menu_bar() const
    {
        return _menu_bar;
    }

    void UserInterface::update_content()
    {
        // no automatic width/height needed
    }

    //
    // attributes

    UserInterface::MousePosition UserInterface::mouse_position() const
    {
        auto pos = ImGui::GetMousePos();
        return { pos.x, pos.y };
    }

    float UserInterface::rem() const
    {
        return _im_gui_context->IO.FontDefault
            ? _im_gui_context->IO.FontDefault->FontSize
            : 12.f;
    }

    void UserInterface::set_mouse_cursor_scale(float value)
    {
        ImGui::GetStyle().MouseCursorScale = value;
    }

    float UserInterface::mouse_cursor_scale() const
    {
        return ImGui::GetStyle().MouseCursorScale;
    }

    void UserInterface::set_anti_aliased_lines(bool value)
    {
        ImGui::GetStyle().AntiAliasedLines = value;
    }

    bool UserInterface::anti_aliased_lines() const
    {
        return ImGui::GetStyle().AntiAliasedLines;
    }

    ImGuiContext& UserInterface::im_gui_context() const
    {
        return *_im_gui_context;
    }

    ImPlotContext& UserInterface::im_plot_context() const
    {
        return *_im_plot_context;
    }

    void UserInterface::update_restyle(Context& context, bool force_restyle)
    {
        if (needs_update() && (needs_restyle()))
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
        Node::update_restyle(context, force_restyle);
    }

    void UserInterface::render(
        Context& context, 
        float width, 
        float height)
    {
        ImGui::NewFrame();

        // TODO: remove this
        auto& style = ImPlot::GetStyle();
        ImVec4* colors = style.Colors;
        colors[ImPlotCol_PlotBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        style.LineWeight = 3.0f;

        //
        // apply theme
        std::optional<OnScopeExit> theme_guard;
        if (_theme_apply_function)
            theme_guard = _theme_apply_function();

        //
        // make context
        ImGui::SetCurrentContext(_im_gui_context.get());
        ImPlot::SetCurrentContext(_im_plot_context.get());
        //
        // it is important that the destr. is executed after tree
        // traversal
        update_restyle(context);

        //
        // begin window
        ImGuiWindowFlags flags =
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoDecoration;
        if (_menu_bar)
            flags = flags | ImGuiWindowFlags_MenuBar;
        ImGui::SetNextWindowPos(ImVec2(.0f, .0f), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(static_cast<float>(width), static_cast<float>(height)), ImGuiCond_Always);
        ImGui::Begin("Window", 0, flags);

        //
        // draw optional menu bar
        if (_menu_bar)
        {
            if (ImGui::BeginMenuBar())
            {
                for (auto& menu : _menu_bar->children())
                    menu->render(context, 0, 0);
                ImGui::EndMenuBar();
            }
        }

        //
        // draw optional content
        auto content_region = ImGui::GetContentRegionAvail();
        if (_content)
            _content->render(context, content_region.x, content_region.y);

        //
        // draw optional child windows
        for (auto& child_window : _child_windows)
            child_window->render(
                context,
                child_window->width(content_region.x),
                child_window->height(content_region.y));

        //
        // draw optional popups
        _popups.erase(std::remove_if(_popups.begin(), _popups.end(), [&](auto& popup) {
            popup->render(context, popup->width(content_region.x), popup->height(content_region.y));
            return !popup->opened();
        }), _popups.end());

        //
        // 
        ImGui::End();

        //
        // generate draw-lists
        ImGui::Render();
    }

    void UserInterface::set_anti_aliased_fill(bool value)
    {
        ImGui::GetStyle().AntiAliasedFill = value;
    }

    bool UserInterface::anti_aliased_fill() const
    {
        return ImGui::GetStyle().AntiAliasedFill;
    }

    void UserInterface::set_curve_tessellation_tolerance(float value)
    {
        ImGui::GetStyle().CurveTessellationTol = value;
    }

    float UserInterface::curve_tessellation_tolerance() const
    {
        return ImGui::GetStyle().CurveTessellationTol;
    }

    void UserInterface::set_circle_tessellation_maximum_error(float value)
    {
        ImGui::GetStyle().CircleTessellationMaxError = value;
    }

    float UserInterface::circle_tessellation_maximum_error() const
    {
        return ImGui::GetStyle().CircleTessellationMaxError;
    }

}
