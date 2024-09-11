#pragma once

#include <nsm/common.h>

namespace nsm {

    class AnyVector {
    public:
        AnyVector();
        ~AnyVector();

        [[nodiscard]] void* getBuffer() const { return mBuffer; }
        [[nodiscard]] std::size_t getElementSize() const { return mElementSize; }

        void addElement(const std::size_t newCount);
        void removeElement(const std::size_t newCount, const std::size_t index);

        void setElementData(const std::size_t index, const void* data);
        void setElementSize(const std::size_t elementSize) { mElementSize = elementSize; }

    private:
        void* mBuffer;
        std::size_t mElementSize;
    };

}
