#pragma once

#include <nsm/entity/entity.h>

#include <glm/glm.hpp>

namespace nsm {

    class TransformComponent : public EntityComponent {
    public:
        TransformComponent(const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& rotation = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f))
            : mPosition(position), mRotation(rotation), mScale(scale)
        { }

        ~TransformComponent() override = default;

        [[nodiscard]] const glm::vec3& getPosition() const { return mPosition; }
        [[nodiscard]] const glm::vec3& getRotation() const { return mRotation; }
        [[nodiscard]] const glm::vec3& getScale() const { return mScale; }

        void setPosition(const glm::vec3& position) { mPosition = position; }
        void setRotation(const glm::vec3& rotation) { mRotation = rotation; }
        void setScale(const glm::vec3& scale) { mScale = scale; }

    private:
        glm::vec3 mPosition;
        glm::vec3 mRotation;
        glm::vec3 mScale;
    };

}
