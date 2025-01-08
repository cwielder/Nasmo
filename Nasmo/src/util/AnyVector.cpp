#include <nsm/util/AnyVector.h>
#include <nsm/debug/Assert.h>
#include <cstdlib>

nsm::AnyVector::AnyVector()
    : mBuffer(nullptr)
    , mElementSize(0)
{ }

nsm::AnyVector::~AnyVector() {
    if (mBuffer != nullptr) {
        std::free(mBuffer);
    }
}

void nsm::AnyVector::addElement(const std::size_t newCount) {
    if (mBuffer == nullptr) {
        mBuffer = std::malloc(newCount * mElementSize);
    } else {
        mBuffer = std::realloc(mBuffer, newCount * mElementSize);
    }
}

void nsm::AnyVector::removeElement(const std::size_t newCount, const std::size_t index) {
    u8* newBuffer = static_cast<u8*>(std::malloc(mElementSize * newCount));

    // Copy the data before the removed instance
    std::memcpy(newBuffer, mBuffer, mElementSize * index);

    // Copy the data after the removed instance (if any) but skip the removed instance
    std::memcpy(newBuffer + mElementSize * index, static_cast<u8*>(mBuffer) + mElementSize * (index + 1), mElementSize * (newCount - index));

    // Free the old buffer
    std::free(mBuffer);

    mBuffer = newBuffer;
}

void nsm::AnyVector::setElementData(const std::size_t index, const void* data) {
    NSM_ASSERT(mBuffer != nullptr, "Buffer is nullptr");
    NSM_ASSERT(mElementSize > 0, "Element size is 0");

    std::memcpy(static_cast<u8*>(mBuffer) + (mElementSize * index), data, mElementSize);
}
