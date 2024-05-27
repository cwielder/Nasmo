#include <nsm/gfx/vertexarray.h>

#include <nsm/gfx/vertexbuffer.h>
#include <nsm/gfx/indexbuffer.h>

#include <nsm/gfx/opengl.h>

nsm::VertexArray::VertexArray()
    : mId(GL_NONE)
{
    glGenVertexArrays(1, &mId);
}

nsm::VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &mId);
}

void nsm::VertexArray::bind() const {
    glBindVertexArray(mId);
}

void nsm::VertexArray::linkBuffer(const VertexBuffer& buffer) const {
    this->bind();
    buffer.bind();
}

void nsm::VertexArray::linkIndices(const IndexBuffer& indices) const {
    this->bind();
    indices.bind();
}
