#include <nsm/entity/entity.h>

#include <nsm/entity/component/tagcomponent.h>

namespace nsm {

    Entity::Registry::Registry(const Entity::Registry::entityFactory factory, const std::string& identifier) 
        : mFactory(factory)
        , mIdentifier(identifier)
        , mIdentifierHash(std::hash<std::string>{}(identifier))
    {
        Registry::getRegistry()[identifier] = this;
    }

    std::map<std::string, Entity::Registry*>& Entity::Registry::getRegistry() {
        static std::map<std::string, Entity::Registry*> registry;

        return registry;
    }

    Entity::~Entity() {
        for (auto& [type, vector] : mComponents) {
            for (auto& component : vector) {
                delete component;
            }
        }

        mComponents.clear();
    }

    bool Entity::hasTag(const std::string& tag) {
        std::span<TagComponent*> tagComponents = this->getComponents<TagComponent>();

        for (auto component : tagComponents) {
            if (component->getTag() == tag) {
                return true;
            }
        }

        return false;
    }

}
