#include <nsm/entity/component/lightcomponent.h>

#include <nsm/gfx/shader.h>
#include <nsm/gfx/renderinfo.h>
#include <nsm/gfx/framebuffer.h>
#include <nsm/entity/component/cameracomponent.h>
#include <nsm/gfx/primitiveshape.h>

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

namespace {
    struct LightVolume {
        LightVolume() = default;

        nsm::VertexArray vao;
        nsm::VertexBuffer vbo;
        nsm::IndexBuffer ibo;
    };

    static const LightVolume* getLightVolume() {
        static bool volumeInited = false;
        static LightVolume* sLightVolume = nullptr;
        if (volumeInited) {
            return sLightVolume;
        }

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
            nsm::VertexArray::Attribute{ 0, 3, nsm::VertexArray::DataType::Float, 0, 0, false }
        };

        sLightVolume->vao.setLayout(attributes);
        sLightVolume->vao.linkBuffer(sLightVolume->vbo, 0);
        sLightVolume->vao.linkIndices(sLightVolume->ibo);

        volumeInited = true;

        return sLightVolume;
    }
}

nsm::PointLightComponent::PointLightComponent(const glm::vec3& position, const glm::vec3& color, const f32 intensity)
    : LightComponent(position, color, intensity)
    , mShaderProgram("nsm/assets/shaders/light_volume.vsh", "nsm/assets/shaders/point_light.fsh")
{ }

void nsm::PointLightComponent::drawOpaque(const RenderInfo& renderInfo) {    
    if (mDirty) {
        constexpr f32 cAttenuationLinear = 1.175f;
        constexpr f32 cAttenuationExp = 0.186f;

        const f32 maxChannel = glm::max(glm::max(mColor.r, mColor.g), mColor.b);

        // TODO: This calculation needs to be improved
        const f32 radius = (-cAttenuationLinear + std::sqrt(cAttenuationLinear * cAttenuationLinear - 4 * cAttenuationExp * (cAttenuationExp - 256 * mIntensity)));

        mModelMatrix = glm::translate(glm::mat4(1.0f), mPosition);
        mModelMatrix = glm::scale(mModelMatrix, glm::vec3(radius));
        mDirty = false;
    }

    mShaderProgram.bind();
    mShaderProgram.setOptionalMat4("uViewProjMtx", renderInfo.camera->getViewProjection());
    mShaderProgram.setOptionalMat4("uModelMtx", mModelMatrix);
    mShaderProgram.setOptionalVec2("uScreenSize", renderInfo.framebuffer->getTextureBuffer(0)->getSize());
    mShaderProgram.setOptionalVec3("uCamPos", renderInfo.camera->getPosition());
    mShaderProgram.setOptionalVec3("uLightPos", mPosition);
    mShaderProgram.setOptionalVec3("uLightColor", mColor * mIntensity);

    renderInfo.framebuffer->getTextureBuffer(0)->bind(0); // normal+metallic
    renderInfo.framebuffer->getTextureBuffer(1)->bind(1); // albedo+roughness
    renderInfo.framebuffer->getDepthStencil()->bind(2); // depth

    getLightVolume()->vao.bind();
    getLightVolume()->ibo.draw();
}

// DirectionalLightComponent

nsm::DirectionalLightComponent::DirectionalLightComponent(const f32 yaw, const f32 pitch, const glm::vec3& color, const f32 intensity)
    : LightComponent(glm::vec3(0.0f), color, intensity)
    , mShaderProgram("nsm/assets/shaders/screen.vsh", "nsm/assets/shaders/directional_light.fsh")
    , mDirection(yawPitchToDirection(yaw, pitch))
{ }

void nsm::DirectionalLightComponent::drawOpaque(const RenderInfo& renderInfo) {
    mShaderProgram.bind();
    mShaderProgram.setOptionalMat4("uViewProjMtx", renderInfo.camera->getViewProjection());
    mShaderProgram.setOptionalVec3("uCamPos", renderInfo.camera->getPosition());
    mShaderProgram.setOptionalVec3("uLightDir", mDirection);
    mShaderProgram.setOptionalVec3("uLightColor", mColor * mIntensity);

    renderInfo.framebuffer->getTextureBuffer(0)->bind(0); // normal+metallic
    renderInfo.framebuffer->getTextureBuffer(1)->bind(1); // albedo+roughness
    renderInfo.framebuffer->getDepthStencil()->bind(2); // depth

    PrimitiveShape::getQuadVAO().bind();
    PrimitiveShape::getQuadIBO().draw();
}

glm::vec3 nsm::DirectionalLightComponent::yawPitchToDirection(const f32 yaw, const f32 pitch) {
    const f32 yawCos = std::cos(glm::radians(yaw));
    const f32 yawSin = std::sin(glm::radians(yaw));
    const f32 pitchCos = std::cos(glm::radians(pitch));
    const f32 pitchSin = std::sin(glm::radians(pitch));

    return glm::vec3(
        yawCos * pitchCos,
        yawSin * pitchCos,
        pitchSin
    );
}
