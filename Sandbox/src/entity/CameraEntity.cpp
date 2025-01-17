#include "entity/CameraEntity.h"

#include "entity/PlayerEntity.h"

#include <nsm/gfx/Graphics.h>
#include <nsm/debug/Log.h>
#include <nsm/util/JsonHelpers.h>
#include <nsm/entity/Scene.h>
#include <imgui.h>

#include <numbers>

CameraEntity::CameraEntity(nsm::Entity::Properties& properties)
    : mTransform(nullptr)
    , mPlayerEntity(nullptr)
{ }

void CameraEntity::onCreate(nsm::Entity::Properties& properties) {
    const f32 aspectRatio = static_cast<f32>(nsm::Graphics::getFramebufferSize().x) / nsm::Graphics::getFramebufferSize().y;

    const std::vector<nsm::Entity*>& entities = mScene->getEntities();
    for (nsm::Entity* entity : entities) {
        if (entity->getIdentifier() == "PlayerEntity") {
            mPlayerEntity = static_cast<PlayerEntity*>(entity);

            mPlayerEntity->setCameraShootCallback([this]() {
                mQuake = 70.0;
            });

            break;
        }
    }

    NSM_ASSERT(mPlayerEntity != nullptr, "Player entity not found!");

    const glm::vec3 playerPosition = mPlayerEntity->getComponents<nsm::TransformComponent>()[0]->getPosition();
    mTargetPosition = playerPosition + cOffset;
    const glm::vec3 initialPosition = playerPosition + glm::vec3(-228.0f, 88.0f, 0.0f);

    mTransform = new nsm::TransformComponent(initialPosition);
    this->addComponent<nsm::TransformComponent>(mTransform);

    nsm::CameraComponent* cameraComponent = new nsm::PerspectiveCameraComponent(
        initialPosition,
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        45.0f,
        aspectRatio,
        0.01f,
        10000.0f
    );

    cameraComponent->addTargetLayer("main");
    cameraComponent->addTargetLayer("forward");

    this->addComponent<nsm::CameraComponent>(cameraComponent);
    mCamera = cameraComponent;
}

void CameraEntity::onUpdate(const f64 timeStep) {
    const glm::vec3 playerPosition = mPlayerEntity->getComponents<nsm::TransformComponent>()[0]->getPosition();

    mTargetPosition = playerPosition + cOffset;

    const auto noise = [](f64 x, f64(*f)(f64)) -> f64 {
        return 0.3 * (-3.2 * f(-1.3 * x) - 1.2 * f(-1.7 * std::numbers::e * x) + 1.9 * f(0.7 * std::numbers::pi * x) );
    };

    mTime += timeStep;
    f64 quakeX = noise(mTime * 25.0, glm::sin) * mQuake;
    f64 quakeY = noise(mTime * 25.0, glm::cos) * mQuake;

    mTargetPosition.z += quakeX;
    mTargetPosition.y += quakeY;

    glm::vec3 currentPosition = mTransform->getPosition();
    glm::vec3 displacement = mTargetPosition - currentPosition;
    glm::vec3 smoothedStep = displacement * (1.0f - glm::exp(-cFollowSpeed * static_cast<f32>(timeStep)));

    // Update the position
    currentPosition += smoothedStep;

    mTransform->setPosition(currentPosition);

    mQuake = glm::mix(mQuake, mQuakeBase, timeStep * 5.0);

    mCamera->setView(currentPosition, playerPosition + glm::vec3(24.0f, 0.0f, 0.0f));
}

NSM_REGISTER_ENTITY(CameraEntity);
