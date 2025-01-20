#include <nsm/entity/Entity.h>
#include <nsm/entity/component/TransformComponent.h>
#include <nsm/entity/component/ModelComponent.h>

#include "entity/TerrainEntity.h"
#include "entity/PlayerEntity.h"

#include <nsm/util/JsonHelpers.h>
#include <nsm/entity/Scene.h>

#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

#include <list>

class InfiniteTerrainEntity final : public nsm::Entity {
public:
    InfiniteTerrainEntity(nsm::Entity::Properties& properties);
    ~InfiniteTerrainEntity() override = default;

    void onCreate(nsm::Entity::Properties& properties) override;
    void onUpdate(const f64 timeStep) override;

private:
    static constexpr glm::vec3 cCreatePosition = glm::vec3(286.0f, -79.3f, 0.0f);
    static constexpr f32 cCreateScale = 18.3f;
    static constexpr glm::vec3 cCreateRotation = glm::vec3(0.0f, 90.0f, 0.0f);

    void spawnChild(const f32 xMultiplier);

private:
    std::list<TerrainEntity*> mChildren;
    nsm::ModelComponent* mWater;
    PlayerEntity* mPlayer;
    bool mGameOver;
};

NSM_REGISTER_ENTITY(InfiniteTerrainEntity);

InfiniteTerrainEntity::InfiniteTerrainEntity(nsm::Entity::Properties& properties)
    : mChildren()
    , mWater(nullptr)
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

    this->spawnChild(1.0f);
    this->spawnChild(2.0f);
    this->spawnChild(3.0f);
}

void InfiniteTerrainEntity::onUpdate(const f64 timeStep) {
    if (mGameOver) {
        return;
    }

    static constexpr f32 cSpeed = 512.0f;

    bool spawn = false;

    for (TerrainEntity* child : mChildren) {
        if (child == nullptr) {
            continue;
        }
        
        glm::vec3 position = child->getTransform()->getPosition();
        position.x -= cSpeed * mPlayer->getThrust() * static_cast<f32>(timeStep);
        child->getTransform()->setPosition(position);

        if (position.x < (cCreatePosition.x - 1173.0f)) {
            spawn = true;
            child->kill();
        }
    }

    mChildren.remove_if([](TerrainEntity* child) {
        return !child->isAlive();
    });

    if (spawn) {
        this->spawnChild(3.0f);
    }
}

void InfiniteTerrainEntity::spawnChild(const f32 xMultiplier) {
    mChildren.push_back(nullptr);
    const std::string newProps = R"({
        "position": [)" + std::to_string(cCreatePosition.x + 1173.0f * (xMultiplier-1)) + ", " + std::to_string(cCreatePosition.y) + ", " + std::to_string(cCreatePosition.z) + R"(],
        "scale": )" + std::to_string(cCreateScale) + R"(,
        "rotation": [)" + std::to_string(cCreateRotation.x) + ", " + std::to_string(cCreateRotation.y) + ", " + std::to_string(cCreateRotation.z) + R"(]
    })";
    mScene->spawnEntity("TerrainEntity", newProps, reinterpret_cast<nsm::Entity**>(&mChildren.back()));
}
