#include <nsm/gfx/material.h>

#include <nsm/gfx/textureenum.h>
#include <nsm/gfx/shader.h>
#include <nsm/util/jsonhelpers.h>
#include <nsm/debug/assert.h>

#include <fastgltf/core.hpp>
#include <fastgltf/glm_element_traits.hpp>

namespace {
    nsm::Texture::FilterMode getFilterMode(const fastgltf::Filter filter) {
        switch (filter) {
            case fastgltf::Filter::Linear: return nsm::Texture::FilterMode::Linear;
            case fastgltf::Filter::Nearest: return nsm::Texture::FilterMode::Nearest;
            case fastgltf::Filter::LinearMipMapLinear: return nsm::Texture::FilterMode::LinearMipmapLinear;
            case fastgltf::Filter::LinearMipMapNearest: return nsm::Texture::FilterMode::LinearMipmapNearest;
            case fastgltf::Filter::NearestMipMapLinear: return nsm::Texture::FilterMode::NearestMipmapLinear;
            case fastgltf::Filter::NearestMipMapNearest: return nsm::Texture::FilterMode::NearestMipmapNearest;
            default: nsm::warn("Unknown filter mode"); return nsm::Texture::FilterMode::Linear;
        }
    }

    nsm::Texture::WrapMode getWrapMode(const fastgltf::Wrap wrap) {
        switch (wrap) {
            case fastgltf::Wrap::ClampToEdge: return nsm::Texture::WrapMode::ClampToEdge;
            case fastgltf::Wrap::MirroredRepeat: return nsm::Texture::WrapMode::MirroredRepeat;
            case fastgltf::Wrap::Repeat: return nsm::Texture::WrapMode::Repeat;
            default: nsm::warn("Unknown wrap mode"); return nsm::Texture::WrapMode::Repeat;
        }
    }
}

// UniformVar

void nsm::Material::UniformVar::apply(const nsm::ShaderProgram* shader) const {
    switch (type) {
        case Type::Int:
            shader->setOptionalInt(name, value.i);
            break;
        case Type::Float:
            shader->setOptionalFloat(name, value.f);
            break;
        case Type::Double:
            shader->setOptionalDouble(name, value.d);
            break;
        case Type::Vec2:
            shader->setOptionalVec2(name, value.v2);
            break;
        case Type::Vec3:
            shader->setOptionalVec3(name, value.v3);
            break;
        case Type::Vec4:
            shader->setOptionalVec4(name, value.v4);
            break;
        case Type::Mat4:
            shader->setOptionalMat4(name, value.m4);
            break;
        default:
            break;
    }
}

// Material

std::map<std::string, nsm::Material*> nsm::Material::sMaterialCache;

nsm::Material* nsm::Material::get(const std::string& identifier, const fastgltf::Asset& asset, const fastgltf::Material& resource) {
    auto it = sMaterialCache.find(identifier);
    if (it != sMaterialCache.end()) {
        return it->second;
    }

    Material* material = new Material(asset, resource);

    sMaterialCache[identifier] = material;

    return material;
}

nsm::Material::Material(const fastgltf::Asset& asset, const fastgltf::Material& resource)
    : mShader(nullptr)
    , mTextures()
    , mUniforms()
    , mIsTranslucent(false)
{
    mShader = new ShaderProgram("nsm/assets/shaders/model.vsh", "nsm/assets/shaders/model.fsh");

    // Albedo
    UniformVar uAlbedoTexturePresent;
    uAlbedoTexturePresent.name = "uAlbedoTexturePresent";
    uAlbedoTexturePresent.type = UniformVar::Type::Int;
    if (resource.pbrData.baseColorTexture.has_value()) {
        this->addTexture(asset, asset.textures[resource.pbrData.baseColorTexture.value().textureIndex]);
        uAlbedoTexturePresent.value.i = true;
    } else {
        uAlbedoTexturePresent.value.i = false;
        
        UniformVar uAlbedoFactor;
        uAlbedoFactor.name = "uAlbedoFactor";
        uAlbedoFactor.type = UniformVar::Type::Vec4;
        std::memcpy(&uAlbedoFactor.value.v4, &resource.pbrData.baseColorFactor, sizeof(glm::vec4));
        mUniforms.push_back(uAlbedoFactor);
    }
    mUniforms.push_back(uAlbedoTexturePresent);

    // Metallic-Roughness
    UniformVar uMetallicRoughnessTexturePresent;
    uMetallicRoughnessTexturePresent.name = "uMetallicRoughnessTexturePresent";
    uMetallicRoughnessTexturePresent.type = UniformVar::Type::Int;
    if (resource.pbrData.metallicRoughnessTexture.has_value()) {
        this->addTexture(asset, asset.textures[resource.pbrData.metallicRoughnessTexture.value().textureIndex]);
        uMetallicRoughnessTexturePresent.value.i = true;
    } else {
        uMetallicRoughnessTexturePresent.value.i = false;
        
        UniformVar uMetallicFactor;
        uMetallicFactor.name = "uMetallicFactor";
        uMetallicFactor.type = UniformVar::Type::Float;
        uMetallicFactor.value.f = resource.pbrData.metallicFactor;
        mUniforms.push_back(uMetallicFactor);

        UniformVar uRoughnessFactor;
        uRoughnessFactor.name = "uRoughnessFactor";
        uRoughnessFactor.type = UniformVar::Type::Float;
        uRoughnessFactor.value.f = resource.pbrData.roughnessFactor;
        mUniforms.push_back(uRoughnessFactor);
    }
    mUniforms.push_back(uMetallicRoughnessTexturePresent);

    // Normal
    UniformVar uNormalTexturePresent;
    uNormalTexturePresent.name = "uNormalTexturePresent";
    uNormalTexturePresent.type = UniformVar::Type::Int;
    if (resource.normalTexture.has_value()) {
        this->addTexture(asset, asset.textures[resource.normalTexture.value().textureIndex]);
        uNormalTexturePresent.value.i = true;
    } else {
        uNormalTexturePresent.value.i = false;
    }
    mUniforms.push_back(uNormalTexturePresent);
}

nsm::Material::~Material() {
    delete mShader;
}

void nsm::Material::addTexture(const fastgltf::Asset& asset, const fastgltf::Texture& tex) {
    NSM_ASSERT(tex.imageIndex.has_value(), "Texture2D has no image index");

    const fastgltf::DataSource& texData = asset.images[tex.imageIndex.value()].data;
    
    Texture::FilterMode enlargeFilter = Texture::FilterMode::Linear;
    Texture::FilterMode shrinkFilter = Texture::FilterMode::Linear;
    Texture::WrapMode wrapS = Texture::WrapMode::Repeat;
    Texture::WrapMode wrapT = Texture::WrapMode::Repeat;

    if (tex.samplerIndex.has_value()) {
        const fastgltf::Sampler& sampler = asset.samplers[tex.samplerIndex.value()];
        
        wrapS = getWrapMode(sampler.wrapS);
        wrapT = getWrapMode(sampler.wrapT);

        if (sampler.magFilter.has_value()) {
            enlargeFilter = getFilterMode(sampler.magFilter.value());
        }
        if (sampler.minFilter.has_value()) {
            shrinkFilter = getFilterMode(sampler.minFilter.value());
        }
    }

    const fastgltf::sources::URI* uri = std::get_if<fastgltf::sources::URI>(&texData);
    const fastgltf::sources::BufferView* bufferView = std::get_if<fastgltf::sources::BufferView>(&texData);
    const fastgltf::sources::Array* array = std::get_if<fastgltf::sources::Array>(&texData);

    if (uri) {
        NSM_ASSERT(uri->fileByteOffset == 0, "Non-zero file byte offset not supported");
        NSM_ASSERT(uri->uri.isLocalPath(), "Non-local path not supported");

        const std::string path{uri->uri.path().begin(), uri->uri.path().end()};
        mTextures.emplace_back(path, true, enlargeFilter, shrinkFilter, wrapS, wrapT);
    } else if (bufferView) {
        const fastgltf::BufferView& view = asset.bufferViews[bufferView->bufferViewIndex];
        const fastgltf::Buffer& buffer = asset.buffers[view.bufferIndex];

        const fastgltf::sources::Array* bufferViewArray = std::get_if<fastgltf::sources::Array>(&buffer.data);
        NSM_ASSERT(bufferViewArray, "Buffer data is not an array");

        const std::byte* data = bufferViewArray->bytes.data() + view.byteOffset;
        const std::size_t size = view.byteLength;

        mTextures.emplace_back(reinterpret_cast<const u8*>(data), size, true, enlargeFilter, shrinkFilter, wrapS, wrapT);
    } else if (array) {
        mTextures.emplace_back(reinterpret_cast<const u8*>(array->bytes.data()), array->bytes.size(), true, enlargeFilter, shrinkFilter, wrapS, wrapT);
    } else {
        NSM_ASSERT(false, "Unknown texture data type");
    }
}

void nsm::Material::bind() const {
    mShader->bind();

    for (const UniformVar& uniform : mUniforms) {
        uniform.apply(mShader);
    }

    for (std::size_t i = 0; i < mTextures.size(); ++i) {
        mTextures[i].bind(static_cast<u32>(i));
    }
}

void nsm::Material::clearCache() {
    for (auto& [name, material] : sMaterialCache) {
        delete material;
    }

    sMaterialCache.clear();
}
