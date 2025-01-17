#pragma once

#include <nsm/entity/Entity.h>
#include <nsm/entity/component/ModelComponent.h>
#include <nsm/entity/component/TransformComponent.h>
#include <nsm/entity/component/ParticleComponent.h>
#include <nsm/entity/component/InputComponent.h>
#include <nsm/entity/component/AudioComponent.h>
#include <nsm/entity/component/SphereColliderComponent.h>
#include <nsm/audio/SoundHandle.h>
#include <nsm/util/RandomSource.h>

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
    nsm::TransformComponent* mTransform;
    nsm::ModelComponent* mModel;
    nsm::ParticleComponent* mExhaustParticleLeft;
    nsm::ParticleComponent* mExhaustParticleRight;
    nsm::InputComponent* mInput;
    nsm::AudioComponent* mAudio;
    nsm::SphereColliderComponent* mCollider;

    std::function<void()> mCameraShootCallback;

    nsm::RandomSource mRandom;

    bool mLeftPressed = false, mRightPressed = false;
    f32 mAcceleration = 0.0f;
    f32 mVelocity = 0.0f;
};
