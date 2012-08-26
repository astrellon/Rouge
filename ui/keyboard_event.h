#pragma once

#include "event.h"
#include "keyboard_common.h"

namespace am {
namespace ui {

	class KeyboardEvent : public Event {
	public:
		KeyboardEvent(KeyboardEventType type, int key, bool systemKey);
		~KeyboardEvent();

		int getKey() const;
		bool isSystemKey() const;

		KeyboardEventType getKeyboardEventType() const;

	protected:

		KeyboardEventType mKeyboardEventType;
		int mKey;
		bool mSystemKey;
	};

}
}
