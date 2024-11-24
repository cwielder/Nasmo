#include <nsm/entity/entity.h>
#include <nsm/entity/component/uicomponent.h>

#include <nsm/ui/uielement.h>

#include <nsm/gfx/texture2D.h>
#include <nsm/ui/uishape.h>

#include <nsm/debug/log.h>

#include <glm/gtc/matrix_transform.hpp>

class StarElement : public nsm::UIElement {
public:
    StarElement(const glm::vec2& position, const f32 z)
        : UIElement(nullptr, position, z)
    { 
        this->mShape.setVertices(
            {
                glm::vec2(0.128, 0.505 - 0.2),
                glm::vec2(0.654, 0.503 - 0.2),
                glm::vec2(0.384, 0.067 - 0.2),
                glm::vec2(0.45, -0.411 - 0.2),
                glm::vec2(0, -0.068 - 0.2),
                glm::vec2(-0.45, -0.411 - 0.2),
                glm::vec2(-0.384, 0.067 - 0.2),
                glm::vec2(-0.654, 0.503 - 0.2),
                glm::vec2(-0.128, 0.505 - 0.2),
                glm::vec2(0, 1 - 0.2),
            }
        );
    }

    ~StarElement() override = default;

    void draw(const nsm::RenderInfo& renderInfo) override {
        auto size = glm::vec2(1.0f, 1.0f);
        auto rotation = glm::radians(0.0f);
        auto position = glm::vec2(0.0f, 0.0f);
        auto color = glm::vec4(104.0f / 255.0f, 104.0f / 255.0f, 104.0f / 255.0f, 1.0f);

        this->getRenderer().drawPolygonSolid(renderInfo, mShape, color, size, rotation, position);
    }

private:
    nsm::UIShape mShape;
};

class BackdropElement : public nsm::UIElement {
public:
    BackdropElement(const glm::vec2& position, const f32 z)
        : UIElement(nullptr, position, z)
    { 
        this->mShape.setVertices(
            {
                // red
                glm::vec2(-0.13, 1),
                // green
                glm::vec2(1.75, 1),
                // orange
                glm::vec2(0.8, -1),
                // purple
                glm::vec2(-1.37, -1.006)
            }
        );
    }

    ~BackdropElement() override = default;

    void draw(const nsm::RenderInfo& renderInfo) override {
        auto size = glm::vec2(1.0f, 1.0f);
        auto rotation = glm::radians(0.0f);
        auto position = glm::vec2(0.0f, 0.0f);
        auto color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

        this->getRenderer().drawPolygonSolid(renderInfo, mShape, color, size, rotation, position);
    }

private:
    nsm::UIShape mShape;
};

class RankUpLayout {
public:
    RankUpLayout(nsm::Entity* parent)
        : mUIComponent(nullptr)
        , mParent(parent)
    { }

    void init(const std::string& layer) {
        mUIComponent = new nsm::UIComponent();
        mUIComponent->setTargetLayer(layer);

        mParent->addComponent<nsm::DrawableComponent>(mUIComponent);

        // black bg
        nsm::UIElement* backdrop = new BackdropElement(glm::vec2(0.0f, 0.0f), 0);

        // grey star (parent to ui)
        nsm::UIElement* star = new StarElement(glm::vec2(0.0f, 0.0f), 1);

        mUIComponent
            ->addElement("backdrop", backdrop)
            ->addElement("star1", star)
        ;
    }
private:
    nsm::UIComponent* mUIComponent;
    nsm::Entity* mParent;
};

class UIRankUpEntity final : public nsm::Entity {
public:
    UIRankUpEntity(nsm::Entity::Properties&)
        : mUserInterface(this)
    { }

    ~UIRankUpEntity() override = default;

    void onCreate(nsm::Entity::Properties&) override {
        mUserInterface.init("ui");
    }

private:
    RankUpLayout mUserInterface;
};

NSM_REGISTER_ENTITY(UIRankUpEntity);
