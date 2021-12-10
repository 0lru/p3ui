#include "RenderBackend.h"

class SkCanvas;

namespace p3
{

    class Node;
    class Context;

    //
    // an element that provides a render layer is usually a window
    // or a scrollarea. render layers are used to render skia content
    // or custom gl content. for the moment, we do not support tiling,
    // but it's foreseen.
    //
    // the layer is lazily initialized. in consequence, it's buffer is created
    // on first usage. The buffer will be destroyed on resize, or 
    // if it wasn't used by any children during the render traversal.
    //
    // the buffer will always as big as the content area size (the viewport
    // of the owning container element
    //
    // in debug mode, a layer will appear
    //    * green, if the buffer is in use
    //    * red, if inactive
    class RenderLayer
    {
    public:
        using Viewport = std::array<double, 4>;
        ~RenderLayer();

        void push_to(Context&);
        void pop_from_context_and_render(Context&, Node&);

        /// increases _object_count internally
        void register_object();

        bool dirty() const { return _dirty; }
        void set_dirty();

        Viewport const& viewport() const { return _viewport; }

    private:
        void _draw_debug();

        bool _dirty = true;
        std::uint32_t _object_count = 0;
        
        Viewport _viewport{ 0, 0, 0, 0 };
        std::uint32_t _requested_width = 0;
        std::uint32_t _requested_height = 0;
        
        std::shared_ptr<RenderBackend> _render_backend = nullptr;
        RenderBackend::RenderTarget* _render_target = nullptr;
    };

}
