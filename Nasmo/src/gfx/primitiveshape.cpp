#include <nsm/gfx/primitiveshape.h>

nsm::VertexBuffer* nsm::PrimitiveShape::sQuadVBO = nullptr;
nsm::IndexBuffer* nsm::PrimitiveShape::sQuadIBO = nullptr;
nsm::VertexArray* nsm::PrimitiveShape::sQuadVAO = nullptr;

void nsm::PrimitiveShape::init() {
    // Quad data
    constexpr f32 quadVertexData[] = {
        -1.0f, -1.0f,  // bottom left
         1.0f, -1.0f,  // bottom right
         1.0f,  1.0f,  // top right
        -1.0f,  1.0f,  // top left
    };

    constexpr u32 quadIndexData[] = {
        0, 1, 2, // first triangle
        2, 3, 0  // second triangle
    };

    sQuadVBO = new VertexBuffer(quadVertexData, sizeof(quadVertexData), 2 * sizeof(f32), nsm::BufferUsage::StaticDraw);
    sQuadIBO = new IndexBuffer(quadIndexData, sizeof(quadIndexData), nsm::BufferUsage::StaticDraw);

    const std::array<nsm::VertexArray::Attribute, 1> quadAttributes = {
        nsm::VertexArray::Attribute{0, 2, nsm::VertexArray::DataType::Float, 0, 0, false}
    };

    sQuadVAO = new VertexArray();
    sQuadVAO->setLayout(quadAttributes);
    sQuadVAO->linkBuffer(*sQuadVBO, 0);
    sQuadVAO->linkIndices(*sQuadIBO);
}

void nsm::PrimitiveShape::destroy() {
    delete sQuadVBO;
    delete sQuadIBO;
    delete sQuadVAO;
}
