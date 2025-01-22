#include "entity/ShipEntity.h"

#include "entity/InfiniteTerrainEntity.h"

#include <nsm/util/JsonHelpers.h>
#include <nsm/entity/Scene.h>

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>

NSM_REGISTER_ENTITY(ShipEntity);

ShipEntity::ShipEntity(nsm::Entity::Properties&)
    : mTransform(nullptr)
    , mModel(nullptr)
    , mCollider(nullptr)
{ }

void ShipEntity::onCreate(nsm::Entity::Properties& properties) {
    mTransform = new nsm::TransformComponent();
    mTransform->setPosition(nsm::JsonHelpers::getVec3(properties, "position"));
    mTransform->setScale(glm::vec3(nsm::JsonHelpers::getFloat(properties, "scale")));
    this->addComponent<nsm::TransformComponent>(mTransform);

    mModel = new nsm::ModelComponent("models/ship.glb", "main");
    this->addComponent<nsm::DrawableComponent>(mModel);

    mCollider = new nsm::SphereColliderComponent(this, 40.0f, mTransform, [this](nsm::ColliderComponent* other) {
        const std::string identifier = other->getOwner()->getIdentifier();

        if (identifier == "MissileEntity" || identifier == "PlayerEntity") {
            const auto& entities = mScene->getEntities();
            for (auto& entity : entities) {
                if (entity->getIdentifier() == "InfiniteTerrainEntity") {
                    static_cast<InfiniteTerrainEntity*>(entity)->removeSelfBoat(this);
                }
            }

            this->kill();
        }
    });
    this->addComponent<nsm::SphereColliderComponent>(mCollider);

    this->updateModel();
}

void ShipEntity::onUpdate(const f64) {
    this->updateModel();
}

void ShipEntity::updateModel() {
    glm::mat4 mtx = glm::mat4(1.0f);
    mtx = glm::translate(mtx, mTransform->getPosition());
    mtx = glm::rotate(mtx, mTransform->getRotation().y, glm::vec3(0.0f, 1.0f, 0.0f));
    mtx = glm::scale(mtx, mTransform->getScale());

    mModel->setTransformAll(mtx);
}
