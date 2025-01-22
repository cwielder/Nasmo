#pragma once

#include <nsm/Common.h>

#include <nsm/entity/Entity.h>

#include <string>
#include <vector>
#include <tuple>

namespace nsm {

    class Scene final {
    public:
        Scene(const std::string& path);
        ~Scene();

        void update(const f64 timeStep);
        void onEvent(const Event* event);

        void switchScene(const std::string& path);
        void reloadScene();
        void spawnEntity(const std::string_view type, const std::string& propertiesJson, Entity** out = nullptr);

        [[nodiscard]] const std::vector<Entity*>& getEntities() const { return mEntities; }
        [[nodiscard]] const std::string& getPath() const { return mPath; }

    private:
        Entity* spawnEntity(const std::string_view type, Entity::Properties& properties);
        void loadScene(const std::string& path);

        std::vector<Entity*> mEntities;
        std::vector<std::tuple<std::string, std::string, Entity**>> mEntitiesToSpawn;
        std::string mPath;
    };

}
