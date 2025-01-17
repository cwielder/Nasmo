#include <nsm/entity/component/ColliderComponent.h>

nsm::ColliderComponent::ColliderComponent(Entity* owner, const TransformComponent* transform, std::function<void(ColliderComponent*)> collisionCallback)
    : mOwner(owner)
    , mCollisionCallback(collisionCallback)
    , mTransform(transform)
{ }
