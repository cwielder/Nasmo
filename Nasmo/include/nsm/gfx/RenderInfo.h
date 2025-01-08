#pragma once

namespace nsm {

    class Framebuffer;
    class CameraComponent;

    struct RenderInfo final {
        CameraComponent* camera;
        const Framebuffer* framebuffer;
    };

}
