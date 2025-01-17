#include <nsm/entity/component/SphereColliderComponent.h>

#include <nsm/entity/component/TransformComponent.h>
#include <glm/glm.hpp>

nsm::SphereColliderComponent::SphereColliderComponent(Entity* owner, const f32 radius, const TransformComponent* transform, std::function<void(ColliderComponent*)> collisionCallback)
    : ColliderComponent(owner, transform, collisionCallback)
    , mRadius(radius)
{ }

bool nsm::SphereColliderComponent::isColliding(const SphereColliderComponent* a, const SphereColliderComponent* b) {
    const glm::vec3 pos1 = a->getTransform()->getPosition();
    const glm::vec3 pos2 = b->getTransform()->getPosition();

    const f32 distance = glm::distance(pos1, pos2);

    return distance < a->getRadius() + b->getRadius();
}
