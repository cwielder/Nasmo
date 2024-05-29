#pragma once

#include <nsm/common.h>

#include <nsm/entity/entity.h>

#include <string>
#include <vector>

namespace nsm {

    class Scene final {
    public:
        Scene(const std::string& path);
        ~Scene();

        void update(const f32 timeStep);
        void onEvent(const Event* event);

        void switchScene(const std::string& path);
        Entity* spawnEntity(const std::string_view type, const std::string& propertiesJson);

        const std::vector<Entity*>& getEntities() const { return mEntities; }

        [[nodiscard]] const std::string& getPath() const { return mPath; }

    private:
        Entity* spawnEntity(const std::string_view type, Entity::Properties& properties);
        void loadScene(const std::string& path);

        std::vector<Entity*> mEntities;
        std::string mPath;
    };

}
