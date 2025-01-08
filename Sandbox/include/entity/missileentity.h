#pragma once

#include <nsm/entity/entity.h>
#include <nsm/entity/component/modelcomponent.h>
#include <nsm/entity/component/transformcomponent.h>
#include <nsm/entity/component/particlecomponent.h>
#include <nsm/entity/component/lightcomponent.h>

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
