#pragma once

#include "event.h"
#include "keyboard_common.h"

namespace am {
namespace ui {

	class KeyboardEvent : public Event {
	public:
		KeyboardEvent(KeyboardEventType type, int key);
		~KeyboardEvent();

		int getKey() const;

		KeyboardEventType getKeyboardEventType() const;

	protected:

		KeyboardEventType mKeyboardEventType;
		int mKey;
	};

}
}
