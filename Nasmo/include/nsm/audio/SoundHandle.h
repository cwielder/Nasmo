#pragma once

#include <nsm/Common.h>

#include <soloud.h>

#include <vector>

namespace nsm {

    class SoundHandle {
    private:
        struct Track {
            Track(SoLoud::handle handle)
                : voice(handle)
            { }

            SoLoud::handle voice;
        };

    private:
        friend class Audio;

        SoundHandle(SoLoud::Soloud* soLoud, SoLoud::handle voiceGroup, std::vector<Track>&& tracks);

    public:
        SoundHandle() = default;
        ~SoundHandle() = default;
        
        void pause(bool pause) const;
        void stop() const;

        void setTrackVolume(u32 trackBitFlag, f32 volume, f32 fade);

        [[nodiscard]] f32 getPlayPosition() const { return static_cast<f32>(mSoLoud->getStreamPosition(mVoiceGroupHandle)); }
        [[nodiscard]] std::size_t getTrackNum() const { return mTracks.size(); }

    private:
        SoLoud::Soloud* mSoLoud;
        SoLoud::handle mVoiceGroupHandle;
        std::vector<Track> mTracks;
    };

}
