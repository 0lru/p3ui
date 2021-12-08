
#pragma once

#include <string>
#include <functional>

#include "Node.h"
#include "Texture.h"

namespace p3
{

    class Image 
        : public Node
        , public Texture::Observer
    {
    public:
        using OnClick = std::function<void()>;

        Image();
        ~Image();

        StyleStrategy& style_strategy() const override;
        void render_impl(Context&, float width, float height) override;
        void update_content() override;

        void set_texture(std::shared_ptr<Texture>);
        std::shared_ptr<Texture> texture() const;

        void set_scale(double);
        double scale() const;

        void set_on_click(OnClick);
        OnClick on_click() const;

        void on_texture_resized() override;

        virtual void synchronize_with(Synchronizable&) override;

    protected:
        void dispose() override;

    private:
        std::shared_ptr<Texture> _texture = nullptr;
        double _scale = 1.;
        OnClick _on_click;
    };

}
