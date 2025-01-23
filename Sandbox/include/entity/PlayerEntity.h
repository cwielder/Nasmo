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
    void addDieCallback(const std::function<void()>& callback) { mDieCallbacks.push_back(callback); }

    [[nodiscard]] f32 getThrust() const { return mThrust; }

private:
    void spawnMissile();
    void explode();

private:
    nsm::TransformComponent* mTransform;
    nsm::ModelComponent* mModel;
    nsm::ParticleComponent* mExhaustParticleLeft;
    nsm::ParticleComponent* mExhaustParticleRight;
    nsm::InputComponent* mInput;
    nsm::AudioComponent* mAudio;
    nsm::SphereColliderComponent* mCollider;

    std::function<void()> mCameraShootCallback;
    std::vector<std::function<void()>> mDieCallbacks;

    nsm::RandomSource mRandom;

    bool mLeftPressed = false, mRightPressed = false;
    bool mForwardPressed = false, mBackwardPressed = false;
    f32 mAcceleration = 0.0f;
    f32 mVelocity = 0.0f;
    f32 mThrust = 0.7f;
    f32 mShootCooldown = 0.0f;
};
