#pragma once

namespace nsm {

    class Framebuffer;
    class CameraComponent;

    struct RenderInfo {
        const CameraComponent* camera;
        const Framebuffer* framebuffer;
    };

}
