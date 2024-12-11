#include <string>
#include <glm/vec4.hpp>
#include <nsm/common.h>
#include <sstream>
#include <iomanip>
#include <nsm/debug/assert.h>

namespace nsm {

    class Color {
    public:
        Color(const std::string& hex);
        Color(const glm::vec4& rgba);
        ~Color() = default;

        std::string getHex() const;
        glm::vec4 getRGBA() const;

    private:
        glm::vec4 mRGBA;
    };
}
