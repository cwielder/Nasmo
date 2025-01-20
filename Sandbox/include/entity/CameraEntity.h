#pragma once

#include <nsm/entity/Entity.h>
#include <nsm/entity/component/CameraComponent.h>
#include <nsm/entity/component/TransformComponent.h>

class PlayerEntity;

class CameraEntity : public nsm::Entity {
public:
    CameraEntity(nsm::Entity::Properties& properties);
    ~CameraEntity() override = default;

    void onCreate(nsm::Entity::Properties& properties) override;
    void onUpdate(const f64 timeStep) override;

private:
    static constexpr glm::vec3 cOffset = glm::vec3(-70.0f, 84.0f, 0.0f);
    static constexpr f32 cFollowSpeed = 1.0f;

private:
    nsm::TransformComponent* mTransform;
    nsm::CameraComponent* mCamera;
    PlayerEntity* mPlayerEntity;
    glm::vec3 mTargetPosition;
    f64 mQuakeBase = 2.0;
    f64 mQuake = 2.0;
    f64 mTime;
    glm::vec3 mLastPlayerPosition;
    bool mGameOver = false;
};
