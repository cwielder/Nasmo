#include <nsm/app/application.h>

#include <nsm/gfx/opengl.h>

nsm::Application::Application(const ApplicationInfo& info)
    : mWindow(info.window)
{

}

nsm::Application::~Application() { }

#include <nsm/gfx/shader.h>
#include <nsm/gfx/vertexarray.h>
#include <nsm/gfx/vertexbuffer.h>
#include <nsm/gfx/indexbuffer.h>

void nsm::Application::run() {
    constexpr f32 positions[] = {
        -1.0f,  1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
         1.0f, -1.0f, 0.0f
    };

    constexpr f32 colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f
    };

    constexpr u32 indices[] = {
        0, 1, 2, 1, 3, 2
    };

    VertexBuffer positionsVbo;
    positionsVbo.init(positions, sizeof(positions), 3 * sizeof(f32), BufferUsage::StaticDraw);
    positionsVbo.markAttribute(0, 3, VertexBuffer::DataType::Float, 0, false);

    VertexBuffer colorsVbo;
    colorsVbo.init(colors, sizeof(colors), 3 * sizeof(f32), BufferUsage::StaticDraw);
    colorsVbo.markAttribute(1, 3, VertexBuffer::DataType::Float, 0, false);

    IndexBuffer ibo(indices, sizeof(indices));

    VertexArray vao;
    vao.linkBuffer(positionsVbo);
    vao.linkBuffer(colorsVbo);
    vao.linkIndices(ibo);

    ShaderProgram shader("test.vsh", "test.fsh");

    while (mWindow.update()) {
        glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        vao.bind();
        ibo.bind();
        shader.bind();
        glDrawArrays(GL_TRIANGLES, 0, ibo.getCount());
    }
}
