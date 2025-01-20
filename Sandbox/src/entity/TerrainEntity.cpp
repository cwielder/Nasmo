#include "entity/TerrainEntity.h"

#include <nsm/util/JsonHelpers.h>

#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

NSM_REGISTER_ENTITY(TerrainEntity);

TerrainEntity::TerrainEntity(nsm::Entity::Properties& properties)
    : mTransform(nullptr)
    , mModel(nullptr)
{ }

void TerrainEntity::onCreate(nsm::Entity::Properties& properties) {
    mTransform = new nsm::TransformComponent();
    mTransform->setPosition(nsm::JsonHelpers::getVec3(properties, "position"));
    mTransform->setScale(glm::vec3(nsm::JsonHelpers::getFloat(properties, "scale")));
    mTransform->setRotation(nsm::JsonHelpers::getVec3(properties, "rotation"));
    this->addComponent<nsm::TransformComponent>(mTransform);

    mModel = new nsm::ModelComponent("models/terrainOpen.glb", "main");
    this->addComponent<nsm::DrawableComponent>(mModel);

    this->updateModel();
}

void TerrainEntity::onUpdate(const f64 timeStep) {
    this->updateModel();
}

void TerrainEntity::updateModel() {
    glm::mat4 mtx = glm::mat4(1.0f);
    mtx = glm::translate(mtx, mTransform->getPosition());
    mtx = glm::rotate(mtx, glm::radians(mTransform->getRotation().x), glm::vec3(1.0f, 0.0f, 0.0f));
    mtx = glm::rotate(mtx, glm::radians(mTransform->getRotation().y), glm::vec3(0.0f, 1.0f, 0.0f));
    mtx = glm::rotate(mtx, glm::radians(mTransform->getRotation().z), glm::vec3(0.0f, 0.0f, 1.0f));
    mtx = glm::scale(mtx, mTransform->getScale());

    mModel->setTransformAll(mtx);
}
