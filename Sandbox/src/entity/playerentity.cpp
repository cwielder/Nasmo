#include <nsm/entity/entity.h>
#include <nsm/debug/log.h>
#include <nsm/event/events.h>
#include <nsm/entity/component/modelcomponent.h>
#include <nsm/entity/component/transformcomponent.h>
#include <nsm/entity/component/particlecomponent.h>
#include <nsm/entity/component/inputcomponent.h>
#include <nsm/entity/scene.h>
#include <nsm/util/jsonhelpers.h>

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
            .setStartSize(glm::vec3(1.0f))
            .setEndSize(glm::vec3(0.0f, 0.0f, 1.0f))
            .setTexture("textures/exhaust_b2.png")
            .setDepth(false)
        ;
    }
}

class PlayerEntity final : public nsm::Entity {
public:
    PlayerEntity(nsm::Entity::Properties& properties)
        : mTransform(nullptr)
        , mModel(nullptr)
        , mStartPosition(nsm::JsonHelpers::getVec3(properties, "position"))
        , mStartScale(nsm::JsonHelpers::getVec3(properties, "scale"))
    { }

    ~PlayerEntity() override = default;

    void onCreate(nsm::Entity::Properties& properties) override {
        mTransform = new nsm::TransformComponent();
        mTransform->setPosition(mStartPosition);
        mTransform->setScale(mStartScale);
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

    void onUpdate(const f64 timeStep) override {
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

private:
    void spawnMissile() {
        f32 velocity = mVelocity;
        glm::vec3 position = mTransform->getPosition();

        std::string properties = R"({
            "position": [)" + std::to_string(position.x + 12.0f) + ", " + std::to_string(position.y - 4.0f) + ", " + std::to_string(position.z - 4.0f) + R"(],
            "velocity": )" + std::to_string(velocity) + R"(
        })";

        mScene->spawnEntity("MissileEntity", properties);

        properties = R"({
            "position": [)" + std::to_string(position.x + 12.0f) + ", " + std::to_string(position.y - 4.0f) + ", " + std::to_string(position.z + 4.0f) + R"(],
            "velocity": )" + std::to_string(velocity) + R"(
        })";

        mScene->spawnEntity("MissileEntity", properties);
    }

private:
    glm::vec3 mStartPosition, mStartScale;
    nsm::TransformComponent* mTransform;
    nsm::ModelComponent* mModel;
    nsm::ParticleComponent* mExhaustParticleLeft;
    nsm::ParticleComponent* mExhaustParticleRight;
    nsm::InputComponent* mInput;

    bool mLeftPressed = false, mRightPressed = false;
    f32 mAcceleration = 0.0f;
    f32 mVelocity = 0.0f;
};

NSM_REGISTER_ENTITY(PlayerEntity);
