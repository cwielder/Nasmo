#include <nsm/app/deltatime.h>

f64 nsm::DeltaTime::update() {
    auto current = std::chrono::steady_clock::now();
    mTimeStep = std::chrono::duration<f64>(current - mLastTime).count();
    mLastTime = current;

    return mTimeStep;
}
