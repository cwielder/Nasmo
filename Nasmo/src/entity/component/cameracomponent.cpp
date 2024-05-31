#include <nsm/entity/component/cameracomponent.h>

#include <nsm/gfx/graphics.h>

#include <glm/gtc/matrix_transform.hpp>

nsm::CameraComponent::CameraComponent(const glm::vec3& position, const glm::vec3& lookTarget, const glm::vec3& up)
    : EntityComponent()
    , mPosition(position)
    , mTargetLayerHash(0)
    , mIsDirty(true)
{
    this->setView(position, lookTarget, up);
}

void nsm::CameraComponent::setView(const glm::vec3& position, const glm::vec3& lookTarget, const glm::vec3& up) {
    mView = glm::lookAt(position, lookTarget, up);
    
    mPosition = position;
    mIsDirty = true;
}

glm::vec3 nsm::CameraComponent::unProject(const glm::vec2& screenPos) const {
    const glm::vec4 viewport = glm::vec4(0.0f, 0.0f, static_cast<f32>(nsm::Graphics::getFramebufferSize().x), static_cast<f32>(nsm::Graphics::getFramebufferSize().y));
    return glm::unProject(glm::vec3(screenPos, 0.0f), mView, mProjection, viewport);
}

nsm::OrthographicCameraComponent::OrthographicCameraComponent(const glm::vec3& position, const glm::vec3& lookTarget, const glm::vec3& up, const f32 top, const f32 bottom, const f32 left, const f32 right, const f32 near, const f32 far)
    : CameraComponent(position, lookTarget, up)
{
    this->setProjection(top, bottom, left, right, near, far);
}

void nsm::OrthographicCameraComponent::setProjection(const f32 top, const f32 bottom, const f32 left, const f32 right, const f32 near, const f32 far) {
    mProjection = glm::ortho(left, right, bottom, top, near, far);

    mIsDirty = true;
}
