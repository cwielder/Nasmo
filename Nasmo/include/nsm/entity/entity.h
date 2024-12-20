#pragma once

#include <nsm/common.h>
#include <nsm/debug/assert.h>

#include <simdjson.h>

#include <map>
#include <typeindex>

namespace nsm {

    class Scene;
    class Event;

    class EntityComponent {
    public:
        EntityComponent() = default;
        virtual ~EntityComponent() = default;

        virtual void onEvent(const Event* event) { }
        virtual void onUpdate(const f32 timeStep) { }
    };

    class Entity {
    public:
        using Properties = simdjson::simdjson_result<simdjson::ondemand::object>;

        class Registry {
        public:
            using entityFactory = Entity* (*)(Entity::Properties& properties);
        
            Registry() = default;
            Registry(entityFactory factory, const std::string& identifier);
        
            static std::map<std::string, Registry*>& getRegistry();

            entityFactory getFactory() const { return mFactory; }
            const std::string& getIdentifier() const { return mIdentifier; }
            std::size_t getIdentifierHash() const { return mIdentifierHash; }

        private:
            entityFactory mFactory;
            std::string mIdentifier;
            std::size_t mIdentifierHash;
        };

        template <typename T>
        class RegisterEntity : public Registry {
        public:
            RegisterEntity(const std::string& identifier)
                : Registry([](Entity::Properties& properties) -> Entity* {
                    return new T(properties);
                }, identifier)
            { }
        };

    public:
        Entity() = default;
        virtual ~Entity();

        template <typename T>
        void addComponent(T* component) requires std::is_base_of_v<EntityComponent, T> {
            mComponents[std::type_index(typeid(T))].push_back(component);
        }

        template <typename T>
        std::span<T*> getComponents() requires std::is_base_of_v<EntityComponent, T> {
            std::vector<EntityComponent*>& components = mComponents[std::type_index(typeid(T))];

            return std::span<T*>(reinterpret_cast<T**>(components.data()), components.size());
        }

        template <typename T>
        bool hasComponent() requires std::is_base_of_v<EntityComponent, T> {
            return mComponents.find(std::type_index(typeid(T))) != mComponents.end();
        }

        template <typename t>
        void removeComponent(std::size_t index) {
            NSM_ASSERT(index < mComponents[std::type_index(typeid(t))].size(), "Index out of bounds!");

            delete mComponents[std::type_index(typeid(t))][index];
            mComponents[std::type_index(typeid(t))].erase(mComponents[std::type_index(typeid(t))].begin() + index);
        }

        [[nodiscard]] const std::string& getIdentifier() const { return mRegistry->getIdentifier(); }
        [[nodiscard]] std::size_t getIdentifierHash() const { return mRegistry->getIdentifierHash(); }

        bool hasTag(const std::string& tag);
        void setAlive(const bool isAlive) { mIsAlive = isAlive; }

    protected:
        Scene* mScene = nullptr;

    private:
        friend class Scene;

        virtual void onCreate(Entity::Properties& properties) { }
        virtual void onUpdate(const f32 timeStep) { }
        virtual void onEvent(const Event* event) { }

        std::map<std::type_index, std::vector<EntityComponent*>> mComponents;
        const Registry* mRegistry = nullptr;
        bool mIsAlive = true;
    };

}

#define NSM_REGISTER_ENTITY(CLASS) static const nsm::Entity::RegisterEntity<CLASS> NSM_CONCAT(nsm_register_entity_, __LINE__){#CLASS}
