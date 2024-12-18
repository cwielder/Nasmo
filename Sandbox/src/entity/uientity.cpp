#include <nsm/entity/entity.h>
#include <nsm/entity/component/uicomponent.h>

#include <nsm/ui/uielement.h>

#include <nsm/gfx/texture2D.h>
#include <nsm/gfx/font.h>
#include <nsm/ui/uishape.h>

#include <nsm/debug/log.h>

#include <glm/gtc/matrix_transform.hpp>

class LogoComponent : public nsm::UIElement {
public:
    LogoComponent(const nsm::UIElement* anchor, const glm::vec2& position)
        : UIElement(anchor, position, 10.0f)
        , mTexture("textures/nsmbu.png", false)
        , mStarShape()
    {
        mStarShape.setVertices(
			{
				glm::vec2(0.0f, 1.0f),
				glm::vec2(0.3f, 0.25f),
				glm::vec2(1.0f, 0.0f),
				glm::vec2(0.5f, -0.2f),
				glm::vec2(0.6f, -1.0f),
				glm::vec2(0.0f, -0.25f),
				glm::vec2(-0.8f, -1.0f),
				glm::vec2(-0.5f, 0.0f),
				glm::vec2(-1.0f, 0.1f),
				glm::vec2(-0.3f, 0.25f)
			}
        );
        
        mSquareShape.setVertices(
            {
                glm::vec2(-0.5f, 0.5f),
                glm::vec2(0.5f, 0.5f),
                glm::vec2(0.5f, -0.5f),
                glm::vec2(-0.5f, -0.5f)
            }
        );

        mFont = nsm::Font::get("nsm/assets/fonts/SegoeUI.ttf");
        NSM_ASSERT(mFont != nullptr, "Font is null!");
    }

    ~LogoComponent() override = default;

    void draw(const nsm::RenderInfo& renderInfo) override {
        glm::vec2 size;
        f32 rotation;
        glm::vec2 position;
        glm::vec4 color;

        nsm::UIRenderer& renderer = this->getRenderer();

        //* 1. Draw a texture (which is a rectangular image) of any resolution with a specific width/height/rotation (transform) at a specific position
        size = glm::vec2(0.821f, 0.333f);
        rotation = glm::radians(0.0f);
        position = glm::vec2(0.0f, 0.0f);
        renderer.drawTexture(renderInfo, mTexture, size, rotation, position);

        //* 2. Draw a free-form polygon shape using a list of vertices and filled with either a solid color or used as a mask for a texture (gradients added later)
        size = glm::vec2(0.5f, 0.5f);
        rotation = glm::radians(0.0f);
        position = glm::vec2(0.0f, -0.5f);
        color = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
        renderer.drawPolygonSolid(renderInfo, mStarShape, color, size, rotation, position, nsm::UIRenderer::MaskMode::AsMask);

        size = glm::vec2(0.75f, 0.75f);
        rotation = glm::radians(45.0f);
        position = glm::vec2(0.0f, -0.5f);
        color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        renderer.drawPolygonSolid(renderInfo, mSquareShape, color, size, rotation, position, nsm::UIRenderer::MaskMode::Masked);

        //* 3. Render text
        
    }

private:
    nsm::Texture2D mTexture;
    nsm::UIShape mStarShape, mSquareShape;
    nsm::Font* mFont;
};

class HotbarComponent : public nsm::UIElement {
public:
    HotbarComponent(const nsm::UIElement* anchor, const glm::vec2& position)
        : UIElement(anchor, position, -100.0f)
        , mTexture("textures/hotbar.png", false, nsm::Texture::FilterMode::Nearest, nsm::Texture::FilterMode::Nearest)
    { }

    ~HotbarComponent() override = default;

    void draw(const nsm::RenderInfo& renderInfo) override {
        this->getRenderer().drawTexture(renderInfo, mTexture, glm::vec2(0.827f, 0.1f), glm::radians(0.0f), glm::vec2(0.0f, 0.0f));
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

        mUIComponent
            ->addElement("logo", logo)
            ->addElement("hotbar", hotbar)
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
