#include <nsm/gfx/shaderstorage.h>

#include <nsm/gfx/opengl.h>

nsm::ShaderStorage::ShaderStorage()
    : mId(GL_NONE)
{
    glGenBuffers(1, &mId);
}

nsm::ShaderStorage::ShaderStorage(const std::size_t size, const void* data, const BufferUsage usage)
    : ShaderStorage()
{
    this->setData(size, data, usage);
}

nsm::ShaderStorage::~ShaderStorage() {
    if (mId == GL_NONE) {
        return;
    }

    glDeleteBuffers(1, &mId);
}

void nsm::ShaderStorage::bind(const u32 bindingPoint) const {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, mId);
}

void nsm::ShaderStorage::setData(const std::size_t size, const void* data, const BufferUsage usage) const {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mId);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, static_cast<GLenum>(usage));
}
