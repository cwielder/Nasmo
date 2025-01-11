#include <nsm/audio/SoundHandle.h>

nsm::SoundHandle::SoundHandle(SoLoud::Soloud* soLoud, SoLoud::handle voiceGroup, std::vector<Track>&& tracks)
    : mSoLoud(soLoud)
    , mVoiceGroupHandle(voiceGroup)
    , mTracks(tracks)
{ }

void nsm::SoundHandle::pause(bool pause) const {
    mSoLoud->setPause(mVoiceGroupHandle, pause);
}

void nsm::SoundHandle::stop() const {
    mSoLoud->stop(mVoiceGroupHandle);
}

void nsm::SoundHandle::setTrackVolume(u32 trackBitFlag, f32 volume, f32 fade) {
    for (u32 i = 0; i < mTracks.size(); i++) {
        if (trackBitFlag & (1 << i)) {
            mSoLoud->fadeVolume(mTracks[i].voice, volume, fade);
        }
    }
}
