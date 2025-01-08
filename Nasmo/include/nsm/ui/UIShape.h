#pragma once

#include <nsm/Common.h>

#include <glm/vec2.hpp>

#include <vector>

namespace nsm {

    class UIShape {
    public:
        static constexpr u32 cMaxVertices = 46;

    public:
        UIShape() = default;
        ~UIShape() = default;

        /**
         * @note Ensure list is in clockwise order
         */
        void setVertices(const std::vector<glm::vec2>& vertices);

        const std::vector<glm::vec2>& getVertices() const;
        const std::vector<glm::vec2> getVerticesTriangulated() const;

    private:
        std::vector<glm::vec2> mVertices;
    };

}


