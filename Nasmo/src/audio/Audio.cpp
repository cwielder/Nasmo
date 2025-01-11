#include <nsm/audio/Audio.h>

#include <nsm/entity/component/AudioComponent.h>
#include <nsm/entity/component/TransformComponent.h>
#include <nsm/entity/component/CameraComponent.h>

#include <glm/vec3.hpp>

#include <imgui.h>

nsm::Audio::Audio(const AudioInfo& info) {
    mSoLoud.init(1, SoLoud::Soloud::MINIAUDIO, info.sampleRate, info.bufferSize, info.channelCount);
#ifdef NSM_DEV_MODE
    mSoLoud.setVisualizationEnable(true);
#endif
}

nsm::Audio::~Audio() {
    mSoLoud.deinit();
}

void nsm::Audio::update() {
    mSoLoud.update3dAudio();

#ifdef NSM_DEV_MODE
    if (ImGui::Begin("Audio")) {
        static constexpr u32 cWaveBufBlockSize = 480;
        static constexpr u32 cWaveBufBlock = 256;
        static constexpr u32 cWaveBufSize = cWaveBufBlockSize * cWaveBufBlock;

        static f32 sWaveBuf[cWaveBufSize] = { 0.0f };

        const f32* buf = mSoLoud.getWave();
        const f32* fft = mSoLoud.calcFFT();

        static f32 sPrevSample = 0.0f;
        bool update = false;

        if (sPrevSample != buf[0]) {
            sPrevSample = buf[0];
            update = true;
        }

        if (update) {
            for (u32 i = 0; i < cWaveBufBlock - 1; i++) {
                u32 dstOffset = cWaveBufBlockSize * i;
                u32 srcOffset = cWaveBufBlockSize * (i + 1);
                std::memcpy(sWaveBuf + dstOffset, sWaveBuf + srcOffset, cWaveBufBlockSize * sizeof(f32));
            }

            u32 dstOffset = cWaveBufBlockSize * (cWaveBufBlock - 1);
            std::memcpy(sWaveBuf + dstOffset, buf, cWaveBufBlockSize * sizeof(f32));
        }

        ImGui::PlotLines("##Wave", sWaveBuf, cWaveBufSize, 0, "Wave", -1, 1, ImVec2(264, 80));
        ImGui::PlotHistogram("##FFT", fft, 256 / 2, 0, "FFT", 0, 10, ImVec2(264, 80), 8);

        ImGui::Text("Active Voices: %u", mSoLoud.getActiveVoiceCount());
        ImGui::Text("Buffer Size: %u", mSoLoud.getBackendBufferSize());

        ImGui::Separator();
        ImGui::Text("Resources");

        const auto& sounds = mResourceLoader.getResources();
        for (const auto& [name, sound] : sounds) {
            ImGui::Text("%s", name.c_str());
        }
    } ImGui::End();
#endif
}

void nsm::Audio::transferData(const std::vector<Entity*>& entities) {
    for (auto entity : entities) {
        // TODO: Is it possible to have multiple listeners?
        for (auto cameraComponent : entity->getComponents<CameraComponent>()) {
            const glm::vec3& pos = cameraComponent->getPosition();
            mSoLoud.set3dListenerPosition(pos.x, pos.y, pos.z);
        }

        for (auto audioComponent : entity->getComponents<AudioComponent>()) {
            if (!audioComponent->isInited()) {
                for (const std::string& sound : audioComponent->getSounds()) {
                    mResourceLoader.load(sound);
                }

                audioComponent->setInited(true);
            }

            const TransformComponent* transform = audioComponent->getTransform();

            for (const auto& request : audioComponent->getRequests()) {
                std::unique_ptr<SoundHandle> handle = this->startSound(request, transform);
                if (request.handle) {
                    *request.handle = std::move(handle);
                }
            }

            audioComponent->clearRequests();
        }
    }
}

std::unique_ptr<nsm::SoundHandle> nsm::Audio::startSound(const AudioComponent::Request& request, const TransformComponent* transform) {
    const AudioResourceLoader::Resource* res = mResourceLoader.getResource(request.sound);
    NSM_ASSERT(res != nullptr, "Resource not found");

    SoLoud::handle voiceGroup = mSoLoud.createVoiceGroup();
    NSM_ASSERT(voiceGroup != 0, "Failed to create voice group");

    const std::vector<SoLoud::AudioSource*>& sources = res->getAudioSources();

    SoLoud::handle voice[cSoundTrackNum];
    for (u32 i = 0; i < sources.size(); i++) {
        if (transform) {
            const glm::vec3& position = transform->getPosition();
            voice[i] = mSoLoud.play3d(*sources[i], position.x, position.y, position.z, 0.0f, 0.0f, 0.0f, -1.0f, true); // TODO: Velocity
        } else {
            voice[i] = mSoLoud.play(*sources[i], -1.0f, 0.0f, true);
        }

        if (voice[i] == 0) {
            // TODO?
        }

        if (transform) {
            // TODO: Fix these
            //mSoLoud.set3dSourceMinMaxDistance(voice[i], 0.1f, 100.0f);
            //mSoLoud.set3dSourceAttenuation(voice[i], 1, 1.0f);
        }

        mSoLoud.addVoiceToGroup(voiceGroup, voice[i]);
    }

    mSoLoud.setPause(voiceGroup, false);
    mSoLoud.setRelativePlaySpeed(voiceGroup, request.speed);

    std::vector<SoundHandle::Track> tracks;
    for (u32 i = 0; i < sources.size(); i++) {
        tracks.emplace_back(voice[i]);
    }

    return std::unique_ptr<SoundHandle>(new SoundHandle(&mSoLoud, voiceGroup, std::move(tracks)));
}
