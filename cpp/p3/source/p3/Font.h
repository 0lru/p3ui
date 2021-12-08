
#pragma once

#include <memory>

struct ImFont;
struct ImFontAtlas;

namespace p3
{

    class UserInterface;

    class Font
    {
    public:
        Font(std::shared_ptr<UserInterface>, ImFont*);
        operator bool() const;
        float size() const;
        float scale() const;
        UserInterface const& user_interface() const;
        ImFont& native() const;

    private:
        std::shared_ptr<UserInterface> _user_interface;
        ImFont *_font;
    };

    class FontAtlas
    {
    public:
        FontAtlas(std::shared_ptr<UserInterface>, ImFontAtlas*);

        std::size_t size() const;
        Font operator[](std::size_t) const;
        
        ImFontAtlas& native() const;

    private:
        std::shared_ptr<UserInterface> _user_interface;
        ImFontAtlas* _atlas;
    };

}