#include <nsm/entity/component/cameracomponent.h>

#include <nsm/gfx/graphics.h>
#include <nsm/event/events.h>

#include <glm/gtc/matrix_transform.hpp>

// CameraComponent

nsm::CameraComponent::CameraComponent(const glm::vec3& position, const glm::vec3& lookTarget, const glm::vec3& up)
    : EntityComponent()
    , mPosition(position)
    , mTargetLayerHashes()
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

// OrthographicCameraComponent

nsm::OrthographicCameraComponent::OrthographicCameraComponent(const glm::vec3& position, const glm::vec3& lookTarget, const glm::vec3& up, const f32 top, const f32 bottom, const f32 left, const f32 right, const f32 near, const f32 far)
    : CameraComponent(position, lookTarget, up)
    , mTop(top)
    , mBottom(bottom)
    , mNear(near)
    , mFar(far)
{
    this->setProjection(top, bottom, left, right, near, far);
}

void nsm::OrthographicCameraComponent::setProjection(const f32 top, const f32 bottom, const f32 left, const f32 right, const f32 near, const f32 far) {
    mProjection = glm::ortho(left, right, bottom, top, near, far);

    mTop = top;
    mBottom = bottom;
    mNear = near;
    mFar = far;
    mIsDirty = true;
}

void nsm::OrthographicCameraComponent::onEvent(const Event* event) {
    if (event->getType() == nsm::EventType::WindowResize) {
        const nsm::WindowResizeEvent* e = static_cast<const nsm::WindowResizeEvent*>(event);

        const f32 aspectRatio = static_cast<f32>(e->getSize().x) / static_cast<f32>(e->getSize().y);
        this->setProjection(mTop, mBottom, aspectRatio, -aspectRatio, mNear, mFar);
    }
}

// PerspectiveCameraComponent

nsm::PerspectiveCameraComponent::PerspectiveCameraComponent(const glm::vec3& position, const glm::vec3& lookTarget, const glm::vec3& up, const f32 fov, const f32 aspectRatio, const f32 near, const f32 far)
    : CameraComponent(position, lookTarget, up)
    , mFov(fov)
    , mNear(near)
    , mFar(far)
{
    this->setProjection(fov, aspectRatio, near, far);
}

void nsm::PerspectiveCameraComponent::setProjection(const f32 fov, const f32 aspectRatio, const f32 near, const f32 far) {
    mProjection = glm::perspective(fov, aspectRatio, near, far);

    mFov = fov;
    mNear = near;
    mFar = far;
    mIsDirty = true;
}

void nsm::PerspectiveCameraComponent::onEvent(const Event* event) {
    if (event->getType() == nsm::EventType::WindowResize) {
        const nsm::WindowResizeEvent* e = static_cast<const nsm::WindowResizeEvent*>(event);

        const f32 aspectRatio = static_cast<f32>(e->getSize().x) / static_cast<f32>(e->getSize().y);
        this->setProjection(mFov, aspectRatio, mNear, mFar);
    }
}
