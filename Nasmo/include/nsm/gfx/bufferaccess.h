#pragma once

#include <nsm/gfx/opengl.h>

namespace nsm {

    enum class BufferAccess {
        ReadOnly = GL_READ_ONLY,
        WriteOnly = GL_WRITE_ONLY,
        ReadWrite = GL_READ_WRITE
    };

}
