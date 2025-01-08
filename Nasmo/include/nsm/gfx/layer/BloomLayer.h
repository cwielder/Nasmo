#pragma once

#include <nsm/gfx/layer/Layer.h>
#include <nsm/gfx/Shader.h>
#include <nsm/gfx/Framebuffer.h>

namespace nsm {

    class Viewport;

    class BloomLayer final : public Layer {
    public:
        static constexpr u32 cPasses = 9; // TODO: Fix crash when resolution is too low for this number of passes
    
    public:
        BloomLayer(const std::string& name);
        ~BloomLayer() override = default;

        /**
         * @note Expects the thresholded image in the second (index 1) texture buffer of the framebuffer.
         */
        void draw(const RenderInfo& renderInfo);
        
        void resize(const glm::u32vec2& size) override;

    private:
        void downsample(const RenderInfo& renderInfo, Viewport& viewport);
        void upsample(const RenderInfo& renderInfo, Viewport& viewport);
        void blit(const RenderInfo& renderInfo, Viewport& viewport);

        Framebuffer mFramebuffers[cPasses];
        ShaderProgram mShaderDownsample, mShaderUpsample, mShaderBlit;
    };

}
