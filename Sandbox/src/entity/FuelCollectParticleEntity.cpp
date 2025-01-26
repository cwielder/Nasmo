#include <nsm/entity/Entity.h>
#include <nsm/entity/component/TransformComponent.h>
#include <nsm/entity/component/ParticleComponent.h>
#include <nsm/util/JsonHelpers.h>
#include <nsm/entity/Scene.h>

#include <glm/gtc/matrix_transform.hpp>

// TODO: Move this to PlayerEntity so it follows

class FuelCollectParticleEntity final : public nsm::Entity {
public:
    FuelCollectParticleEntity(nsm::Entity::Properties& properties);
    ~FuelCollectParticleEntity() override = default;

    void onCreate(nsm::Entity::Properties& properties) override;
    void onUpdate(const f64 timeStep) override;

    [[nodiscard]] nsm::TransformComponent* getTransform() const { return mTransform; }

private:
    nsm::TransformComponent* mTransform;
    nsm::ParticleComponent* mParticle;
    f32 mLife = 0.0f;
};

NSM_REGISTER_ENTITY(FuelCollectParticleEntity);

FuelCollectParticleEntity::FuelCollectParticleEntity(nsm::Entity::Properties& properties)
    : mTransform(nullptr)
    , mParticle(nullptr)
{ }

void FuelCollectParticleEntity::onCreate(nsm::Entity::Properties& properties) {
    mTransform = new nsm::TransformComponent();
    mTransform->setPosition(nsm::JsonHelpers::getVec3(properties, "position"));
    mTransform->setScale(glm::vec3(nsm::JsonHelpers::getFloat(properties, "scale")));
    this->addComponent<nsm::TransformComponent>(mTransform);

    mParticle = new nsm::ParticleComponent();
    mParticle->setTargetLayer("forward");
    this->addComponent<nsm::DrawableComponent>(mParticle);

    mParticle->getEmitter()
        .setEmitRadius(mTransform->getScale().x / 2.0f)
        .setEmitRate(100.0f * mTransform->getScale().x * mTransform->getScale().x)
        .setLifeSpan(0.5f)
        .setLifeSpanVariance(0.3f)
        .setInitialVelocity(glm::vec3(25.0f * mTransform->getScale().x))
        .setInitialVelocityVariance(glm::vec3(10.0f))
        .setDirectionMode(true)
        .setStartSize(glm::vec3(0.5f * mTransform->getScale().x))
        .setEndSize(glm::vec3(0.0f))
        .setTexture("textures/oil.png")
        .setDepth(true)
        .setPosition(mTransform->getPosition())
    ;
}

void FuelCollectParticleEntity::onUpdate(const f64 timeStep) {
    mLife += static_cast<f32>(timeStep);
    if (mLife >= 0.5f) {
        mParticle->getEmitter().setEmitRate(0.0f);
    }
    if (mLife >= 1.0f) {
        this->kill();
    }
}
