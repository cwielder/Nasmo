#include <nsm/util/color.h>

#include <cmath>

nsm::Color::Color(const std::string& hex) {
    NSM_ASSERT(!hex.empty(), "Hex string can't be empty: ", hex);
    NSM_ASSERT(hex.length() == 4 || hex.length() == 5 || hex.length() == 7 || hex.length() == 9, "Invalid hex string length of ", hex.length());
    NSM_ASSERT(hex[0] == '#', "Hex string must start with #");
    
    std::string expandedHex;

    if (hex.length() == 4 || hex.length() == 5) {
        for (u32 i = 1; i < hex.length(); i++) {
            expandedHex += hex[i];
            expandedHex += hex[i];
        }
    } else {
        expandedHex = hex.substr(1); 
    }

    u32 r, g, b, a = 255;
    std::istringstream stream;

    stream.str(expandedHex.substr(0, 2));
    stream >> std::hex >> r;
    stream.clear();

    stream.str(expandedHex.substr(2, 2));
    stream >> std::hex >> g;
    stream.clear();

    stream.str(expandedHex.substr(4, 2));
    stream >> std::hex >> b;
    stream.clear();

    if (expandedHex.length() == 8) {
        stream.str(expandedHex.substr(6, 2));
        stream >> std::hex >> a;
    }

    mRGBA = glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}

nsm::Color::Color(const glm::vec4& rgba)
    : mRGBA(rgba)
{ }

std::string nsm::Color::getHex() const {
    std::ostringstream stream;
    stream << "#";

    stream << std::hex << std::setw(2) << std::setfill('0') << static_cast<u32>(std::round(mRGBA.r * 255.0f));
    stream << std::hex << std::setw(2) << std::setfill('0') << static_cast<u32>(std::round(mRGBA.g * 255.0f));
    stream << std::hex << std::setw(2) << std::setfill('0') << static_cast<u32>(std::round(mRGBA.b * 255.0f));

    u32 a = static_cast<u32>(std::round(mRGBA.a * 255.0f));

    if (a < 255) {
        stream << std::hex << std::setw(2) << std::setfill('0') << a;
    }

    return stream.str();
}

glm::vec4 nsm::Color::getRGBA() const {
    return mRGBA;
}
