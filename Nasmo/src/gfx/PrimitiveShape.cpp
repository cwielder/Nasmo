#include <nsm/gfx/PrimitiveShape.h>

nsm::VertexBuffer* nsm::PrimitiveShape::sQuadVBO = nullptr;
nsm::IndexBuffer* nsm::PrimitiveShape::sQuadIBO = nullptr;
nsm::VertexArray* nsm::PrimitiveShape::sQuadVAO = nullptr;

nsm::VertexBuffer* nsm::PrimitiveShape::sCubeVBO = nullptr;
nsm::IndexBuffer* nsm::PrimitiveShape::sCubeIBO = nullptr;
nsm::VertexArray* nsm::PrimitiveShape::sCubeVAO = nullptr;

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

    // Cube data
    constexpr f32 cubeVertexData[] = {
	    -1.0f, -1.0f,  1.0f,//        7--------6
	     1.0f, -1.0f,  1.0f,//       /|       /|
	     1.0f, -1.0f, -1.0f,//      4--------5 |
	    -1.0f, -1.0f, -1.0f,//      | |      | |
	    -1.0f,  1.0f,  1.0f,//      | 3------|-2
	     1.0f,  1.0f,  1.0f,//      |/       |/
	     1.0f,  1.0f, -1.0f,//      0--------1
	    -1.0f,  1.0f, -1.0f
    };

    constexpr u32 cubeIndexData[] = {
        // Right
        1, 2, 6,
        6, 5, 1,
        // Left
        0, 4, 7,
        7, 3, 0,
        // Top
        4, 5, 6,
        6, 7, 4,
        // Bottom
        0, 3, 2,
        2, 1, 0,
        // Back
        0, 1, 5,
        5, 4, 0,
        // Front
        3, 7, 6,
        6, 2, 3
    };

    sCubeVBO = new VertexBuffer(cubeVertexData, sizeof(cubeVertexData), 3 * sizeof(f32), nsm::BufferUsage::StaticDraw);
    sCubeIBO = new IndexBuffer(cubeIndexData, sizeof(cubeIndexData), nsm::BufferUsage::StaticDraw);

    const std::array<nsm::VertexArray::Attribute, 1> cubeAttributes = {
        nsm::VertexArray::Attribute{0, 3, nsm::VertexArray::DataType::Float, 0, 0, false}
    };

    sCubeVAO = new VertexArray();
    sCubeVAO->setLayout(cubeAttributes);
    sCubeVAO->linkBuffer(*sCubeVBO, 0);
    sCubeVAO->linkIndices(*sCubeIBO);
}

void nsm::PrimitiveShape::destroy() {
    delete sQuadVBO;
    delete sQuadIBO;
    delete sQuadVAO;

    delete sCubeVBO;
    delete sCubeIBO;
    delete sCubeVAO;
}
