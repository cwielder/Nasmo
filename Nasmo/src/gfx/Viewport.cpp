#include <nsm/gfx/Viewport.h>

#include <nsm/gfx/OpenGL.h>
#include <nsm/gfx/Texture2D.h>

nsm::Viewport::Viewport(const glm::u32vec2& size)
    : mSize(size)
{ }

nsm::Viewport::Viewport(const Texture2D& texture)
    : Viewport(texture.getSize())
{ }

void nsm::Viewport::apply() const {
    glViewport(0, 0, mSize.x, mSize.y);
}
