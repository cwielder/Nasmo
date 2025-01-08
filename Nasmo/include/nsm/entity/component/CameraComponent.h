#pragma once

#include <nsm/entity/Entity.h>

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

        void addTargetLayer(const std::string& layerName) { mTargetLayerHashes.push_back(std::hash<std::string>{}(layerName)); }
        void removeFromTargetLayer(const std::string& layerName) { mTargetLayerHashes.erase(std::remove(mTargetLayerHashes.begin(), mTargetLayerHashes.end(), std::hash<std::string>{}(layerName)), mTargetLayerHashes.end()); }

        [[nodiscard]] bool isTargetLayer(const std::size_t layerHash) const { return std::find(mTargetLayerHashes.begin(), mTargetLayerHashes.end(), layerHash) != mTargetLayerHashes.end(); }

        [[nodiscard]] glm::vec3 unProject(const glm::vec2& screenPos) const;

        void onEvent(const Event* event) override = 0;

    protected:
        glm::mat4 mProjection, mView, mViewProjection;
        glm::vec3 mPosition;
        std::vector<size_t> mTargetLayerHashes;
        bool mIsDirty;
    };

    class OrthographicCameraComponent final : public CameraComponent {
    public:
        OrthographicCameraComponent(const glm::vec3& position, const glm::vec3& lookTarget, const glm::vec3& up, const f32 top, const f32 bottom, const f32 left, const f32 right, const f32 near, const f32 far);
        ~OrthographicCameraComponent() override = default;

        void setProjection(const f32 top, const f32 bottom, const f32 left, const f32 right, const f32 near, const f32 far);

        void onEvent(const Event* event) override;

    private:
        f32 mTop, mBottom;
        f32 mNear, mFar;
    };

    class PerspectiveCameraComponent final : public CameraComponent {
    public:
        PerspectiveCameraComponent(const glm::vec3& position, const glm::vec3& lookTarget, const glm::vec3& up, const f32 fov, const f32 aspectRatio, const f32 near, const f32 far);
        ~PerspectiveCameraComponent() override = default;

        void setProjection(const f32 fov, const f32 aspectRatio, const f32 near, const f32 far);

        void onEvent(const Event* event) override;

    private:
        f32 mFov;
        f32 mNear, mFar;
    };

}
