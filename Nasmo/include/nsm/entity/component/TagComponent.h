#pragma once 

#include <nsm/entity/Entity.h>

namespace nsm {

    class TagComponent : public EntityComponent {
    public:
        TagComponent(const std::string& tag)
            : mTag(tag)
        { }
        
        ~TagComponent() override = default;

        [[nodiscard]] const std::string& getTag() const { return mTag; }

    private:
        std::string mTag;
    };

}
