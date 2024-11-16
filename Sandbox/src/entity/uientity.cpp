#include <nsm/entity/entity.h>
#include <nsm/entity/component/uicomponent.h>

#include <nsm/ui/uielement.h>

#include <nsm/gfx/texture2D.h>

#include <nsm/debug/log.h>

#include <glm/gtc/matrix_transform.hpp>

class TestComponent : public nsm::UIElement {
public:
    TestComponent(const glm::vec2& position)
        : UIElement(position)
        , mTexture("textures/nsmbu.png", false)
    { }

    ~TestComponent() override = default;

    void draw(const nsm::RenderInfo& renderInfo) override {
        // 1. Draw a texture (which is a rectangular image) of any resolution with a specific width/height/rotation (transform) at a specific position
        this->getRenderer().drawTexture(renderInfo, mTexture, glm::vec2(0.821f, 0.333f), glm::radians(0.0f), glm::vec2(0.0f, 0.0f));

        // 2. Draw a free-form polygon shape using a list of vertices and filled with either a solid color or used as a mask for a texture (gradients added later)

        // 3. Later, advanced users can create components which render with more direct control with opengl to draw using shader code
    }

private:
    nsm::Texture2D mTexture;
};


class UIEntity final : public nsm::Entity {
public:
    UIEntity(nsm::Entity::Properties&)
        : mUIComponent(nullptr)
    { }

    ~UIEntity() override = default;

    void onCreate(nsm::Entity::Properties&) override {
        mUIComponent = new nsm::UIComponent();
        mUIComponent->setTargetLayer("ui");
        (*mUIComponent) // build UI layout here (can make a function to build it all later)
            .addElement("test_component", new TestComponent(glm::vec2(0.0f, 0.0f)))
        ;

        this->addComponent<nsm::DrawableComponent>(mUIComponent);
    }

private:
    nsm::UIComponent* mUIComponent;
};

NSM_REGISTER_ENTITY(UIEntity);
