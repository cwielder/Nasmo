#pragma once

#include <nsm/entity/Entity.h>
#include <nsm/entity/component/TransformComponent.h>
#include <nsm/entity/component/ModelComponent.h>
#include <nsm/util/RandomSource.h>

#include <list>

class PlayerEntity;

class InfiniteTerrainEntity final : public nsm::Entity {
public:
    InfiniteTerrainEntity(nsm::Entity::Properties& properties);
    ~InfiniteTerrainEntity() override = default;

    void onCreate(nsm::Entity::Properties& properties) override;
    void onUpdate(const f64 timeStep) override;

    void removeSelfBoat(nsm::Entity* entity);

private:
    static constexpr glm::vec3 cCreatePosition = glm::vec3(286.0f, -79.3f, 0.0f);
    static constexpr f32 cCreateScale = 18.3f;
    static constexpr glm::vec3 cCreateRotation = glm::vec3(0.0f, 90.0f, 0.0f);

    void spawnTerrainChunk(const f32 xMultiplier);
    void spawnBoat();

private:
    std::list<nsm::Entity*> mTerrainChunks;
    std::list<nsm::Entity*> mBoats;
    nsm::ModelComponent* mWater;
    PlayerEntity* mPlayer;
    f32 mBoatTimer = 0.0f;
    nsm::RandomSource mRandom;
    bool mGameOver;
};
