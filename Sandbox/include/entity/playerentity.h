#pragma once

#include <nsm/entity/entity.h>
#include <nsm/entity/component/modelcomponent.h>
#include <nsm/entity/component/transformcomponent.h>
#include <nsm/entity/component/particlecomponent.h>
#include <nsm/entity/component/inputcomponent.h>
#include <nsm/util/jsonhelpers.h>

#include <glm/glm.hpp>

class PlayerEntity final : public nsm::Entity {
public:
    PlayerEntity(nsm::Entity::Properties& properties);
    ~PlayerEntity() override = default;

    void onCreate(nsm::Entity::Properties& properties) override;
    void onUpdate(const f64 timeStep) override;

    void setCameraShootCallback(const std::function<void()>& callback) { mCameraShootCallback = callback; }

private:
    void spawnMissile();

private:
    glm::vec3 mStartPosition, mStartScale;
    nsm::TransformComponent* mTransform;
    nsm::ModelComponent* mModel;
    nsm::ParticleComponent* mExhaustParticleLeft;
    nsm::ParticleComponent* mExhaustParticleRight;
    nsm::InputComponent* mInput;
    std::function<void()> mCameraShootCallback;

    bool mLeftPressed = false, mRightPressed = false;
    f32 mAcceleration = 0.0f;
    f32 mVelocity = 0.0f;
};
