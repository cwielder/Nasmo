#pragma once

#include <glm/vec2.hpp>

#include <string>
#include <tuple>
#include <vector>

namespace nsm {

    class Component;

    class UILayer { 
    public:
        UILayer();
        ~UILayer();

        UILayer& addComponent(Component* component) {
            return *this;
        }

    private:
        std::vector<Component*> mComponents;
    };
    
}
