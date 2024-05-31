#include <nsm/gfx/vertexarray.h>

#include <nsm/gfx/vertexbuffer.h>
#include <nsm/gfx/indexbuffer.h>
#include <nsm/gfx/opengl.h>

#include <nsm/debug/log.h>

nsm::VertexArray::VertexArray()
    : mId(GL_NONE)
{
    glGenVertexArrays(1, &mId);
    nsm::trace("Creating vertex array with id: ", mId);
}

nsm::VertexArray::~VertexArray() {
    nsm::trace("Deleting vertex array with id: ", mId);
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
