#include "entity/PlayerEntity.h"

#include <nsm/debug/Log.h>
#include <nsm/event/Events.h>
#include <nsm/util/JsonHelpers.h>
#include <nsm/entity/Scene.h>

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

PlayerEntity::PlayerEntity(nsm::Entity::Properties& properties)
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

    mAudio = new nsm::AudioComponent({ "SE_ENGINE", "SE_SHOOT" }, mTransform);
    this->addComponent<nsm::AudioComponent>(mAudio);
    mAudio->startSound("SE_ENGINE");

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

            case GLFW_KEY_SPACE: {
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
        }
    });

    glm::mat4 mtx = glm::mat4(1.0f);
    mtx = glm::translate(mtx, mTransform->getPosition());
    mtx = glm::rotate(mtx, glm::radians(mVelocity), glm::vec3(1.0f, 1.0f, 0.0f));
    mtx = glm::scale(mtx, mTransform->getScale());

    mModel->setTransformAll(mtx);
}

void PlayerEntity::onUpdate(const f64 timeStep) {
    nsm::ParticleEmitter& emitter = mExhaustParticleLeft->getEmitter();

    static constexpr f32 cMaxVelocity = 0.06f;
    static constexpr f32 cAcceleration = 0.2f;
    static constexpr f32 cFriction = 0.1f;

    if (mLeftPressed) {
        mAcceleration = -cAcceleration;
    } else if (mRightPressed) {
        mAcceleration = cAcceleration;
    } else {
        mAcceleration = 0.0f;
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

    glm::vec4 leftExhaustPos = mtx * cExhaustOffset;
    glm::vec4 rightExhaustPos = mtx * glm::vec4(cExhaustOffset.x, cExhaustOffset.y, -cExhaustOffset.z, cExhaustOffset.w);

    mExhaustParticleLeft->getEmitter().setPosition(glm::vec3(leftExhaustPos));
    mExhaustParticleRight->getEmitter().setPosition(glm::vec3(rightExhaustPos));
}

void PlayerEntity::spawnMissile() {
    mCameraShootCallback();

    f32 direction = glm::radians(-mVelocity * 500.0f);

    glm::mat4 mtx = glm::mat4(1.0f);
    mtx = glm::translate(mtx, mTransform->getPosition());
    mtx = glm::rotate(mtx, glm::radians(-mVelocity * 500.0f), glm::vec3(-1.0f, 1.0f, 0.0f));
    mtx = glm::scale(mtx, mTransform->getScale());

    constexpr glm::vec4 cMissileOffset(120.0f, -40.0f, -40.0f, 1.0f);

    glm::vec4 leftMissilePos = mtx * cMissileOffset;
    glm::vec4 rightMissilePos = mtx * glm::vec4(cMissileOffset.x, cMissileOffset.y, -cMissileOffset.z, cMissileOffset.w);

    std::string properties = R"({
        "position": [)" + std::to_string(leftMissilePos.x) + ", " + std::to_string(leftMissilePos.y) + ", " + std::to_string(leftMissilePos.z) + R"(],
        "direction": )" + std::to_string(direction) + R"(,
        "velocity": )" + std::to_string(mVelocity) + R"(
    })";

    mScene->spawnEntity("MissileEntity", properties);

    properties = R"({
        "position": [)" + std::to_string(rightMissilePos.x) + ", " + std::to_string(rightMissilePos.y) + ", " + std::to_string(rightMissilePos.z) + R"(],
        "direction": )" + std::to_string(direction) + R"(,
        "velocity": )" + std::to_string(mVelocity) + R"(
    })";

    mScene->spawnEntity("MissileEntity", properties);
}

NSM_REGISTER_ENTITY(PlayerEntity);
