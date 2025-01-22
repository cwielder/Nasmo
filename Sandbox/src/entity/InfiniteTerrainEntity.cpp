#include "entity/InfiniteTerrainEntity.h"

#include "entity/TerrainEntity.h"
#include "entity/PlayerEntity.h"
#include "entity/ShipEntity.h"

#include <nsm/util/JsonHelpers.h>
#include <nsm/entity/Scene.h>

#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

NSM_REGISTER_ENTITY(InfiniteTerrainEntity);

InfiniteTerrainEntity::InfiniteTerrainEntity(nsm::Entity::Properties& properties)
    : mWater(nullptr)
    , mPlayer(nullptr)
    , mGameOver(false)
{ }

void InfiniteTerrainEntity::onCreate(nsm::Entity::Properties& properties) {
    mWater = new nsm::ModelComponent("models/water.glb", "main");
    this->addComponent<nsm::DrawableComponent>(mWater);

    const auto& entities = mScene->getEntities();
    for (nsm::Entity* entity : entities) {
        if (entity->getIdentifier() == "PlayerEntity") {
            mPlayer = static_cast<PlayerEntity*>(entity);

            mPlayer->addDieCallback([this]() {
                mGameOver = true;
            });

            break;
        }
    }

    glm::mat4 mtx = glm::mat4(1.0f);
    mtx = glm::translate(mtx, cCreatePosition + glm::vec3(0.0f, -5.0f, 0.0f));
    mtx = glm::rotate(mtx, glm::radians(cCreateRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    mtx = glm::rotate(mtx, glm::radians(cCreateRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    mtx = glm::rotate(mtx, glm::radians(cCreateRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    mtx = glm::scale(mtx, glm::vec3(cCreateScale));
    mWater->setTransformAll(mtx);

    for (float i = 1.0f; i <= 10.0f; i += 1.0f) {
        this->spawnTerrainChunk(i);
    }
}

void InfiniteTerrainEntity::onUpdate(const f64 timeStep) {
    if (mGameOver) {
        return;
    }

    static constexpr f32 cSpeed = 512.0f;

    bool spawn = false;

    for (nsm::Entity* child : mTerrainChunks) {
        if (child == nullptr) {
            continue;
        }
        
        glm::vec3 position = child->getComponents<nsm::TransformComponent>()[0]->getPosition();
        position.x -= cSpeed * mPlayer->getThrust() * static_cast<f32>(timeStep);
        child->getComponents<nsm::TransformComponent>()[0]->setPosition(position);

        if (position.x < (cCreatePosition.x - 1173.0f)) {
            spawn = true;
            child->kill();
        }
    }

    mTerrainChunks.remove_if([](nsm::Entity* child) {
        return !child->isAlive();
    });

    if (spawn) {
        this->spawnTerrainChunk(10.0f);
    }

    mBoatTimer -= static_cast<f32>(timeStep);
    if (mBoatTimer <= 0.0f) {
        mBoatTimer = mRandom.getF32(1.0f, 3.0f);
        this->spawnBoat();
    }

    mBoats.remove_if([](nsm::Entity* boat) {
        return (boat != nullptr) && !boat->isAlive();
    });

    for (nsm::Entity* boat : mBoats) {
        if (boat == nullptr) {
            continue;
        }

        glm::vec3 position = boat->getComponents<nsm::TransformComponent>()[0]->getPosition();
        position.x -= cSpeed * mPlayer->getThrust() * static_cast<f32>(timeStep);
        boat->getComponents<nsm::TransformComponent>()[0]->setPosition(position);
    }
}

void InfiniteTerrainEntity::removeSelfBoat(nsm::Entity* entity) {
    mBoats.remove(entity);
}

void InfiniteTerrainEntity::spawnTerrainChunk(const f32 xMultiplier) {
    mTerrainChunks.push_back(nullptr);
    const std::string newProps = R"({
        "position": [)" + std::to_string(cCreatePosition.x + 1173.0f * (xMultiplier-1)) + ", " + std::to_string(cCreatePosition.y) + ", " + std::to_string(cCreatePosition.z) + R"(],
        "scale": )" + std::to_string(cCreateScale) + R"(,
        "rotation": [)" + std::to_string(cCreateRotation.x) + ", " + std::to_string(cCreateRotation.y) + ", " + std::to_string(cCreateRotation.z) + R"(]
    })";
    mScene->spawnEntity("TerrainEntity", newProps, &mTerrainChunks.back());
}

void InfiniteTerrainEntity::spawnBoat() {
    const f32 zPos = mRandom.getF32(-45.0f, 52.0f);

    mBoats.push_back(nullptr);
    const std::string newProps = R"({
        "position": [)" + std::to_string(1111.0f) + ", " + std::to_string(-64.0f) + ", " + std::to_string(zPos) + R"(],
        "scale": )" + std::to_string(8.5f) + R"(
    })";
    mScene->spawnEntity("ShipEntity", newProps, &mBoats.back());
}
