#pragma once

#include <nsm/entity/component/drawablecomponent.h>
#include <nsm/gfx/shader.h>
#include <nsm/gfx/texturecubemap.h>

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
