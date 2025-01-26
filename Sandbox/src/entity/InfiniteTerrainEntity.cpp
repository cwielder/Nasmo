#include "entity/InfiniteTerrainEntity.h"

#include "entity/PlayerEntity.h"

#include <nsm/util/JsonHelpers.h>
#include <nsm/entity/Scene.h>
#include <nsm/gfx/Graphics.h>

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
    mWater->getModel()->forEachMaterial([](nsm::Material* material) {
        material->setShader("nsm/assets/shaders/model.vsh", "shaders/water.fsh");

        auto& uniforms = material->getUniforms();
        bool has_uTime = false;
        bool has_uScroll = false;
        for (const auto& uniform : uniforms) {
            if (uniform.name == "uTime") {
                has_uTime = true;
            } else if (uniform.name == "uScroll") {
                has_uScroll = true;
            }
        }

        if (!has_uTime) {
            nsm::Material::UniformVar uTime;
            uTime.type = nsm::Material::UniformVar::Type::Float;
            uTime.name = "uTime";
            uTime.value.f = 0.0f;
            uniforms.push_back(uTime);
        }

        if (!has_uScroll) {
            nsm::Material::UniformVar uScroll;
            uScroll.type = nsm::Material::UniformVar::Type::Float;
            uScroll.name = "uScroll";
            uScroll.value.f = 0.0f;
            uniforms.push_back(uScroll);
        }
    });

    const auto& entities = mScene->getEntities();
    for (const auto entity : entities) {
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
    static constexpr f32 cSpeed = 512.0f;

    mWater->getModel()->forEachMaterial([timeStep](nsm::Material* material) {
        for (auto& uniform : material->getUniforms()) {
            if (uniform.name == "uTime") {
                uniform.value.f += static_cast<f32>(timeStep);
            }
        }
    });

    if (mGameOver) {
        return;
    }

    mWater->getModel()->forEachMaterial([timeStep, this](nsm::Material* material) {
        for (auto& uniform : material->getUniforms()) {
            if (uniform.name == "uScroll") {
                uniform.value.f += mPlayer->getThrust() * static_cast<f32>(timeStep);
            }
        }
    });

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
    
    mFuelTimer -= static_cast<f32>(timeStep);
    if (mFuelTimer <= 0.0f) {
        mFuelTimer = mRandom.getF32(4.5f, 6.5f);
        this->spawnFuel();
    }
    
    mFuels.remove_if([](nsm::Entity* fuel) {
        return (fuel != nullptr) && !fuel->isAlive();
    });
    
    for (nsm::Entity* fuel : mFuels) {
        if (fuel == nullptr) {
            continue;
        }
        
        glm::vec3 position = fuel->getComponents<nsm::TransformComponent>()[0]->getPosition();
        position.x -= cSpeed * mPlayer->getThrust() * static_cast<f32>(timeStep);
        fuel->getComponents<nsm::TransformComponent>()[0]->setPosition(position);
    }
}

void InfiniteTerrainEntity::removeSelfBoat(nsm::Entity* entity) {
    mBoats.remove(entity);
}

void InfiniteTerrainEntity::removeSelfFuel(nsm::Entity* entity) {
    mFuels.remove(entity);
}

void InfiniteTerrainEntity::spawnTerrainChunk(const f32 xMultiplier) {
    mTerrainChunks.push_back(nullptr);
    const std::string newProps = R"({
        "position": [)" + std::to_string(cCreatePosition.x + 1165.0f * (xMultiplier-1)) + ", " + std::to_string(cCreatePosition.y) + ", " + std::to_string(cCreatePosition.z) + R"(],
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

void InfiniteTerrainEntity::spawnFuel() {
    const f32 zPos = mRandom.getF32(-45.0f, 52.0f);

    mFuels.push_back(nullptr);
    const std::string newProps = R"({
        "position": [)" + std::to_string(1111.0f) + ", " + std::to_string(-88.0f) + ", " + std::to_string(zPos) + R"(],
        "scale": )" + std::to_string(50.5f) + R"(
    })";
    mScene->spawnEntity("FuelEntity", newProps, &mFuels.back());
}
