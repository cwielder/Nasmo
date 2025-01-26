#include "entity/FuelEntity.h"

#include <glm/gtc/matrix_transform.hpp>
#include <nsm/util/JsonHelpers.h>
#include <nsm/entity/Scene.h>

#include "entity/InfiniteTerrainEntity.h"


NSM_REGISTER_ENTITY(FuelEntity);

FuelEntity::FuelEntity(nsm::Entity::Properties&)
    : mTransform(nullptr)
    , mModel(nullptr)
    , mCollider(nullptr)
{ }

void FuelEntity::onCreate(nsm::Entity::Properties& properties) {
    mTransform = new nsm::TransformComponent();
    mTransform->setPosition(nsm::JsonHelpers::getVec3(properties, "position"));
    mTransform->setRotation(glm::vec3(mRandom.getF32(0.0f, 70.0f), mRandom.getF32(0.0f, 70.0f), 270.0f));
    mTransform->setScale(glm::vec3(nsm::JsonHelpers::getFloat(properties, "scale")));
    this->addComponent<nsm::TransformComponent>(mTransform);

    mModel = new nsm::ModelComponent("models/fuel.glb", "main");
    this->addComponent<nsm::DrawableComponent>(mModel);
    
    mCollider = new nsm::SphereColliderComponent(this, 30.0f, mTransform, [this](nsm::ColliderComponent* other) {
        const std::string identifier = other->getOwner()->getIdentifier();

        if (identifier == "MissileEntity" || identifier == "PlayerEntity") {
            const auto& entities = mScene->getEntities();
            for (auto& entity : entities) {
                if (entity->getIdentifier() == "InfiniteTerrainEntity") {
                    static_cast<InfiniteTerrainEntity*>(entity)->removeSelfFuel(this);
                }
            }

            this->kill();
        }
    });
    this->addComponent<nsm::SphereColliderComponent>(mCollider);

    this->updateModel();
}

void FuelEntity::onUpdate(const f64) {
    this->updateModel();
}

void FuelEntity::updateModel() {
    glm::mat4 mtx = glm::mat4(1.0f);
    mtx = glm::translate(mtx, mTransform->getPosition());
    mtx = glm::rotate(mtx, glm::radians(mTransform->getRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
    mtx = glm::rotate(mtx, glm::radians(mTransform->getRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
    mtx = glm::rotate(mtx, glm::radians(mTransform->getRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
    mtx = glm::scale(mtx, mTransform->getScale());

    mModel->setTransformAll(mtx);
}

