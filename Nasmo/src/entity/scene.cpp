#include <nsm/entity/scene.h>

#include <nsm/entity/component/tagcomponent.h>
#include <nsm/debug/assert.h>
#include <nsm/event/events.h>
#include <nsm/app/application.h>

nsm::Scene::Scene(const std::string& path) {
    this->loadScene(path);
}

nsm::Scene::~Scene() {
    for (auto& entity : mEntities) {
        delete entity;
    }
}

void nsm::Scene::update(const f32 timeStep) {
    std::vector<nsm::Entity*> entitiesToDestroy;
    for (auto& entity : mEntities) {
        if (entity->mIsAlive) {
            continue;
        }

        entitiesToDestroy.push_back(entity);
    }

    std::erase_if(mEntities, [](nsm::Entity* entity) {
        return !entity->mIsAlive;
    });

    for (auto& entity : entitiesToDestroy) {
        delete entity;
    }

    for (int i = 0; i < mEntities.size(); ++i) {
        nsm::Entity* entity = mEntities[i];

        entity->onUpdate(timeStep);

        for (auto [hash, componentVector] : entity->mComponents) {
            for (nsm::EntityComponent* component : componentVector) {
                component->onUpdate(timeStep);
            }
        }
    }
}

void nsm::Scene::onEvent(const Event* event) {
    for (auto entity : mEntities) {
        entity->onEvent(event);

        for (auto [hash, componentVector] : entity->mComponents) {
            for (nsm::EntityComponent* component : componentVector) {
                component->onEvent(event);
            }
        }
    }

    if (event->getType() == nsm::EventType::SceneSwitch) {
        const nsm::SceneSwitchEvent* sceneSwitchEvent = static_cast<const nsm::SceneSwitchEvent*>(event);

        for (auto& entity : mEntities) {
            delete entity;
        }

        mEntities.clear();

        this->loadScene(sceneSwitchEvent->getPath());
    }
}

void nsm::Scene::switchScene(const std::string& path) {
    nsm::Application::raiseEvent(new SceneSwitchEvent(path));
}

nsm::Entity* nsm::Scene::spawnEntity(const std::string_view type, const std::string& propertiesJson) {
    simdjson::ondemand::parser parser;
    simdjson::padded_string padded_string = propertiesJson;
    simdjson::ondemand::document doc = parser.iterate(padded_string);
    auto r = doc.get_object();
    NSM_ASSERT(r.error() == simdjson::SUCCESS, "Failed to create properties from string");

    return this->spawnEntity(type, r);
}

nsm::Entity* nsm::Scene::spawnEntity(const std::string_view type, Entity::Properties& properties) {
    const auto& registry = Entity::Registry::getRegistry();
    const auto entityRegistration = registry.find(std::string{type});
    NSM_ASSERT(entityRegistration != registry.end(), "Entity '", type, "' not found in registry");
 
    const auto& [name, data] = *entityRegistration;

    Entity* newEntity = data->getFactory()(properties);

    newEntity->mRegistry = data;
    newEntity->mScene = this;
    auto tags = properties["tags"].get_array();
    if (tags.error() == simdjson::NO_SUCH_FIELD) {
        newEntity->onCreate(properties);
        mEntities.push_back(newEntity);
        return newEntity;
    }
    for (auto tag : tags) {
        newEntity->addComponent<nsm::TagComponent>(new nsm::TagComponent(std::string{tag.get_string().value()}));
    }

    newEntity->onCreate(properties);
    mEntities.push_back(newEntity);
    return newEntity;
}

void nsm::Scene::loadScene(const std::string& path) {
    /**
     * TODO: Preload assets while running the current scene before switching to the new scene to avoid stuttering (including audio)
     * * NOTE: Move SceneSwitchEvent firing to after the assets have been loaded because the audio cache might be cleared too early
    */

    mPath = path;

    simdjson::ondemand::parser parser;
    simdjson::padded_string json = simdjson::padded_string::load(path);
    simdjson::ondemand::document document = parser.iterate(json);

    try {
        auto entities = document["entities"].get_array();

        for (auto entity : entities) {
            auto entityObject = entity.value().get_object();
            const std::string_view type = entityObject["type"].get_string().value();
            nsm::Entity::Properties properties = entityObject["properties"].get_object();

            this->spawnEntity(type, properties);
        }
    } catch (simdjson::simdjson_error& error) {
        nsm::error("Error parsing scene file '", path, "': ", error.what());
    }
}

