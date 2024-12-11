#include <nsm/entity/entity.h>
#include <nsm/entity/component/uicomponent.h>

#include <nsm/ui/uielement.h>

#include <nsm/gfx/texture2D.h>
#include <nsm/ui/uishape.h>
#include <nsm/util/color.h>

#include <nsm/debug/log.h>

#include <glm/gtc/matrix_transform.hpp>

class StarElement : public nsm::UIElement {
public:
    StarElement(const glm::vec2& position, const glm::vec2& size, const nsm::Color color, const f32 z)
        : UIElement(nullptr, position, z)
        , mSize(size)
        , mColor(color)
    { 
        this->mShape.setVertices(
            {
                glm::vec2(0.282, 0.384),
                glm::vec2(1.113, 0.37),
                glm::vec2(0.43, -0.1),
                glm::vec2(0.677, -1.043),
                glm::vec2(-0.0434, -0.4176),
                glm::vec2(-0.792, -0.9087),
                glm::vec2(-0.4244, -0.0845),
                glm::vec2(-0.94, 0.35),
                glm::vec2(-0.207, 0.374),
                glm::vec2(0.097, 1.0137)
            }
        );
    }

    ~StarElement() override = default;

    void draw(const nsm::RenderInfo& renderInfo) override {
        auto rotation = glm::degrees(0.0f);
        auto position = glm::vec2(0.0f, 0.0f);

        this->getRenderer().drawPolygonSolid(renderInfo, mShape, mColor.getRGBA(), mSize, rotation, position);
    }

private:
    nsm::Color mColor;
    glm::vec2 mSize;
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
                glm::vec2(-0.18, 1),
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
        nsm::UIElement* star1 = new StarElement(glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), nsm::Color("#353535"), 1);
        nsm::UIElement* star2 = new StarElement(glm::vec2(0.0f, 0.0f), glm::vec2(0.50f, 0.50f), nsm::Color("#000000"), 2);
        nsm::UIElement* star3 = new StarElement(glm::vec2(0.0f, 0.0f), glm::vec2(0.40f, 0.40f), nsm::Color("#353535"), 3);
        nsm::UIElement* star4 = new StarElement(glm::vec2(0.0f, 0.0f), glm::vec2(0.30f, 0.30f), nsm::Color("#000000"), 4);
        nsm::UIElement* star5 = new StarElement(glm::vec2(0.0f, 0.0f), glm::vec2(0.20f, 0.20f), nsm::Color("#353535"), 5);

        nsm::UIElement* goldStar = new StarElement(glm::vec2(0.0f, 0.0f), glm::vec2(0.10f, 0.10f), nsm::Color("#d19719"), 6);
        
        mUIComponent
            ->addElement("backdrop", backdrop)
            ->addElement("star1", star1)
            ->addElement("star2", star2)
            ->addElement("star3", star3)
            ->addElement("star4", star4)
            ->addElement("star5", star5)
            ->addElement("goldStar", goldStar)
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
