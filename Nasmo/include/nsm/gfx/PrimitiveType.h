#pragma once

#include <nsm/gfx/OpenGL.h>

namespace nsm {

    enum class PrimitiveType {
        Points = GL_POINTS,

        Lines = GL_LINES,
        LineStrip = GL_LINE_STRIP,
        LineLoop = GL_LINE_LOOP,

        Triangles = GL_TRIANGLES,
        TriangleStrip = GL_TRIANGLE_STRIP,
        TriangleFan = GL_TRIANGLE_FAN,
    };

}
