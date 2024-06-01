#pragma once

#include <vector>

namespace nsm {

    class Application;
    class Entity;

    class System {
    public:
        virtual void transferData(Application& app, const std::vector<Entity*>& entities) = 0;
    };

}
