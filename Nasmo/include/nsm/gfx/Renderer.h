#pragma once

#include <nsm/gfx/Shader.h>
#include <nsm/gfx/Framebuffer.h>
#include <nsm/gfx/RenderState.h>
#include <nsm/gfx/RenderInfo.h>
#include <nsm/gfx/Viewport.h>

#include <glm/vec2.hpp>

#include <type_traits>
#include <string>

namespace nsm {

    class Layer;
    class DrawableComponent;
    class RenderPipeline;

    class Renderer final {
    public:
        Renderer(const glm::u32vec2 &size);
        ~Renderer();

        template <typename T>
        void usePipeline() requires std::is_base_of_v<RenderPipeline, T> {
            delete mPipeline;

            mPipeline = new T();
        }

        void resize(const glm::u32vec2& size);

        void pushDrawable(DrawableComponent* drawable);
        void applyCamera(CameraComponent* camera);

        void render();

    private:
        using LayerContainer = std::vector<std::pair<std::size_t, Layer*>>; // TODO: Use a map instead of a vector for faster lookup?

        LayerContainer::iterator getLayerIterator(const std::size_t hash);

        RenderPipeline* mPipeline;
        Framebuffer mFramebuffer;
        ShaderProgram mCompositorShader;
        RenderState mRenderState;
        Viewport mViewport;
    };

}
