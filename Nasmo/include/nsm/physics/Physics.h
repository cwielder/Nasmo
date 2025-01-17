#pragma once

#include <vector>

namespace nsm {

    class Entity;

    class Physics final {
    public:
        Physics() = default;
        ~Physics() = default;

        void update(const std::vector<Entity*>& entities);
    };

}
