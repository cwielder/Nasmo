#include <nsm/gfx/PrimitiveShape.h>

#include <nsm/debug/Assert.h>

#include <fastgltf/core.hpp>
#include <fastgltf/glm_element_traits.hpp>
#include <glm/glm.hpp>

#include <vector>

nsm::VertexBuffer* nsm::PrimitiveShape::sQuadVBO = nullptr;
nsm::IndexBuffer* nsm::PrimitiveShape::sQuadIBO = nullptr;
nsm::VertexArray* nsm::PrimitiveShape::sQuadVAO = nullptr;

nsm::VertexBuffer* nsm::PrimitiveShape::sCubeVBO = nullptr;
nsm::IndexBuffer* nsm::PrimitiveShape::sCubeIBO = nullptr;
nsm::VertexArray* nsm::PrimitiveShape::sCubeVAO = nullptr;

nsm::VertexBuffer* nsm::PrimitiveShape::sSphereVBO = nullptr;
nsm::IndexBuffer* nsm::PrimitiveShape::sSphereIBO = nullptr;
nsm::VertexArray* nsm::PrimitiveShape::sSphereVAO = nullptr;

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

    // Sphere data reads from a file
    {
        fastgltf::Parser parser;
        fastgltf::Expected<fastgltf::GltfDataBuffer> data = fastgltf::GltfDataBuffer::FromPath(std::filesystem::path{"nsm/assets/models/light_volume.glb"});
        NSM_ASSERT(data.error() == fastgltf::Error::None, "Failed to load light volume model: ", fastgltf::getErrorMessage(data.error()));
        fastgltf::Expected<fastgltf::Asset> asset = parser.loadGltf(data.get(), std::filesystem::path{"nsm/assets/models/light_volume.glb"}.parent_path());
        NSM_ASSERT(asset.error() == fastgltf::Error::None, "Failed to parse light volume model: ", fastgltf::getErrorMessage(asset.error()));
        NSM_ASSERT(fastgltf::validate(asset.get()) == fastgltf::Error::None, "Failed to validate light volume model");

        const fastgltf::Mesh& mesh = asset.get().meshes[0];
        const fastgltf::Primitive& primitive = mesh.primitives[0];
        const fastgltf::Accessor* accessor = &asset.get().accessors[primitive.findAttribute("POSITION")->accessorIndex];
        std::vector<glm::vec3> positions(accessor->count);
        for (std::size_t i = 0; i < accessor->count; ++i) {
            positions[i] = fastgltf::getAccessorElement<glm::vec3>(asset.get(), *accessor, i);
        }

        accessor = &asset.get().accessors[primitive.indicesAccessor.value()];
        std::vector<u32> indices(accessor->count);
        switch (accessor->componentType) {
            case fastgltf::ComponentType::UnsignedByte: {
                for (std::size_t i = 0; i < accessor->count; i++) {
                    indices[i] = static_cast<u32>(fastgltf::getAccessorElement<u8>(asset.get(), *accessor, i));
                }
                break;
            }
            case fastgltf::ComponentType::UnsignedShort: {
                for (std::size_t i = 0; i < accessor->count; i++) {
                    indices[i] = static_cast<u32>(fastgltf::getAccessorElement<u16>(asset.get(), *accessor, i));
                }
                break;
            }
            case fastgltf::ComponentType::UnsignedInt: {
                for (std::size_t i = 0; i < accessor->count; i++) {
                    indices[i] = fastgltf::getAccessorElement<u32>(asset.get(), *accessor, i);
                }
                break;
            }
        }

        sSphereVBO = new VertexBuffer(positions.data(), positions.size() * sizeof(glm::vec3), sizeof(glm::vec3), nsm::BufferUsage::StaticDraw);
        sSphereIBO = new IndexBuffer(indices.data(), indices.size() * sizeof(u32), nsm::BufferUsage::StaticDraw);

        static const std::array<nsm::VertexArray::Attribute, 1> attributes = {
            nsm::VertexArray::Attribute{ 0, 3, nsm::VertexArray::DataType::Float, 0, 0, false }
        };

        sSphereVAO = new VertexArray();
        sSphereVAO->setLayout(attributes);
        sSphereVAO->linkBuffer(*sSphereVBO, 0);
        sSphereVAO->linkIndices(*sSphereIBO);
    }
}

void nsm::PrimitiveShape::destroy() {
    delete sQuadVBO;
    delete sQuadIBO;
    delete sQuadVAO;

    delete sCubeVBO;
    delete sCubeIBO;
    delete sCubeVAO;
}
