#pragma once

#include <nsm/common.h>

#include <nsm/gfx/texture.h>
#include <nsm/gfx/shader.h>

#include <string>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

namespace fastgltf {
    class Asset;
    struct Material;
    struct Texture;
}

namespace nsm {

    class ShaderProgram;

    class Material final {
    public:
        class UniformVar final {
        public:
            enum class Type {
                Int,
                Float,
                Double,
                Vec2,
                Vec3,
                Vec4,
                Mat4,

                Count
            };

        public:
            UniformVar()
                : type(Type::Count)
                , name("")
            { }

            void apply(const ShaderProgram* shader) const;

            Type type;
            std::string name;
            union Values {
                Values() {}

                i32 i;
                f32 f;
                f64 d;
                glm::vec2 v2;
                glm::vec3 v3;
                glm::vec4 v4;
                glm::mat4 m4;
            } value;
        };
    
    public:
        [[nodiscard]] static Material* get(const std::string& identifier, const fastgltf::Asset& asset, const fastgltf::Material& resource);

        void bind() const;

        [[nodiscard]] ShaderProgram* getShader() const { return mShader; }
        [[nodiscard]] const std::vector<Texture>& getTextures() const { return mTextures; }
        [[nodiscard]] const std::vector<UniformVar>& getUniforms() const { return mUniforms; }
        [[nodiscard]] bool isTranslucent() const { return mIsTranslucent; }

        static void clearCache();

    private:
        Material(const fastgltf::Asset& asset, const fastgltf::Material& resource);
        ~Material();

        void addTexture(const fastgltf::Asset& asset, const fastgltf::Texture& tex);

        static std::map<std::string, Material*> sMaterialCache;

        ShaderProgram* mShader;
        std::vector<Texture> mTextures;
        std::vector<UniformVar> mUniforms;
        bool mIsTranslucent;
    };

}
