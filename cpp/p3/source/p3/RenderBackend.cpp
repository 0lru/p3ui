#include "RenderBackend.h"

#include <algorithm>

namespace p3
{

    void RenderBackend::gc()
    {
        for (auto texture : _deleted_textures)
        {
            // 20: std::erase_if(_textures, [&](auto& it) { return it.get() == texture; });
            _textures.erase(std::remove_if(_textures.begin(), _textures.end(),
                [&](auto& it) { return it.get() == texture; }), _textures.end());
        }
        _deleted_textures.clear();
        for (auto render_target : _deleted_render_targets)
            // 20: std::erase_if(_render_targets, [&](auto& it) { return it.get() == render_target; });
            _render_targets.erase(std::remove_if(_render_targets.begin(), _render_targets.end(),
                [&](auto& it) { return it.get() == render_target; }), _render_targets.end());
        _deleted_render_targets.clear();
        for (auto& t : _tasks)
            t();
        _tasks.clear();
    }

    void RenderBackend::shutdown()
    {
        _skia_context.reset();
        gc();
        _textures.clear();
        _render_targets.clear();
    }

    void RenderBackend::delete_texture(Texture* texture)
    {
        _deleted_textures.push_back(texture);
    }

    void RenderBackend::delete_render_target(RenderTarget* render_target)
    {
        _deleted_render_targets.push_back(render_target);
    }

    void RenderBackend::exec(std::function<void()>&& task)
    {
        _tasks.push_back(std::move(task));
    }

}
