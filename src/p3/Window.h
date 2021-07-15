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

#include "Context.h"
#include "Node.h"
#include "Theme.h"

#include <imgui.h>

#include <memory>
#include <string>
#include <functional>

struct GLFWwindow;

namespace p3
{
    
    class ChildWindow;
    class MenuBar;
    class Popup;

    class Window 
        : public Node
        , public Theme::Observer
    {
    public:
        using UpdateCallback = std::function<void(std::shared_ptr<Window>)>;

        Window(std::shared_ptr<Context>);
        ~Window();

        void loop(UpdateCallback);

        void set_title(std::string);
        const std::string& title() const;

        void set_content(std::shared_ptr<Node>);
        std::shared_ptr<Node> content() const;

        void add(std::shared_ptr<ChildWindow>);

        void add(std::shared_ptr<Popup>);
        void remove(std::shared_ptr<Popup>());

        void update_content() override;
        void render_impl(float width, float height) override;

        void set_menu_bar(std::shared_ptr<MenuBar>);
        std::shared_ptr<MenuBar> const& menu_bar() const;

        void set_theme(std::shared_ptr<Theme>);
        std::shared_ptr<Theme> const& theme() const;

        void on_theme_changed() override;
        void update_restyle();
        void update_restyle(Context&, bool whole_tree) override;

    private:
        void init_styling();
        void update_style();

        // native glfw window
        std::shared_ptr<GLFWwindow> _glfw_window;
        std::shared_ptr<Context> _context;
        std::shared_ptr<Node> _content;
        std::vector<std::shared_ptr<Popup>> _popups;
        std::vector<std::shared_ptr<ChildWindow>> _child_windows;
        std::string _title = "p3";
        int _width = 1024, _height = 768;
        std::array<float, 2> _mouse_position = std::array<float, 2>{0.f, 0.f};
        std::shared_ptr<MenuBar> _menu_bar;
        std::shared_ptr<Theme> _theme;
        Theme::ApplyFunction _theme_apply_function;
        std::optional<OnScopeExit> _theme_observer;
    };

}
