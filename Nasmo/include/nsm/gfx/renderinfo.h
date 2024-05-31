#pragma once

namespace nsm {

    class Framebuffer;
    class CameraComponent;

    struct RenderInfo {
        CameraComponent* camera;
        const Framebuffer* framebuffer;
    };

}
