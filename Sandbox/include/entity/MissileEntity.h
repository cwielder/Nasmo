#pragma once

#include <nsm/entity/Entity.h>
#include <nsm/entity/component/ModelComponent.h>
#include <nsm/entity/component/TransformComponent.h>
#include <nsm/entity/component/ParticleComponent.h>
#include <nsm/entity/component/LightComponent.h>
#include <nsm/entity/component/SphereColliderComponent.h>
#include <nsm/util/RandomSource.h>

class MissileEntity final : public nsm::Entity {
public:
    MissileEntity(nsm::Entity::Properties& properties);
    ~MissileEntity() override = default;

    void onCreate(nsm::Entity::Properties& properties) override;
    void onUpdate(const f64 timeStep) override;

private:
    void explode();

private:
    nsm::TransformComponent* mTransform;
    nsm::ModelComponent* mModel;
    nsm::ParticleComponent* mExhaustParticle;
    nsm::SphereColliderComponent* mCollider;
    nsm::PointLightComponent* mExhaustLight;

    f32 mDirection;
    glm::vec3 mAcceleration;
    glm::vec3 mVelocity;

    f32 mLifeTime;
    nsm::RandomSource mRandom;
};
