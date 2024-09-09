#pragma once

#include <nsm/common.h>
#include <nsm/gfx/opengl.h>

namespace nsm {

    class Texture final {
    public:
        enum class FilterMode : u16 {
            Nearest = GL_NEAREST,
            Linear = GL_LINEAR,

            NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
            LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
            NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
            LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR
        };

        enum class WrapMode : u16 {
            Repeat = GL_REPEAT,
            MirroredRepeat = GL_MIRRORED_REPEAT,
            ClampToEdge = GL_CLAMP_TO_EDGE,
            ClampToBorder = GL_CLAMP_TO_BORDER
        };

        enum class Format : u16 {
            R11G11B10F = GL_R11F_G11F_B10F,
            RGB16F = GL_RGB16F,
            RGB32F = GL_RGB32F,
            R8 = GL_R8,
            RGB8 = GL_RGB8,
            RGBA8 = GL_RGBA8,
            RGBA16F = GL_RGBA16F,
            RGBA32F = GL_RGBA32F,
            SRGB8 = GL_SRGB8,
            SRGBA8 = GL_SRGB8_ALPHA8,
            Depth24Stencil8 = GL_DEPTH24_STENCIL8,
            Depth32FStencil8 = GL_DEPTH32F_STENCIL8
        };
    };

}
