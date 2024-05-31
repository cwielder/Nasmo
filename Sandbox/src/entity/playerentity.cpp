#include <nsm/entity/entity.h>
#include <nsm/debug/log.h>
#include <nsm/event/events.h>
#include <nsm/entity/component/drawablecomponent.h>
#include <nsm/gfx/vertexarray.h>
#include <nsm/gfx/vertexbuffer.h>
#include <nsm/gfx/indexbuffer.h>
#include <nsm/gfx/shader.h>
#include <glm/gtc/matrix_transform.hpp>

class RenderTestComponent : public nsm::DrawableComponent {
public:
    static constexpr f32 positions[] = {
        -1.0f,  1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
         1.0f, -1.0f, 0.0f
    };

    static constexpr u32 indices[] = {
        0, 1, 2, 1, 3, 2
    };

    RenderTestComponent()
        : nsm::DrawableComponent()
        , mRotation(0.0f)
        , mVertexBuffer(positions, sizeof(positions), 3 * sizeof(f32), nsm::BufferUsage::StaticDraw)
        , mIndexBuffer(indices, sizeof(indices), nsm::BufferUsage::StaticDraw)
        , mVertexArray()
        , mShader("test.vsh", "test.fsh")
    {
        mVertexBuffer.markAttribute(0, 3, nsm::VertexBuffer::DataType::Float, false);

        mVertexArray.linkBuffer(mVertexBuffer);
        mVertexArray.linkIndices(mIndexBuffer);

        mViewProjMtx = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f) * glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void draw(const nsm::RenderInfo& renderInfo) override {
        nsm::info("Drawing RenderTestComponent");

        mModelMtx = glm::mat4(1.0f);
        mModelMtx = glm::translate(mModelMtx, glm::vec3(0.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f)) * glm::rotate(glm::mat4(1.0f), glm::radians(mRotation), glm::vec3(0.0f, 0.0f, 1.0f));

        mShader.bind();
        mShader.setMat4("uModelMtx", mModelMtx);
        mShader.setMat4("uViewProj", mViewProjMtx);
        mVertexArray.bind();
        mIndexBuffer.bind();
        glDrawElements(GL_TRIANGLES, mIndexBuffer.getCount(), GL_UNSIGNED_INT, nullptr);
    }

    f32 mRotation;
    nsm::VertexBuffer mVertexBuffer;
    nsm::IndexBuffer mIndexBuffer;
    nsm::VertexArray mVertexArray;
    nsm::ShaderProgram mShader;
    glm::mat4 mModelMtx;
    glm::mat4 mViewProjMtx;
};

class PlayerEntity : public nsm::Entity {
public:
    PlayerEntity(const nsm::Entity::Properties&) { }
    ~PlayerEntity() override = default;

    void onCreate() override {
        this->addComponent<nsm::DrawableComponent>(new RenderTestComponent());
        this->getComponents<nsm::DrawableComponent>()[0]->setTargetLayer("main");
    }

    void onUpdate(const f32 timeStep) override {
        RenderTestComponent* renderer = static_cast<RenderTestComponent*>(this->getComponents<nsm::DrawableComponent>()[0]);
        renderer->mRotation += 20.0f * timeStep;
    }

    void onEvent(const nsm::Event* event) override {
        if (event->getType() == nsm::EventType::WindowResize) {
            nsm::error("resize event");
        }
    }
};

NSM_REGISTER_ENTITY(PlayerEntity);
