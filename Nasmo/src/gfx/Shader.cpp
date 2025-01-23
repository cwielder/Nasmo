#include <nsm/gfx/Shader.h>

#include <nsm/gfx/OpenGL.h>
#include <nsm/debug/Assert.h>

#include <fstream>

#include <glm/gtc/type_ptr.hpp>

std::unordered_map<std::string, std::string> nsm::ShaderProgram::sSourceCache;
std::map<u32, u32> nsm::ShaderProgram::sProgramCacheRefCounter;
std::unordered_map<std::string, std::pair<u32, std::unordered_map<std::string, i32>>> nsm::ShaderProgram::sProgramCache;

// ShaderProgram::Shader

nsm::ShaderProgram::Shader::Shader(const std::string& path, const ShaderType type)
    : mId(GL_NONE)
{
    auto compile = [=](const std::string& source) -> u32 {
        nsm::trace("Compiling shader: ", path.c_str(), "...");

        const u32 shader = glCreateShader(static_cast<GLenum>(type));
        const char* src = source.c_str();

        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        i32 success = false;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            i32 length = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

            std::vector<char> error(length);
            glGetShaderInfoLog(shader, length, &length, error.data());

            NSM_ASSERT(false, "Shader compilation failed: ", error.data());
        }

        return shader;
    };

    for (const auto& [path_, code_] : sSourceCache) {
        if (path_ == path) {
            mId = compile(code_);
            return;
        }
    }

    std::ifstream file(path);
    NSM_ASSERT(file.is_open(), "Failed to open shader file: ", path.c_str());

    const std::string shader(std::istreambuf_iterator<char>{file}, {});
    mId = compile(shader);

    sSourceCache[path] = shader;
}

nsm::ShaderProgram::Shader::~Shader() {
    glDeleteShader(mId);
}

// ShaderProgram

nsm::ShaderProgram::ShaderProgram(const std::string& vshPath, const std::string& fshPath)
    : mId(GL_NONE)
    , mUniformLocations()
{
    if (vshPath.empty() && fshPath.empty())
        return;

    for (const auto& [identifier, program] : sProgramCache) {
        if (identifier == vshPath + fshPath) {
            const auto& [id, uniformLocations] = program;

            mId = id;
            mUniformLocations = uniformLocations;
            mIdentifier = identifier;

            sProgramCacheRefCounter[mId]++;
            return;
        }
    }

    const Shader vertexShader(vshPath, ShaderType::Vertex);
    const Shader fragmentShader(fshPath, ShaderType::Fragment);

    mId = ShaderProgram::link(vertexShader, fragmentShader);

    this->cacheUniforms();

    sProgramCache.emplace(vshPath + fshPath, std::make_pair(mId, mUniformLocations));
    sProgramCacheRefCounter[mId]++;
}

nsm::ShaderProgram::ShaderProgram(const std::string& vshPath, const std::string& fshPath, const std::string& gshPath)
    : mId(GL_NONE)
    , mUniformLocations()
{
    for (const auto& [identifier, program] : sProgramCache) {
        if (identifier == vshPath + fshPath + gshPath) {
            const auto& [id, uniformLocations] = program;
            
            mId = id;
            mUniformLocations = uniformLocations;

            sProgramCacheRefCounter[mId]++;
            return;
        }
    }

    const Shader vertexShader(vshPath, ShaderType::Vertex);
    const Shader fragmentShader(fshPath, ShaderType::Fragment);
    const Shader geometryShader(gshPath, ShaderType::Geometry);

    mId = ShaderProgram::link(vertexShader, fragmentShader, &geometryShader);

    this->cacheUniforms();

    sProgramCache.emplace(vshPath + fshPath + gshPath, std::make_pair(mId, mUniformLocations));
    sProgramCacheRefCounter[mId]++;
}

nsm::ShaderProgram::~ShaderProgram() {
    if (mId == GL_NONE)
        return;

    sProgramCacheRefCounter[mId]--;

    if (sProgramCacheRefCounter[mId] == 0) {
        nsm::trace("Deleting shader program: ", mId, "...");

        glDeleteProgram(mId);
        sProgramCache.erase(std::ranges::find_if(sProgramCache.begin(), sProgramCache.end(), [this](const auto& program) {
            return program.second.first == mId;
        }));

        sProgramCacheRefCounter.erase(mId);
    }
}

void nsm::ShaderProgram::bind() const {
    glUseProgram(mId);
}

void nsm::ShaderProgram::cacheUniforms() {
    i32 count = 0;
    glGetProgramiv(mId, GL_ACTIVE_UNIFORMS, &count);

    i32 maxLength = 0;
    glGetProgramiv(mId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength);

    if (count == 0)
        return;

    std::vector<char> name(maxLength);

    for (std::int_fast16_t i = 0; i < count; ++i) {
        u32 data; i32 length, size;
        glGetActiveUniform(mId, i, maxLength, &length, &size, &data, name.data());

        mUniformLocations.emplace(name.data(), glGetUniformLocation(mId, name.data()));
    }
}

i32 nsm::ShaderProgram::getLocation(const std::string& name) const {
    auto location = mUniformLocations.find(name);

    NSM_ASSERT(location != mUniformLocations.end(), "Uniform location not found: ", name.c_str());
    
    return location->second;
}

i32 nsm::ShaderProgram::getOptionalLocation(const std::string& name) const {
    auto location = mUniformLocations.find(name);

    if (location == mUniformLocations.end())
        return -1;

    return location->second;
}

u32 nsm::ShaderProgram::link(const Shader& vsh, const Shader& fsh, const Shader* gsh) {
    const u32 program = glCreateProgram();
    nsm::trace("Creating shader program: ", program, "...");

    glAttachShader(program, vsh.getId());
    glAttachShader(program, fsh.getId());

    if (gsh != nullptr) {
        glAttachShader(program, gsh->getId());
    }

    glLinkProgram(program);


    #ifdef NSM_DEBUG
        i32 success = false;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            i32 length = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

            std::vector<char> error(length);
            glGetProgramInfoLog(program, length, &length, error.data());

            NSM_ASSERT(false, "Shader program linking failed: ", error.data());
        }
    #endif

    glDetachShader(program, vsh.getId());
    glDetachShader(program, fsh.getId());

    return program;
}

void nsm::ShaderProgram::setBool(const std::string& name, const bool value) const {
    glUniform1i(this->getLocation(name), value);
}

void nsm::ShaderProgram::setInt(const std::string& name, const int value) const {
    glUniform1i(this->getLocation(name), value);
}

void nsm::ShaderProgram::setFloat(const std::string& name, const float value) const {
    glUniform1f(this->getLocation(name), value);
}

void nsm::ShaderProgram::setDouble(const std::string& name, const double value) const {
    glUniform1d(this->getLocation(name), value);
}

void nsm::ShaderProgram::setVec2(const std::string& name, const glm::vec2& value) const {
    glUniform2f(this->getLocation(name), value.x, value.y);
}

void nsm::ShaderProgram::setVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3f(this->getLocation(name), value.x, value.y, value.z);
}

void nsm::ShaderProgram::setVec4(const std::string& name, const glm::vec4& value) const {
    glUniform4f(this->getLocation(name), value.x, value.y, value.z, value.w);
}

void nsm::ShaderProgram::setMat4(const std::string& name, const glm::mat4& value) const {
    glUniformMatrix4fv(this->getLocation(name), 1, false, glm::value_ptr(value));
}

void nsm::ShaderProgram::setBool(const i32 location, const bool value) const {
    glUniform1i(location, value);
}

void nsm::ShaderProgram::setInt(const i32 location, const int value) const {
    glUniform1i(location, value);
}

void nsm::ShaderProgram::setFloat(const i32 location, const float value) const {
    glUniform1f(location, value);
}

void nsm::ShaderProgram::setDouble(const i32 location, const double value) const {
    glUniform1d(location, value);
}

void nsm::ShaderProgram::setVec2(const i32 location, const glm::vec2& value) const {
    glUniform2f(location, value.x, value.y);
}

void nsm::ShaderProgram::setVec3(const i32 location, const glm::vec3& value) const {
    glUniform3f(location, value.x, value.y, value.z);
}

void nsm::ShaderProgram::setVec4(const i32 location, const glm::vec4& value) const {
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void nsm::ShaderProgram::setMat4(const i32 location, const glm::mat4& value) const {
    glUniformMatrix4fv(location, 1, false, glm::value_ptr(value));
}

void nsm::ShaderProgram::setOptionalBool(const std::string& name, const bool value) const {
    const i32 location = this->getOptionalLocation(name);
    if (location == -1)
        return;

    glUniform1i(location, value);
}

void nsm::ShaderProgram::setOptionalInt(const std::string& name, const int value) const {
    const i32 location = this->getOptionalLocation(name);
    if (location == -1)
        return;

    glUniform1i(location, value);
}

void nsm::ShaderProgram::setOptionalFloat(const std::string& name, const float value) const {
    const i32 location = this->getOptionalLocation(name);
    if (location == -1)
        return;

    glUniform1f(location, value);
}

void nsm::ShaderProgram::setOptionalDouble(const std::string& name, const double value) const {
    const i32 location = this->getOptionalLocation(name);
    if (location == -1)
        return;

    glUniform1d(location, value);
}

void nsm::ShaderProgram::setOptionalVec2(const std::string& name, const glm::vec2& value) const {
    const i32 location = this->getOptionalLocation(name);
    if (location == -1)
        return;

    glUniform2f(location, value.x, value.y);
}

void nsm::ShaderProgram::setOptionalVec3(const std::string& name, const glm::vec3& value) const {
    const i32 location = this->getOptionalLocation(name);
    if (location == -1)
        return;

    glUniform3f(location, value.x, value.y, value.z);
}

void nsm::ShaderProgram::setOptionalVec4(const std::string& name, const glm::vec4& value) const {
    const i32 location = this->getOptionalLocation(name);
    if (location == -1)
        return;

    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void nsm::ShaderProgram::setOptionalMat4(const std::string& name, const glm::mat4& value) const {
    const i32 location = this->getOptionalLocation(name);
    if (location == -1)
        return;

    glUniformMatrix4fv(location, 1, false, glm::value_ptr(value));
}
