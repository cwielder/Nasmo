#pragma once

#include <nsm/Common.h>

namespace nsm {

    class RandomSource {
    public:
        RandomSource();
        RandomSource(const u64 seed);
        ~RandomSource() = default;

        void permute();

        bool getBool();

        u32 getU32();

        u64 getU64();

        f32 getF32();
        f32 getF32(const f32 min, const f32 max);

        f64 getF64();

    private:
        u64 mSeed;
    };

}
