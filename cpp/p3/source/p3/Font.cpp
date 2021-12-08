
#include "Font.h"
#include "UserInterface.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace p3
{

    Font::Font(std::shared_ptr<UserInterface> user_interface, ImFont* font)
        : _user_interface(std::move(user_interface))
        , _font(font)
    {
    }

    Font::operator bool() const
    {
        return _font != nullptr;
    }

    float Font::scale() const
    {
        return _font->Scale;
    }

    UserInterface const& Font::user_interface() const
    {
        return *_user_interface;
    }

    ImFont& Font::native() const
    {
        return *_font;
    }

    float Font::size() const
    {
        return _font->FontSize;
    }

    FontAtlas::FontAtlas(std::shared_ptr<UserInterface> user_interface, ImFontAtlas* atlas)
        : _user_interface(std::move(user_interface))
        , _atlas(atlas)
    {
    }

    std::size_t FontAtlas::size() const
    {
        return static_cast<std::size_t>(_user_interface->im_gui_context().IO.Fonts->Fonts.Size);
    }

    Font FontAtlas::operator[](std::size_t index) const
    {
        return Font(_user_interface, _user_interface->im_gui_context().IO.Fonts->Fonts[static_cast<int>(index)]);
    }

    ImFontAtlas& FontAtlas::native() const
    {
        return *_atlas;
    }

}
