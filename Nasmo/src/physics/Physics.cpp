#include <nsm/physics/Physics.h>

#include <nsm/entity/component/SphereColliderComponent.h>

void nsm::Physics::update(const std::vector<Entity*>& entities) {
    // TODO: Don't allocate every frame.
    std::vector<SphereColliderComponent*> sphereColliders;
    sphereColliders.reserve(entities.size());
    
    for (const auto& entity : entities) {
        for (const auto& sphereCollider : entity->getComponents<SphereColliderComponent>()) {
            sphereColliders.push_back(sphereCollider);
        }
    }

    for (std::size_t i = 0; i < sphereColliders.size(); ++i) {
        for (std::size_t j = i + 1; j < sphereColliders.size(); ++j) {
            if (SphereColliderComponent::isColliding(sphereColliders[i], sphereColliders[j])) {
                sphereColliders[i]->getCollisionCallback()(sphereColliders[j]);
                sphereColliders[j]->getCollisionCallback()(sphereColliders[i]);
            }
        }
    }
}
