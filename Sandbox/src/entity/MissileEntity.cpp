#include "entity/MissileEntity.h"

#include <nsm/util/JsonHelpers.h>
#include <nsm/util/Color.h>
#include <nsm/entity/Scene.h>

#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>

namespace {
    static void CreateMissileExhaustParticle(nsm::ParticleComponent* ptcl) {
        ptcl->getEmitter()
            .setEmitRadius(0.9f)
            .setEmitRate(980.0f)
            .setLifeSpan(0.1f)
            .setLifeSpanVariance(0.09f)
            .setAcceleration(glm::vec3(0.0f, 0.5f, 0.0f))
            .setStartSize(glm::vec3(0.75f))
            .setEndSize(glm::vec3(0.0f, 0.0f, 1.0f))
            .setTexture("textures/exhaust_missile.png")
            .setEmission(4.0f)
            .setDepth(true)
        ;
    }
}

MissileEntity::MissileEntity(nsm::Entity::Properties& properties)
    : mTransform(nullptr)
    , mModel(nullptr)
{ }

void MissileEntity::onCreate(nsm::Entity::Properties& properties) {
    mLifeTime = 1.35f;

    f32 direction = nsm::JsonHelpers::getFloat(properties, "direction");

    mDirection = direction;
    mVelocity = glm::vec3(glm::cos(mDirection), 0.0f, -glm::sin(mDirection) + nsm::JsonHelpers::getFloat(properties, "velocity") * 10.0f) * 10.0f;
    mAcceleration = glm::vec3(12.0f, -5.81f, 0.0f) + glm::vec3(glm::cos(mDirection), 0.0f, -glm::sin(mDirection)) * 10.0f;

    mTransform = new nsm::TransformComponent();
    mTransform->setPosition(nsm::JsonHelpers::getVec3(properties, "position"));
    mTransform->setScale(glm::vec3(10.0f));
    this->addComponent<nsm::TransformComponent>(mTransform);

    mCollider = new nsm::SphereColliderComponent(this, 10.0f, mTransform, [this](nsm::ColliderComponent* other) {
        if (other->getOwner()->getIdentifier() == "ShipEntity") {
            this->explode();
        }
    });
    this->addComponent<nsm::SphereColliderComponent>(mCollider);

    mExhaustParticle = new nsm::ParticleComponent();
    mExhaustParticle->setTargetLayer("forward");
    this->addComponent<nsm::DrawableComponent>(mExhaustParticle);
    CreateMissileExhaustParticle(mExhaustParticle);

    mExhaustLight = new nsm::PointLightComponent(mTransform->getPosition(), nsm::Color{"#E83800"}.getRGBA(), 1000.0f);
    mExhaustLight->setTargetLayer("lighting_point");
    this->addComponent<nsm::DrawableComponent>(mExhaustLight);

    mModel = new nsm::ModelComponent("models/missile.glb", "main");
    this->addComponent<nsm::DrawableComponent>(mModel);

    glm::mat4 mtx = glm::mat4(1.0f);
    mtx = glm::translate(mtx, mTransform->getPosition());
    mtx = glm::rotate(mtx, mDirection + glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    mtx = glm::scale(mtx, mTransform->getScale());

    mModel->setTransformAll(mtx);
}

void MissileEntity::onUpdate(const f64 timeStep) {
    mLifeTime -= static_cast<f32>(timeStep);
    if (mLifeTime <= 0.0f) {
        this->kill();
        return;
    }

    mVelocity += mAcceleration * glm::vec3(timeStep);
    mVelocity = glm::clamp(mVelocity, -100.0f, 100.0f);

    glm::vec3 position = mTransform->getPosition();
    position += mVelocity * glm::vec3(static_cast<f32>(timeStep * 10.0f));
    mTransform->setPosition(position);

    glm::mat4 mtx = glm::mat4(1.0f);
    mtx = glm::translate(mtx, mTransform->getPosition());
    mtx = glm::rotate(mtx, mDirection + glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    mtx = glm::rotate(mtx, glm::radians(mVelocity.y), glm::vec3(1.0f, 0.0f, 0.0f));
    mtx = glm::scale(mtx, mTransform->getScale());

    mModel->setTransformAll(mtx);

    glm::vec4 exhaustOffset(0.0f, 0.0f, 1.5f, 1.0f);
    glm::vec4 exhaustPos = mtx * exhaustOffset;

    mExhaustParticle->getEmitter().setPosition(glm::vec3(exhaustPos));

    static constexpr glm::vec3 cExhaustLightOffset(0.0f, 0.0f, 2.5f);

    glm::vec4 exhaustLightPos = mtx * glm::vec4(cExhaustLightOffset, 1.0f);
    mExhaustLight->setPosition(glm::vec3(exhaustLightPos));

    if (position.z > 135.0f || position.z < -118.0f) {
        this->explode();
    }
}

void MissileEntity::explode() {
    this->kill();

    const glm::vec3& position = mTransform->getPosition();

    const std::string properties = R"({
        "position": [)" + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(position.z) + R"(],
        "scale": )" + std::to_string(0.5f) + R"(
    })";

    mScene->spawnEntity("ExplosionEntity", properties);
}

NSM_REGISTER_ENTITY(MissileEntity);
