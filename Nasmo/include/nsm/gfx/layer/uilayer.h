#pragma once

#include <nsm/gfx/layer/layer.h>
#include <nsm/gfx/framebuffer.h>
#include <nsm/entity/component/cameracomponent.h>

namespace nsm {

    class UILayer : public Layer {
    public:
        UILayer(const std::string& name);
        ~UILayer() override = default;

        void draw(const RenderInfo& renderInfo);

        void resize(const glm::u32vec2& size) override;

    private:
        f32 getAspectRatio() const;

        OrthographicCameraComponent mCamera;
        Framebuffer mWorkBuffer;
    };

}
