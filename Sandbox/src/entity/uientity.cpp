#include <nsm/entity/entity.h>
#include <nsm/entity/component/uicomponent.h>

#include <nsm/ui/uielement.h>

#include <nsm/gfx/texture2D.h>

#include <nsm/debug/log.h>

#include <glm/gtc/matrix_transform.hpp>

class LogoComponent : public nsm::UIElement {
public:
    LogoComponent(const nsm::UIElement* anchor, const glm::vec2& position)
        : UIElement(anchor, position, 10.0f)
        , mTexture("textures/nsmbu.png", false)
    { }

    ~LogoComponent() override = default;

    void draw(const nsm::RenderInfo& renderInfo) override {
        // 1. Draw a texture (which is a rectangular image) of any resolution with a specific width/height/rotation (transform) at a specific position
        this->getRenderer().drawTexture(renderInfo, mTexture, glm::vec2(0.821f, 0.333f), glm::radians(0.0f), glm::vec2(0.0f, 0.0f));

        // 2. Draw a free-form polygon shape using a list of vertices and filled with either a solid color or used as a mask for a texture (gradients added later)

        // 3. Later, advanced users can create components which render with more direct control with opengl to draw using shader code
    }

private:
    nsm::Texture2D mTexture;
};

class HotbarComponent : public nsm::UIElement {
public:
    HotbarComponent(const nsm::UIElement* anchor, const glm::vec2& position)
        : UIElement(anchor, position, -100.0f)
        , mTexture("textures/hotbar.png", false, nsm::Texture::FilterMode::Nearest, nsm::Texture::FilterMode::Nearest)
    { }

    ~HotbarComponent() override = default;

    void draw(const nsm::RenderInfo& renderInfo) override {
        static double time = 0.0;
        time += 0.01;

        // wobble
        f32 rotation = glm::sin(time) * (glm::pi<f32>() / 2.0f);

        this->getRenderer().drawTexture(renderInfo, mTexture, glm::vec2(8.27f * 0.1f, 1.0f * 0.1f), glm::radians(rotation), glm::vec2(0.0f, 0.0f));
    }

private:
    nsm::Texture2D mTexture;
};

class DemoLayout {
public:
    DemoLayout(nsm::Entity* owner)
        : mUIComponent(nullptr)
        , mOwner(owner)
    { }

    void init(const std::string& layer) {
        mUIComponent = new nsm::UIComponent();
        mUIComponent->setTargetLayer(layer);

        mOwner->addComponent<nsm::DrawableComponent>(mUIComponent);

        nsm::UIElement* logo = new LogoComponent(nullptr, glm::vec2(0.0f, 0.66f));
        nsm::UIElement* hotbar = new HotbarComponent(logo, glm::vec2(0.0f,-1.5f));

        (*mUIComponent)
            .addElement("logo", logo)
            .addElement("hotbar", hotbar)
        ;
    }

private:
    nsm::UIComponent* mUIComponent;
    nsm::Entity* mOwner;
};

class UIEntity final : public nsm::Entity {
public:
    UIEntity(nsm::Entity::Properties&)
        : mUserInterface(this)
    { }

    ~UIEntity() override = default;

    void onCreate(nsm::Entity::Properties&) override {
        mUserInterface.init("ui");
    }

private:
    DemoLayout mUserInterface;
};

NSM_REGISTER_ENTITY(UIEntity);
