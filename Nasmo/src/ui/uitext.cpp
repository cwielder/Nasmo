#include <nsm/ui/uitext.h>

nsm::UIText::UIText(const std::string& text)
    : mText(text)
{ }

void nsm::UIText::setText(const std::string& text) {
    mText = text;
}
