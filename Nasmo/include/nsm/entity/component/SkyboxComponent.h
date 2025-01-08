#pragma once

#include <nsm/entity/component/DrawableComponent.h>
#include <nsm/gfx/Shader.h>
#include <nsm/gfx/TextureCubemap.h>

namespace nsm {

    class SkyboxComponent : public DrawableComponent {
    public:
        SkyboxComponent(const std::string& path, const std::string& extension = ".png");
        ~SkyboxComponent() = default;

        void drawOpaque(const RenderInfo& renderInfo) override;
        void drawTranslucent(const RenderInfo& renderInfo) override { }

    private:
        ShaderProgram mShaderProgram;
        TextureCubemap mCubemap;
    };

}
