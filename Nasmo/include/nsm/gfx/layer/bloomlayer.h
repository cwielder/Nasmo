#pragma once

#include <nsm/gfx/layer/layer.h>
#include <nsm/gfx/shader.h>
#include <nsm/gfx/framebuffer.h>

namespace nsm {

    class Viewport;

    class BloomLayer final : public Layer {
    public:
        static constexpr u32 cPasses = 9;
    
    public:
        BloomLayer(const std::string& name);
        ~BloomLayer() override = default;

        void draw(const RenderInfo& renderInfo) override;
        void resize(const glm::u32vec2& size) override;

    private:
        void downsample(const RenderInfo& renderInfo, Viewport& viewport);
        void upsample(const RenderInfo& renderInfo, Viewport& viewport);
        void blit(const RenderInfo& renderInfo, Viewport& viewport);

        Framebuffer mFramebuffers[cPasses];
        ShaderProgram mShaderDownsample, mShaderUpsample, mShaderBlit;
    };

}
