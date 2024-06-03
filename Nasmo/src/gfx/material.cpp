#include <nsm/gfx/material.h>

#include <nsm/gfx/shader.h>
#include <nsm/util/jsonhelpers.h>

#include <simdjson.h>

static nsm::Texture::FilterMode getFilterMode(const std::string& mode) {
    if (mode == "nearest") {
        return nsm::Texture::FilterMode::Nearest;
    } else if (mode == "linear") {
        return nsm::Texture::FilterMode::Linear;
    } else if (mode == "nearest_mipmap_nearest") {
        return nsm::Texture::FilterMode::NearestMipmapNearest;
    } else if (mode == "linear_mipmap_nearest") {
        return nsm::Texture::FilterMode::LinearMipmapNearest;
    } else if (mode == "nearest_mipmap_linear") {
        return nsm::Texture::FilterMode::NearestMipmapLinear;
    } else if (mode == "linear_mipmap_linear") {
        return nsm::Texture::FilterMode::LinearMipmapLinear;
    }

    return nsm::Texture::FilterMode::Count;
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

nsm::Material* nsm::Material::get(const std::string& path) {
    auto it = sMaterialCache.find(path);
    if (it != sMaterialCache.end()) {
        return it->second;
    }

    Material* material = new Material(path);

    sMaterialCache[path] = material;

    return material;
}

nsm::Material::Material(const std::string& path)
    : mShader(nullptr)
    , mUniforms()
{
    simdjson::ondemand::parser parser;
    simdjson::padded_string json = simdjson::padded_string::load(path);
    simdjson::ondemand::document doc = parser.iterate(json);

    mIsTranslucent = doc["translucent"].get_bool().value();    
    mShader = new ShaderProgram(std::string{doc["vsh"].get_string().value()}, std::string{doc["fsh"].get_string().value()});

    auto textures = doc["textures"].get_array();
    mTextures.reserve(textures.count_elements());

    std::int_fast8_t i = 0;
    for (auto it : textures) {
        auto texObj = it.get_object();

        const std::string texPath = std::string{texObj["path"].get_string().value()};
        const Texture::FilterMode filter = getFilterMode(std::string{texObj["filter"].get_string().value()});
        const bool srgb = texObj["srgb"].get_bool().value();

        mTextures.emplace_back(texPath, srgb, filter);
    }

    auto uniforms = doc["uniforms"].get_array();
    mUniforms.resize(uniforms.count_elements());

    i = 0;
    for (auto it : uniforms) {
        UniformVar& uniform = mUniforms[i++];
        auto uniformObj = it.get_object();

        const std::string name = std::string{uniformObj["name"].get_string().value()};
        uniform.name = name;

        const std::string type = std::string{uniformObj["type"].get_string().value()};
        if (type == "int") {
            uniform.type = UniformVar::Type::Int;
            uniform.value.i = static_cast<i32>(uniformObj["value"].get_int64().value());
        } else if (type == "float") {
            uniform.type = UniformVar::Type::Float;
            uniform.value.f = static_cast<f32>(uniformObj["value"].get_double().value());
        } else if (type == "double") {
            uniform.type = UniformVar::Type::Double;
            uniform.value.d = uniformObj["value"].get_double().value();
        } else if (type == "vec2") {
            uniform.type = UniformVar::Type::Vec2;
            uniform.value.v2 = JsonHelpers::getVec2(uniformObj, "value");
        } else if (type == "vec3") {
            uniform.type = UniformVar::Type::Vec3;
            uniform.value.v3 = JsonHelpers::getVec3(uniformObj, "value");
        } else if (type == "vec4") {
            uniform.type = UniformVar::Type::Vec4;
            uniform.value.v4 = JsonHelpers::getVec4(uniformObj, "value");
        } else if (type == "mat4") {
            uniform.type = UniformVar::Type::Mat4;
            uniform.value.m4 = JsonHelpers::getMat4(uniformObj, "value");
        }
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
