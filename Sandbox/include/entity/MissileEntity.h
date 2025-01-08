#pragma once

#include <nsm/entity/Entity.h>
#include <nsm/entity/component/ModelComponent.h>
#include <nsm/entity/component/TransformComponent.h>
#include <nsm/entity/component/ParticleComponent.h>
#include <nsm/entity/component/LightComponent.h>

class MissileEntity final : public nsm::Entity {
public:
    MissileEntity(nsm::Entity::Properties& properties);
    ~MissileEntity() override = default;

    void onCreate(nsm::Entity::Properties& properties) override;
    void onUpdate(const f64 timeStep) override;

private:
    nsm::TransformComponent* mTransform;
    nsm::ModelComponent* mModel;
    nsm::ParticleComponent* mExhaustParticle;

    f32 mInertia;
    glm::vec3 mAcceleration;
    glm::vec3 mVelocity;

    f32 mLifeTime;
};
