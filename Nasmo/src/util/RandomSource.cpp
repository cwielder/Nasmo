#include <nsm/util/RandomSource.h>

nsm::RandomSource::RandomSource(const u64 seed)
    : mSeed(seed)
{ }

void nsm::RandomSource::permute() {
    mSeed = (mSeed ^ (mSeed >> 30)) * 0xBF58476D1CE4E5B9UL;
    mSeed = (mSeed ^ (mSeed >> 27)) * 0x94D049BB133111EBUL;
    mSeed = mSeed ^ (mSeed >> 31);
}

bool nsm::RandomSource::getBool() {
    return this->getU64() & 1;
}

u32 nsm::RandomSource::getU32() {
    return static_cast<u32>(this->getU64());
}

u64 nsm::RandomSource::getU64() {
    this->permute();
    return mSeed;
}

f32 nsm::RandomSource::getF32() {
    return (this->getU64() >> (64 - 24)) * 0x1.0p-24F;
}

f32 nsm::RandomSource::getF32(const f32 min, const f32 max) {
    return min + this->getF32() * (max - min);
}

f64 nsm::RandomSource::getF64() {
    return (this->getU64() >> (64 - 53)) * 0x1.0p-53;
}
