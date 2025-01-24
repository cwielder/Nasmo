#include <nsm/entity/Entity.h>

#include "entity/PlayerEntity.h"

#include <nsm/ui/UIElement.h>
#include <nsm/gfx/Texture2D.h>
#include <nsm/entity/component/UIComponent.h>
#include <nsm/entity/Scene.h>

namespace {
    class TankBG : public nsm::UIElement {
    public:
        TankBG(const glm::vec2& position, const f32 z)
            : UIElement(position, z)
            , mTexture("textures/tank_bg.png", false)
        { }

        void draw(const nsm::RenderInfo& renderInfo) override {
            this->getRenderer().drawTexture(renderInfo, mTexture, 
                glm::vec2(1.0f * 0.33f, 0.333f * 0.33f), 0.0f, glm::vec2(0.0f)
            );
        }

    private:
        nsm::Texture2D mTexture;
    };

    class TankIndicator : public nsm::UIElement {
    public:
        TankIndicator(const UIElement* anchor, const glm::vec2& position, const f32 z)
            : UIElement(anchor, position, z)
            , mTexture("textures/tank_indicator.png", false)
        { }

        void draw(const nsm::RenderInfo& renderInfo) override {
            this->getRenderer().drawTexture(renderInfo, mTexture,
                glm::vec2(0.2f * 0.33f), 0.0f, glm::vec2(0.0f)
            );
        }

        void setFuel(f32 fuel) {
            glm::vec3 full = glm::vec3(-0.33f, 0.0f, 0.0f);
            glm::vec3 empty = glm::vec3(0.29f, 0.0f, 0.0f);

            mPosition.setOffset(glm::vec2(empty.x + (full.x - empty.x) * fuel, 0.255f * 0.33));
        }

    private:
        nsm::Texture2D mTexture;
    };

    void CreateHUDLayout(nsm::UIComponent* c) {       
        TankBG* tank_bg = new TankBG({ -1.3f, -0.75f }, 0.0f);

        c
            ->addElement("tank_bg", tank_bg)
            ->addElement("tank_indicator", new TankIndicator(tank_bg, { 0.0f, 0.0f }, -1.0f))
        ;
    }
}

class HUDEntity : public nsm::Entity {
public:
    HUDEntity(nsm::Entity::Properties&);
    ~HUDEntity() override = default;

    void onCreate(nsm::Entity::Properties&) override;
    void onUpdate(const f64) override;

private:
    nsm::UIComponent* mLayout;
    PlayerEntity* mPlayer;
};

NSM_REGISTER_ENTITY(HUDEntity);

HUDEntity::HUDEntity(nsm::Entity::Properties& properties)
    : mLayout(nullptr)
    , mPlayer(nullptr)
{ }

void HUDEntity::onCreate(nsm::Entity::Properties& properties) {
    mLayout = new nsm::UIComponent();
    mLayout->setTargetLayer("ui");
    CreateHUDLayout(mLayout);
    this->addComponent<nsm::DrawableComponent>(mLayout);

    const auto& entities = mScene->getEntities();
    for (const auto entity : entities) {
        if (entity->getIdentifier() == "PlayerEntity") {
            mPlayer = static_cast<PlayerEntity*>(entity);

            mPlayer->addDieCallback([this]() {
                mPlayer = nullptr;
            });

            break;
        }
    }
}

void HUDEntity::onUpdate(const f64) {
    if (mPlayer == nullptr) {
        return;
    }

    f32 fuel = mPlayer->getFuel();
    static_cast<TankIndicator*>(mLayout->getElement("tank_indicator"))->setFuel(fuel);
}
