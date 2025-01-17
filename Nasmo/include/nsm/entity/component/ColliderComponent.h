#pragma once

#include <nsm/entity/Entity.h>

#include <functional>

namespace nsm {

    class TransformComponent;

    class ColliderComponent : public EntityComponent {
    public:
        ColliderComponent(Entity* owner, const TransformComponent* transform, std::function<void(ColliderComponent*)> collisionCallback = nullptr);
        ~ColliderComponent() override = default;

        [[nodiscard]] Entity* getOwner() const { return mOwner; }
        [[nodiscard]] const TransformComponent* getTransform() const { return mTransform; }
        
        [[nodiscard]] std::function<void(ColliderComponent*)> getCollisionCallback() const { return mCollisionCallback; }
    
    private:
        Entity* const mOwner;
        std::function<void(ColliderComponent*)> mCollisionCallback;
        const TransformComponent* mTransform;
    };

}
