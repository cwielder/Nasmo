#include "entity/MissileEntity.h"

#include <nsm/util/JsonHelpers.h>

#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>

namespace {
    static void CreateMissileExhaustParticle(nsm::ParticleComponent *ptcl) {
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

MissileEntity::MissileEntity(nsm::Entity::Properties &properties)
    : mTransform(nullptr)
    , mModel(nullptr)
{ }

void MissileEntity::onCreate(nsm::Entity::Properties &properties) {
    mLifeTime = 5.0f;

    f32 direction = nsm::JsonHelpers::getFloat(properties, "direction");

    mDirection = direction;
    mVelocity = glm::vec3(glm::cos(mDirection), 0.0f, -glm::sin(mDirection) + nsm::JsonHelpers::getFloat(properties, "velocity") * 10.0f) * 10.0f;
    mAcceleration = glm::vec3(12.0f, -9.81f, 0.0f) + glm::vec3(glm::cos(mDirection), 0.0f, -glm::sin(mDirection)) * 10.0f;

    mTransform = new nsm::TransformComponent();
    mTransform->setPosition(nsm::JsonHelpers::getVec3(properties, "position"));
    mTransform->setScale(glm::vec3(10.0f));
    this->addComponent<nsm::TransformComponent>(mTransform);

    mCollider = new nsm::SphereColliderComponent(this, 20.0f, mTransform, [](nsm::ColliderComponent*){
        nsm::info("Missile collided!");
    });
    this->addComponent<nsm::SphereColliderComponent>(mCollider);

    mExhaustParticle = new nsm::ParticleComponent();
    mExhaustParticle->setTargetLayer("forward");
    this->addComponent<nsm::DrawableComponent>(mExhaustParticle);
    CreateMissileExhaustParticle(mExhaustParticle);

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
        this->setAlive(false);
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
}

NSM_REGISTER_ENTITY(MissileEntity);
