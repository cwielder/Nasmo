#include "entity/PlayerEntity.h"

#include <nsm/debug/Log.h>
#include <nsm/event/Events.h>
#include <nsm/util/JsonHelpers.h>
#include <nsm/entity/Scene.h>
#include <nsm/util/Color.h>

#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>

namespace {
    static void CreateB2ExhaustParticle(nsm::ParticleComponent* ptcl) {
        ptcl->getEmitter()
            .setEmitRadius(0.9f)
            .setEmitRate(980.0f)
            .setLifeSpan(0.151f)
            .setLifeSpanVariance(0.15f)
            .setInitialVelocity(glm::vec3(-64.0f, 0.0f, 0.0f))
            .setAcceleration(glm::vec3(0.0f, 0.5f, 0.0f))
            .setStartSize(glm::vec3(0.8f))
            .setEndSize(glm::vec3(0.1f, 0.1f, 1.0f))
            .setTexture("textures/exhaust_b2.png")
            .setEmission(0.75f)
            .setDepth(false)
        ;
    }
}

PlayerEntity::PlayerEntity(nsm::Entity::Properties&)
    : mTransform(nullptr)
    , mModel(nullptr)
{ }

void PlayerEntity::onCreate(nsm::Entity::Properties& properties) {
    mTransform = new nsm::TransformComponent();
    mTransform->setPosition(nsm::JsonHelpers::getVec3(properties, "position"));
    mTransform->setScale(nsm::JsonHelpers::getVec3(properties, "scale"));
    this->addComponent<nsm::TransformComponent>(mTransform);

    mModel = new nsm::ModelComponent("models/B2.glb", "main");
    this->addComponent<nsm::DrawableComponent>(mModel);

    mExhaustParticleLeft = new nsm::ParticleComponent();
    mExhaustParticleLeft->setTargetLayer("forward");
    this->addComponent<nsm::DrawableComponent>(mExhaustParticleLeft);
    CreateB2ExhaustParticle(mExhaustParticleLeft);

    mExhaustParticleRight = new nsm::ParticleComponent();
    mExhaustParticleRight->setTargetLayer("forward");
    this->addComponent<nsm::DrawableComponent>(mExhaustParticleRight);
    CreateB2ExhaustParticle(mExhaustParticleRight);

    mExhaustLight = new nsm::PointLightComponent(mTransform->getPosition(), nsm::Color{"#AE91F0"}.getRGBA(), 2000.0f);
    mExhaustLight->setTargetLayer("lighting_point");
    this->addComponent<nsm::DrawableComponent>(mExhaustLight);

    mAudio = new nsm::AudioComponent({ "SE_ENGINE", "SE_SHOOT", "SE_EXPLODE" }, mTransform);
    this->addComponent<nsm::AudioComponent>(mAudio);
    mAudio->startSound("SE_ENGINE");

    mCollider = new nsm::SphereColliderComponent(this, 10.0f, mTransform, [this](nsm::ColliderComponent* other) {
        if (other->getOwner()->getIdentifier() == "ShipEntity") {
            this->explode();
        }
    });
    this->addComponent<nsm::SphereColliderComponent>(mCollider);

    mInput = new nsm::InputComponent();
    this->addComponent<nsm::InputComponent>(mInput);
    mInput->setOnKeyPress([this](const nsm::KeyPressEvent* event) {
        switch (event->getKey()) {
            case GLFW_KEY_A:
            case GLFW_KEY_LEFT: {
                mLeftPressed = true;
                break;
            }

            case GLFW_KEY_D:
            case GLFW_KEY_RIGHT: {
                mRightPressed = true;
                break;
            }

            case GLFW_KEY_W:
            case GLFW_KEY_UP: {
                mForwardPressed = true;
                break;
            }

            case GLFW_KEY_S:
            case GLFW_KEY_DOWN: {
                mBackwardPressed = true;
                break;
            }

            case GLFW_KEY_SPACE: {
                if (mShootCooldown > 0.0f) {
                    return;
                }

                this->spawnMissile();
                mAudio->startSound("SE_SHOOT", nullptr, mRandom.getF32(1.0f, 1.3f));
                break;
            }
        }
    });

    mInput->setOnKeyRelease([this](const nsm::KeyReleaseEvent* event) {
        switch (event->getKey()) {
            case GLFW_KEY_A:
            case GLFW_KEY_LEFT: {
                mLeftPressed = false;
                break;
            }

            case GLFW_KEY_D:
            case GLFW_KEY_RIGHT: {
                mRightPressed = false;
                break;
            }

            case GLFW_KEY_W:
            case GLFW_KEY_UP: {
                mForwardPressed = false;
                break;
            }

            case GLFW_KEY_S:
            case GLFW_KEY_DOWN: {
                mBackwardPressed = false;
                break;
            }
        }
    });

    glm::mat4 mtx = glm::mat4(1.0f);
    mtx = glm::translate(mtx, mTransform->getPosition());
    mtx = glm::rotate(mtx, glm::radians(mVelocity), glm::vec3(1.0f, 1.0f, 0.0f));
    mtx = glm::scale(mtx, mTransform->getScale());

    mModel->setTransformAll(mtx);
}

void PlayerEntity::onUpdate(const f64 timeStep) {
    mShootCooldown -= static_cast<f32>(timeStep);

    static constexpr f32 cMaxVelocity = 0.1f;
    static constexpr f32 cAcceleration = 0.2f;
    static constexpr f32 cFriction = 0.1f;

    if (mLeftPressed) {
        mAcceleration = -cAcceleration;
    } else if (mRightPressed) {
        mAcceleration = cAcceleration;
    } else {
        mAcceleration = 0.0f;
    }

    if (mForwardPressed) {
        mThrust = glm::min(mThrust + timeStep, 1.0);
    } else if (mBackwardPressed) {
        mThrust = glm::max(mThrust - timeStep, 0.45);
    }

    mVelocity += mAcceleration * timeStep;
    mVelocity = glm::clamp(mVelocity, -cMaxVelocity, cMaxVelocity);

    // Friction
    if (mVelocity > 0.0f) {
        mVelocity -= cFriction * timeStep;
        mVelocity = glm::max(mVelocity, 0.0f);
    } else if (mVelocity < 0.0f) {
        mVelocity += cFriction * timeStep;
        mVelocity = glm::min(mVelocity, 0.0f);
    }

    glm::vec3 position = mTransform->getPosition();
    position += glm::vec3(0.0f, 0.0f, mVelocity * 1000.0f * static_cast<f32>(timeStep));

    mTransform->setPosition(position);

    glm::mat4 mtx = glm::mat4(1.0f);
    mtx = glm::translate(mtx, mTransform->getPosition());
    mtx = glm::rotate(mtx, glm::radians(-mVelocity * 500.0f), glm::vec3(-1.0f, 1.0f, 0.0f));
    mtx = glm::scale(mtx, mTransform->getScale());

    mModel->setTransformAll(mtx);

    static constexpr glm::vec4 cExhaustOffset(-72.0f, 0.0f, -38.4f, 1.0f);
    static f32 cExhaustSpeed = mExhaustParticleLeft->getEmitter().getInitialVelocity().x;
    static f32 cExhaustEmit = mExhaustParticleLeft->getEmitter().getEmitRate();

    f32 angle = glm::radians(-mVelocity * 500.0f);

    glm::vec4 leftExhaustPos = mtx * cExhaustOffset;
    glm::vec4 rightExhaustPos = mtx * glm::vec4(cExhaustOffset.x, cExhaustOffset.y, -cExhaustOffset.z, cExhaustOffset.w);

    mExhaustParticleLeft->getEmitter()
        .setPosition(glm::vec3(leftExhaustPos))
        .setInitialVelocity(glm::vec3(cExhaustSpeed * mThrust * glm::cos(angle), 0.0f, cExhaustSpeed * mThrust * glm::sin(angle)))
        .setEmitRate(cExhaustEmit * mThrust)
    ;
    mExhaustParticleRight->getEmitter()
        .setPosition(glm::vec3(rightExhaustPos))
        .setInitialVelocity(glm::vec3(cExhaustSpeed * mThrust, 0.0f, cExhaustSpeed * mThrust * glm::sin(angle)))
        .setEmitRate(cExhaustEmit * mThrust)
    ;

    static constexpr glm::vec3 cExhaustLightOffset(-239.0f, 0.0f, 58.8f);

    glm::vec4 exhaustLightPos = mtx * glm::vec4(cExhaustLightOffset, 1.0f);
    mExhaustLight->setPosition(glm::vec3(exhaustLightPos));

    if (position.z > 135.0f || position.z < -118.0f) {
        this->explode();
    }
}

void PlayerEntity::spawnMissile() {
    mCameraShootCallback();

    mShootCooldown = 1.0f;

    f32 direction = glm::radians(-mVelocity * 500.0f);

    glm::mat4 mtx = glm::mat4(1.0f);
    mtx = glm::translate(mtx, mTransform->getPosition());
    mtx = glm::rotate(mtx, glm::radians(-mVelocity * 500.0f), glm::vec3(-1.0f, 1.0f, 0.0f));
    mtx = glm::scale(mtx, mTransform->getScale());

    constexpr glm::vec4 cMissileOffset(120.0f, -40.0f, 0.0f, 1.0f);

    glm::vec4 leftMissilePos = mtx * cMissileOffset;

    std::string properties = R"({
        "position": [)" + std::to_string(leftMissilePos.x) + ", " + std::to_string(leftMissilePos.y) + ", " + std::to_string(leftMissilePos.z) + R"(],
        "direction": )" + std::to_string(direction) + R"(,
        "velocity": )" + std::to_string(mVelocity) + R"(
    })";

    mScene->spawnEntity("MissileEntity", properties);
}

void PlayerEntity::explode() {
    const glm::vec3& position = mTransform->getPosition();
    
    this->kill();

    mAudio->startSound("SE_EXPLODE", nullptr, mRandom.getF32(0.5f, 0.8f));

    const std::string properties = R"({
        "position": [)" + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(position.z) + R"(],
        "scale": )" + std::to_string(2.0f) + R"(
    })";

    mScene->spawnEntity("ExplosionEntity", properties);

    for (const auto& callback : mDieCallbacks) {
        callback();
    }
}

NSM_REGISTER_ENTITY(PlayerEntity);
