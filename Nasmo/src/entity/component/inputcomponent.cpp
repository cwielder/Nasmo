#include <nsm/entity/component/inputcomponent.h>

void nsm::InputComponent::onEvent(const nsm::Event* event) {
    switch (event->getType()) {
        case nsm::EventType::KeyPress: {
            if (mOnKeyPress) {
                mOnKeyPress(static_cast<const nsm::KeyPressEvent*>(event));
            }

            break;
        }

        case nsm::EventType::KeyRelease: {
            if (mOnKeyRelease) {
                mOnKeyRelease(static_cast<const nsm::KeyReleaseEvent*>(event));
            }

            break;
        }

        case nsm::EventType::KeyRepeat: {
            if (mOnKeyRepeat) {
                mOnKeyRepeat(static_cast<const nsm::KeyRepeatEvent*>(event));
            }

            break;
        }

        case nsm::EventType::MouseMove: {
            if (mOnMouseMove) {
                mOnMouseMove(static_cast<const nsm::MouseMoveEvent*>(event));
            }

            break;
        }

        case nsm::EventType::MousePress: {
            if (mOnMousePress) {
                mOnMousePress(static_cast<const nsm::MousePressEvent*>(event));
            }

            break;
        }

        case nsm::EventType::MouseRelease: {
            if (mOnMouseRelease) {
                mOnMouseRelease(static_cast<const nsm::MouseReleaseEvent*>(event));
            }

            break;
        }

        case nsm::EventType::MouseScroll: {
            if (mOnMouseScroll) {
                mOnMouseScroll(static_cast<const nsm::MouseScrollEvent*>(event));
            }

            break;
        }

        default: {
            break;
        }
    }
}
