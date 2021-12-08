
#include "Image.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <iostream>

namespace p3
{

    namespace
    {
        //
        // an image does not grow/shrink per default
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

    StyleStrategy& Image::style_strategy() const
    {
        return _style_strategy;
    }

    Image::Image()
        : Node("Image")
    {
    }

    Image::~Image()
    {
        if (_texture)
            _texture->remove_observer(*this);
    }

    void Image::render_impl(Context& context, float width, float height)
    {
        if (!_texture)
            return;
        ImVec2 size(width, height);
        auto id = reinterpret_cast<ImTextureID>(_texture->use(context));
        ImGui::Image(id, size);
        if (ImGui::IsItemClicked() && _on_click && !disabled())
            postpone([f=_on_click]() {
                f();
            });
        update_status();
    }

    void Image::update_content()
    {
        _automatic_height = _texture ? float(_texture->height() * _scale) : 0.f;
        _automatic_width = _texture ? float(_texture->width() * _scale) : 0.f;
    }

    void Image::set_texture(std::shared_ptr<Texture> texture)
    {
        if (_texture)
            _texture->remove_observer(*this);
        _texture = std::move(texture);
        if (_texture)
        {
            _texture->add_observer(*this);
            _texture->synchronize_with(*this);
        }
        set_needs_update();
    }

    std::shared_ptr<Texture> Image::texture() const
    {
        return _texture;
    }

    void Image::set_scale(double scale)
    {
        _scale = scale;
        set_needs_update();
    }

    double Image::scale() const
    {
        return _scale;
    }

    void Image::on_texture_resized()
    {
        Node::set_needs_update();
    }

    void Image::synchronize_with(Synchronizable& synchronizable)
    {
        Node::synchronize_with(synchronizable);
        if (_texture)
            _texture->synchronize_with(synchronizable);
    }

    void Image::dispose()
    {
        _on_click = nullptr;
        Node::dispose();
    }

    void Image::set_on_click(OnClick on_click)
    {
        _on_click = on_click;
    }

    Image::OnClick Image::on_click() const
    {
        return _on_click;
    }

}
