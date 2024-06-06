#pragma once

#include <nsm/common.h>

#include <string>
#include <unordered_map>
#include <map>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <nsm/gfx/opengl.h>

namespace nsm {

    class ShaderProgram final {
        NSM_NO_COPY(ShaderProgram);

    public:
        enum class ShaderType {
            Vertex = GL_VERTEX_SHADER,
            Geometry = GL_GEOMETRY_SHADER,
            Fragment = GL_FRAGMENT_SHADER,

            Count
        };

    private:
        class Shader {
        public:
            Shader(const std::string& path, const ShaderType type);
            ~Shader();

            u32 id;
        };

    public:
        ShaderProgram(const std::string& vshPath, const std::string& fshPath);
        ShaderProgram(const std::string& vshPath, const std::string& fshPath, const std::string& gshPath);
        ~ShaderProgram();

        void bind() const;

        [[nodiscard]] bool exists() const { return mId != 0; }

        // Uniforms
        //* Note: You must bind the shader before calling these functions.
        
        void setBool(const std::string& name, const bool value) const;
        void setInt(const std::string& name, const int value) const;
        void setFloat(const std::string& name, const f32 value) const;
        void setDouble(const std::string& name, const f64 value) const;
        void setVec2(const std::string& name, const glm::vec2& value) const;
        void setVec3(const std::string& name, const glm::vec3& value) const;
        void setVec4(const std::string& name, const glm::vec4& value) const;
        void setMat4(const std::string& name, const glm::mat4& value) const;

        void setBool(const i32 location, const bool value) const;
        void setInt(const i32 location, const int value) const;
        void setFloat(const i32 location, const f32 value) const;
        void setDouble(const i32 location, const f64 value) const;
        void setVec2(const i32 location, const glm::vec2& value) const;
        void setVec3(const i32 location, const glm::vec3& value) const;
        void setVec4(const i32 location, const glm::vec4& value) const;
        void setMat4(const i32 location, const glm::mat4& value) const;

        void setOptionalBool(const std::string& name, const bool value) const;
        void setOptionalInt(const std::string& name, const int value) const;
        void setOptionalFloat(const std::string& name, const f32 value) const;
        void setOptionalDouble(const std::string& name, const f64 value) const;
        void setOptionalVec2(const std::string& name, const glm::vec2& value) const;
        void setOptionalVec3(const std::string& name, const glm::vec3& value) const;
        void setOptionalVec4(const std::string& name, const glm::vec4& value) const;
        void setOptionalMat4(const std::string& name, const glm::mat4& value) const;
    
    private:
        void cacheUniforms();
        i32 getLocation(const std::string& name) const;
        i32 getOptionalLocation(const std::string& name) const;
    
        static u32 link(const Shader& vsh, const Shader& fsh, const Shader* gsh = nullptr);
        static std::unordered_map<std::string, std::string> sSourceCache;
        static std::map<u32, u32> sProgramCacheRefCounter;
        static std::unordered_map<std::string, std::pair<u32, std::unordered_map<std::string, i32>>> sProgramCache;

        u32 mId;
        std::unordered_map<std::string /*name*/, i32 /*location*/> mUniformLocations;
    };

}
