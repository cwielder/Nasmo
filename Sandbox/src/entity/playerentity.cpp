#include <nsm/entity/entity.h>
#include <nsm/debug/log.h>
#include <nsm/event/events.h>
#include <nsm/entity/component/modelcomponent.h>
#include <nsm/entity/component/transformcomponent.h>
#include <nsm/entity/component/particlecomponent.h>
#include <nsm/entity/component/inputcomponent.h>
#include <nsm/util/jsonhelpers.h>

#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>

namespace {
    static void CreateExhaustParticle(nsm::ParticleComponent* ptcl) {
        ptcl->getEmitter()
            .setEmitRadius(0.9f)
            .setEmitRate(980.0f)
            .setLifeSpan(0.4f)
            .setLifeSpanVariance(0.05f)
            .setInitialVelocity(glm::vec3(-64.0f, 0.0f, 0.0f))
            .setAcceleration(glm::vec3(0.0f, 0.5f, 0.0f))
            .setStartSize(glm::vec3(1.0f))
            .setEndSize(glm::vec3(0.0f, 0.0f, 1.0f))
            .setTexture("textures/exhaust.png")
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
        CreateExhaustParticle(mExhaustParticleLeft);

        mExhaustParticleRight = new nsm::ParticleComponent();
        mExhaustParticleRight->setTargetLayer("forward");
        this->addComponent<nsm::DrawableComponent>(mExhaustParticleRight);
        CreateExhaustParticle(mExhaustParticleRight);

        static constexpr f32 cAcceleration = 0.25f;

        mInput = new nsm::InputComponent();
        this->addComponent<nsm::InputComponent>(mInput);
        mInput->setOnKeyPress([&](const nsm::KeyPressEvent* event) {
            switch (event->getKey()) {
                case GLFW_KEY_A:
                case GLFW_KEY_LEFT: {
                    mAcceleration = -cAcceleration;
                    break;
                }

                case GLFW_KEY_D:
                case GLFW_KEY_RIGHT: {
                    mAcceleration = cAcceleration;
                    break;
                }
            }
        });

        mInput->setOnKeyRelease([&](const nsm::KeyReleaseEvent* event) {
            switch (event->getKey()) {
                case GLFW_KEY_A:
                case GLFW_KEY_LEFT:
                case GLFW_KEY_D:
                case GLFW_KEY_RIGHT: {
                    mAcceleration = 0.0f;
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

        if (mAcceleration != 0.0f) {
            mVelocity += mAcceleration * timeStep;
            mVelocity = glm::clamp(mVelocity, -cMaxVelocity, cMaxVelocity);
        }

        // Friction
        static constexpr f32 cFriction = 0.1f;
        if (mVelocity > 0.0f) {
            mVelocity -= cFriction * timeStep;
            mVelocity = glm::max(mVelocity, 0.0f);
        } else if (mVelocity < 0.0f) {
            mVelocity += cFriction * timeStep;
            mVelocity = glm::min(mVelocity, 0.0f);
        }

        nsm::info("Velocity: ", mVelocity);

        glm::vec3 position = mTransform->getPosition();
        position += glm::vec3(0.0f, 0.0f, mVelocity);

        mTransform->setPosition(position);

        mExhaustParticleLeft->getEmitter().setPosition(position + glm::vec3(-7.0f, -0.5f, -4.0f));
        mExhaustParticleRight->getEmitter().setPosition(position + glm::vec3(-7.0f, -0.5f, 4.0f));

        glm::mat4 mtx = glm::mat4(1.0f);
        mtx = glm::translate(mtx, mTransform->getPosition());
        mtx = glm::rotate(mtx, glm::radians(-mVelocity * 400.0f), glm::vec3(1.0f, 1.0f, 0.0f));
        mtx = glm::scale(mtx, mTransform->getScale());

        mModel->setTransformAll(mtx);
    }

private:
    glm::vec3 mStartPosition, mStartScale;
    nsm::TransformComponent* mTransform;
    nsm::ModelComponent* mModel;
    nsm::ParticleComponent* mExhaustParticleLeft;
    nsm::ParticleComponent* mExhaustParticleRight;
    nsm::InputComponent* mInput;

    f32 mAcceleration = 0.0f;
    f32 mVelocity = 0.0f;
};

NSM_REGISTER_ENTITY(PlayerEntity);
