#include <nsm/entity/entity.h>
#include <nsm/debug/log.h>

class PlayerEntity : public nsm::Entity {
public:
    PlayerEntity(const nsm::Entity::Properties&) { }
    ~PlayerEntity() override = default;

    void onCreate() override {
        nsm::warn("PlayerEntity onCreate");
    }

    void onUpdate(const f32 timeStep) override {
        nsm::warn("PlayerEntity onUpdate");
    }
};

NSM_REGISTER_ENTITY(PlayerEntity);
