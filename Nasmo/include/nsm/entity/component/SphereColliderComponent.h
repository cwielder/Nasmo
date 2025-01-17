#pragma once

#include <nsm/entity/component/ColliderComponent.h>

namespace nsm {

    class SphereColliderComponent final : public ColliderComponent {
    public:
        SphereColliderComponent(Entity* owner, const f32 radius, const TransformComponent* transform, std::function<void(ColliderComponent*)> collisionCallback = nullptr);
        ~SphereColliderComponent() override = default;

        [[nodiscard]] f32 getRadius() const { return mRadius; }

        static bool isColliding(const SphereColliderComponent* a, const SphereColliderComponent* b);

    private:
        f32 mRadius;
    };

}
