#include <nsm/gfx/viewport.h>
#include <nsm/gfx/opengl.h>
#include <nsm/gfx/texture.h>

nsm::Viewport::Viewport(const glm::u32vec2& size)
    : mSize(size)
{ }

nsm::Viewport::Viewport(const Texture& texture)
    : Viewport(texture.getSize())
{ }

void nsm::Viewport::apply() const {
    glViewport(0, 0, mSize.x, mSize.y);
}
