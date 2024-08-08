#include <nsm/entity/component/lightcomponent.h>

#include <nsm/gfx/shader.h>
#include <nsm/gfx/renderinfo.h>
#include <nsm/gfx/framebuffer.h>
#include <nsm/entity/component/cameracomponent.h>

#include <glm/gtc/matrix_transform.hpp>
#include <fastgltf/core.hpp>
#include <fastgltf/glm_element_traits.hpp>

// LightComponent

nsm::LightComponent::LightComponent(const glm::vec3& position, const glm::vec3& color, const f32 intensity)
    : mPosition(position)
    , mColor(color)
    , mIntensity(intensity)
    , mDirty(true)
{ }

// PointLightComponent

nsm::PointLightComponent::LightVolume* nsm::PointLightComponent::sLightVolume = nullptr;

nsm::PointLightComponent::PointLightComponent(const glm::vec3& position, const glm::vec3& color, const f32 intensity, const f32 radius)
    : LightComponent(position, color, intensity)
    , mRadius(radius)
    , mShaderProgram("nsm/assets/shaders/light_volume.vsh", "nsm/assets/shaders/point_light.fsh")
{
    static bool volumeInited = false;
    if (!volumeInited) {
        sLightVolume = new LightVolume();

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

        sLightVolume->vbo.init(positions.data(), positions.size() * sizeof(glm::vec3), sizeof(glm::vec3), nsm::BufferUsage::StaticDraw);
        sLightVolume->ibo.init(indices.data(), indices.size() * sizeof(u32), nsm::BufferUsage::StaticDraw);

        static const std::array<nsm::VertexArray::Attribute, 1> attributes = {
            nsm::VertexArray::Attribute{ 0, 3, nsm::VertexArray::DataType::Float, 0, sizeof(glm::vec3), false }
        };

        sLightVolume->vao.setLayout(attributes);
        sLightVolume->vao.linkBuffer(sLightVolume->vbo, 0);
        sLightVolume->vao.linkIndices(sLightVolume->ibo);

        volumeInited = true;
    }
}

nsm::ShaderProgram* nsm::PointLightComponent::getShaderProgram() {
    return &mShaderProgram;
}

void nsm::PointLightComponent::drawOpaque(const RenderInfo& renderInfo) {    
    if (mDirty) {
        const f32 maxChannel = glm::max(glm::max(mColor.r, mColor.g), mColor.b);
        const f32 attenuationLinear = 0.7f;
        const f32 attenuationExp = 1.8f;
        f32 radius = (-attenuationLinear + sqrtf(attenuationLinear * attenuationLinear -
        4 * attenuationExp * (attenuationExp - 256 * maxChannel * mIntensity)));

        mModelMatrix = glm::translate(glm::mat4(1.0f), mPosition);
        mModelMatrix = glm::scale(mModelMatrix, glm::vec3(radius));
        mDirty = false;
    }

    mShaderProgram.bind();
    mShaderProgram.setMat4("uViewProjMtx", renderInfo.camera->getViewProjection());
    mShaderProgram.setMat4("uModelMtx", mModelMatrix);
    mShaderProgram.setVec2("uScreenSize", renderInfo.framebuffer->getTextureBuffer(0)->getSize());

    renderInfo.framebuffer->getTextureBuffer(0)->bind(0); // normal+metallic
    renderInfo.framebuffer->getTextureBuffer(1)->bind(1); // albedo+roughness
    renderInfo.framebuffer->getDepthStencil()->bind(2); // depth

    sLightVolume->vao.bind();
    sLightVolume->ibo.bind();
    GLsizei count = static_cast<GLsizei>(sLightVolume->ibo.getCount());
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)0x123456789);
}
