#include <nsm/entity/entity.h>
#include <nsm/debug/log.h>
#include <nsm/event/events.h>

class PlayerEntity : public nsm::Entity {
public:
    PlayerEntity(const nsm::Entity::Properties&) { }
    ~PlayerEntity() override = default;

    void onCreate() override {
        
    }

    void onUpdate(const f32 timeStep) override {
        
    }

    void onEvent(const nsm::Event* event) override {
        if (event->getType() == nsm::EventType::WindowResize) {
            nsm::error("resize event");
        }
    }
};

NSM_REGISTER_ENTITY(PlayerEntity);
