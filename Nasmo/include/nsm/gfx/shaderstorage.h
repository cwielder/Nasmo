#pragma once

#include <nsm/common.h>
#include <nsm/gfx/bufferusage.h>

namespace nsm {

    class ShaderStorage final {
        NSM_NO_COPY(ShaderStorage);

    public:
        ShaderStorage();
        ShaderStorage(const std::size_t size, const void* data, const BufferUsage usage = BufferUsage::DynamicDraw);
        ~ShaderStorage();

        ShaderStorage(ShaderStorage&& other) noexcept
            : mId(other.mId)
        {
            other.mId = 0;
        }

        ShaderStorage& operator=(ShaderStorage&& other) noexcept {
            if (this == &other) {
                mId = other.mId;
                other.mId = 0;
            }

            return *this;
        }

        void bind(const u32 bindingPoint) const;
        void setData(const std::size_t size, const void* data, const BufferUsage usage = BufferUsage::DynamicDraw) const;

    private:
        u32 mId;
    };

}
