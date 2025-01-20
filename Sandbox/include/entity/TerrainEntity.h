#pragma once

#include <nsm/entity/Entity.h>
#include <nsm/entity/component/TransformComponent.h>
#include <nsm/entity/component/ModelComponent.h>

class TerrainEntity final : public nsm::Entity {
public:
    TerrainEntity(nsm::Entity::Properties& properties);
    ~TerrainEntity() override = default;

    void onCreate(nsm::Entity::Properties& properties) override;
    void onUpdate(const f64 timeStep) override;

    [[nodiscard]] nsm::TransformComponent* getTransform() const { return mTransform; }

private:
    void updateModel();

private:
    nsm::TransformComponent* mTransform;
    nsm::ModelComponent* mModel;
};
