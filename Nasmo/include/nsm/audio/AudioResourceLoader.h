#pragma once

#include <soloud_wav.h>

#include <string>
#include <unordered_map>
#include <vector>

namespace nsm {

    class AudioResourceLoader {
    public:
        class Resource {
        public:
            Resource() = default;
            ~Resource();

            bool load(const std::string& name, const std::string& path);

            [[nodiscard]] const std::vector<SoLoud::AudioSource*>& getAudioSources() const { return mAudioSources; }

        private:
            std::vector<SoLoud::AudioSource*> mAudioSources;
        };

    public:
        AudioResourceLoader() = default;
        ~AudioResourceLoader();

        void load(const std::string& name);
        void unload(const std::string& name);

        const Resource* getResource(const std::string& name) const;

        [[nodiscard]] const std::unordered_map<std::string, Resource*>& getResources() const { return mResources; }

    private:
        std::unordered_map<std::string, Resource*> mResources;
    };

}
