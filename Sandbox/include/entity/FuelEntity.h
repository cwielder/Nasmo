#pragma once

#include <nsm/util/RandomSource.h>
#include <nsm/entity/Entity.h>

#include <nsm/entity/component/TransformComponent.h>
#include <nsm/entity/component/ModelComponent.h>
#include <nsm/entity/component/SphereColliderComponent.h>

class FuelEntity final : public nsm::Entity {
public:
    FuelEntity(nsm::Entity::Properties& properties);
    ~FuelEntity() override = default;

    void onCreate(nsm::Entity::Properties& properties) override;
    void onUpdate(const f64 timeStep) override;

private:
    void updateModel();

private:
    nsm::TransformComponent* mTransform;
    nsm::ModelComponent* mModel;
    nsm::SphereColliderComponent* mCollider;
    nsm::RandomSource mRandom;
};
