#pragma once

#include <nsm/common.h>

#include <chrono>

namespace nsm {

    class DeltaTime final {
    public:
        f64 update();

    private:
        std::chrono::time_point<std::chrono::steady_clock> mLastTime;
        f64 mTimeStep;
    };

}
