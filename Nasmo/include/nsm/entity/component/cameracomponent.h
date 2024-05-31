#pragma once

#include <nsm/entity/entity.h>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <string>

namespace nsm {

    class CameraComponent : public EntityComponent {
    protected:
        CameraComponent(const glm::vec3& position, const glm::vec3& lookTarget, const glm::vec3& up = { 0.0f, 1.0f, 0.0f });
        ~CameraComponent() override = default;

    public:
        void setView(const glm::vec3& position, const glm::vec3& lookTarget, const glm::vec3& up = { 0.0f, 1.0f, 0.0f });

        [[nodiscard]] const glm::mat4& getProjection() const { return mProjection; }
        [[nodiscard]] const glm::mat4& getView() const { return mView; }
        [[nodiscard]] const glm::mat4& getViewProjection() { return mIsDirty ? mViewProjection = mProjection * mView, mIsDirty = false, mViewProjection : mViewProjection; }
        [[nodiscard]] const glm::vec3& getPosition() const { return mPosition; }

        void setTargetLayer(const std::string& layerName) { mTargetLayerHash = std::hash<std::string>{}(layerName); }
        [[nodiscard]] std::size_t getTargetLayerHash() const { return mTargetLayerHash; }

        [[nodiscard]] glm::vec3 unProject(const glm::vec2& screenPos) const;

    protected:
        glm::mat4 mProjection, mView, mViewProjection;
        glm::vec3 mPosition;
        std::size_t mTargetLayerHash;
        bool mIsDirty;
    };

    class OrthographicCameraComponent final : public CameraComponent {
    public:
        OrthographicCameraComponent(const glm::vec3& position, const glm::vec3& lookTarget, const glm::vec3& up, const f32 top, const f32 bottom, const f32 left, const f32 right, const f32 near, const f32 far);
        ~OrthographicCameraComponent() override = default;

        void setProjection(const f32 top, const f32 bottom, const f32 left, const f32 right, const f32 near, const f32 far);
    };

    // TODO: Implement PerspectiveCameraComponent

}
