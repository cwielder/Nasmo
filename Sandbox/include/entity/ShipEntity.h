#pragma once

#include <nsm/entity/Entity.h>

#include <nsm/entity/component/TransformComponent.h>
#include <nsm/entity/component/ModelComponent.h>
#include <nsm/entity/component/SphereColliderComponent.h>

class ShipEntity final : public nsm::Entity {
public:
    ShipEntity(nsm::Entity::Properties& properties);
    ~ShipEntity() override = default;

    void onCreate(nsm::Entity::Properties& properties) override;
    void onUpdate(const f64 timeStep) override;

private:
    void updateModel();

private:
    nsm::TransformComponent* mTransform;
    nsm::ModelComponent* mModel;
    nsm::SphereColliderComponent* mCollider;
};
